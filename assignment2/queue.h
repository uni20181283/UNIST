#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>

using namespace std;


template <typename type>
class Queue_Node {

public:
  type data;
  Queue_Node<type>* link;
};


template <typename type>
class Queue {

public:
  // Constructor
  explicit Queue();

  // Destructor
  ~Queue();

  // Store new element
  void push(const type& item);

  // Return first element
  type& front() const;

  // Remove first element
  void pop();

  // Return true if empty, false otherwise
  bool empty() const;

  // Number of elements stored
  int size() const;

private:
  Queue_Node<type>* first;
};

// Implement functions below

template <typename type>
Queue<type>::Queue() {
  first = NULL;
}

template <typename type>
Queue<type>::~Queue() {
  while(!empty()){
    pop();
  }
}


template <typename type>
void Queue<type>::push(const type &item) {
  Queue_Node<type>* new_node = new Queue_Node<type>;
  new_node->data = item;
  if(first == NULL)
    first = new_node;
  else{
    Queue_Node<type>* count_node = first;
    while(count_node->link != NULL){
      count_node = count_node->link;
    }
    count_node->link = new_node;
  }
}

template <typename type>
type& Queue<type>::front() const {
  // if(first != NULL)
    return first->data;
// warning: non-void function does not return a value in all control paths [-Wreturn-type]
}

template <typename type>
void Queue<type>::pop() {
  // delete first->data;
  first = first->link;
}

template <typename type>
bool Queue<type>::empty() const {
  if(first == NULL) return true;
  else return false;
}

template <typename type>
int Queue<type>::size() const {
  if(first == NULL){
    return 0;
  }
  else{
    Queue_Node<type>* count_node = first;
    int k = 1;
    while(count_node->link != NULL){
      count_node=count_node->link;
      k++;
    }
    return k;
  }
}

#endif //QUEUE_H
