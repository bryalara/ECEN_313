/* 
    File: free_list.cpp

    Author: <your name>
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the class FreeList.

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <iostream>
#include <assert.h>

#include "free_list.hpp"

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
/* FUNCTIONS FOR CLASS SegmentHeader */
/*--------------------------------------------------------------------------*/

SegmentHeader::SegmentHeader(size_t _length, bool _is_free) {
  length = _length;
  is_free = _is_free;
  cookie = COOKIE_VALUE;
  next= nullptr;
  prev= nullptr;
  // You may need to initialize more members here!
}

SegmentHeader::~SegmentHeader() {
  // You may need to add code here.
}


void SegmentHeader::CheckValid() {
  if (cookie != COOKIE_VALUE) {
    cout << "INVALID SEGMENT HEADER!!" << endl;
    assert(false);
    // You will need to check with the debugger to see how we got into this
    // predicament.
  }
}

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR CLASS FreeList */
/*--------------------------------------------------------------------------*/

FreeList::FreeList(int fib_size) {
  // You will need to add code here!
  header= nullptr;
  fibNum=fib_size;
}
SegmentHeader* FreeList::GetHeader(){
  return header;
}

FreeList::~FreeList() {
  // You may need to add code here.
}

bool FreeList::Add(SegmentHeader * _segment) {
  cout<<"Adding : "<<_segment <<" to freeList\n";
  if( header== nullptr && _segment!= nullptr){
    cout<<"Using add 1\n";
    header=_segment;
    // header->next=nullptr;
    // header->prev=nullptr;
    cout<<"Added\n";
    return true;
  }
  else if(header!= nullptr && _segment!= nullptr && header!=_segment   ){
    // if(header->prev== nullptr){
      cout<<"Using add 2\n";
      header->prev= _segment;
    // }
    // else{
    //   cout<< "Header prev = "<< header ->prev<< endl;
    //   cout<<"Using add 3\n";
    //   header->prev->next=_segment;
    //   _segment-> prev = header->prev;
    //   header->prev= _segment;
    // }
    _segment-> next = header;
    cout<< _segment<<endl;
    header= _segment;
    cout<<"Added\n";
    return true;
  }
  else{
    cout<<"Adding Failed!!\n";
    return false;
  }
  //assert(false); // This implementation does nothing, other than abort.
}

void FreeList::print(){
  SegmentHeader* temp= header;
  cout<<"Printing Addresses\n";
  int i=0;
  if(temp==nullptr){
    cout<<"FreeList is Empty\n";
  }
  while(temp != nullptr && i<=10){
    cout<< "Address "<< i <<": " << temp <<" length = "<<temp->length<< endl;
    i++;
    temp= temp->next;
  }
}

bool FreeList::Remove(SegmentHeader * _segment) {
  SegmentHeader* temp=header;
  cout<<"Removing : "<<_segment<<"...\n";
  while(temp != nullptr){
    cout<<"Temp="<<temp<<" \nSegment: "<<_segment<<endl;
    if(temp->next== nullptr && temp->prev == nullptr && temp ==_segment  ){
      cout<<"Using remove 1\n";
      header->next=nullptr;
      header->prev=nullptr;

      header=nullptr;
      cout<<"removed\n";
      return true;
    }
    else if(temp->next!= nullptr && temp->prev == nullptr && temp ==_segment  ){
      cout<<"Using remove 2\n";
      header= temp->next;
      header->prev=nullptr;
      cout<<"removed\n";
      return true;
    }
    else if(temp->next== nullptr && temp->prev != nullptr && temp ==_segment  ){
      cout<<"Using remove 3\n";
      temp->prev->next= temp->next;
      cout<<"removed\n";
      return true;
    }
    else if(temp->next!= nullptr && temp->prev != nullptr && temp ==_segment  ){
      cout<<"Using remove 4\n";
      _segment->prev->next= _segment->next;
      _segment->next->prev= _segment->prev;
      cout<<"removed\n";
      return true;\
    
    }
      
    temp=temp->next;
  }
  cout<<"Could not remove\n";
  return false;


  //assert(false); // This implementation does nothing, other than abort.
}
