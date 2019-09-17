/* 
    File: my_allocator.cpp

    Author: Bryan Lara
            Department of Computer Science
            Texas A&M University
    Date  : 09/08/2019

    Modified: 

    This file contains the implementation of the class MyAllocator.

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>
#include "my_allocator.hpp"
#include <assert.h>
#include <iostream>

/*--------------------------------------------------------------------------*/
/* NAME SPACES */ 
/*--------------------------------------------------------------------------*/

using namespace std;
/* I know, it's a bad habit, but this is a tiny program anyway... */

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR CLASS MyAllocator */
/*--------------------------------------------------------------------------*/

MyAllocator::MyAllocator(size_t _basic_block_size, size_t _size) {
  blockSize=_basic_block_size;
  start_addr= static_cast<char *> (std::malloc(_size));
  SegmentHeader* newSegment = new(start_addr) SegmentHeader(_size, true);
  frlist.Add(newSegment);
  //cout<<"orig: "<< &orig_start<<"\nstart_add: "<< &start_addr<<"\nAvailable: "<<&available<<endl;
  //cout<<"\norig: "<< static_cast<void*>(orig_start)<<"\nstart_add: "<< static_cast<void*>(start_addr)<<"\nAvailable: "<<static_cast<void*>(available)<<endl;
  
}
 
MyAllocator::~MyAllocator() {
  std::free(start_addr);
}

Addr MyAllocator::Malloc(size_t _length) {
  int requiredLength;
  int min= _length+ sizeof(SegmentHeader);
  int blockRemain= (min) % blockSize ;

  if(blockRemain==0){
    requiredLength= min;
  }
  else{
    requiredLength= min+blockSize-blockRemain ;
  }
  
  SegmentHeader* temp= frlist.GetHeader();
  
  while(temp != nullptr){
    if(temp->length >= requiredLength /*&& temp->is_free==true*/){
      size_t newLength= temp->length - requiredLength - sizeof(SegmentHeader);
      SegmentHeader* newSegment = new(start_addr + requiredLength /*+ sizeof(SegmentHeader)*/) SegmentHeader(newLength - sizeof(SegmentHeader), true);
      frlist.Add(newSegment);
      frlist.Remove(temp);
      temp->length=requiredLength;
      cout<<newSegment<<endl;
      temp->is_free=false;
      Addr p= newSegment;
      return p;
    }
    temp= temp->next;
  }

  return nullptr;


}


  // if(start_addr+_length <= available){
  //   char * temp= start_addr;
  //   start_addr+=_length;
  //   return temp;
  // }
  // else return NULL;
  

bool MyAllocator::Free(Addr _a) {

  // This empty implementation just uses C standard library free
  //std::free(_a);
  return true;
}

