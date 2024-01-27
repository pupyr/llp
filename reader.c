//
// Created by olega on 12.12.2023.
//

#include <stdio.h>
#include "reader.h"

long long int sizeOfMapping = 0x10000000;

void getAddressOfMapping(char* name){
    hFile = CreateFile(name,
                       GENERIC_READ | GENERIC_WRITE,
                       FILE_SHARE_READ | FILE_SHARE_WRITE,
                       NULL//,CREATE_ALWAYS
            ,OPEN_ALWAYS,
                       FILE_ATTRIBUTE_TEMPORARY,
                       NULL);
    mapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0x00000004, 0x00000000, NULL);
    addr = MapViewOfFile (mapping, FILE_MAP_ALL_ACCESS,
                          0, 0, sizeOfMapping);
    addrChild=addr;
    addrParent=addr;
    rootSegment=addr;
    curSegment=0;
}

void readSegment(int elem, int** address){
    int num = (int)(elem/(sizeOfMapping/4));
    if(num){
        printf("!23");
    }
    *address = MapViewOfFile (mapping, FILE_MAP_ALL_ACCESS,
                              0, num*sizeOfMapping, sizeOfMapping);
    if(num){
        printf("!23");
    }
}

void saveFile(){
    UnmapViewOfFile(addr);
    UnmapViewOfFile(addrChild);
    UnmapViewOfFile(addrParent);
    CloseHandle(hFile);
    CloseHandle(mapping);

}