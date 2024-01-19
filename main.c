#include "reader.h"
//#include "fileStruct.h"
#include "alloc.h"
//#include "parser.h"

int main() {
    getAddressOfMapping("text.txt");
    initPiramid();
    if(!addr[0])init();
    initMap();

//    int el = addChild(sizeofHeader(),4,0, "Oleg");
//
//    int elem2=addChild(sizeofHeader(),7,0, "Nastya!");
//    removeChild(47);
    addType("type a { 1 };");
    addType("type b { 5,5 };");
    addObj("add b (5,0);");
    addObj("add a (27,0);");
    addObj("add a (27,1);");
    addObj("add Float (49,0) 1.5;");
    addObj("add Float (71,0) 2.5;");
    findInformation("find a ( 0>1.0 );");

//    printer();
//    //printAlloc();
//    printPiramid();
//    printMap();
    saveAlloc();
    saveMap();
    return 0;
}