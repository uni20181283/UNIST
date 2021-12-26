
package Hw3
import chisel3._
import chisel3.util._
import chisel3.iotesters.PeekPokeTester
import firrtl.FirrtlProtos.Firrtl.Expression.PrimOp.Op
// The final version of HW4

object OpCode {
  val aluImm   = "b0010011".U
  val aluReg   = "b0110011".U
  val store    = "b0100011".U
  val load     = "b0000011".U
  val branch   = "b1100011".U
  val jal      = "b1101111".U
  val jalr     = "b1100111".U
}

class Control extends Module {    // 완료
  val io = IO(new Bundle{
    val in = Input(UInt(7.W))
    val write_reg = Output(Bool())
    val aluSrcFromReg = Output(Bool())
    val memWrite = Output(Bool())
    val memRead = Output(Bool())
    val memToReg = Output(Bool())
    val aluOp = Output(UInt(2.W))
    val branch = Output(Bool())
    val link = Output(Bool())
    val jal = Output(Bool())
    val indir = Output(Bool())
  })
  io.aluOp := false.B
  io.write_reg := false.B
  io.jal := false.B
  io.link := false.B
  io.indir := false.B
  io.aluSrcFromReg := false.B
  io.memWrite := false.B
  io.memToReg := false.B
  io.branch := false.B
  io.memRead := false.B
  switch(io.in){
    is(OpCode.aluImm){
      io.write_reg := true.B
      io.aluSrcFromReg := true.B
      io.aluOp := "b11".U
    }
    is(OpCode.aluReg){
      io.write_reg := true.B
      io.aluOp := "b11".U
    }
    is(OpCode.store){
      io.memWrite := true.B
      io.aluSrcFromReg := true.B
      io.aluOp :=  AluOp.sd
    }
    is(OpCode.load){ // ld
      io.aluSrcFromReg := true.B
      io.write_reg := true.B
      io.memToReg := true.B
      io.memRead := true.B
      // io.memWrite := true.B
      io.aluOp := AluOp.ld
    }
    is(OpCode.branch){
      io.branch := true.B
      io.aluOp := AluOp.beq
    }
    is(OpCode.jal){   // 확인 필요 //  jal x1, foo -> jal ra, 10 <foo>
      io.jal := true.B
      io.link := true.B
      io.write_reg := true.B
    }
    is(OpCode.jalr){   // 확인 필요 // jalr x0, 0(x1) -> ret
      io.indir := true.B
      io.link := true.B      
    }
  }
}


class RegFile extends Module {
  val io = IO(new Bundle {
    val rd = Input(UInt(5.W))
    val rs1 = Input(UInt(5.W))
    val rs2 = Input(UInt(5.W))
    val write = Input(Bool())
    val wdata = Input(UInt(64.W))
    val rs1_out= Output(UInt(64.W))
    val rs2_out = Output(UInt(64.W))
  })

  /* Your code starts here */
  val registers = RegInit(VecInit(Seq.fill(32)(0.U(64.W))))
  registers(0) := 0.U
  when(io.write) {
    registers(io.rd) := io.wdata
  }
  io.rs1_out := registers(io.rs1)
  io.rs2_out := registers(io.rs2)
  /* Your code ends here */
}



class Decoder extends Module {
  val io = IO(new Bundle {
    val in = Input(UInt(32.W))
    val opcode = Output(UInt(7.W))
    val rd = Output(UInt(5.W))
    val funct3 = Output(UInt(3.W))
    val funct7 = Output(UInt(7.W))
    val rs1 = Output(UInt(5.W))
    val rs2 = Output(UInt(5.W))
  })
  /* Your code starts here */
 
  io.opcode := io.in(6,0)
  io.rd := io.in(11,7)
  io.funct3 := io.in(14,12)
  io.rs1 := io.in(19,15)
  io.rs2 := io.in(24,20)
  io.funct7 := io.in(31,25)
   /* Your code ends here */


}

object AluOp {
  val ld = "b00".U
  val sd = "b01".U
  val beq = "b10".U
  val reg = "b11".U
}

class ALUControl extends Module { // 일단 완료
  val io = IO(new Bundle {
    val aluOp = Input(UInt(2.W))
    val funct3 = Input(UInt(3.W))
    val funct7 = Input(UInt(7.W))
    val aluCtrl = Output(UInt(4.W))
  })

  /* Your code starts here */
  io.aluCtrl := "b0010".U
  switch(io.aluOp){
    is("b00".U){  // ld
     io.aluCtrl := "b0010".U
    }
    is("b01".U){  // sd
      io.aluCtrl := "b0010".U
    }
    is("b10".U){  // beq
      io.aluCtrl := "b0110".U
    }
    is("b11".U){ // 산술 연산 할 때 쓰는 듯? regsister option
      io.aluCtrl := "b0010".U
    }
  }
  /* Your code ends here */

}


class ImmGen extends Module {
  val io = IO(new Bundle {
    val insn = Input(UInt(32.W))
    val imm = Output(UInt(64.W))
  })

  /* Your code starts here */
  val opcode = io.insn(6,0)
  io.imm := 292.U
  switch(opcode){
    is("b0010011".U){ // addi : I-Format
      val a = Fill(52, io.insn(31))
      io.imm := Cat(a ,io.insn(31,20))
    }
    is("b0110011".U){ // add : R-type
      val b = Fill(52, io.insn(31))
      io.imm := Cat(b, io.insn(24,11))
    }

    is("b0100011".U){ //sd : S-type
      val a = Cat(io.insn(31,25), io.insn(11,7))
      val b = Fill(52, io.insn(31))
      io.imm := Cat(b, a)
    }

    is("b0000011".U){ // ld : I-type
      val a = Fill(52, io.insn(31))
      io.imm := Cat(a ,io.insn(31,20))
    }

    is("b1100011".U){ // beq : SB-type
      val a = Cat(io.insn(30, 25))
      val b = Cat(io.insn(7), a)
      val c = Cat(b, io.insn(11, 8))
      val e = Fill(53, io.insn(31))
      io.imm := Cat(e, c)
    }
    is("b1101111".U){ // jal : J-type
      val a = io.insn(31)
      val b = io.insn(19,12)
      val c = io.insn(20)
      val d = io.insn(30,21)
      val fill = Fill(43, io.insn(31))
      val f_a = Cat(fill,a)
      val f_b = Cat(f_a , b)
      val f_c = Cat(f_b, c)
      val f_d = Cat(f_c, d)
      io.imm := Cat(f_d, 0.U)
    }
    is("b1100111".U){ // jalr : I-type
      val a = Fill(52, io.insn(31))
      io.imm := Cat(a ,io.insn(31,20))
      // printf("          jalr imm is : %x\n", io.imm)
    }
  }
  /* Your code ends here */

}


object AluCtrl {
  val and = "b0000".U(4.W)
  val or  = "b0001".U(4.W)
  val add = "b0010".U(4.W)
  val sub = "b0110".U(4.W)
}

class ALU extends Module {  // 일단 완료
  val io = IO(new Bundle {
    val ctrl = Input(UInt(4.W))
    val a = Input(UInt(64.W))
    val b = Input(UInt(64.W))
    val res = Output(UInt(64.W))
    val zero = Output(Bool())
  })

  /* Your code starts here */
  io.res := 0.U
  io.zero := false.B
  switch(io.ctrl){
    is("b0000".U){
      io.res := io.a & io.b
    }
    is("b0001".U){
      io.res := io.a | io.b
    }
    is("b0010".U){
      io.res := io.a + io.b
    }
    is("b0110".U){
      io.res := io.a - io.b
      when(io.a === io.b){
        io.zero := true.B
      }.otherwise{
        io.zero := false.B
      }
    }
  }
  /* Your code ends here */
}



class PCGen extends Module {
  val io = IO(new Bundle{
    val this_pc = Input(UInt(64.W))
    val branch = Input(Bool())
    val jal = Input(Bool())
    val indir = Input(Bool())
    val zero = Input(Bool())
    val rs1  = Input(UInt(64.W))
    val next_pc = Output(UInt(64.W))
    val imm64 = Input(UInt(64.W)) // from immgen
  })

  /* Your code starts here */
  when(io.jal){
    io.next_pc := io.imm64 + io.this_pc
  }.elsewhen(io.indir){
    io.next_pc := io.rs1 + io.imm64
  }.otherwise{
    val left = io.imm64 << 1.U
    val before_mux = io.this_pc + left
    val is_true = io.branch & io.zero // &일까 |일까?
    io.next_pc :=  Mux(is_true, before_mux, io.this_pc + 4.U)
  }
  /* Your code ends here */
  

}


class Core extends Module   {
  val io = IO(new Bundle {
    //val reset = Input(Bool())
    val imem_addr = Output(UInt(64.W))
    val imem_insn = Input(UInt(32.W))
    val dmem_addr = Output(UInt(64.W))
    val dmem_write = Output(Bool())
    val dmem_read = Output(Bool())
    val dmem_wdata = Output(UInt(64.W))
    val dmem_rdata = Input(UInt(64.W))

    
    val halted = Output(Bool())

  })

  /* Support for halt, don't touch start */
  val started = RegInit(false.B)
  started := true.B
  val halted = RegInit(false.B)
  when(io.imem_insn === "x00005073".U) {
    //printf("halting\n")
    halted := true.B
  }.otherwise {
    halted := halted
  }
  io.halted := halted
  /* Support for halt, don't touch end */

  /* Modules, don't touch start */

  val pc = RegInit(0.U(64.W))
  val control = Module(new Control())
  val pcGen = Module(new PCGen())
  val decoder = Module(new Decoder())
  val aluControl = Module(new ALUControl())
  val regfile = Module(new RegFile())
  val alu = Module(new ALU())
  val immGen = Module(new ImmGen())
  pc := Mux(started, pcGen.io.next_pc, pc)

  /* Modules, don't touch end */

  /* Your code starts here */
  //io.dmem_read := false.B

  pcGen.io.branch := control.io.branch  // Ok
  pcGen.io.this_pc := pc // Ok
  pcGen.io.indir := control.io.indir  // Ok

  // Fetch
  io.imem_addr := pc  // Ok

  // Deocder
  decoder.io.in := io.imem_insn // 무조건 맞음

  control.io.in := decoder.io.opcode  // 무조건 맞음
  
  aluControl.io.funct3 := decoder.io.funct3 // 무조건 맞음

  aluControl.io.funct7 := decoder.io.funct7 // 무조건 맞음

  aluControl.io.aluOp := control.io.aluOp // 무조건 맞음


  immGen.io.insn := io.imem_insn  // 무조건 맞음

  pcGen.io.imm64 := immGen.io.imm // 이건 무조건 맞음

  regfile.io.rs1 := decoder.io.rs1  // 무조건 맞음

  regfile.io.rs2 := decoder.io.rs2  // 무조건 맞음

  regfile.io.rd := decoder.io.rd
  regfile.io.write := control.io.write_reg  // 무조건 맞음

  // EX
  alu.io.ctrl := aluControl.io.aluCtrl // 무조건 맞음

  alu.io.a := regfile.io.rs1_out // 무조건 맞음

  alu.io.b := Mux(control.io.aluSrcFromReg, immGen.io.imm, regfile.io.rs2_out) // 무조건 맞음


  pcGen.io.zero := alu.io.zero  // 무조건 맞음

  pcGen.io.rs1 := regfile.io.rs1_out // ??????? //jal && jalr
  pcGen.io.jal := control.io.jal // 무조건 맞음 //jal && jalr


  // MEM
  io.dmem_addr := alu.io.res // Ok

  io.dmem_write := control.io.memWrite  // Ok
  io.dmem_read := control.io.memRead  // Ok

  io.dmem_wdata := regfile.io.rs2_out

  // WB
  regfile.io.wdata := Mux(control.io.memToReg, io.dmem_rdata, alu.io.res)
  when(control.io.link){
    regfile.io.wdata := pc + 4.U
  }

  /* Your code endshere */

  // Logs for debugging, freely modify
  // printf("(pc: %x, instruction: %x, next_pc: %x, wdata: %x, write: %x)\n",
  // pc, io.imem_insn, pcGen.io.next_pc, regfile.io.wdata, regfile.io.write)
  // printf("data memory dmem_addr : %x, dmem_wdata : %x, dmem_rdata : %x\n",
  // io.dmem_addr, io.dmem_wdata, io.dmem_rdata)
  // printf("alu.a : %x, alu.b : %x, alu.ctrl : %x, alu.result : %x",
  // alu.io.a, alu.io.b, alu.io.ctrl, alu.io.res)
  // printf("\n")

}