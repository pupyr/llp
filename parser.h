//
// Created by olega on 15.12.2023.
//

#ifndef LLP_PARSER_H
#define LLP_PARSER_H

#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <inttypes.h>
#include "fileStruct.h"
#include "transport.h"

struct schema{
    int s;
    char* name;
};

struct schema* map;
int sizeOfMap;
int numOfSchemas;
int mainTypes;

void addType(const char* str, int type, struct answer* a);
void addObj(const char* in, struct answer* a, int updOrIns);
void findInformation(const char* in, struct answer* a);
void initMap();
void saveMap();
void printMap(struct answer* a);
void intInAnswer(int i, struct answer* a);
#endif //LLP_PARSER_H
