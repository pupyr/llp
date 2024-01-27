//
// Created by olega on 13.12.2023.
//

#ifndef LLP_FILESTRUCT_H
#define LLP_FILESTRUCT_H

#define NCOC 16

#include <stdio.h>
#include <stddef.h>
#include "alloc.h"
#include "parser.h"

struct header{
    int sizeOfFile;
    int sizeOfPiramid;
    int numPiramidElem;
    int sizeOfMap;
    int numOfMapSchemas;
    int sizeOfMapInInt;
};

struct body{
    int sizeOfElem;
    int type;
    int numOfChildren;
    int maxSizeChildren;
    int parent;
    //children
    int data;
    int dataSize;
};

int sizeofHeader();
int sizeofBody(); // возможно стоит перевести в define
int dataSizeInt(int sizeOfData);

void serialize(int* addr,struct body* elem);
void deserialize(const int* addr, struct body* elem);

void init();

int addChild(int parent, int numOfChildren, int sizeOfData,int type, const char data[sizeOfData], struct answer* a);
void dropChild(int elem, struct answer* a);
void removeChild(int elem, struct answer* a);
int addChildPlace(int parent);
//int updateChild(int elem, int size, const char* str);
void printOneElem(struct body elem, int i, struct answer* a);
void printer(struct answer* a);
int getAddr(int elem);
int getChildAddr(int elem);
#endif //LLP_FILESTRUCT_H
