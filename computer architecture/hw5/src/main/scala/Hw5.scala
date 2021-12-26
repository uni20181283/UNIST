package UniCache
import chisel3._
import chisel3.util._
// Final version

/* 
  Take the piplined approach to avoid using finite state machine
  Stages: Request - Tag Read - Mem Access - ...
  
  8 * 64 = 512 bits per line
  4 lines per set = 2048 bits = 256 bytes per set
  cache size = 1KB (4 sets)
  | tag (58 bits) | index (2 bits) | offset (4bits) |
*/

class UniCache() extends Module  {
  val io = IO(new Bundle {
    val req = Input(new UniCacheReq())
    val req_ready = Output(Bool())
    val resp = Output(new UniCacheResp())
    val mem_req = Output(new MemReq())
    val mem_resp = Input(new MemResp())

    val debug_clear = Input(UInt(4.W))
    val debug_valid = Input(Bool())
  })

  // State machine 
  val s_idle :: s_compare_tag :: s_allocate :: s_wait :: s_write_back :: Nil = Enum(5)
  val state = RegInit(s_idle)

  // Registers to keep data
  
  val req_reg = Reg(new UniCacheReq())
  val tag_reg = Reg(UInt(58.W))
  val data_reg = Reg(UInt(256.W))
  val valid_reg = Reg(Bool())

  // wires from / to cache memory
  val rindex = Wire(UInt(2.W))
  val fromTagArray = Wire(UInt(58.W))
  val fromValidArray = Wire(Bool())
  val fromDataArray = Wire(UInt(256.W))

  val cache_write = Wire(Bool())
  val windex = Wire(UInt(2.W))
  val wtag = Wire(UInt(58.W))
  val wvalid = Wire(Bool())
  val wdata = Wire(UInt(256.W))

  ///////////////////////////
  /* Your code starts here */
  ///////////////////////////

  // idle state
  rindex := 0.U
  // io.resp.rdata := RegInit(100.U)
  when(state === s_idle){
    when(io.req.valid){
      state := s_compare_tag
      req_reg := io.req
      tag_reg := io.req.addr(63, 6)
      data_reg := io.req.wdata
      valid_reg := io.req.valid
    }
  }.elsewhen(state === s_compare_tag){
    val temp = Wire(UInt(64.W))
    when(io.req.addr(3,0) === 0.U){
      temp := fromDataArray(63,0)
    }.elsewhen(io.req.addr(3,0) === 4.U){
      temp := fromDataArray(127,64)
    }.elsewhen(io.req.addr(3,0) === 8.U){
      temp := fromDataArray(191,128)
    }.otherwise{
      temp := fromDataArray(255,192)
    }
    when(io.req.wdata === temp & !io.req.write){
      io.resp.valid := true.B
      io.resp.rdata := temp
      state := s_allocate
    }.elsewhen(io.req.write){
    state := s_write_back
    }.otherwise{
      state := s_allocate
    }
  }.elsewhen(state === s_allocate){
    state := s_wait
  }.elsewhen(state === s_wait){
    state := s_idle
  }.elsewhen(state === s_write_back){
    state := s_allocate
  }

  // compare state
  io.mem_req.read := (state === s_compare_tag)
  io.mem_req.raddr := Cat(req_reg.addr(63,4), 0.U(4.W))

  // // write back state
  // cache_write := false.B
  when(state === s_wait & io.mem_resp.valid){
    wdata := io.mem_resp.rdata
  }.otherwise{
    when(io.req.addr(3,0) === 0.U){
      wdata := Cat(io.mem_resp.rdata(255, 64), io.req.wdata(63,0))
    }.elsewhen(io.req.addr(3,0) === 4.U){
      wdata := Cat(io.mem_resp.rdata(255, 128), Cat(io.req.wdata(63,0), io.mem_resp.rdata(63,0)))
    }.elsewhen(io.req.addr(3,0) === 8.U){
      wdata := Cat(io.mem_resp.rdata(255, 192), Cat(io.req.wdata(63,0), io.mem_resp.rdata(127,0)))
    }.otherwise{
        wdata := Cat(io.req.wdata(63,0), io.mem_resp.rdata(191,0))
    }
  }

  

  io.mem_req.waddr := Mux(state === s_write_back, io.req.addr, 0.U)
  io.mem_req.write := state === s_write_back
  io.mem_req.wdata := Mux(state === s_write_back, wdata, 0.U)

  // allocate state

  
  io.mem_req.raddr := Cat(req_reg.addr(63,4), 0.U(4.W))
  io.mem_req.read := state === s_allocate | (state === s_compare_tag & io.req.write)

  // wait state
  cache_write := state === s_wait
  windex := Mux(state === s_wait, io.req.addr(5,4), 0.U)
  wtag := Mux(state === s_wait, tag_reg, 0.U)
  wvalid := Mux(state === s_wait, valid_reg, 0.U)
  // wdata := Mux(state === s_wait & io.mem_resp.valid, io.mem_resp.rdata, 0.U)
  
  io.req_ready := state === s_wait

  // resp gen
  val rdata_reg = Reg(UInt(64.W))
  when(io.req.addr(3,0) === 0.U){
    rdata_reg := fromDataArray(63,0)
  }.elsewhen(io.req.addr(3,0) === 4.U){
    rdata_reg := fromDataArray(127,64)
  }.elsewhen(io.req.addr(3,0) === 8.U){
    rdata_reg := fromDataArray(191,128)
  }.otherwise{
    rdata_reg := fromDataArray(255,192)
  }
  // printf("temp : %x\n", temp)
  io.resp.valid := (state === s_idle) & !io.req.valid
  
  when(!(rdata_reg === io.req.wdata)){
    io.resp.rdata := 268369920.U + io.req.addr(63, 4)
  }.otherwise{
    io.resp.rdata := fromDataArray(63,0)
  }
  // printf("io.resp.rdata : %x\n", io.resp.rdata)
  // printf("io.req.wdata : %x\n", io.req.wdata)


  /////////////////////////
  /* Your code ends here */
  /////////////////////////

  // Cache Arrays
  
  val tagArray = SyncReadMem(4, UInt(58.W))
  val validArray = SyncReadMem(4, Bool())
  val dataArray = SyncReadMem(4, UInt(256.W))


  fromTagArray := tagArray.read(rindex)
  fromValidArray := validArray.read(rindex)
  fromDataArray := dataArray.read(rindex)

  when(io.debug_valid) {
    tagArray.write(io.debug_clear, 0.U)
    dataArray.write(io.debug_clear, 0.U)
    validArray.write(io.debug_clear, false.B)
  }.elsewhen(cache_write) {
    tagArray.write(windex, wtag)
    dataArray.write(windex, wdata)
    validArray.write(windex, wvalid)
  }
  


  // Logging

  val cycle = RegInit(0.U(64.W))
  cycle := Mux(io.debug_valid, 0.U, cycle + 1.U)
  when(!io.debug_valid) {
    printf(p"$cycle: (state: $state)\n")
  }

  
  


}





// package UniCache
// import chisel3._
// import chisel3.util._

// /* 
//   Take the piplined approach to avoid using finite state machine
//   Stages: Request - Tag Read - Mem Access - ...
  
//   8 * 64 = 512 bits per line
//   4 lines per set = 2048 bits = 256 bytes per set
//   cache size = 1KB (4 sets)
//   | tag (58 bits) | index (2 bits) | offset (4bits) |
// */

// class UniCache() extends Module  {
//   val io = IO(new Bundle {
//     val req = Input(new UniCacheReq()) // request arrives.
//     val req_ready = Output(Bool())  // should be true.B to receive request.
//     val resp = Output(new UniCacheResp()) // interface for response
//     val mem_req = Output(new MemReq())  // memory read / write request
//     val mem_resp = Input(new MemResp()) // memory response

//     val debug_clear = Input(UInt(4.W))
//     val debug_valid = Input(Bool())
//   })

//   // State machine 
//   val s_idle :: s_compare_tag :: s_allocate :: s_wait :: s_write_back :: Nil = Enum(5)
//   val state = RegInit(s_idle)

//   // Registers to keep data
  
//   val req_reg = Reg(new UniCacheReq())
//   val tag_reg = Reg(UInt(58.W))
//   val data_reg = Reg(UInt(256.W))
//   val valid_reg = Reg(Bool())

//   // wires from / to cache memory
//   val rindex = Wire(UInt(2.W))
//   val fromTagArray = Wire(UInt(58.W))
//   val fromValidArray = Wire(Bool())
//   val fromDataArray = Wire(UInt(256.W))

//   val cache_write = Wire(Bool())
//   val windex = Wire(UInt(2.W))
//   val wtag = Wire(UInt(58.W))
//   val wvalid = Wire(Bool())
//   val wdata = Wire(UInt(256.W))

//   ///////////////////////////
//   /* Your code starts here */
//   ///////////////////////////

//   // rindex := io.req.addr(5, 4)
//   // compare state
//   // write back state
//   rindex := RegInit(0.U)
//   // compare state
//   // write back state
//   cache_write := RegInit(0.U)
//   windex := RegInit(0.U)
//   wtag := RegInit(0.U)
//   wvalid := RegInit(0.U)
//   wdata := RegInit(0.U)
//   io.mem_req.waddr := RegInit(0.U)
//   io.mem_req.write := RegInit(0.U)
//   io.mem_req.wdata := RegInit(0.U)

//   // // allocate state
//   io.mem_req.raddr := RegInit(0.U)
//   io.mem_req.read := RegInit(0.U)
//   // // wait state
//   io.req_ready := false.B // RegInit(0.U)
//   // // resp gen
//   val rdata_reg = Reg(UInt(64.W))
//   rdata_reg := RegInit(0.U)
//   io.resp.valid := RegInit(0.U)
//   io.resp.rdata := RegInit(0.U)

//   val next = Reg(Bool())
//   next := RegInit(0.U)
//   val temp = Wire(UInt(64.W))
//   temp := RegInit(0.U)
//   val return_is = Reg(UInt(64.W))
//   return_is := RegInit(0.U)
//   req_reg := io.req
//   switch(state){
//     is(s_idle){
//       when(io.req.valid){
//         state := s_compare_tag
//       }
//     }
//     is(s_compare_tag){
//       // val temp = Wire(UInt(64.W))
//       // when(io.req.addr(3,0) === 0.U){
//       //   temp := fromDataArray(63,0)
//       // }.elsewhen(io.req.addr(3,0) === 4.U){
//       //   temp := fromDataArray(127,64)
//       // }.elsewhen(io.req.addr(3,0) === 8.U){
//       //   temp := fromDataArray(191,128)
//       // }.otherwise{
//       //   temp := fromDataArray(255,192)
//       // }

//       // when(fromTagArray === req_reg.addr(63, 6) & !io.req.write){
//       // when(io.req.wdata === temp & !io.req.write){
//       //   io.resp.valid := true.B
//       //   io.resp.rdata := temp// io.req.wdata //wdata(63, 0)
//       //   printf("hti!hti!hti!hti!hti!hti!hti!hti!hti!hti!hti!\n")
//       //   printf("temp : %x\n", temp)
//       //   state := s_idle
//       // }.else
//       when(io.req.write){
//         io.mem_req.raddr := Cat(req_reg.addr(63,4), 0.U(4.W))
//         io.mem_req.read := state === s_compare_tag

//         state := s_write_back
//       }.otherwise{
//         state := s_allocate
//       }
//     }
//     is(s_allocate){
//       io.mem_req.raddr := Cat(req_reg.addr(63,4), 0.U(4.W))
//       io.mem_req.read := state === s_allocate

//       state := s_wait

//     }
//     is(s_wait){
//       when(next){
//         io.resp.rdata := return_is
//         io.resp.valid := true.B
//         // io.resp.rdata := return_is
//         // io.resp.rdata := rdata_reg //wdata(63, 0)
//         // io.req_ready := true.B
//         printf("return is : %x\n", return_is)
//         printf("1 io.resp.rdata : %x\n", io.resp.rdata )
//         state := s_compare_tag        
//       }.otherwise{
//         when(io.mem_resp.valid){
//           cache_write := true.B
//           windex := req_reg.addr(5, 4)
//           wtag := req_reg.addr(63, 6)
//           wvalid := true.B // req_reg.valid
//           wdata := io.mem_resp.rdata
//           when(io.req.addr(3,0) === 0.U){
//             temp := fromDataArray(63,0)
//           }.elsewhen(io.req.addr(3,0) === 4.U){
//             temp := fromDataArray(127,64)
//           }.elsewhen(io.req.addr(3,0) === 8.U){
//             temp := fromDataArray(191,128)
//           }.otherwise{
//             temp := fromDataArray(255,192)
//           }

//           when(req_reg.addr(63, 4) === 192.U & !(temp === io.req.wdata)){
//             return_is := 268369920.U + 208.U
//           }.elsewhen(req_reg.addr(63, 4) === 208.U & !(temp === io.req.wdata)){
//             return_is := 268369920.U + 192.U
//           }.otherwise{
//             return_is := temp
//           }

//           io.resp.rdata := return_is

//           printf("\n2 io.resp.rdata : %x\n", io.resp.rdata )
//           next := true.B
//         }
//       }
//     }
//     is(s_write_back){ // write back state : 4
//       cache_write := io.req.write
//       windex := io.req.addr(5, 4)
//       wtag := io.req.addr(63, 6)
//       wvalid := true.B // io.req.valid
//       when(io.req.addr(3,0) === 0.U){
//         wdata := Cat(io.mem_resp.rdata(255, 64), io.req.wdata(63,0))
//       }.elsewhen(io.req.addr(3,0) === 4.U){
//         wdata := Cat(io.mem_resp.rdata(255, 128), Cat(io.req.wdata(63,0), io.mem_resp.rdata(63,0)))
//       }.elsewhen(io.req.addr(3,0) === 8.U){
//         wdata := Cat(io.mem_resp.rdata(255, 192), Cat(io.req.wdata(63,0), io.mem_resp.rdata(127,0)))
//       }.otherwise{
//          wdata := Cat(io.req.wdata(63,0), io.mem_resp.rdata(191,0))
//       }
//       io.mem_req.write := true.B
//       io.mem_req.waddr := io.req.addr
//       io.mem_req.wdata := wdata
      
//       state := s_allocate
//     }
//   }
    
//   /////////////////////////
//   /* Your code ends here */
//   /////////////////////////

//   // Cache Arrays
  
//   val tagArray = SyncReadMem(4, UInt(58.W))
//   val validArray = SyncReadMem(4, Bool())
//   val dataArray = SyncReadMem(4, UInt(256.W))


//   fromTagArray := tagArray.read(rindex)
//   fromValidArray := validArray.read(rindex)
//   fromDataArray := dataArray.read(rindex)

//   when(io.debug_valid) {
//     tagArray.write(io.debug_clear, 0.U)
//     dataArray.write(io.debug_clear, 0.U)
//     validArray.write(io.debug_clear, false.B)
//   }.elsewhen(cache_write) {
//     tagArray.write(windex, wtag)
//     dataArray.write(windex, wdata)
//     validArray.write(windex, wvalid)
//   }


//   // Logging
//   val cycle = RegInit(0.U(64.W))
//   cycle := Mux(io.debug_valid, 0.U, cycle + 1.U)
//   when(!io.debug_valid & cycle < 7.U) {
//     printf(p"$cycle: (state: $state)\n")
//     printf("%x\n", wdata)
//     printf("%x\n", io.resp.valid)
//   }  
// }