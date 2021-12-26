#ifndef STACK_H
#define STACK_H

#include <iostream>

using namespace std;

template <typename type>
class Stack_Node {

public:
  type data;
  Stack_Node<type>* link;
};

template <typename type>
class Stack {

public:
  // Constructor
  explicit Stack();

  // Destructor
  ~Stack();

  // Store new element
  void push(const type& item);

  // Return first element
  type& top() const;

  // Remove first element
  void pop();

  // Return true if empty, false otherwise
  bool empty() const;

  // Number of elements stored
  int size() const;

private:
  Stack_Node<type>* first;
};

// Implement functions below

template <typename type>
Stack<type>::Stack() {
  first = NULL;
}

template <typename type>
Stack<type>::~Stack() {
  while(!empty()){
    pop();
  }
}

template <typename type>
void Stack<type>::push(const type &item) {
  Stack_Node<type>* new_node = new Stack_Node<type>;
  new_node->data = item;
  if(first == NULL)
    first = new_node;
  else{
    new_node->link = first;
    first = new_node;
  }
}

template <typename type>
type& Stack<type>::top() const {
  // if(first!=NULL)
    return first->data;
}

template <typename type>
void Stack<type>::pop() {
  if(first == NULL)
    first = NULL;
  else if(first->link != NULL)
    first = first->link;
  else first = NULL;
}

template <typename type>
bool Stack<type>::empty() const {
  if(first == NULL) return true;
  else return false;
}

template <typename type>
int Stack<type>::size() const {
  if(first == NULL){
    return 0;
  }
  else{
    Stack_Node<type>* count_node = first;
    int k = 1;
    while(count_node->link != NULL){
      count_node=count_node->link;
      k++;
    }
    return k;
  }
}

#endif //STACK_H
