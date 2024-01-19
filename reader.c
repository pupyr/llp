//
// Created by olega on 12.12.2023.
//

#include "reader.h"

void getAddressOfMapping(char* name){
    hFile = CreateFile(name,
                              GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE,
                              NULL//,CREATE_ALWAYS
                              ,OPEN_ALWAYS,
                              FILE_ATTRIBUTE_TEMPORARY,
                              NULL);
    mapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0x00000000, 0xf000000, NULL);
    addr = MapViewOfFile (mapping, FILE_MAP_ALL_ACCESS,
                          0, 0, 0x00010000);
    rootSegment=addr;
    curSegment=0;
}

void readSegment(int elem, int** address){
    int num = (int)(elem/0x00010000);
    *address = MapViewOfFile (mapping, FILE_MAP_ALL_ACCESS,
                          0, num*0x00010000, 0x00010000);
    curSegment=num;
}

void saveFile(){
    UnmapViewOfFile(addr);
    UnmapViewOfFile(addrChild);
    UnmapViewOfFile(addrParent);
    CloseHandle(hFile);
    CloseHandle(mapping);

}