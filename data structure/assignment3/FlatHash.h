#ifndef FLATHASH_H
#define FLATHASH_H

#include <iostream>
// Real Final comitt of mine
// Flag(hint) for overflow handling
enum overflow_handle {
  LINEAR_PROBING = 0,
  QUADRATIC_PROBING
};

class FlatHash
{
private:
  unsigned int* hashtable;
  // Variable for overflow handling
  enum overflow_handle flag;
  // Loading factor
  float alpha;
  // Size of hash table
  unsigned int table_size;
  // Nums of keys
  unsigned int num_of_keys;

public:
  FlatHash(enum overflow_handle _flag, float _alpha);

  ~FlatHash();

  unsigned int hashFunction(const unsigned int key) { return key % table_size; }

  unsigned int getTableSize() { return table_size; }

  unsigned int getNumofKeys() { return num_of_keys; }

  // Return time cost
  int insert(const unsigned int key);

  // Remove function in lecture. Return time cost
  int remove(const unsigned int key);

  // Return time cost
  int search(const unsigned int key);

  // Delete tombstones
  void clearTombstones();

  void print();

  unsigned int* give_hashtable() { return hashtable; }
};

FlatHash::FlatHash(enum overflow_handle _flag, float _alpha)
{
  // Initial table size is 1000 
  table_size = 1000;
  num_of_keys = 0;
  flag = _flag;
  alpha = _alpha;

  // Write your code
  if(flag == LINEAR_PROBING){     // for Linear_probing
    hashtable = new unsigned int[table_size];
    for(unsigned int i = 0 ; i < table_size ; i++){
      hashtable[i] = 0;
    }
  }
  else if(flag == QUADRATIC_PROBING){     // for quadratic probing
    hashtable = new unsigned int[table_size];
    for(unsigned int i = 0 ; i < table_size ; i++){
      hashtable[i] = 0;
    }
  }
  // else{ // flag is neither linear nor quadratic
  //   std::cout << "No case in FlatHash" << std::endl;
  // }
}

FlatHash::~FlatHash()
{
  // Write your code
  if(hashtable){
    // delete[] hashtable;
    hashtable = NULL;
  }
}

int FlatHash::insert(const unsigned int key)
{
  // You have to implement two overflow handling by using flag
  // Write your code
  if(flag == LINEAR_PROBING){           // for Linear_probing
    int time_output = 1;
    bool is_it_find = true;
    for(unsigned int i = 0 ; i < table_size ; i++){
      unsigned int index = hashFunction(key + i);
      if(hashtable[index] == key){
        time_output = -1 * time_output;
        is_it_find = false;
        break;
      }
      if(hashtable[index] == 0){
        hashtable[index] = key;
        is_it_find = true;
        break;
      }
      time_output++;
    }

    if(is_it_find){
      num_of_keys++;
    }

    if(num_of_keys >= table_size * alpha){  // if bigger than alpha then resizing
      FlatHash resizing(flag, alpha);
      resizing.table_size = table_size * 2;
      resizing.num_of_keys = 0;
      resizing.hashtable = new unsigned int[resizing.table_size];
      for(unsigned int i = 0 ; i < resizing.table_size ; i++){
        resizing.hashtable[i] = 0;
      }
      for(unsigned int i = 0 ; i < table_size ; i++){
        if(hashtable[i] != 0){
          resizing.insert(hashtable[i]);
        }
      }
      table_size = resizing.table_size;
      delete[] hashtable;
      hashtable = new unsigned int[table_size];
      hashtable = resizing.hashtable;
    }

    return time_output;
  }


  else{   // for quadratic probing
    int time_output = 1;
    bool is_it_find = false;
    bool linear_prov = true;
    for(unsigned int i = 0 ; i < table_size ; i++){
      int square = i*i;
      int index = hashFunction(key+square);
      if(hashtable[index] == 0){
        hashtable[index] = key;
        is_it_find = true;
        linear_prov = false;
        break;
      }
      if(hashtable[index] == key){
        linear_prov = false;
        break;
      }
      time_output++;
    }
    
    if(linear_prov){    // for case of not find in quadratic probing  
      for(unsigned int i = 0 ; i < table_size ; i++){
        int index = hashFunction(key+i);
        if(hashtable[index] == 0){
          hashtable[index] = key;
          is_it_find = true;
          break;
        }
        if(hashtable[index] == key){
          break;
        }
        time_output++;
      }
    }
    if(is_it_find)
      num_of_keys++;

    if(num_of_keys >= table_size * alpha){  // if bigger than alpha then resizing
      FlatHash resizing(flag, alpha);
      resizing.table_size = table_size * 2;
      resizing.num_of_keys = 0;
      resizing.hashtable = new unsigned int[resizing.table_size];
      for(unsigned int i = 0 ; i < resizing.table_size ; i++){
        resizing.hashtable[i] = 0;
      }
      for(unsigned int i = 0 ; i < table_size ; i++){
        if(hashtable[i] != 0){
          resizing.insert(hashtable[i]);
        }
      }
      table_size = resizing.table_size;
      delete[] hashtable;
      hashtable = new unsigned int[table_size];
      hashtable = resizing.hashtable;
    }

    return time_output;
  }
}

int FlatHash::remove(const unsigned int key)
{
  // Write your code
  if(flag == LINEAR_PROBING){
    int time_output = 1;
    int index = hashFunction(key);
    bool must_be_removed = false;
    for(unsigned int i = 0 ; i < table_size ; i++){
      if(hashtable[index] == key){
        must_be_removed = true;
        hashtable[index] = 0;
        num_of_keys--;
        break;
      }
      if(hashtable[index] == 0){
        break;
      }
      index = hashFunction(index + 1);
      time_output++;
    }
    if(must_be_removed){
      index = hashFunction(index + 1);
      unsigned int cluster[table_size];
      for(unsigned int i = 0 ; i < table_size ; i++){
        cluster[i] = 0;
      }
      for(unsigned int i = 0 ; i < table_size ; i++){
        if(hashtable[index] == 0){
          break;
        }
        else{
          cluster[i] = hashtable[index];
          hashtable[index] = 0;
          num_of_keys--;
        }
        index = hashFunction(index + 1);
      }
      int i = 0 ;
      while(cluster[i] != 0){
        insert(cluster[i]);
        i++;
      }
    }
    else{
      time_output = -1 * time_output;
    }
    return time_output;
  }


  else if(flag == QUADRATIC_PROBING){   // for quadratic probing
    int time_output = 1;
    bool is_it_find = false;
    bool linear_probing_is_need = true;
    for(unsigned int i = 0 ; i < table_size ; i++){
      int square = i*i;
      int index = hashFunction(key+square);
      if(hashtable[index] == key){
        hashtable[index] = 1000001; // get tomstone
        is_it_find = true;
        linear_probing_is_need = false;
        break;
      }
      if(hashtable[index] == 0){
        linear_probing_is_need = false;
        break;
      }
      time_output++;
    }

    if(linear_probing_is_need){    // for case of not find in quadratic probing  
      for(unsigned int i = 0 ; i < table_size ; i++){
        int index = hashFunction(key+i);
        if(hashtable[index] == key){
          hashtable[index] = 1000001;
          is_it_find = true;
          break;
        }
        if(hashtable[index] == 0){
          is_it_find = false;
          break;
        }
        time_output++;
      }
    }
    if(is_it_find == true){
      num_of_keys--;
    }
    else{
      time_output = -1 * time_output;
    }
    return time_output;
  }

  else{
    return 1;
  }
}

int FlatHash::search(const unsigned int key)
{
  // Write your code
  if(flag == LINEAR_PROBING){ 
    int time_output = 1;
    for(unsigned int i = 0 ; i < table_size ; i++){
      int index = hashFunction(key+i);
      if(hashtable[index] == key){
        break;
      }
      if(hashtable[index+1] == 0){
        time_output = -1 * time_output;
        break;
      }
      time_output++;
    }
    return time_output;
  }

  else{   // for quadratic probing
    int time_output = 1;
    bool is_it_find = false;
    for(unsigned int i = 0 ; i < table_size ; i++){
      int square = i*i;
      int index = hashFunction(key+square);
      if(hashtable[index] == key){
        is_it_find = true;
        break;
      }
      if(hashtable[index] == 0){
        is_it_find = true;
        time_output = -1 * time_output;
        break;
      }
      time_output++;
    }
    if(is_it_find == false){    // for case of not find in quadratic probing  
      for(unsigned int i = 0 ; i < table_size ; i++){
        int index = hashFunction(key+i);
        if(hashtable[index] == 0){
          hashtable[index] = key;
          break;
        }
        time_output++;
      }
    }
    return time_output;
  }
}

void FlatHash::clearTombstones()
{
  // Write your code
  if(flag == QUADRATIC_PROBING){
    for(unsigned int i = 0 ; i < table_size ; i++){
      if(hashtable[i] == 1000001){
        hashtable[i] = 0;
        // clear it;
      }
    }
    FlatHash rehash(flag, alpha);
    rehash.table_size = table_size;
    rehash.num_of_keys = 0;
    rehash.hashtable = new unsigned int[rehash.table_size];
    for(unsigned int i = 0 ; i < rehash.table_size ; i++){
      rehash.hashtable[i] = 0;
    }
    for(unsigned int i = 0 ; i < table_size ; i++){
      if(hashtable[i] != 0){
        rehash.insert(hashtable[i]);
      }
    }
    hashtable = rehash.hashtable;
  }
}

void FlatHash::print()
{
  // Print valid key pair - (index1:key1,index2:key2)
  // Give **NO** space between each character
  // e.g., (1:3,3:7,5:1)
  std::cout << "(";

  // Write your code
  bool is_first = true;
  for(unsigned int i = 0 ; i < table_size ; i++){
    if(hashtable[i] != 0 && is_first == false){
      std::cout << "," << i << ":" << hashtable[i];
    }
    else if(hashtable[i] != 0 && is_first == true){
      std::cout << i << ":" << hashtable[i];
      is_first = false;
    }
  }
  std::cout << ")" << std::endl;
}

#endif
