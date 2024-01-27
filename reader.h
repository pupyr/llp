//
// Created by olega on 12.12.2023.
//

#ifndef LLP_READER_H
#define LLP_READER_H
#include <Windows.h>
#include <winbase.h>
long long int sizeOfMapping;
int* addr;
int* addrParent;
int* addrChild;
int* rootSegment;
HANDLE hFile;
HANDLE mapping;
int curSegment;
int curParSegment;
int curChildSegment;
void getAddressOfMapping(char* name);
void readSegment(int elem, int** address);
void saveFile();

#endif //LLP_READER_H
