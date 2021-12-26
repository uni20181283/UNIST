sealed trait IntList
case object Nil extends IntList
case class Cons(v: Int, t: IntList) extends IntList

sealed trait BTree
case object Leaf extends BTree
case class IntNode(v: Int, left: BTree, right: BTree)
extends BTree

sealed trait Formula
case object True extends Formula
case object False extends Formula
case class Not(f: Formula) extends Formula
case class Andalso(left: Formula, right: Formula) extends Formula
case class Orelse(left: Formula, right: Formula)  extends Formula
case class Implies(left: Formula, right: Formula) extends Formula

object Hw1 extends App {

  println("Hw1!!!!")

  def gcd(a: Int, b: Int): Int = {
    if ( b == 0 ) return a
    else return gcd(b, a%b)
  }

  def oddSum(f: Int=>Int, n: Int): Int = {
    if ( n == 1 ) return f(1)
    else if (n == 0) return f(0)
    else if (n % 2 == 0 ) return oddSum(f, n-1)
    else return f(n) + oddSum(f, n-2)
  }

  def foldRight(init: Int, ftn: (Int, Int)=>Int, list: IntList): Int = {
    list match {
      case Nil => 0
      case Cons(h, Nil) => ftn(init, h)
      case Cons(h, t) => foldRight(init, ftn, t)
    }
  }

  def map(f: Int=>Int, list: IntList): IntList = {
    list match {
      case Nil => list
      case Cons(h, t) => Cons(f(h), map(f, t))
    }
  }

  def iter[A](f: A => A, n: Int): A => A = (x: A) => {
    if ( n == 1 ) f(x)
    else f(iter[A](f, n-1)(x))
  }

  def insert(t: BTree, a: Int): BTree = {
    t match{
      case Leaf => IntNode(a, Leaf, Leaf)
      case IntNode(v, left, right) => if ( v < a ) IntNode(v, left, insert(right, a)) else IntNode(v, insert(left, a), right)
    }
  }

  def eval(f: Formula): Boolean = {
    f match {
      case True => true
      case False => false
      case Not(ff) => if ( eval(ff) == true ) return false else return true
      case Andalso(left, right) => if ( eval(left) == true && eval(right) == true ) return true else return false
      case Orelse(left, right) => if ( eval(left) == false && eval(right) == false ) return false else return true
      case Implies(left, right) => if ( eval(left) == true && eval(right) == false ) return false else return true
    }
  }
}