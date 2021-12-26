package hw4

import scala.collection.immutable.HashMap 
import hw4._

// Final form
package object hw4 {
  type Env = HashMap[Var,LocVal]
}

case class Mem(m: HashMap[LocVal,Val], top: Int) {
  def extended(v: Val): (Mem, LocVal) = {
    val new_mem = Mem(m.updated(LocVal(top),v), top+1)
    (new_mem,LocVal(top))
  }
  def updated(l: LocVal, new_val: Val): Option[Mem] = {
    m.get(l) match {
      case Some(v) => Some(Mem(m.updated(l, new_val), top))
      case None => None
    }
  }
  def get(l: LocVal): Option[Val] = m.get(l)
  def getLocs(): List[LocVal] = m.keySet.toList
}

sealed trait Val
case object SkipVal extends Val
case class IntVal(n: Int) extends Val
case class BoolVal(b: Boolean) extends Val
case class ProcVal(args: List[Var], expr: Expr, env: Env) extends Val
case class LocVal(l: Int) extends Val
sealed trait RecordValLike extends Val
case object EmptyRecordVal extends RecordValLike
case class RecordVal(field: Var, loc: LocVal, next: RecordValLike) extends RecordValLike


sealed trait Program
sealed trait Expr extends Program
case object Skip extends Expr
case object False extends Expr
case object True extends Expr
case class NotExpr(expr: Expr) extends Expr
case class Const(n: Int) extends Expr
case class Var(s: String) extends Expr {
  override def toString = s"Var(${"\""}${s}${"\""})"
}
case class Add(l: Expr, r: Expr) extends Expr
case class Sub(l: Expr, r: Expr) extends Expr
case class Mul(l: Expr, r: Expr) extends Expr
case class Div(l: Expr, r: Expr) extends Expr
case class LTEExpr(l: Expr, r: Expr) extends Expr
case class EQExpr(l: Expr, r: Expr) extends Expr
case class Iszero(c: Expr) extends Expr
case class Ite(c: Expr, t: Expr, f: Expr) extends Expr
case class Let(i: Var, v: Expr, body: Expr) extends Expr
case class Proc(args: List[Var], expr: Expr) extends Expr
case class Asn(v: Var, e: Expr) extends Expr
case class BeginEnd(expr: Expr) extends Expr
case class FieldAccess(record: Expr, field: Var) extends Expr
case class FieldAssign(record: Expr, field: Var, new_val: Expr) extends Expr
case class Block(f: Expr, s: Expr) extends Expr
case class PCallV(ftn: Expr, arg: List[Expr]) extends Expr
case class PCallR(ftn: Expr, arg: List[Var]) extends Expr
case class WhileExpr(cond: Expr, body: Expr) extends Expr
sealed trait RecordLike extends Expr
case object EmptyRecordExpr extends RecordLike
case class RecordExpr(field: Var, initVal: Expr, next: RecordLike) extends RecordLike








object MiniCInterpreter {

  case class Result(v: Val, m: Mem)
  case class UndefinedSemantics(msg: String = "", cause: Throwable = None.orNull) extends Exception("Undefined Semantics: " ++ msg, cause)
    
  
  def eval(env: Env, mem: Mem, expr: Expr): Result = expr match {
    // constants and variables
    case Skip => Result(SkipVal, mem)
    case True => Result(BoolVal(true), mem)
    case False => Result(BoolVal(false), mem)
    case Const(n : Int) => Result(IntVal(n), mem)
    case Var(s : String) => env.contains(Var(s)) match {
      case true => mem.m.contains(env(Var(s))) match{
        case true => Result(mem.m.apply(env(Var(s))), mem)
        case _=> throw new UndefinedSemantics(s"message ${Var}")
      }
      case _=> throw new UndefinedSemantics(s"message ${Var}")
    }
    case Proc(args: List[Var], _expr: Expr) => Result(ProcVal(args, _expr, env), mem)

    // Unary and Binary Operations
    case Add(l: Expr, r: Expr) => (eval(env, mem, l), eval(env, eval(env, mem, l).m, r)) match{
      case (Result(IntVal(n1), mem1), Result(IntVal(n2), mem2)) => Result(IntVal(n1+n2), mem2)
      case _=> throw new UndefinedSemantics(s"message ${Add}")
    }
    case Sub(l: Expr, r: Expr) => (eval(env, mem, l), eval(env, eval(env, mem, l).m, r)) match{
      case (Result(IntVal(n1), mem1), Result(IntVal(n2), mem2)) => Result(IntVal(n1-n2), mem2)
      case _=> throw new UndefinedSemantics(s"message ${Sub}")
    }
    case Mul(l: Expr, r: Expr) => (eval(env, mem, l), eval(env, eval(env, mem, l).m, r)) match{
      case (Result(IntVal(n1), mem1), Result(IntVal(n2), mem2)) => Result(IntVal(n1*n2), mem2)
      case _=> throw new UndefinedSemantics(s"message ${Mul}")
    }
    case Div(l: Expr, r: Expr) => (eval(env, mem, l), eval(env, eval(env, mem, l).m, r)) match{
      case (Result(IntVal(n1), mem1), Result(IntVal(n2), mem2)) => n2 match{
        case 0 => throw new UndefinedSemantics(s"message ${Div}")
        case _ => Result(IntVal(n1/n2), mem2)

      }
      case _=> throw new UndefinedSemantics(s"message ${Div}")
    }
    case LTEExpr(l: Expr, r: Expr) => (eval(env, mem, l), eval(env, eval(env, mem, l).m, r)) match{
      case (Result(IntVal(n1), mem1), Result(IntVal(n2), mem2)) => {
        if(n1 <= n2) Result(BoolVal(true), mem2)
        else Result(BoolVal(false), mem2)
      }
      case _=> throw new UndefinedSemantics(s"message ${LTEExpr}")
    }
    case EQExpr(l: Expr, r: Expr) => (eval(env, mem, l), eval(env, eval(env, mem, l).m, r)) match{
      case (Result(IntVal(n1), mem1), Result(IntVal(n2), mem2)) => {
        if(n1 == n2) Result(BoolVal(true), mem2)
        else Result(BoolVal(false), mem2)
      }
      case (Result(BoolVal(b1), mem1), Result(BoolVal(b2), mem2)) => {
        if(b1 == b2) Result(BoolVal(true), mem2)
        else Result(BoolVal(false), mem2)
      }
      case (Result(SkipVal, mem1), Result(SkipVal, mem2)) => {
        Result(BoolVal(true), mem2)
      }
      case _=> throw new UndefinedSemantics(s"message ${EQExpr}")
    }
    case Iszero(c: Expr) => eval(env, mem, c) match {
      case Result(IntVal(n1), mem1) => n1 match {
        case 0 => Result(BoolVal(true), mem1)
        case _=> throw new UndefinedSemantics(s"message ${Iszero}")
      }
      case _=> throw new UndefinedSemantics(s"message ${Iszero}")
    }
    case NotExpr(_expr: Expr) => eval(env, mem, _expr) match {
      case Result(BoolVal(true), mem1) => Result(BoolVal(false), mem1)
      case Result(BoolVal(false), mem1) => Result(BoolVal(true), mem1)
      case _=> throw new UndefinedSemantics(s"message ${NotExpr}")
    }

    // Flow Control
    case Ite(c: Expr, t: Expr, f: Expr) => eval(env, mem, c) match {
      case Result(BoolVal(true), mem1) => eval(env, mem1, t)
      case Result(BoolVal(false), mem1) => eval(env, mem1, f)
      case _=> throw new UndefinedSemantics(s"message ${Ite}")
    }
    case WhileExpr(cond: Expr, body: Expr) => eval(env, mem, cond) match {
      case Result(BoolVal(false), mem1) => Result(SkipVal, mem1)
      case Result(BoolVal(true), mem0) => eval(env, eval(env, mem0, cond).m, body) match {
        case Result(a : Val, mem1) => eval(env, mem1,WhileExpr(cond, body))
        case _=> throw new UndefinedSemantics(s"message ${WhileExpr}")
      }
      case _=> throw new UndefinedSemantics(s"message ${WhileExpr}")
    }
    case Block(f: Expr, s: Expr) => eval(env, mem, f) match {
      case Result(v1, mem1) => eval(env, mem1, s)
      // eval(env, eval(env, mem, f).m, s)
      case _=> throw new UndefinedSemantics(s"message ${Block}")
    }
    case BeginEnd(_expr: Expr) => {
      val new_eval = eval(env, mem, _expr)
      Result(new_eval.v, new_eval.m)
      
    }

    // Records
    case EmptyRecordExpr => Result(EmptyRecordVal, mem)
    case RecordExpr(field: Var, initVal: Expr, next: RecordLike) => eval(env, mem, initVal) match {
      case Result(v1, mem1) => {
        val extention = mem1.extended(v1)
        val new_mem = extention._1
        val new_locval = extention._2
        eval(env, new_mem, next) match{
          case Result(temp:RecordValLike, mem2) => {
            Result(RecordVal(field, new_locval, temp), mem2)
          }
          case _=> throw new UndefinedSemantics(s"message ${RecordExpr}")
        }
      }
      case _=> throw new UndefinedSemantics(s"message ${RecordExpr}")
    }
    case FieldAccess(record: Expr, field: Var) => eval(env, mem, record) match {
      case Result(r : RecordVal, mem1) => {
        val return_it = find_locval(field, mem1, r)
        Result(mem.m(return_it), mem1)
      }
      case _=> throw new UndefinedSemantics(s"message ${FieldAccess}")
    }
    case FieldAssign(record: Expr, field: Var, new_val: Expr) => eval(env, mem, record) match {
      case Result(r: RecordValLike, mem1) => eval(env, mem1, new_val) match {
        case Result(v : Val, mem2) => {
          val return_it = find_locval(field, mem2, r)
          val return_mem = Mem(mem2.m.updated(return_it, v), mem2.top)          
          Result(v, return_mem)
        }
        case _=> throw new UndefinedSemantics(s"message ${FieldAssign}")
      }
      case _=> throw new UndefinedSemantics(s"message ${FieldAssign}")
    }

    // Assignments
    case Asn(v: Var, e: Expr) => eval(env, mem, e) match {
      case Result(a : Val, mem1) => {
        if(env.contains(v)) {
          val new_mem = Mem(mem1.m.updated(env.apply(v), a), mem1.top)
          Result(a, new_mem)
        }
        else throw new UndefinedSemantics(s"message ${Asn}")
      }
      case _=> throw new UndefinedSemantics(s"message ${Asn}")
    }
    case Let(i: Var, v: Expr, body: Expr) => eval(env, mem, v) match{
      case Result(v1, mem1) => {
        val extention = mem1.extended(v1)
        val new_mem = extention._1
        val new_locval = extention._2
        eval(env.updated(i, new_locval), new_mem, body)
      }
      case _=> throw new UndefinedSemantics(s"message ${Let}")
    }

    // Procedure Calls
    case PCallV(ftn: Expr, arg: List[Expr]) => eval(env, mem, ftn) match {
      case Result(ProcVal(_args: List[Var], _expr: Expr, env_prime: Env), mem0) => {
        if(arg.length == _args.length){
          val given = recursive_PcallV(env, mem0, 0, arg.length - 1, arg, _args)
          val new_env = given._1
          val return_env = env_prime.++(new_env)
          val new_mem = given._2
          eval(return_env, new_mem, _expr)
          // eval(new_env, new_mem, _expr)
        }
        else throw new UndefinedSemantics(s"message ${PCallV}")
      }
      case _=> throw new UndefinedSemantics(s"message ${PCallV}")
    }
    case PCallR(ftn: Expr, arg: List[Var]) => eval(env, mem, ftn) match {
      case Result(ProcVal(_args: List[Var], _expr: Expr, env_prime: Env), mem0) => is_contain(env, arg, arg.length - 1, 0) match {
        case true => {
          if(arg.length == _args.length){
            // val locval = env(arg.apply(0))
            // val new_env = env.updated(_args.apply(0), locval)
            val new_env = recursive_PcallR(env, mem0, 0, arg.length - 1, arg, _args)
            eval(new_env, mem0, _expr)
          }
          else throw new UndefinedSemantics(s"message ${PCallR}")
        }
        case _=> throw new UndefinedSemantics(s"message ${PCallR}")
      }
      case _=> throw new UndefinedSemantics(s"message ${PCallR}")
    }
  }

  def gc(env: Env, mem: Mem): Mem = {
    if(env.isEmpty) Mem(mem.m.empty, mem.top)
    else {
      val env_values = env.values.toList
      val return_it = gc_recursive(mem.m, env_values, new HashMap[LocVal, Val], 0, env_values.length - 1)
      Mem(return_it, mem.top)
    }
  }

  def apply(program: String): (Val, Mem) = {
    val parsed = MiniCParserDriver(program)
    val res = eval(new Env(), Mem(new HashMap[LocVal,Val],0), parsed)
    (res.v, res.m)
  }

  def mem_recursive(check : HashMap[LocVal,Val], input : LocVal, what_return : HashMap[LocVal,Val]) : HashMap[LocVal, Val] = check(input) match {
    case locval : LocVal => {
      val plus_gc = what_return.updated(input, locval)
      mem_recursive(check, input, plus_gc)
    }
    case EmptyRecordVal => what_return.updated(input, check(input))
    case RecordVal(field: Var, loc: LocVal, next: RecordValLike) => next match {
      case EmptyRecordVal => {
        val return_it = what_return.updated(input, check(input))
        return_it.updated(loc, check(loc))
      }
      case RecordVal(_field: Var, _loc: LocVal, _next: RecordValLike) => {
        val return_it = what_return.updated(input, check(input))
        val plus_gc = return_it.updated(loc, check(loc))
        mem_recursive(check, _loc, plus_gc)
      }
    }
    case _ => {
      what_return.updated(input, check(input))
    }
  }

  def gc_recursive(check : HashMap[LocVal,Val], input : List[LocVal], what_return : HashMap[LocVal,Val], count : Int, lenth : Int) : HashMap[LocVal, Val] = {
    if(count == lenth){
      if(check.contains(input(count))) check(input.apply(count)) match {
        case locval : LocVal => {
          mem_recursive(check, locval, what_return.updated(input(count), check(input.apply(count))))
        }
        case RecordVal(field: Var, loc: LocVal, next: RecordValLike) => next match {
          case EmptyRecordVal => {
            val return_it = what_return.updated(input(count), check(input.apply(count)))
            return_it.updated(input(count), loc)
          }
          case RecordVal(_field: Var, _loc: LocVal, _next: RecordValLike) => {
            what_return.updated(input(count), check(input.apply(count)))
            val plus_gc = what_return.updated(input(count), loc)
            mem_recursive(check, _loc, plus_gc)
          }
        }
        case _=> what_return.updated(input(count), check(input.apply(count)))
      }
      else{
        throw new UndefinedSemantics(s"message ${gc_recursive _}")
      }
    }
    else{
      if(check.contains(input(count))) check(input.apply(count)) match {
        case locval : LocVal => {
          val plus_gc = mem_recursive(check, locval, what_return.updated(input(count), check(input.apply(count))))
          gc_recursive(check, input, plus_gc, count + 1, lenth)
        }
        case RecordVal(field: Var, loc: LocVal, next: RecordValLike) => next match {
          case EmptyRecordVal => {
            val plus_gc = what_return.updated(input(count), loc)
            gc_recursive(check, input, plus_gc, count + 1, lenth)
          }
          case RecordVal(_field: Var, _loc: LocVal, _next: RecordValLike) => {
            val plus_gc = what_return.updated(input(count), loc)
            val recursived_mem = mem_recursive(check, _loc, plus_gc)
            gc_recursive(check, input, recursived_mem, count + 1, lenth)
          }
        }
        case _=> {
          val plus_gc = what_return.updated(input(count), check(input.apply(count)))
          gc_recursive(check, input, plus_gc, count + 1, lenth)
        }
      }
      else{
        throw new UndefinedSemantics(s"message ${gc_recursive _}")
      }
    }
  }

  def is_contain(env : Env, arg : List[Var], legth : Int, count : Int) : Boolean = {
    if(count == legth){
      if(env.contains(arg.apply(count))) true
      else false      
    }
    else{
      if(env.contains(arg.apply(count))) is_contain(env, arg, legth, count + 1)
      else false
    }
  }

  def recursive_PcallV(env: Env, mem: Mem, count : Int, legth : Int, arg: List[Expr], _args: List[Var]):(Env, Mem) = {
    if(count == legth) {
      val value_is = eval(env, mem, arg.apply(count)).v
      val new_one = mem.extended(value_is)
      val new_env = env.updated(_args.apply(count), new_one._2)
      val new_mem = new_one._1
      (new_env, new_mem)
    }
    else if(count < legth) {
      val value_is = eval(env, mem, arg.apply(count)).v
      val new_one = mem.extended(value_is)
      val new_env = env.updated(_args.apply(count), new_one._2)
      val new_mem = new_one._1
      recursive_PcallV(new_env, new_mem, count + 1, legth, arg, _args)
    }
    else throw new UndefinedSemantics(s"message ${recursive_PcallV _}")
  }
  
  def find_locval(field : Var, mem : Mem, next : RecordValLike) : LocVal = next match {
    case EmptyRecordVal =>  throw new UndefinedSemantics(s"message ${find_locval _}")
    case RecordVal(what_need_var: Var, what_return : LocVal, next_next : RecordValLike) => {
      if(field == what_need_var) what_return
      else find_locval(field, mem, next_next)
    }
    case _=> throw new UndefinedSemantics(s"message ${find_locval _}")
  }

  def recursive_PcallR(env: Env, mem: Mem, count : Int, legth : Int, arg: List[Var], _args: List[Var]):Env = {
    if(count == legth) {
      val locval = env(arg.apply(count))
      val new_env = env.updated(_args.apply(count), locval)
      new_env
    }
    else if(count < legth){
      val locval = env(arg.apply(count))
      val new_env = env.updated(_args.apply(count), locval)
      recursive_PcallR(new_env, mem, count + 1, legth, arg, _args)
    }
    else throw new UndefinedSemantics(s"message ${recursive_PcallR _}")
  }

}


object Hw4App extends App {
  
  println("Hello from Hw4!")

}