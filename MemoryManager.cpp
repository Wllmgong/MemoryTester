#include "MemoryManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <iostream>
//William Gong
namespace MemoryManager
{
  // IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT 
  //
  // This is the only static memory that you may use, no other global variables may be
  // created, if you need to save data make it fit in MM_pool
  //
  // IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT

//Didnt have time to test the project, had a midterm and computer science project at the same week as this application
//I attempted to comment step by step of my thinking process throughout this project
//This is my first project in C++, it took longer than I would've liked and there may be linguring syntax errors 
//Thank you for considering my application

  const int MM_POOL_SIZE = //////;
  char MM_pool[MM_POOL_SIZE];

  // Initialize set up any data needed to manage the memory pool
  //[65536\0] 
  //max space takes up 6 bits
  //[6 bit size of the following allocated block][aSize size of allocated block][6 bit size of the free space before the next block]
  //everytime allocating a new memory block, addition 6 bits is required to allocate 
  //after the allocated block, addditional 6 is created to show the free space.
  void initializeMemoryManager(void)
  {
    std::strcpy(&MM_pool[0], "65536\0");
  }

  // return a pointer inside the memory pool
  // If no chunk can accommodate aSize call onOutOfMemory()
  //[6 bit size of the following allocated block][aSize size of allocated block][6 bit size of the free space before the next block]
  void* allocate(int aSize)
  {
    // checks size is applicable 
    if(aSize < 0 || aSize > MM_POOL_SIZE) {
      onIllegalOperation("%d is not a valid allocation size.", aSize);
      return ((void*) 0);
    }
    
    int loc = 0;
    int temp = 0;

    //the while loop goes through the memory pool array to find 
    //if the location reaches the end, then out of memory
    while( loc < MM_POOL_SIZE) {
      char next[6];
      memcpy(&next, &MM_pool[temp], 6); 
      int k = atoi(next);   //extract the first 6 in the memory
      if(k < 0) {
        int alloc = (-1) * k;
        loc = loc + alloc + 6;
      } else {
        
        if(k >= aSize + 6) {  // checks if free space is large enough to allocate the new memory block
          
          int nextFree = k - aSize - 6;
          snprintf(next, sizeof(next), "%d", (-1) * aSize);
          //itoa((-1) * aSize, next, 10);
          std::strcpy(&MM_pool[loc], next);       //set to negative to show the following spaces are allocated
          if(nextFree > 6) {
            snprintf(next, sizeof(next), "%d", nextFree);
            //itoa(nextFree, next, 10);
            std::strcpy(&MM_pool[loc+aSize+1], next);   //to show the size of the spaces after the allocated space is free
          } 
        }
          return (void *)&MM_pool[loc+6];                          //return the allocated location, 
      }

      
    }

    onOutOfMemory(); //out of memory

    return ((void*) 0);
    
  }

  // Free up a chunk previously allocated
  void deallocate(void* aPointer)
  {
    char next[6];
    int *loc = static_cast<int*>(aPointer);                    //finds the location to be deallocated
                                                               //the size of the allocated space 
    memcpy(&next, &MM_pool[*loc], 6);        //negative says the following space is allocaed
    int size = (-1) * atoi(next);
    int free = 0;
    int freeAfter = 0;
    if(*loc + size + 8 < MM_POOL_SIZE) {                //checks to see if there are free space after the allocated space,
                                                      //if so combine the two. 
      memcpy(&next, &MM_pool[*loc + size + 1], 6);
      freeAfter = atoi(next);
      if (freeAfter > 0) free = freeAfter;
    }
    free = free + size + 6;                           //the total free space

    memset(&MM_pool[*loc - 6], '\0', size + 6);        //set the allocated spaces to null

    //need to implement previous free space
    int travel = 0;
    int temp = 0;
    int prev = 0;

    memcpy(&next, &MM_pool[travel], 6);
    travel = atoi(next);                   //find the previous free space, if it exists

    if(travel < 0) {
      travel = (-1) * travel;                            //negative says the following space is allocaed
    }

    while(travel < *loc) {                                //travels through the memory pool and to the the previous free space or allocated space
      prev = temp;

      memcpy(&next, &MM_pool[travel], 6);
      travel = atoi(next);
      temp = travel;
      if(travel < 0) {
        travel = (-1) * travel;        //negative says the following space is allocaed
      }
    }

    if(prev > 0) {                                      //if the previous free space exists, the add to the prev location free space
      memcpy(&next, &MM_pool[prev], 6);
      int preFree = atoi(next);

      snprintf(next, sizeof(next), "%d", free+preFree);
      //itoa(free+preFree, next, 10);
      std::strcpy(&MM_pool[prev], next);
    } else {
      snprintf(next, sizeof(next), "%d", free);
      //itoa(free, next, 10);
      std::strcpy(&MM_pool[*loc], next);                //set the free space available 
    }
  }

  //---
  //--- support routines
  //--- 

  // Will scan the memory pool and return the total free space remaining
  int freeRemaining(void)
  {
    int travel = 0;
    int temp = 0;
    //int prev = 0;
    int freeTotal = 0;
    char next[6];

    memcpy(&next, &MM_pool[travel], 6);
    temp = atoi(next);                   //find the free spaces, if it exists

    if(temp < 0) {
      temp = (-1) * temp;
      
    } else {
      freeTotal += temp;
      
    }
    travel+= temp;
                              //adds to the free spaces together 
    while(travel < MM_POOL_SIZE) {
     // prev = temp;
      memcpy(&next, &MM_pool[travel], 6);
      temp = atoi(next);
      if(temp < 0) {
        temp = (-1) * temp;
      
      } else {
        freeTotal = temp;
      }
      travel+= temp;
    }
    return freeTotal;
  }

  // Will scan the memory pool and return the largest free space remaining
  int largestFree(void)
  {
    int travel = 0;
    int temp = 0;
    //int prev = 0;
    int freeTotal = 0;
    char next[6];

    
    memcpy(&next, &MM_pool[travel], 6);
    temp = atoi(next);                   //find the free spaces, if it exists

    if(temp < 0) {
      temp = (-1) * temp;
      
    } else {
      freeTotal = temp;
      
    }
    travel+= temp;

      //finds the largest free space by traveling through the rest of memory pool
    while(travel < MM_POOL_SIZE) {
      //prev = temp;

      memcpy(&next, &MM_pool[travel], 6);
      temp = atoi(next);                   //find the free spaces, if it exists

      if(temp < 0) {
        temp = (-1) * temp;
      
      } else {
        if(freeTotal < travel) freeTotal = travel;
      }
    travel+= temp;
    }

    return freeTotal;
  }

  // will scan the memory pool and return the smallest free space remaining
  int smallestFree(void)
  {
    int travel = 0;
    int temp = 0;
    //int prev = 0;
    int freeTotal = 0;
    char next[6];
    
    memcpy(&next, &MM_pool[travel], 6);
    temp = atoi(next);                   //find the free spaces, if it exists

    if(temp < 0) {
      temp = (-1) * temp;
      
    } else {
      freeTotal = temp;
      
    }
    travel+= temp;
                              //finds the smallest free space
    while(travel < MM_POOL_SIZE) {
      //prev = temp;

      memcpy(&next, &MM_pool[travel], 6);
      temp = atoi(next);                   //find the free spaces, if it exists

      if(temp < 0) {
        temp = (-1) * temp;
      
      } else {
        if(freeTotal > travel) freeTotal = travel;
      }
    travel+= temp;
    }
    return freeTotal;

  }
 }