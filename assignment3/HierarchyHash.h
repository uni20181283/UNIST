#ifndef HIERARCHYHASH_H
#define HIERARCHYHASH_H

#include <iostream>
#include "FlatHash.h"

// Real Final comitt of mine
class HierarchyHash
{
private:
  unsigned int** hashtable;
  // Variable for overflow handling
  enum overflow_handle flag;
  // Loading factor
  float alpha;
  // Size(range) of full hash table. Initially 1000
  unsigned int table_size;
  // Size of subhash table. Fixed by 100
  unsigned int sub_table_size;
  // Nums of keys
  unsigned int num_of_keys;


public:
  HierarchyHash(enum overflow_handle _flag, float _alpha);

  ~HierarchyHash();

  unsigned int hashFunction(const unsigned int key) { return key % table_size; }

  unsigned int getTableSize() { return table_size; }

  unsigned int getNumofKeys() { return num_of_keys; }

  // Return the size of allocated sub hash table
  unsigned int getAllocatedSize();

  // Return time cost
  int insert(const unsigned int key);

  // Return time cost
  int remove(const unsigned int key);

  // Return time cost
  int search(const unsigned int key);

  // Delete tombstones
  void clearTombstones();

  void print();
};

HierarchyHash::HierarchyHash(enum overflow_handle _flag, float _alpha)
{
  // Initial map size is 1000
  table_size = 1000;
  // Table size is fixed to 100
  sub_table_size = 100;
  flag = _flag;
  alpha = _alpha;

  // Write your code
  num_of_keys = 0;
  if(flag == LINEAR_PROBING){     // for Linear_probing
    hashtable = new unsigned int*[table_size / sub_table_size];
    for(unsigned int i = 0 ; i < table_size / sub_table_size ; i++){
      hashtable[i] = NULL;
    }
  }
  if(flag == QUADRATIC_PROBING){
    hashtable = new unsigned int*[table_size / sub_table_size];
    for(unsigned int i = 0 ; i < table_size / sub_table_size ; i++){
      hashtable[i] = NULL;
    }
  }
}

HierarchyHash::~HierarchyHash()
{
  // Write your code
  if(hashtable){
    for(unsigned int i = 0 ; i < table_size / sub_table_size ; i++){
      // if(hashtable[i]){
      //   delete[] hashtable[i];
      // }
      hashtable[i] = NULL;
    }
    // delete[] hashtable;
  }
  hashtable = NULL;
}

unsigned int HierarchyHash::getAllocatedSize()
{
  // Write your code
  int output = 0;
  for(unsigned int i = 0 ; i < table_size / sub_table_size ; i++){
    if(hashtable[i]){
      output += sub_table_size;
    }
  }
  return output;
}

int HierarchyHash::insert(const unsigned int key)
{
  // Write your code
 if(flag == LINEAR_PROBING){           // for Linear_probing
    int time_output = 1;
    bool is_it_inserted = false;
    for(unsigned int i = 0 ; i < sub_table_size ; i++){
      unsigned int index = hashFunction(key + i);
      unsigned int ith_index = index / sub_table_size;
      unsigned int jth_index = index % sub_table_size;
      if(!hashtable[ith_index]){
        hashtable[ith_index] = new unsigned int[sub_table_size];
        for(unsigned int j = 0 ; j < sub_table_size ; j++){
          hashtable[ith_index][j] = 0;
        }
      }
      if(hashtable[ith_index][jth_index] == 0){
        is_it_inserted = true;
        hashtable[ith_index][jth_index] = key;
        break;
      }
      if(hashtable[ith_index][jth_index] == key){
        is_it_inserted = false;
        time_output = -1 * time_output;
        break;
      }

      time_output++;
    }
    if(is_it_inserted){
      num_of_keys++;
    }

    if(num_of_keys  >= table_size * alpha){  // if bigger than alpha then resizing // testcases 만들면서 수정.
      HierarchyHash resizing(flag, alpha);
      resizing.table_size = table_size * 2;
      resizing.sub_table_size = sub_table_size;
      resizing.num_of_keys = 0;
      resizing.hashtable = new unsigned int*[resizing.table_size / resizing.sub_table_size];

      for(unsigned int i = 0 ; i < resizing.table_size / resizing.sub_table_size ; i++){
        resizing.hashtable[i] = NULL;
      }

      for(unsigned int i = 0 ; i < table_size / sub_table_size ; i ++){
        if(hashtable[i]){
          for(unsigned int j = 0 ; j < sub_table_size ; j++){
            if(hashtable[i][j] != 0){
              resizing.insert(hashtable[i][j]);
            }
          }
        }
      }
      for(unsigned int i = 0 ; i < table_size / sub_table_size ; i ++){
        delete[] hashtable[i];
      } // 배열 삭제
      delete[] hashtable; // hashtable 삭제
      table_size = resizing.table_size; // table 크기 늘려주고
      hashtable = new unsigned int*[table_size / sub_table_size];
      for(unsigned int i = 0 ; i < table_size / sub_table_size ; i++){
        if(resizing.hashtable[i]){
          hashtable[i] = resizing.hashtable[i];
        }
        else{
          hashtable[i] = NULL;
        }
      }
    }
    return time_output;
  }


  else if(flag == QUADRATIC_PROBING){
    unsigned int time_output = 1;
    bool is_it_inserted = false;
    bool linear_need = true;
    for(unsigned int i = 0 ; i < table_size ; i++){
      unsigned int index = hashFunction(key + i*i);
      unsigned int ith_index = index / sub_table_size;
      unsigned int jth_index = index % sub_table_size;
      if(!hashtable[ith_index]){     // if doesn't exist.
        hashtable[ith_index] = new unsigned int[sub_table_size];
        for(unsigned int i = 0 ; i < sub_table_size ; i++){
          hashtable[ith_index][i] = 0;
        }
      }
      if(hashtable[ith_index][jth_index] == 0){
        hashtable[ith_index][jth_index] = key;
        is_it_inserted = true;
        linear_need = false;
        break;
      }
      if(hashtable[ith_index][jth_index] == key){
        linear_need = false;
        time_output = -1 * time_output;
        break;
      }
      time_output++;
    }
    if(linear_need){
      for(unsigned int i = 0 ; i < table_size ; i++){
        unsigned int index = hashFunction(key + i);
        unsigned int ith_index = index / sub_table_size;
        unsigned int jth_index = index % sub_table_size;
        if(!hashtable[ith_index]){     // if doesn't exist.
          hashtable[ith_index] = new unsigned int[sub_table_size];
          for(unsigned int i = 0 ; i < sub_table_size ; i++){
            hashtable[ith_index][i] = 0;
          }
        }
        if(hashtable[ith_index][jth_index] == 0){
          hashtable[ith_index][jth_index] = key;
          is_it_inserted = true;
          linear_need = false;
          break;
        }
        if(hashtable[ith_index][jth_index] == key){
          linear_need = false;
          time_output = -1 * time_output;
          break;
        }
        time_output++;
      }
    }

    if(is_it_inserted){
      num_of_keys++;
    }
    if(num_of_keys >= alpha * table_size){  // if bigger than alpha then resizing
      HierarchyHash resizing(flag, alpha);
      resizing.table_size = table_size * 2;
      resizing.sub_table_size = sub_table_size;
      resizing.num_of_keys = 0;
      resizing.hashtable = new unsigned*[resizing.table_size / resizing.sub_table_size];
      for(unsigned int i = 0 ; i < resizing.table_size / resizing.sub_table_size ; i++){
        resizing.hashtable[i] = NULL;
      }
      for(unsigned int i = 0 ; i < table_size / sub_table_size ; i++){
        if(hashtable[i]){
          for(unsigned int j = 0 ; j < sub_table_size ; j++){
            if(hashtable[i][j] != 0){
              resizing.insert(hashtable[i][j]);
            }
          }
        }
      }
      for(unsigned int i = 0 ; i <table_size/sub_table_size;i++){
        delete[] hashtable[i];
      }
      delete[] hashtable;
      // resizing.print();
      table_size = resizing.table_size;
      hashtable = new unsigned int*[table_size / sub_table_size];
      for(unsigned int i = 0; i < table_size / sub_table_size ; i++){
        if(resizing.hashtable[i]){
          hashtable[i] = resizing.hashtable[i];
        }
        else{
          hashtable[i] = NULL;
        }
      }
    }
    return time_output;
  }
  else{
    return -1;
  }
}

int HierarchyHash::remove(const unsigned int key)
{
  // Write your code
  if(flag == LINEAR_PROBING){
    int time_output = 1;
    bool is_it_found = false;
    unsigned int ith_index = hashFunction(key) / sub_table_size;
    unsigned int jth_index = hashFunction(key) % sub_table_size;
    for(unsigned int i = 0 ; i< table_size ; i++){
      if(!hashtable[ith_index]){
        ith_index ++;
      }
      else{
        if(hashtable[ith_index][jth_index] == key){
          hashtable[ith_index][jth_index] = 0;
          is_it_found = true;
          break;
        }
        if(hashtable[ith_index][jth_index] == 0){
          time_output = -1 * time_output;
          break;
        }
        if(jth_index == sub_table_size - 1){
          ith_index++;
          jth_index = 0;
        }
        else{
          jth_index ++;
        }
      }
      time_output++;
    }
    unsigned int index = ith_index * sub_table_size + jth_index;
    if(is_it_found){
      index = hashFunction(index + 1);
      ith_index = (index) / sub_table_size;
      jth_index = index % sub_table_size;
      unsigned int cluster[table_size];
      for(unsigned int i = 0; i < table_size ; i++){
        cluster[i] = 0;
      }
      for(unsigned int i = 0; i< table_size ; i++){
        if(hashtable[ith_index][jth_index] == 0){
          break;
        }
        else{
          cluster[i] = hashtable[ith_index][jth_index];
          hashtable[ith_index][jth_index] = 0;
          num_of_keys--;
        }
        jth_index++;
        if(jth_index == sub_table_size){
          ith_index++;
          jth_index = 0;
          if(ith_index == table_size/sub_table_size){
            ith_index = 0;
          }
        }
      }
      unsigned int i = 0;
      while(cluster[i] != 0){
        insert(cluster[i]);
        i++;
      }
    }
    if(is_it_found){
      num_of_keys--;
    }
    for(unsigned int i = 0 ; i < table_size / sub_table_size ; i ++){
      if(hashtable[i]){
        bool is_in = false;
        for(unsigned int j = 0 ; j < sub_table_size ; j++){
          if(hashtable[i][j] != 0){
            is_in = true;
          }
        }
        if(is_in == false){
          hashtable[i] = NULL;
        }
      }
    }
    return time_output;
  }
  else{
    unsigned int time_output = 1;
    bool is_find = false;
    bool linear_need = true;
    for(unsigned int i = 0 ; i < table_size ; i++){
      unsigned int index = hashFunction(key + i * i);
      unsigned int ith_index = index / sub_table_size;
      unsigned int jth_index = index % sub_table_size;
      if(!hashtable[ith_index]){
        linear_need = false;
        return -1 * time_output;
      }
      if(hashtable[ith_index][jth_index] == 0){
        time_output = -1 * time_output;
        linear_need = false;
        break;
      }
      if(hashtable[ith_index][jth_index] == key){
        hashtable[ith_index][jth_index] = 1000001;
        is_find = true;
        linear_need = false;
        break;
      }
      time_output++;
    }
    if(linear_need){
      for(unsigned int i = 0 ; i < table_size ; i++){
        unsigned int index = hashFunction(key + i);
        unsigned int ith_index = index / sub_table_size;
        unsigned int jth_index = index % sub_table_size;
        if(!hashtable[ith_index]){
          return -1 * time_output;
        }
        if(hashtable[ith_index][jth_index] == 0){
          time_output = -1 * time_output;
          break;
        }
        if(hashtable[ith_index][jth_index] == key){
          is_find = true;
          hashtable[ith_index][jth_index] = 1000001;
          break;
        }
        time_output++;
      }
    }
    if(is_find){
      num_of_keys--;
    }
    return time_output;
  }
}

int HierarchyHash::search(const unsigned int key)
{
  // Write your code
  if(flag == LINEAR_PROBING){
    int time_output = 1;
    unsigned int index = hashFunction(key);
    unsigned int ith_index = index / sub_table_size;
    unsigned int jth_index = index % sub_table_size;
    for(unsigned int i = 0 ; i < table_size ; i++){
      if(!hashtable[ith_index]){
        ith_index++;
      }
      else{
        if(hashtable[ith_index][jth_index] == key){
          break;
        }
        if(hashtable[ith_index][jth_index] == 0){
          time_output = -1 * time_output;
          break;
        }
        if(jth_index == sub_table_size - 1){
          ith_index++;
          jth_index = 0;
        }
        else{
          jth_index++;
        }
        time_output++;
      }
    }
    return time_output;
  }
  else{
    int time_output = 1;
    unsigned int index = hashFunction(key);
    unsigned int ith_index = index / sub_table_size;
    unsigned int jth_index = index % sub_table_size;
    bool linear_need = true;
    for(unsigned int i = 0 ; i < table_size ; i++){
      index = hashFunction(key + i*i);
      ith_index = index / sub_table_size;
      jth_index = index % sub_table_size;
      if(!hashtable[ith_index]){
        linear_need = false;
        return -1;
      }
      if(hashtable[ith_index][jth_index] == key){
        linear_need = false;
        break;
      }
      if(hashtable[ith_index][jth_index] == 0){
        time_output = -1 * time_output;
        linear_need = false;
        break;
      }
      time_output++;
    }
    if(linear_need){
      for(unsigned int i = 0; i < table_size ; i++){
        index = hashFunction(key + i);
        ith_index = index / sub_table_size;
        jth_index = index % sub_table_size;
        if(!hashtable[ith_index]){
          linear_need = false;
          return -1 * time_output;
        }
        if(hashtable[ith_index][jth_index] == key){
          linear_need = false;
          break;
        }
        if(hashtable[ith_index][jth_index] == 0){
          time_output = -1 * time_output;
          linear_need = false;
          break;
        }
      }
      time_output++;
    }
    return time_output;
  }
}



void HierarchyHash::clearTombstones()
{
  // Write your code
  if(flag == QUADRATIC_PROBING){
    for(unsigned int i = 0 ; i < table_size / sub_table_size ; i++){
      if(hashtable[i]){
        for(unsigned int j = 0 ; j < sub_table_size ; j++){
          if(hashtable[i][j] == 1000001){
            hashtable[i][j] = 0;
          }
        }
      }
    }
    HierarchyHash rehash(flag, alpha);
    rehash.table_size = table_size;
    rehash.sub_table_size = sub_table_size;
    rehash.num_of_keys = 0;
    rehash.hashtable = new unsigned int*[rehash.table_size / rehash.sub_table_size];

    for(unsigned int i = 0 ; i < rehash.table_size / rehash.sub_table_size ; i++){
      rehash.hashtable[i] = NULL;
    }

    for(unsigned int i = 0 ; i < table_size / sub_table_size ; i ++){
      if(hashtable[i]){
        for(unsigned int j = 0 ; j < sub_table_size ; j++){
          if(hashtable[i][j] != 0){
            rehash.insert(hashtable[i][j]);
          }
        }
      }
    }
    for(unsigned int i = 0 ; i < table_size / sub_table_size ; i ++){
      delete[] hashtable[i];
    } // 배열 삭제
    delete[] hashtable; // hashtable 삭제
    hashtable = new unsigned int*[table_size / sub_table_size];
    for(unsigned int i = 0 ; i < table_size / sub_table_size ; i++){
      if(rehash.hashtable[i]){
        hashtable[i] = rehash.hashtable[i];
      }
      else{
        hashtable[i] = NULL;
      }
    }
  }
}
void HierarchyHash::print()
{
  // Print valid key pair for each sub hash table - subtable_id:(index1:key1,index2:key2)
  // Seperate each sub table by endl
  // Give **NO** space between each character
  // e.g., 0:(1:3,3:7,5:1)
  //       1:(101:2,192:10)
  //       9:(902:90,938:82)

  // Exceptionally, keep this code only for the case there is no key in the table
  if( getNumofKeys() == 0){
    std::cout << "()" << std::endl;
    return;
  }
  for(unsigned int i = 0 ; i < table_size/sub_table_size ; i++){
    if(hashtable[i] != NULL){
      int is_here = false;
      for(unsigned int j = 0 ; j < sub_table_size ; j++){
        if(hashtable[i][j] > 0 && hashtable[i][j] < 1000001){
          is_here = true;
        }
      }
      if(is_here){
        std::cout << i << ":(";
        bool is_first = true;
        for(unsigned int j = 0 ; j < sub_table_size ; j++){
          if(hashtable[i][j] != 0 && hashtable[i][j] < 1000001 && !is_first){
            std::cout << "," << sub_table_size*i+j << ":" << hashtable[i][j];
          }
          else if(hashtable[i][j] != 0 && hashtable[i][j] < 1000001 && is_first){
            std::cout << sub_table_size*i+j << ":" << hashtable[i][j];
            is_first = false;
          }
        }
        std::cout << ")" <<std::endl;
      }
    }
  }
  // Write your code

}

#endif
