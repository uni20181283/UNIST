#ifndef NRKFLAT_H
#define NRKFLAT_H

#include "FlatHash.h"
#include <iostream>
#include <stdint.h>

// Real Final comitt of mine
#define INT2VOIDP(i) (void*)(uintptr_t)(i)

class NRKFlat : public FlatHash
{
private:
  // Counter array
  unsigned int* counters;
  // Size of NRK filter (the number of counters)
  unsigned int filter_size;

  float alpha;
  unsigned int in;
public:
  NRKFlat(enum overflow_handle _flag, float _alpha, unsigned int _filter_size);

  ~NRKFlat();
  
  unsigned int hashFunction(const unsigned int key) { return key % filter_size; }
  
  // Hash function
  unsigned int murmurHash2(const void* key);

  void getMMHashValue(const unsigned int key, unsigned int& h1, unsigned int& h2, unsigned int& h3);

  bool filter(const unsigned int key);

  // Overwriting
  int insert(const unsigned int key);
  
  // Overwriting
  int remove(const unsigned int key);

  // Overwriting
  int search(const unsigned int key);
};

NRKFlat::NRKFlat(enum overflow_handle _flag, float _alpha, unsigned int _filter_size) : FlatHash(_flag, _alpha)
{
  filter_size = _filter_size;
  // Write your code
  alpha = _alpha;
  // flag = _flag;
  in = 0;
  counters = new unsigned int[filter_size];
  for(unsigned int i = 0 ; i < filter_size ; i++){
    counters[i] = 0;
  }
}

NRKFlat::~NRKFlat()
{
  // Write your code
  delete[] counters;
  counters = NULL;

}

unsigned int NRKFlat::murmurHash2(const void* key){
  int len = 4;
  unsigned int seed = 2019;

  const unsigned int m = 0x5bd1e995;
  const int r = 24;

  unsigned int h = seed ^ len;

  const unsigned char * data = (const unsigned char *)key;

  while(len >= 4)
  {
    unsigned int k = *(unsigned int *)data;

    k *= m; 
    k ^= k >> r; 
    k *= m; 
    
    h *= m; 
    h ^= k;

    data += 4;
    len -= 4;
  }

  switch(len)
  {
  case 3: h ^= data[2] << 16;
  case 2: h ^= data[1] << 8;
  case 1: h ^= data[0];
          h *= m;
  };

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
}

void NRKFlat::getMMHashValue(const unsigned int key, unsigned int& h1, unsigned int& h2, unsigned int& h3)
{ 
  // You can use h1, h2 and h3 as hashing results which you have to use to decide counter locations
  h1 = murmurHash2(INT2VOIDP(&key));
  h2 = murmurHash2(INT2VOIDP(&h1));
  h3 = murmurHash2(INT2VOIDP(&h2));
  
}

bool NRKFlat::filter(const unsigned int key)
{
  // You can use h1, h2 and h3 as hashing results which you have to use to decide counter locations
  unsigned int h1, h2, h3;
  getMMHashValue(key, h1, h2, h3);

  // Write your code
  if((counters[hashFunction(h1)] == 0) || (counters[hashFunction(h2)] == 0) || (counters[hashFunction(h3)] == 0))
    return true; // which mean there is no.
  else
    return false; // which mean it is can be either exist or Not exist.
}

int NRKFlat::insert(const unsigned int key)
{
  // Write your code
  unsigned int h1, h2, h3;
  getMMHashValue(key, h1, h2, h3);
  int time_output = FlatHash::insert(key);
  if(time_output > 0){
    counters[hashFunction(h1)] = counters[hashFunction(h1)] + 1;
    counters[hashFunction(h2)] = counters[hashFunction(h2)] + 1;
    counters[hashFunction(h3)] = counters[hashFunction(h3)] + 1;
    in++;
  }
  if(in >= filter_size * alpha){
    filter_size = filter_size * 2;
    unsigned int* resizing_counters = new unsigned int[filter_size];
    for(unsigned int i = 0 ; i < filter_size; i++){
      resizing_counters[i] = 0;
    }
    unsigned int* mine = give_hashtable();
    for(unsigned int i = 0 ; i < getTableSize() ; i++){
      if(mine[i] != 0){
        unsigned int h11, h12, h13;
        getMMHashValue(mine[i], h11, h12, h13);
        resizing_counters[hashFunction(h11)] = resizing_counters[hashFunction(h11)] + 1;
        resizing_counters[hashFunction(h12)] = resizing_counters[hashFunction(h12)] + 1;
        resizing_counters[hashFunction(h13)] = resizing_counters[hashFunction(h13)] + 1;
      }
    }
    counters = resizing_counters;
  }
  return time_output;
}

int NRKFlat::remove(const unsigned int key)
{
  // Write your code
  unsigned int h1, h2, h3;
  getMMHashValue(key, h1, h2, h3);
  int time_output = FlatHash::remove(key);
  if(time_output > 0){
    counters[hashFunction(h1)] = counters[hashFunction(h1)] - 1;
    counters[hashFunction(h2)] = counters[hashFunction(h2)] - 1;
    counters[hashFunction(h3)] = counters[hashFunction(h3)] - 1;
    in--;
  }
  return time_output;
}

int NRKFlat::search(const unsigned int key)
{
  // Write your code
  if(filter(key)){
    return 0;
  }
  else{
    int time_output = FlatHash::search(key);
    return time_output;
  }
}

#endif
