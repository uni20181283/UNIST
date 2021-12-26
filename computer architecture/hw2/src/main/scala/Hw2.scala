/*
 * Homework 2, CSE261 Computer Architecture 
 * 2021 Fall
 * UNIST
 * Hyungon Moon
 */

package Hw2
import chisel3._
import chisel3.util._


object OpCode {
  val addi  = "b0010011".U
  val add   = "b0110011".U
  val sd    = "b0100011".U
  val ld    = "b0000011".U
  val beq   = "b1100011".U
}

object AluOp {
  val ld = "b00".U
  val sd = "b00".U
  val beq = "b01".U
  val reg = "b10".U
}

object AluCtrl {
  val and = "b0000".U(4.W)
  val or  = "b0001".U(4.W)
  val add = "b0010".U(4.W)
  val sub = "b0110".U(4.W)
}

/*
Task 1: ALU
*/
class ALU extends Module {
  val io = IO(new Bundle {
    val ctrl = Input(UInt(4.W))
    val a = Input(UInt(64.W))
    val b = Input(UInt(64.W))
    val res = Output(UInt(64.W))
    val zero = Output(Bool())
  })
  /* Your code starts here*/
  io.res := 0.U
  io.zero := true.B
  switch(io.ctrl) {
    is(0.U) {
      io.res := io.a & io.b
      io.zero := false.B
    }
    is(1.U) {
      io.res := io.a | io.b
      io.zero := false.B
    }
    is(2.U) {
      io.res := io.a + io.b
      io.zero := false.B
    }
    is(6.U) {
      io.res := io.a - io.b
      io.zero := true.B
    }
  }
  /*Your code ends here */
}

/*
Task 2: ImmGen
*/
class ImmGen extends Module {
  val io = IO(new Bundle {
    val insn = Input(UInt(32.W))
    val imm = Output(UInt(64.W))
  })
// 19 / 51 / 35 / 3 / 99
  /* Your code starts here*/
  io.imm := io.insn(6,0)
  val opcode = io.insn(6,0)
  // val immediate_value_1 = io.insn(31,25)
  // val first = io.insn(31,25)
  // val second = io.insn(24,20)
  // val third = io.insn(19,15)
  // val fourth = io.insn(14,12)
  // val fifth = io.insn(11,7)
  switch(opcode){
    is(19.U){ // addi : I-Format
    io.imm := io.insn(31,20)
    }

    is(51.U){ // add : R-type
      io.imm := 2.U
    }

    is(35.U){ //sd : S-type
      // io.imm := io.insn(31)
      // io.imm := FFFFFFFFFFFFFFAA4.U
      io.imm := io.insn(31,25) * 32.U + io.insn(11,7)
      // if(io.insn(31)==0) io.imm := io.insn(31,25) * 32.U + io.insn(11,7)
      // else io.imm := 2.U^64.U - (io.insn(31,25) * 32.U + io.insn(11,7))
    }

    is(3.U){ // ld : I-type
      io.imm := io.insn(31,20)
    }

    is(99.U){ // beq : SB-type
    // io.imm := io.insn(31)
       io.imm := io.insn(31,25) * 8.U + io.insn(11,7) * 2.U
    }
  }
  /*Your code ends here */
}

/*
Task 3: ALUControl
*/
class ALUControl extends Module {
  val io = IO(new Bundle {
    val aluOp = Input(UInt(2.W))
    val funct3 = Input(UInt(3.W))
    val funct7 = Input(UInt(7.W))
    val aluCtrl = Output(UInt(4.W))
  })

  /* Your code starts here*/
  io.aluCtrl := 6.U
  switch(io.aluOp) {
    is(0.U) { // ld, sd
      io.aluCtrl := 2.U
    }
    is(1.U) { // beq
      io.aluCtrl := 6.U
    }
    is(2.U) { // R-type
      switch(io.funct3){
        is(0.U){
          switch(io.funct7){
            is(0.U){
              io.aluCtrl := 2.U
            }
          }
        }
        is(7.U){
          io.aluCtrl := 0.U
        }
        is(6.U){
          io.aluCtrl := 1.U
        }
      }
    }
  }
  /*Your code ends here */
}

/*
Task 4: Control
*/
class Control extends Module {
  val io = IO(new Bundle{
    val in = Input(UInt(7.W))

    val write_reg = Output(Bool())
    val aluSrcFromReg = Output(Bool())
    val memWrite = Output(Bool())
    val memToReg = Output(Bool())
    val aluOp = Output(UInt(2.W))
    val branch = Output(Bool())
  })
/* Your code starts here*/
  io.write_reg := false.B
  io.aluSrcFromReg := false.B
  io.memWrite := false.B
  io.memToReg := false.B
  io.aluOp := 0.U
  io.branch := false.B
  switch(io.in){
    is(19.U){
      io.write_reg := true.B
      io.aluSrcFromReg := false.B
      io.memWrite := false.B
      io.memToReg := false.B
      io.aluOp := 2.U
      io.branch := false.B
    }
    is(51.U){
      io.write_reg := true.B
      io.aluSrcFromReg := true.B
      io.memWrite := false.B
      io.memToReg := false.B
      io.aluOp := 2.U
      io.branch := false.B
    }
    is(35.U){
      io.write_reg := false.B
      io.aluSrcFromReg := false.B
      io.memWrite := true.B
      io.memToReg := false.B
      io.aluOp := 0.U
      io.branch := false.B
    }
    is(3.U){
      io.write_reg := true.B
      io.aluSrcFromReg := false.B
      io.memWrite := false.B
      io.memToReg := true.B
      io.aluOp := 0.U
      io.branch := false.B
    }
    is(99.U){
      io.write_reg := false.B
      io.aluSrcFromReg := false.B
      io.memWrite := false.B
      io.memToReg := false.B
      io.aluOp := 1.U
      io.branch := true.B
    }
  }
  /*Your code ends here */
}