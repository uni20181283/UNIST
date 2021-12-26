package hw3

import scala.collection.immutable.HashMap 
import hw3._


package object hw3 {
  type Env = HashMap[Var,Val]
  type Loc = Int
  
}

case class Mem(m: HashMap[Loc,Val], top: Loc) {
  def exists(v: Loc): Boolean = 
    m.exists((a: (Loc, Val)) => a._1 == v)
  def apply(variable: Loc): Val = m(variable)
}

sealed trait Val
case class IntVal(n: Int) extends Val
case class IntListVal(n: List[IntVal]) extends Val
case class BoolVal(b: Boolean) extends Val
case class ProcVal(v: Var, expr: Expr, env: Env) extends Val
case class RecProcVal(fv: Var, av: Var, body: Expr, env: Env) extends Val
case class LocVal(l: Loc) extends Val


sealed trait Program
sealed trait Expr extends Program
case class ConstI(n: Int) extends Expr
case class ConstB(n: Boolean) extends Expr
case class ConstIL(n: List[IntVal]) extends Expr
case class Var(s: String) extends Expr
case class Add(l: Expr, r: Expr) extends Expr
case class Sub(l: Expr, r: Expr) extends Expr
case class Mul(l: Expr, r: Expr) extends Expr
case class Div(l: Expr, r: Expr) extends Expr
case class Rem(l: Expr, r: Expr) extends Expr
case class Cons(l: Expr, r: Expr) extends Expr
case class GTExpr(l: Expr, r: Expr) extends Expr
case class GEQExpr(l: Expr, r: Expr) extends Expr
case class Iszero(c: Expr) extends Expr
case class Ite(c: Expr, t: Expr, f: Expr) extends Expr
case class ValExpr(name: Var, value: Expr, body: Expr) extends Expr
case class VarExpr(name: Var, value: Expr, body: Expr) extends Expr
case class Proc(v: Var, expr: Expr) extends Expr
case class DefExpr(fname: Var, aname: Var, fbody: Expr, ibody: Expr) extends Expr
case class Asn(v: Var, e: Expr) extends Expr
case class Paren(expr: Expr) extends Expr
case class Block(f: Expr, s: Expr) extends Expr
case class PCall(ftn: Expr, arg: Expr) extends Expr







object MiniScalaInterpreter {

  case class Result(v: Val, m: Mem)
  case class UndefinedSemantics(msg: String = "", cause: Throwable = None.orNull) extends Exception("Undefined Semantics: " ++ msg, cause)
  
  
  def eval(env: Env, mem: Mem, expr: Expr): Result = expr match {
    case ConstI(n: Int) => Result(IntVal(n), mem)
    case ConstB(n: Boolean) => Result(BoolVal(n), mem)
    case ConstIL(n: List[IntVal]) => Result(IntListVal(n), mem)
    case Var(s: String) =>{
      if(env.exists((a: (Var,Val)) => a._1 == Var(s) )) env(Var(s)) match {
        case LocVal(b) => {
          println(mem.apply(b))
          if(mem.exists(b)) Result(mem.apply(b), mem)
          else throw new Exception("Undefined Semantics: NO MEM")
        }
        case _=> {
          Result(env(Var(s)), mem)
        }
      }
      else {
        throw new Exception("Undefined Semantics: NO ENV")
      }
    }
    case Cons(l: Expr, r: Expr) => (eval(env, mem, l).v, eval(env, mem, r).v) match {
      case (IntVal(x), IntListVal(List())) => Result(IntVal(x), mem)
      case (IntVal(x), IntVal(y)) => Result(IntListVal(List(IntVal(x), IntVal(y))), mem)
      case (IntVal(x), IntListVal(y))  => Result(eval(env, mem, ConstIL(IntVal(x) :: y)).v, mem)
      case _=> {
        throw new Exception("Undefined Semantics: NO TYPE IN CONS")
      }
    }
    case Add(l: Expr, r: Expr) => (eval(env, mem, l), eval(env, eval(env, mem, l).m, r)) match{
      case (Result(x: IntVal, a), Result(y: IntVal, b)) => eval(env, b, ConstI(x.n + y.n))
      case _=> throw new Exception("Undefined Semantics: NO INTEGER IN ADD")
    }
    case Sub(l: Expr, r: Expr) => (eval(env, mem, l), eval(env, eval(env, mem, l).m, r)) match{
      case (Result(x: IntVal, a), Result(y: IntVal, b)) => eval(env, b, ConstI(x.n - y.n))
      case _=> throw new Exception("Undefined Semantics: NO INTEGER IN SUB")
    }
    case Mul(l: Expr, r: Expr) => (eval(env, mem, l), eval(env, eval(env, mem, l).m, r)) match{
      case (Result(x: IntVal, a), Result(y: IntVal, b)) => eval(env, b, ConstI(x.n * y.n))
      case _=> throw new Exception("Undefined Semantics: NO INTEGER IN MUL")
    }
    case Div(l: Expr, r: Expr) => (eval(env, mem, l), eval(env, eval(env, mem, l).m, r)) match{
      case (Result(x: IntVal, a), Result(y: IntVal, b)) => y.n match {
        case 0 => throw new Exception("Undefined Semantics:NO INTEGER IN DIV")
        case _=> eval(env, b, ConstI(x.n / y.n))
      }
      case _=> throw new Exception("Undefined Semantics:")
    }
    case Rem(l: Expr, r: Expr) => (eval(env, mem, l), eval(env, eval(env, mem, l).m, r)) match{
      case (Result(x: IntVal, a), Result(y: IntVal, b)) => y.n match {
        case 0 => throw new Exception("Undefined Semantics: CANT DIVIDE")
        case _=>  eval(env, b, ConstI(x.n % y.n))
      }
      case _=> throw new Exception("Undefined Semantics: NO INTEGER IN REM")
    }
    case GTExpr(l: Expr, r: Expr) => (eval(env, mem, l), eval(env, eval(env, mem, l).m, r)) match {
      case (Result(x:IntVal, a), Result(y:IntVal, b)) => if(x.n > y.n) Result(BoolVal(true), b)
            else Result(BoolVal(false), b)
      case _=> throw new Exception("Undefined Semantics: NO INTEGER IN GTEXPR")
    }
    case GEQExpr(l: Expr, r: Expr) => (eval(env, mem, l), eval(env, eval(env, mem, l).m, r)) match {
      case (Result(x:IntVal, a), Result(y:IntVal, b)) => if(x.n >= y.n) Result(BoolVal(true), b)
            else Result(BoolVal(false), b)
      case _=> throw new Exception("Undefined Semantics: NO INTEGER IN GEQEXPR")
    }
    case Iszero(c: Expr) => eval(env, mem, c) match {
      case Result(x:IntVal, new_mem) => if(x.n == 0 ){
          Result(BoolVal(true), new_mem)
      }
        else Result(BoolVal(false), new_mem)
      case _=> throw new Exception("Undefined Semantics: NO INTEGER IN ISZERO")
    }
    case Ite(c: Expr, t: Expr, f: Expr) => eval(env, mem, c) match {
      case Result(x:BoolVal, new_mem) => {
        if(x.b) eval(env, new_mem, t)
        else eval(env, new_mem, f)
      }
      case _=> throw new Exception("Undefined Semantics: GIVEN IS NOT BOOLBAL")
    }
    case ValExpr(name: Var, value: Expr, body: Expr) => {
      if(env.exists((a: (Var,Val)) => a._1 == name )){
        eval(env, mem, body)
      }
      else{
        val new_env = env + (name -> eval(env, mem, value).v)
        eval(new_env, eval(env, mem, value).m, body)
      }
    }
    case VarExpr(name: Var, value: Expr, body: Expr) => {
      val new_val = eval(env, mem, value).v
      val new_top = mem.top + 1
      val locval = LocVal(new_top)
      val new_env = env + (name -> locval)
      eval(new_env, Mem(HashMap(new_top -> new_val), new_top), body)
    }
    case Proc(v: Var, expr: Expr) => {
      Result(ProcVal(v, expr, env), mem)
    }
    case Paren(expr: Expr) => eval(env, mem, expr)

    case PCall(ftn: Expr, arg: Expr) => eval(env, mem, ftn).v match {
      case ProcVal(v: Var, expr: Expr, env_env: Env) => {
        val new_mem = eval(env, eval(env, mem, ftn).m, arg).m
        val new_env = env_env + (v -> eval(env, eval(env, mem, ftn).m, arg).v)
        eval(new_env, new_mem, expr)
      }
      case RecProcVal(fv: Var, av: Var, body: Expr, env_prime: Env) => {
        val new_env = env + (av -> eval(env, mem, arg).v)
        eval(new_env, mem, body)
      }
      case _=> throw new Exception("Undefined Semantics: NO SEMENTIC")
    }
    case Block(f:Expr, s:Expr)=>{
      val new_mem = eval(env, mem, f).m
      Result(eval(env, new_mem, f).v, eval(env, new_mem, f).m)
    }
    case Asn(v:Var, e:Expr) => {
      if(env.exists((a: (Var,Val)) => a._1 == v )) env(v) match {
        case LocVal(x) => {
          val new_env =  (v -> LocVal(mem.top + 1))
          val new_mem = Mem(HashMap(mem.top + 1 -> eval(env, mem, e).v), mem.top + 1)
          Result(eval(env, mem, e).v, new_mem)
        }
        case _=> throw new Exception("Undefined Semantics: NO MEMORY")
      }
      else{
        throw new Exception("Undefined Semantics: NO ENV")
      }

    }
    case DefExpr(fname: Var, aname: Var, fbody: Expr, ibody: Expr) => {
        val new_env = env + (fname -> RecProcVal(fname, aname, fbody, env))
        eval(new_env, mem, ibody)
    }
  }
  
  def apply(program: String): Val = {
    val parsed = MiniScalaParserDriver(program)
    eval(new Env(), Mem(new HashMap[Loc,Val],0), parsed).v
  }

}


object Hw3App extends App {
  
  println("Hello from Hw3!")

}