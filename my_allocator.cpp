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
  int requiredLength, index;
  int min= _length+ sizeof(SegmentHeader);
  int rounded_up_required;
  int indicator=0;
  SegmentHeader* available= nullptr;
  
  while (true)
  {
    for(int i=0; i< frlist.size(); i++){
      
      FreeList curr_fr= frlist[i];
      if(curr_fr.fibNum * blockSize == min){
        cout<<"Index equal: "<<i<<endl;
        rounded_up_required= curr_fr.fibNum*blockSize;
        if(curr_fr.header!=nullptr){
          curr_fr.Remove(curr_fr.header);
          return curr_fr.header;
        }
      }
      else if(curr_fr.fibNum*blockSize > min){
        if(indicator==0){
          if(i>1){
            min=frlist[i-1].fibNum*blockSize;
          }
          else if(i==1)
          {
            min=frlist[i+1].fibNum*blockSize;
          }
          else
          {
            min=frlist[i+2].fibNum*blockSize;
          }
          
          
          indicator++;
        }
        cout<<"Index greater: "<<i<<endl;
        cout<<"Fibnum: "<<curr_fr.fibNum<<endl;
        cout<<curr_fr.fibNum*blockSize <<",  "<<min<<endl;
        if(curr_fr.header!= nullptr){
          index=i;
          available=curr_fr.header;
          cout<<"breaking\n";
          break;
        }
      }
    }
    if(available == nullptr){
      cout<<"No available found"<<endl;
      cout<< "Out of memory\n";
      return nullptr;
    }

    split(available,index);
  }
  


  // int blockRemain= (min) % blockSize ;

  // if(blockRemain==0){
  //   requiredLength= min;
  // }
  // else{
  //   requiredLength= min+blockSize-blockRemain ;
  // }
  
  // SegmentHeader* temp= frlist.GetHeader();
  
  // while(temp != nullptr){
  //   frlist.print();
  //   if(temp->length >= requiredLength /*&& temp->is_free==true*/){
  //     size_t newLength= temp->length - requiredLength /*- sizeof(SegmentHeader)*/;
  //     SegmentHeader* newSegment = new(/*start_addr*/ temp + requiredLength /*+ sizeof(SegmentHeader)*/) SegmentHeader(newLength /*- sizeof(SegmentHeader)*/, true);
  //     frlist.Add(newSegment);
      
  //   frlist.print();
  //     temp->length=requiredLength;
  //     temp->is_free=false;
  //     frlist.Remove(temp);

  //   frlist.print();
  //     Addr p= newSegment;
  //     cout<< "p "<<p<<endl;
  //     return p;
  //   }
  //   temp= temp->next;
  // }

  return nullptr;


}
void MyAllocator::split(SegmentHeader* available, int index){
  int index1= index -1;
  int index2= index -2;

  frlist[index].Remove(available);
  cout<<"hello\n";
  SegmentHeader* newSeg= new(available+ frlist[index2].fibNum*blockSize) SegmentHeader(frlist[index1].fibNum*blockSize,true, 1, available->INH);

  cout<<"bye\n";
  available->INH=available->LR;
  available->LR=0;
  available->length=frlist[index2].fibNum*blockSize;
  frlist[index2].Add(available);
  frlist[index1].Add(newSeg);

}
void MyAllocator::initFrlist(size_t _size){
  int fib_prev=0;
  int fib_prev1=1;
  int fib_current=fib_prev1+fib_prev;
  int index=-1;

  int fib_increment=fib_current*blockSize ;

  cout<<"Initializing...\n";
  while(fib_increment <= _size ){
    FreeList fr(fib_current);
    frlist.push_back(fr);
    index++;
    
    fib_increment= blockSize*(fib_current+fib_prev1);
    fib_current=fib_current+fib_prev1;
    fib_prev1=fib_current;
  }

  SegmentHeader* newSegment = new(start_addr) SegmentHeader(fib_increment, true);
  frlist[index].Add(newSegment);
  cout<<"Fr_list initialized, at "<<index<<" Size = "<<frlist.size()<<"\n";
}



void MyAllocator::merge(SegmentHeader * _segment, int index) {
  SegmentHeader* sh= nullptr;
  bool side= _segment->LR;
  if(side && index>0){
    //we have a right buddy go down 1 in the free list and find the left buddy
    sh = frlist[index-1].header;    
  }
  else{
    //we have a left buddy go up 1 in the free list and find the right buddy
    sh = frlist[index+1].header;
  }
  while(sh !=nullptr){
    if(sh->LR==!side){
      //we found a compatible buddy
      //now we need to check if the address difference is equal to the size if them merging
      int dif = ((sh)+sh->length) - _segment;
      int mergeSize= sh->length+_segment->length;
      if(dif==mergeSize){
        frlist[index].Remove(_segment);
        frlist[index].Remove(sh);
        if(sh->LR==0){
          sh->length= mergeSize;
          sh->LR=sh->INH;
          sh->INH= _segment->INH;
          }
        else
        {
          _segment->length=mergeSize;
          _segment->length= mergeSize;
          _segment->LR=_segment->INH;
          _segment->INH= sh->INH;
          
        } 
        break;       
      }
      else{
        //mergesize and address difference dont match
        cout<<"Mergesize: "<<mergeSize<<" AddressDiff: "<< dif;
      }
    }
    sh=sh->next;
  }

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
  SegmentHeader* flh= (SegmentHeader*) ((char*)_a /*- sizeof(SegmentHeader)*/);
  flh->is_free=true;
  cout<<"flh = "<<flh<<endl;
  int index=-1;
  for(int i=0; i<frlist.size()-1; i++){
    
    if(frlist[i].fibNum*blockSize == flh->length){
      index=i;
    }
  }
  if(index == -1){
    cout<< "Could not free because segment length is not a fib*blocksize : "<<flh->length<<endl;
    return false;
  }
  else{
    frlist[index].Add(flh);
    frlist[index].print();
    cout<<"Is address "<<(char*)_a<<" added to freelist?\n"<<endl;
    merge(flh, index);
  }
  return true;
}

