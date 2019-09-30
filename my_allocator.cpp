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
  //initialize frlist vector
  initFrlist(_size);// also creates segment header and puts it into frlist.

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
    frlist.print();
    if(temp->length >= requiredLength /*&& temp->is_free==true*/){
      size_t newLength= temp->length - requiredLength /*- sizeof(SegmentHeader)*/;
      SegmentHeader* newSegment = new(/*start_addr*/ temp + requiredLength /*+ sizeof(SegmentHeader)*/) SegmentHeader(newLength /*- sizeof(SegmentHeader)*/, true);
      frlist.Add(newSegment);
      
    frlist.print();
      temp->length=requiredLength;
      temp->is_free=false;
      frlist.Remove(temp);

    frlist.print();
      Addr p= newSegment;
      cout<< "p "<<p<<endl;
      return p;
    }
    temp= temp->next;
  }

  return nullptr;


}

void MyAllocator::initFrlist(size_t _size){
  int fib_prev=0;
  int fib_prev1=1;
  int fib_current=fib_prev1+fib_prev;
  int index=-1;

  int fib_increment=fib_current*blockSize ;
  while(fib_increment <= _size){
    FreeList fr(fib_increment);
    frlist.push_back(fr);
    index++;
    fib_increment= blockSize*(fib_current+fib_prev1);
  }

  SegmentHeader* newSegment = new(start_addr) SegmentHeader(fib_increment, true);
  frlist[index].Add(newSegment);

}


  // if(start_addr+_length <= available){
  //   char * temp= start_addr;
  //   start_addr+=_length;
  //   return temp;
  // }
  // else return NULL;
  

bool MyAllocator::Free(Addr _a) {
  cout<<"Freeing address: "<<_a<<endl<<endl;
  // This empty implementation just uses C standard library free
  SegmentHeader* flh= (SegmentHeader*) ((char*)_a - sizeof(SegmentHeader));
  flh->is_free=true;
  cout<<"flh = "<<flh<<endl;;
  frlist.Add(flh);
  frlist.print();
  cout<<"Is address "<<(char*)_a<<" added to freelist?\n"<<endl;
  return true;
}

