#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <sstream>
#include <cstdlib>
#include "stack.h"
#include "queue.h"

using namespace std;

/*
 * Recursive binary tree structure for building expression tree
 *
 * Constructors: Tree(char, Tree*, Tree*) and Tree(string, Tree*, Tree*)
 *      pointers will be automatically set to NULL if omitted
 *
 * Destructor: recursively destruct whole tree
 *
 */

struct Tree {
    string expr;
    Tree* left;
    Tree* right;

    explicit Tree(char h, Tree* l = NULL, Tree* r = NULL) {
        expr = string(1, h);
        left = l;
        right = r;
    }

    explicit Tree(const string& h = string(), Tree* l = NULL, Tree* r = NULL) {
        expr = h;
        left = l;
        right = r;
    }

    ~Tree() {
        delete left;
        delete right;
    }

    int size() {
        int size = 1;
        if (left != NULL) { size += left->size(); }
        if (right != NULL) { size += right->size(); }
        return size;
    }


    void print();

    double eval();
};

// -------- implement below --------

void Tree::print() {
    Queue<Tree*> q;
    int k = 1;
    Tree* current = this;
    while(current && k < size()){
        cout << current->expr << ",";
        if(current->left){
            q.push(current->left);
        }
        else{
            Tree* NULL_node = new Tree;
            NULL_node->expr = "NULL";
            NULL_node->left = NULL;
            NULL_node->right = NULL;
            q.push(NULL_node);
        }
        if(current->right){
            q.push(current->right);
        }
        else{
            Tree* NULL_node = new Tree;
            NULL_node->expr = "NULL";
            NULL_node->left = NULL;
            NULL_node->right = NULL;
            q.push(NULL_node);
        }
        while(q.front()->expr == "NULL"){
            cout << "null,";
            q.pop();
        }
        if(q.empty()) return;
        current = q.front();
        q.pop();
	k++;
    }
    cout << current->expr << ")" << endl;
}

double Tree::eval() {
    stringstream ss;
    double number;
    ss.str(expr);
    ss >> number;
    if(expr == "0"){
        return 0;
    }
    else if(number != 0){
        return number;
    }
    else if(expr == "+"){
        return left->eval() + right->eval();
    }
    else if(expr == "*"){
        return left->eval() * right->eval();
    }
    else if(expr == "/"){
        return left->eval() / right->eval();
    }
    else{
        return left->eval() - right->eval();
    }
}

Tree* build_expression_tree(const string& postfix) {
    Stack<Tree*> stack;
    stringstream ss;
    string token;
    ss << postfix;
    while(ss >> token){
        if(token == "+" || token == "*" || token == "/" || token == "-"){
            Tree* Tree_node = new Tree;
            Tree_node->expr = token;
            Tree_node->right = stack.top();
            stack.pop();
            Tree_node->left = stack.top();
            stack.pop();
            stack.push(Tree_node);
            // cout << "node is " << stack.top().expr << endl;
            // cout << "left is " << stack.top().left->expr << endl;
            // cout << "right is " << stack.top().right->expr << endl;
	}
        else{
            Tree *Tree_node = new Tree;
            Tree_node->expr = token;
            Tree_node->left = NULL;
            Tree_node->right = NULL;
            stack.push(Tree_node);
        }
    }
    return stack.top();
}

#endif //TREE_H


// double Tree::eval() {
//     stringstream ss, sss;
//     double number1, number2;
//     ss.str(left->expr);
//     ss >> number1;
//     sss.str(right->expr);
//     sss >> number2;

//     if( (left->expr == "0" || number1 != 0) && (right->expr == "0" || number2 != 0) ){
//         if(expr == "+"){
//             return stod(left->expr) + stod(right->expr);
//         }
//         else if(expr == "*"){
//             return stod(left->expr) * stod(right->expr);
//         }
//         else if(expr == "/"){
//             return stod(left->expr) / stod(right->expr);
//         }
//         else{
//             return stod(left->expr) - stod(right->expr);
//         }
//     }
//     if(left->expr == "0" || number1 != 0){
//         if(expr == "+"){
//             return stod(left->expr) + right->eval();
//         }
//         else if(expr == "*"){
//             return stod(left->expr) * right->eval();
//         }
//         else if(expr == "/"){
//             return stod(left->expr) / right->eval();
//         }
//         else{
//             return stod(left->expr) - right->eval();
//         }
//     }
//     if(right->expr == "0" || number2 != 0){
//         if(expr == "+"){
//             return left->eval() + stod(right->expr);
//         }
//         else if(expr == "*"){
//             return left->eval() * stod(right->expr);
//         }
//         else if(expr == "/"){
//             return left->eval() / stod(right->expr);
//         }
//         else{
//             return left->eval() - stod(right->expr);
//         }
//     }
//     else{
//         if(expr == "+"){
//             return left->eval() + right->eval();
//         }
//         else if(expr == "*"){
//             return left->eval() * right->eval();
//         }
//         else if(expr == "/"){
//             return left->eval() / right->eval();
//         }
//         else if(expr == "–"){
//             return left->eval() - right->eval();
//         }
//     }
//     return -1;
// }
