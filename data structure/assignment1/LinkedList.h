// CSE221 Assignment 1

#ifndef LinkedList_h
#define LinkedList_h

#include <typeinfo>
#include <iostream>

template <typename Type>
class LinkedList_Node
{
public:
	Type data;
	LinkedList_Node<Type>* link;
};

template <typename Type>
class LinkedList
{
public:
	// Constructor
	LinkedList();

	// Destructor
	~LinkedList();

	// Get the value located at index
	Type Get(const int index);

	// Add val at head
	void AddAtHead(const Type& val);

	// Add val at index
	void AddAtIndex(const int index, const Type& val);

	// Delete an element located at index
	void DeleteAtIndex(const int index);

	// Delete val in linked list
	void DeleteValue(const Type& val);

	// Move the first element of val to head
	void MoveToHead(const Type& val);

	// Rotate the linked list right by steps times
	void Rotate(const int steps);

	// Reduce value that repeats multiple times
	void Reduce();

	// Reverse at every k number of nodes at a time
	void K_Reverse(const int k);

	// Sort even and odd numbers separately then append
	void EvenOddSeparateSort();

	// Return the number of elements in the linked list
	int Size();

	// // Delete all elements from the linked list
	void CleanUp();
	
	// Print the list
	void Print();

	void sort();
private:
	LinkedList_Node<Type>* first;
};

/*
 *	Implementation
 */
// Please note that you will, by default, have to write default constructor and destructor:
template <typename Type>
LinkedList<Type>::LinkedList()
{
	first = NULL;
}

template <typename Type>
LinkedList<Type>::~LinkedList()
{
	while(first != NULL){
		first = first->link;
	}
}

template <typename Type>
Type LinkedList<Type>::Get(const int index)
{
	LinkedList_Node<Type>* count_node = first;
	int k = 0;
	if( k < 0 || k > Size() ) return -1;
	else{
		while(k<index){
			count_node = count_node->link;
			k++;
		}
		return count_node->data;
	}
}

template <typename Type>
void LinkedList<Type>::AddAtHead(const Type& val)
{
	LinkedList_Node<Type>* new_node = new LinkedList_Node<Type>;
	new_node->data = val;
	if(first == NULL)
		first = new_node;
	else{
		new_node->link = first;
		first = new_node;
	}
}

template <typename Type>
void LinkedList<Type>::AddAtIndex(const int index, const Type& val)
{
	if( index >= 0 && index <= Size()){
		if(index == 0) AddAtHead(val);
		else if(index == Size()){
			LinkedList_Node<Type>* count_node = first;
			while(count_node->link != NULL){
				count_node = count_node->link;
			}
			// std::cout << count_node->data << std::endl;
			LinkedList_Node<Type>* new_node = new LinkedList_Node<Type>;
			count_node->link = new_node;
			new_node->data = val;
			new_node->link = NULL;
		}
		else{
			LinkedList_Node<Type>* count_node = first;
			int k = 0;
			while(k<index-1){
				count_node = count_node->link;
				k++;
			}
			LinkedList_Node<Type>* new_node = new LinkedList_Node<Type>;
			new_node->data = val;
			new_node->link = count_node->link;
			count_node->link = new_node;
		}
	}
}

template <typename Type>
void LinkedList<Type>::DeleteAtIndex(const int index)
{
	if( index > 0 && index <= Size() ){
		LinkedList_Node<Type>* count_node = first;
		int k = 0;
		while(k<index-1){
			count_node = count_node->link;
			k++;
		}
		if(index == Size()){
			count_node->link = NULL;
		}
		else
			count_node->link = count_node->link->link;
	}
	else if(index == 0){
		if(Size() == 0)
			first = NULL;
		else
			first = first->link;
	}
}

template <typename Type>
void LinkedList<Type>::DeleteValue(const Type& val)
{	
	if(Size()>1){	
		if(first->data == val){
			first = first->link;
		}
		else{
			LinkedList_Node<Type>* count_node = first;
			while(count_node->link != NULL){
				if(count_node->link->data == val){
					if(count_node->link->link == NULL){
						count_node->link = NULL;

					}
					else
						count_node->link = count_node->link->link;
					break;
				}
				count_node = count_node->link;
			}
		}
	}
	if(Size() == 1 && first->data == val){
		first = NULL;
	}
}

template <typename Type>
void LinkedList<Type>::MoveToHead(const Type& val)
{
	if(Size() != 0){
		LinkedList_Node<Type>* count_node = first;
		while(count_node->link != NULL){
			if(count_node->link->data == val){
				LinkedList_Node<Type>* MoveToHead_node = count_node->link;
				count_node->link = count_node->link->link;
				MoveToHead_node->link = first;
				first = MoveToHead_node;
				break;
			}
			count_node = count_node->link;
		}
	}
}

template <typename Type>
void LinkedList<Type>::Rotate(const int steps)
{
	if(Size() != 0 ){
		if(steps>0){	
			for(int i = 0 ; i < steps ; i++ ){
				LinkedList_Node<Type>* count_node = first;
				while(count_node->link != NULL){
					if(count_node->link->link == NULL){
						LinkedList_Node<Type>* MoveToHead_node = count_node->link;
						count_node->link->link = first;
						count_node->link = NULL;
						first = MoveToHead_node;
					}
					else{
						count_node = count_node->link;
					}
				}
			}
		}
	}
}

template <typename Type>
void LinkedList<Type>::Reduce()
{
	LinkedList_Node<Type>* count_node = first;
	while(count_node!= NULL){
		LinkedList_Node<Type>* count_node_two = count_node;
		while(count_node_two->link != NULL){
			if(count_node->data == count_node_two->link->data){
				count_node_two->link = count_node_two->link->link;
			}
			else {
				count_node_two = count_node_two->link;
			}
		}
		count_node = count_node->link;
	}
}
template <typename Type>
void LinkedList<Type>::K_Reverse(const int k) 
{	if(k>0){
		for(int j = 0 ; j < Size()/k ; j++){
			LinkedList_Node<Type>* count_node = first;
			for(int i = 0 ; i < k*j ; i++)
				count_node = count_node->link;
			LinkedList<Type> LL_2;
			for(int i = 0 ; i < k ; i++){
				LL_2.AddAtHead(count_node->data);
				count_node = count_node->link;
			}
			LinkedList_Node<Type>* count_node_2 = LL_2.first;
			LinkedList_Node<Type>* count_node_3 = first;
			for(int i = 0 ; i < k*j-1 ; i++)
				count_node_3 = count_node_3->link;
			while(count_node_2->link != NULL)
				count_node_2 = count_node_2->link;
			if(j==0){
				count_node_2->link = count_node;
				first = LL_2.first;
			}
			else{
				count_node_2->link = count_node;
				count_node_3->link = LL_2.first;
			}
		}
	}
}

template <typename Type>
void LinkedList<Type>::EvenOddSeparateSort() 
{
	LinkedList<Type> LL_odd;
	LinkedList<Type> LL_even;
	LinkedList_Node<Type>* count_node = first;
	sort();
	for(int i = 0; i < Size(); i++){
		if(count_node->data%2 == 0){
			LL_even.AddAtHead(count_node->data);
		}
		else LL_odd.AddAtHead(count_node->data);
		count_node = count_node->link;
	}
	LL_even.K_Reverse(LL_even.Size());
	count_node = LL_even.first;
	for(int i = 0 ; i < LL_even.Size(); i ++){
		if(count_node->link == NULL){
			// std::cout<< "1" << std::endl;
			count_node->link = LL_odd.first;
		}
		else{
		// std::cout<< "2" << std::endl;
		count_node = count_node->link;
		}
	}
	first = LL_even.first;
}

template <typename Type>
int LinkedList<Type>::Size()
{
	LinkedList_Node<Type>* count_node = first;
	int i = 0;
	while(count_node != NULL){
		count_node = count_node->link;
		i++;
	}
	return i;
}

template <typename Type>
void LinkedList<Type>::CleanUp()
{
	while(first != NULL){
		first = first->link;
	}
}

template <typename Type>
void LinkedList<Type>::Print()
{
	if(first != NULL){
		LinkedList_Node<Type>* count_node = first;
		std::cout << "(";
		while(true){
			if(count_node->link == NULL){
				std::cout << count_node->data << ")" << std::endl;
				break;
			}
			else{
				std::cout << count_node->data << ",";
				count_node = count_node->link;
			}
		}
	}
	else {
		std::cout << "()" << std::endl;
	}
}
template <typename Type>
void LinkedList<Type>::sort() 
{
	LinkedList_Node<Type>* count_node = first;
	for(int i = 0 ; i < Size(); i++){
		LinkedList_Node<Type>* check_node = count_node;
		for(int j = i+1 ; j < Size() ; j++){
			if(check_node->data > check_node->link->data){
				Type temp = check_node->data;
				check_node->data = check_node->link->data;
				check_node->link->data = temp;
			}
			check_node = check_node->link;
		}
	}
}
#endif



/*
					std::cout << "check_node->link->data : " << check_node->link->data << std::endl;
					if(first == check_node){
						first = first->link;
						AddAtIndex(1, check_node->data);
						check_node->link = check_node->link->link;
					}
					else{
						LinkedList_Node<Type>* temp_node = first;
						while(temp_node->link != check_node){
							temp_node = temp_node->link;
						}
						LinkedList_Node<Type>* temp_node_2 = check_node;
						temp_node->link = check_node->link;
						if(check_node->link->link != NULL)
							check_node->link = check_node->link->link;
						else
							check_node->link = NULL;
						temp_node->link->link = check_node;
					}
					// std::cout << check_node->data << std::endl;
					// Print();
					// std::cout << temp_node->data << std::endl;
					// std::cout << temp_node->link->data << std::endl;
					// std::cout << check_node->data << std::endl;
*/
