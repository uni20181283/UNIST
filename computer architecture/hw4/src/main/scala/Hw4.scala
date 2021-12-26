
package UniRV64I
import chisel3._
import chisel3.util._
import chisel3.iotesters.PeekPokeTester
import firrtl.FirrtlProtos.Firrtl.Expression.PrimOp.Op
// final commit of mine.

object OpCode {
  val aluImm   = "b0010011".U
  val aluReg   = "b0110011".U
  val store    = "b0100011".U
  val load     = "b0000011".U
  val branch   = "b1100011".U
  val jal      = "b1101111".U
  val jalr     = "b1100111".U
}

class ControlModule extends Module {
  val io = IO(new Bundle{
    val in = Input(UInt(7.W))
    val out = Output(new Control())
  })

  io.out.aluOp := 0.U
  io.out.write_reg := 0.U
  io.out.jal := 0.U
  io.out.link := 0.U
  io.out.indir := 0.U
  io.out.aluSrcFromReg := 0.U
  io.out.memWrite := 0.U
  io.out.memToReg := 0.U
  io.out.branch := 0.U
  io.out.memRead := 0.U
  io.out.stall := false.B
  switch(io.in){
    is(OpCode.aluImm){
      io.out.write_reg := true.B
      io.out.aluSrcFromReg := true.B
      io.out.aluOp := AluOp.reg
      io.out.stall := false.B
    }
    is(OpCode.aluReg){
      io.out.write_reg := true.B
      io.out.aluOp := AluOp.reg
      io.out.stall := false.B
    }
    is(OpCode.store){
      io.out.memWrite := true.B
      io.out.aluSrcFromReg := true.B
      io.out.aluOp :=  AluOp.sd
      io.out.stall := false.B
    }
    is(OpCode.load){
      io.out.aluSrcFromReg := true.B
      io.out.write_reg := true.B
      io.out.memToReg := true.B
      io.out.memRead := true.B
      io.out.aluOp := AluOp.ld
      io.out.stall := false.B
    }
    is(OpCode.branch){
      io.out.branch := true.B
      io.out.aluOp := AluOp.beq
      io.out.stall := true.B
    }
    is(OpCode.jal){
      io.out.jal := true.B
      io.out.link := true.B
      io.out.write_reg := true.B
      // io.out.stall := true.B
    }
    is(OpCode.jalr){
      io.out.indir := true.B
      io.out.link := true.B
      // io.out.stall := true.B
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

  val registers = RegInit(VecInit(Seq.fill(32)(0.U(64.W))))

  io.rs1_out := 0.U
  io.rs2_out := 0.U
  io.rs1_out := 0.U
  io.rs2_out := 0.U
  registers(0) := 0.U
  when(io.write) {
    registers(io.rd) := io.wdata
  }
  io.rs1_out := registers(io.rs1)
  io.rs2_out := registers(io.rs2)

  printf(p"Register content: ${registers}\n")


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
    val imm12 = Output(UInt(12.W))
  })

  io.opcode := io.in(6,0)
  io.rd := io.in(11,7)
  io.funct3 := io.in(14,12)
  io.rs1 := io.in(19,15)
  io.rs2 := io.in(24,20)
  io.funct7 := io.in(31,25)
  io.imm12 := io.in(31,20)


}

object AluOp {
  val ld = "b00".U
  val sd = "b00".U
  val beq = "b01".U
  val reg = "b10".U
}

class ALUControl extends Module {
  val io = IO(new Bundle {
    val aluOp = Input(UInt(2.W))
    val funct3 = Input(UInt(3.W))
    val funct7 = Input(UInt(7.W))
    val aluCtrl = Output(UInt(4.W))
  })

  when(io.aluOp === AluOp.reg){
    // printf("fufufufufufufufufufufufufufufufufufufufufu\n")
    io.aluCtrl := AluCtrl.add
  }.elsewhen(io.aluOp === AluOp.ld){
    io.aluCtrl := AluCtrl.add
  }.elsewhen(io.aluOp === AluOp.sd){
    io.aluCtrl := AluCtrl.add
  }.elsewhen(io.aluOp === AluOp.beq){
    io.aluCtrl := AluCtrl.sub
  }.otherwise{
    io.aluCtrl := 0.U
  }
  
}


class ImmGen extends Module {
  val io = IO(new Bundle {
    val insn = Input(UInt(32.W))
    val imm = Output(UInt(64.W))
  })
  val opcode = io.insn(6,0)
  io.imm := 0.U
  switch(opcode){
    is(OpCode.aluImm){ // addi : I-Format
      val a = Fill(52, io.insn(31))
      io.imm := Cat(a ,io.insn(31,20))
    }
    is(OpCode.aluReg){ // add : R-type
      val b = Fill(52, io.insn(31))
      io.imm := Cat(b, io.insn(24,11))
    }

    is(OpCode.store){ //sd : S-type
      val a = Cat(io.insn(31,25), io.insn(11,7))
      val b = Fill(52, io.insn(31))
      io.imm := Cat(b, a)
    }

    is(OpCode.load){ // ld : I-type
      val a = Fill(52, io.insn(31))
      io.imm := Cat(a ,io.insn(31,20))
    }

    is(OpCode.branch){ // beq : SB-type
      val a = Cat(io.insn(30, 25))
      val b = Cat(io.insn(7), a)
      val c = Cat(b, io.insn(11, 8))
      val e = Fill(53, io.insn(31))
      io.imm := Cat(e, c)
    }
    is(OpCode.jal){ // jal : J-type
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
    is(OpCode.jalr){ // jalr : I-type
      val a = Fill(52, io.insn(31))
      io.imm := Cat(a ,io.insn(31,20))
    }
  }

}


object AluCtrl {
  val and = "b0000".U(4.W)
  val or  = "b0001".U(4.W)
  val add = "b0010".U(4.W)
  val sub = "b0110".U(4.W)
}

class ALU extends Module {
  val io = IO(new Bundle {
    val ctrl = Input(UInt(4.W))
    val a = Input(UInt(64.W))
    val b = Input(UInt(64.W))
    val res = Output(UInt(64.W))
    val zero = Output(Bool())
  })


  io.res := 0.U
  io.zero := 0.U
  switch(io.ctrl){
    is(AluCtrl.and){
      io.res := io.a & io.b
    }
    is(AluCtrl.or){
      io.res := io.a | io.b
    }
    is(AluCtrl.add){
      io.res := io.a + io.b
    }
    is(AluCtrl.sub){
      io.res := io.a - io.b
      when(io.a === io.b){
        io.zero := true.B
      }.otherwise{
        io.zero := false.B
      }
    }
  }
}







class Core extends Module {
  val io = IO(new Bundle {
    //val reset = Input(Bool())
    val imem_addr = Output(UInt(64.W))
    val imem_insn = Input(UInt(32.W))
    val dmem_addr = Output(UInt(64.W))
    val dmem_write = Output(Bool())
    val dmem_read = Output(Bool())
    val dmem_wdata = Output(UInt(64.W))
    val dmem_rdata = Input(UInt(64.W))

    val wb_insn = Output(UInt(32.W))
    val halted = Output(Bool())

  })

  // pipeline registers

  val ifid_reg = Module(new IFIDReg())
  val idex_reg = Module(new IDEXReg())
  val exmem_reg = Module(new EXMEMReg())
  val memwb_reg = Module(new MEMWBReg())

  // combinational modules
  val control = Module(new ControlModule())
  val decoder = Module(new Decoder())
  val immGen = Module(new ImmGen())
  val regfile = Module(new RegFile())
  val aluControl = Module(new ALUControl())
  val alu = Module(new ALU())

  // support for halts
  val halted = RegInit(false.B)
  when(exmem_reg.io.out.debug_insn === "x00005073".U) {
    //printf("halting\n")
    halted := true.B
  }.otherwise {
    halted := halted
  }
  io.halted := halted

  // misc. skeleton
  val next_pc = Wire(UInt(64.W))
  val started = RegInit(true.B)
  started := false.B
  val pc = RegInit(0.U(64.W))
  io.imem_addr := Mux(started, 0.U, pc)
  pc := Mux(started, pc, next_pc)

  /* Your code from here */


  // io.dmem_read := 0.U
  // IF Stage
  ifid_reg.io.stall := Mux(exmem_reg.io.out.control.stall | idex_reg.io.out.control.stall, true.B, false.B)
  when(idex_reg.io.out.control.stall){
    next_pc := pc
  }.otherwise{
    next_pc := Mux(exmem_reg.io.out.control.branch & exmem_reg.io.out.branch_taken, exmem_reg.io.out.branch_target, pc + 4.U)
  }

  // Fetch
  ifid_reg.io.in.pc := pc
  ifid_reg.io.in.insn := io.imem_insn
  ifid_reg.io.in.valid := true.B

  // ID Stage
  decoder.io.in := ifid_reg.io.out.insn
  control.io.in := decoder.io.opcode
  
  immGen.io.insn := ifid_reg.io.out.insn

  regfile.io.rs1 := decoder.io.rs1
  regfile.io.rs2 := decoder.io.rs2
  regfile.io.rd := memwb_reg.io.out.rd
  
  regfile.io.write := memwb_reg.io.out.control.write_reg

  idex_reg.io.in.pc := ifid_reg.io.out.pc
  idex_reg.io.in.rs1_data := regfile.io.rs1_out
  idex_reg.io.in.rs2_data := regfile.io.rs2_out
  idex_reg.io.in.imm64 := immGen.io.imm
  idex_reg.io.in.rd := decoder.io.rd
  idex_reg.io.in.control := control.io.out
  idex_reg.io.in.funct3 := decoder.io.funct3
  idex_reg.io.in.funct7 := decoder.io.funct7
  idex_reg.io.in.debug_insn := ifid_reg.io.out.insn
  idex_reg.io.in.valid := ifid_reg.io.out.valid
//!exmem_reg.io.out.branch_taken &

  // EX
  aluControl.io.funct3 := idex_reg.io.out.funct3
  aluControl.io.funct7 := idex_reg.io.out.funct7
  aluControl.io.aluOp := idex_reg.io.out.control.aluOp
  
  alu.io.ctrl := aluControl.io.aluCtrl
  alu.io.a := idex_reg.io.out.rs1_data
  alu.io.b := Mux(idex_reg.io.out.control.aluSrcFromReg, idex_reg.io.out.imm64, idex_reg.io.out.rs2_data)

  exmem_reg.io.in.branch_target := idex_reg.io.out.pc + (idex_reg.io.out.imm64 << 1.U)
  exmem_reg.io.in.branch_taken := alu.io.zero

  exmem_reg.io.in.alu_res := alu.io.res

  exmem_reg.io.in.rs2_data := idex_reg.io.out.rs2_data
  exmem_reg.io.in.control := idex_reg.io.out.control
  exmem_reg.io.in.pc := idex_reg.io.out.pc
  exmem_reg.io.in.debug_insn := idex_reg.io.out.debug_insn
  exmem_reg.io.in.valid :=  idex_reg.io.out.valid
  exmem_reg.io.in.rd := idex_reg.io.out.rd

  // MEM
  when(exmem_reg.io.out.valid){
    io.dmem_addr := exmem_reg.io.out.alu_res
    io.dmem_write := exmem_reg.io.out.control.memWrite
    io.dmem_read := exmem_reg.io.out.control.memRead
    io.dmem_wdata := exmem_reg.io.out.rs2_data
  }.otherwise{
    io.dmem_addr := 0.U
    io.dmem_write := 0.U
    io.dmem_read := 0.U
    io.dmem_wdata := 0.U
  }
  memwb_reg.io.in.alu_res := exmem_reg.io.out.alu_res
  memwb_reg.io.in.control := exmem_reg.io.out.control
  memwb_reg.io.in.debug_insn := exmem_reg.io.out.debug_insn
  memwb_reg.io.in.pc := exmem_reg.io.out.pc
  when(pc > 8.U){
    memwb_reg.io.in.valid := exmem_reg.io.out.valid
  }.otherwise{
    memwb_reg.io.in.valid := false.B
  }
  memwb_reg.io.in.rd := exmem_reg.io.out.rd

  // WB
  regfile.io.wdata := Mux(memwb_reg.io.out.control.memToReg, io.dmem_rdata, memwb_reg.io.out.alu_res)

  /* Your code to here */
  // Logs and traces
  io.wb_insn := Mux(memwb_reg.io.out.valid, memwb_reg.io.out.debug_insn, 0.U)
  val log_cycles = RegInit(0.U(64.W))
  log_cycles := log_cycles + 1.U
  printf("--------------------------------------------------------------\n")
  printf("--------------------------------------------------------------\n")
  printf("Log @ %d\n", log_cycles)
  printf(p"${ifid_reg.io.out}\n")
  printf(p"${idex_reg.io.out}\n")
  printf(p"${exmem_reg.io.out}\n")
  printf(p"${memwb_reg.io.out}\n")
  printf("--------------------------------------------------------------\n")

}