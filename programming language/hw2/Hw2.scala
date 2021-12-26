package Hw2

import fastparse._
import MultiLineWhitespace._
import scala.collection.immutable.HashMap 

sealed trait Val
case class IntVal(n: Int) extends Val
case class BoolVal(b: Boolean) extends Val
case class ProcVal(v: Var, expr: Expr, env: Env) extends Val
case class RecProcVal(fv: Var, av: Var, body: Expr, expr: Expr, env: Env) extends Val

case class Env(hashmap: HashMap[Var,Val]) {
  def apply(variable: Var): Val = hashmap(variable)
  def exists(v: Var): Boolean = 
    hashmap.exists((a: (Var, Val)) => a._1 == v)
  def add(v: Var, value: Val) = Env(hashmap + (v -> value))
  
}

sealed trait Program
sealed trait Expr extends Program
case class Const(n: Int) extends Expr
case class Var(s: String) extends Expr
case class Add(l: Expr, r: Expr) extends Expr
case class Sub(l: Expr, r: Expr) extends Expr
case class Equal(l: Expr, r: Expr) extends Expr
case class Iszero(c: Expr) extends Expr
case class Ite(c: Expr, t: Expr, f: Expr) extends Expr
case class Let(name: Var, value: Expr, body: Expr) extends Expr
case class Paren(expr: Expr) extends Expr
case class Proc(v: Var, expr: Expr) extends Expr
case class PCall(ftn: Expr, arg: Expr) extends Expr
case class LetRec(fname: Var, aname: Var, fbody: Expr, ibody: Expr) extends Expr

sealed trait IntExpr
case class IntConst(n: Int) extends IntExpr
case object IntVar extends IntExpr
case class IntAdd(l: IntExpr, r: IntExpr) extends IntExpr
case class IntSub(l: IntExpr, r: IntExpr) extends IntExpr
case class IntMul(l: IntExpr, r: IntExpr) extends IntExpr
case class IntSigma(f: IntExpr, t: IntExpr, b: IntExpr) extends IntExpr
case class IntPi(f: IntExpr, t: IntExpr, b: IntExpr) extends IntExpr
case class IntPow(b: IntExpr, e: IntExpr) extends IntExpr



package object Hw2 {

  

}

object IntInterpreter {
  def evalInt(expr: IntExpr, env: Option[Int]): Int = expr match{
    case IntConst(n) => n
    case IntVar => env match {
      case None => 0
      case Some(k) => k
    }
    case IntAdd(a, b) => (evalInt(a, env), evalInt(b, env)) match {
      case (x : Int, y: Int) => x+y
      case _=> throw new Exception("Type error")
    }
    case IntSub(a, b) => (evalInt(a, env), evalInt(b, env)) match {
      case (x : Int, y: Int) => x-y
      case _=> throw new Exception("Type error")
    }
    case IntMul(a, b) => (evalInt(a, env), evalInt(b, env)) match {
      case (x : Int, y: Int) => x*y
      case _=> throw new Exception("Type error")
    }
    case IntPow(a, b) => b match {
      case IntConst(0) => evalInt(IntConst(1), env)
      case IntConst(1) => evalInt(a, env)
      case IntConst(2) => evalInt(IntMul(a, IntPow(a, IntConst(1))), env)
      case IntConst(3) => evalInt(IntMul(a, IntPow(a, IntConst(2))), env)
      case _=> evalInt(IntMul(a, IntPow(a, IntConst(evalInt(IntSub(b, IntConst(1)), env)))), env)
   }
    case IntSigma(a, b, c) => c match {
      case IntConst(_) => evalInt(IntSub(b, a), env) match {
        case 0 => evalInt(c, env)
        case _ => evalInt(IntAdd(c, IntSigma(IntConst(evalInt(IntAdd(a, IntConst(1)), env)), b, c)), env)
      }
      case _ => evalInt(IntSub(b, a), env) match{
        case 0 => evalInt(c, Some(evalInt(a, env)))
        case _ =>evalInt(IntAdd(c, IntSigma(IntConst(evalInt(IntAdd(a, IntConst(1)), Some(evalInt(a, env)))), b, c)), Some(evalInt(a, env)))
      }
    }
   case IntPi(a, b, c) => c match {
      case IntConst(_) => evalInt(IntSub(b, a), env) match {
        case 0 => evalInt(c, env)
        case _ => evalInt(IntMul(c, IntPi(IntConst(evalInt(IntAdd(a, IntConst(1)), env)), b, c)), env)
      }
      case IntVar => evalInt(IntSub(b, a), env) match {
        case 0 => evalInt(a, env)
        case _ => evalInt(IntMul(a, IntPi(IntConst(evalInt(IntAdd(a, IntConst(1)), env)), b, c)), env)
      }
      case _ => evalInt(IntSub(b, a), env) match {
        case 0 => evalInt(c, Some(evalInt(a, env)))
        case _ => evalInt(IntMul(c, IntPi(IntConst(evalInt(IntAdd(a, IntConst(1)), Some(evalInt(a, env)))), b, c)), Some(evalInt(a, env)))
      }
    }
  }
  def apply(s: String): Int = {
    val parsed = IntParser(s)
    evalInt(parsed, None)
  }
}

object LetRecInterpreter {
  def eval(env: Env, expr: Expr): Val = expr match {//BoolVal(false)
    case Const(n) => IntVal(n)
    case Var(s) => {
      if(env.exists(Var(s))) env(Var(s))
      else throw new Exception("1")
    }
    case Add(a,b) => (eval(env,a), eval(env,b)) match {
      case (x: IntVal, y: IntVal) => IntVal(x.n + y.n)
      case _ => throw new Exception("Type Error")
    }
    case Sub(a,b) => (eval(env,a), eval(env,b)) match {
      case (x: IntVal, y: IntVal) => IntVal(x.n - y.n)
      case _ => throw new Exception("Type Error")
      }
    case Equal(a, b) => (eval(env, a), eval(env, b)) match {
      case (x: IntVal, y: IntVal) => IntVal(x.n - y.n) match {
        case IntVal(0) => BoolVal(true)
        case _ => BoolVal(false)
      }
      case _ => throw new Exception("Not inclues this cases from bb")
      // case (x: BoolVal, y, BoolVal) => if(x==BoolVal(true) && y==BoolVal(true)) BoolVal(true) else BoolVal(false)
    }
    // case Iszero(a) => a match {
    //   case Const(0) => BoolVal(true) 
    //   case _ => BoolVal(false)
    // }
    case Iszero(c) => eval(env,c) match {
      case (x: IntVal) => BoolVal(x.n == 0)
      case _ => BoolVal(false)
    }
    case Ite(c, t, f) => eval(env,c) match {
      case v: BoolVal => if (v.b) eval(env,t) else eval(env,f)
      case _ => throw new Exception("Type Error")
    } 
    case Let(name, value, body) => {
      val new_env = env.add(name, eval(env, value))
      eval(new_env, body)
    }
    case Paren(a) => eval(env,a)
    case Proc(a, b) => ProcVal(a, b, env)
    case PCall(a, b) => a match {
      case Proc(x, y) => eval(env, Let(x, b, y))
      case LetRec(x:Var, y:Var, z:Expr, w:Expr) => {
        val new_env = env.add(y, eval(env, b))
        eval(new_env, z)
      }
      case _=> eval(env, b)
    }
    case LetRec(a, b, c, d) => RecProcVal(a, b, c, d, env)
  }
  
  def apply(program: String): Val = {
    val parsed = LetRecParserDriver(program)
    eval(Env(new HashMap[Var,Val]()), parsed)
  }

}

object LetRecToString {
  def apply(expr: Expr): String = expr match {
    case Const(a) => a.toString
    case Add(a, b) => apply(a) + " + " + apply(b)
    case Var(a) => a.toString
    case Sub(a,b) => apply(a) + "-" + apply(b)
    case Equal(a, b) => apply(a) + " == " + apply(b)
    case Iszero(c) => "iszero " + apply(c)
    case Ite(c, t, f) => "if " + apply(c) + " then " + apply(t) + " else " + apply(f)
    case Let(name, value, body) => "let " + apply(name) + " = " + apply(value) + " in " + apply(body)
    case Paren(a) => "("+apply(a)+")"
    case Proc(a, b) => "proc " + apply(a) + " " + apply(b)
    case PCall(a, b) =>  apply(a) + " " + apply(b)
    case LetRec(a, b, c, d) => "letrec " + apply(a) + "(" + apply(b) + ") = " + apply(c) + " in " + apply(d)
  }
}
// "letrec f(x) = " + apply(c) + " in " + apply(d)
// "letrec " + apply(a) + " " + apply(b) + " " + apply(c) + " in " + apply(d)
object Hw2App extends App {
  
  println("Hello from Hw2!")

  val int_prog = """x + 1"""
  val parsed = IntParser(int_prog)
  println(parsed)


}