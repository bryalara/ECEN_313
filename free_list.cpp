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

FreeList::FreeList() {
  // You will need to add code here!
  header= nullptr;
}
SegmentHeader* FreeList::GetHeader(){
  return header;
}

FreeList::~FreeList() {
  // You may need to add code here.
}

bool FreeList::Add(SegmentHeader * _segment) {
  if( header== nullptr && _segment!= nullptr){
    header=_segment;
    header->next=nullptr;
    header->prev=nullptr;
    return true;
  }
  else if(header!= nullptr && _segment!= nullptr){
    if(header->next== nullptr){
      header->next= _segment;
    }
    else{
      header->next->prev=_segment;
      _segment-> next = header->next;
      header->next= _segment;
    }
    _segment-> prev = header;

    header= _segment;
    return true;
  }
  else{
    return false;
  }
  //assert(false); // This implementation does nothing, other than abort.
}

bool FreeList::Remove(SegmentHeader * _segment) {
  SegmentHeader* temp=header;
  while(temp != nullptr){
    if(temp->next== nullptr && temp->prev == nullptr && temp ==_segment  ){
      header=nullptr;
      header->next=nullptr;
      header->prev=nullptr;
      return true;
    }
    else if(temp->next!= nullptr && temp->prev == nullptr && temp ==_segment  ){
      header= temp->next;
      header->prev=nullptr;
      return true;
    }
    else if(temp->next!= nullptr && temp->prev != nullptr && temp ==_segment  ){
      _segment->prev->next= _segment->next;
      _segment->next->prev= _segment->prev;
      return true;
    
    }
      
    temp=temp->next;
  }
  return false;


  //assert(false); // This implementation does nothing, other than abort.
}
