//
// Created by olega on 09.01.2024.
//

#ifndef LLP_ALLOC_H
#define LLP_ALLOC_H

#include <stdio.h>
#include <malloc.h>
#include "fileStruct.h"
#include "reader.h"

struct piramidElem* piramid;
//char* alloc;

struct piramidElem{
    int size;
    int place;
};

int sizeofPiramidStruct();

int getPirParent(int id);
int getPirChild(int id);

//void initAllocator();
void initPiramid();
int addBucket(int size);
//void dropBucket(int place);
void addPiramidElem(int size, int place);
//void printAlloc();
void printPiramid(struct answer* a);
void saveAlloc();
#endif //LLP_ALLOC_H
