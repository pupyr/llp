#include "alloc.h"

//int numOfElemInPiramid=0;
//int sizeOfPiramid=32;
//int sizeOfAlloc = 202400;
//
//int sizeofPiramidStruct(){
//    return sizeof(struct piramidElem)/sizeof(int);
//}
//
//void resizePiramid(){
//    struct piramidElem* nw = (struct piramidElem*) malloc(sizeof(int)*sizeOfPiramid*2);
//    for(int i=0; i<sizeOfPiramid; i++){
//        nw[i]=piramid[i];
//    }
//    sizeOfPiramid*=2;
//    rootSegment[1]*=2;
//    free(piramid);
//    piramid=nw;
//}
//
//void initPiramid(){
//    numOfElemInPiramid=rootSegment[2];
//    int size = rootSegment[1];
//    if(size==0)size=16;
//    sizeOfPiramid=size;
//    piramid = (struct piramidElem*) malloc(sizeof(struct piramidElem)*size);
//    int newAddr = getAddr(rootSegment[0]);
//    for(int i=0; i < size; i++){
//        piramid[i]=*(((struct piramidElem*)(addr+newAddr ))+i);
//        *(((struct piramidElem*)(addr+ newAddr))+i)=(struct piramidElem){0,0};
//    }
//    if(!rootSegment[2]) {
//        piramid[numOfElemInPiramid++] = (struct piramidElem) {sizeOfAlloc, 0};
//        rootSegment[2]++;
//    }
//    else numOfElemInPiramid=rootSegment[2];
//    rootSegment[1]=size;
//}
//
//
//int getPirParent(int id){return (int)((id-1)/2);}
//int getPirChild(int id){return id*2+1;}
//
//void addPiramidElem(int size, int place){
//    if(numOfElemInPiramid+1==sizeOfPiramid)resizePiramid();
//    piramid[numOfElemInPiramid]=(struct piramidElem){size,place};
//    int id=numOfElemInPiramid++;
//    while( id!=0 && piramid[getPirParent(id)].size < piramid[id].size){
//        struct piramidElem var = piramid[getPirParent(id)];
//        piramid[getPirParent(id)]=piramid[id];
//        piramid[id] = var;
//        id=getPirParent(id);
//    }
//}
//
//void removePiramidElem(){
//    piramid[0]=piramid[--numOfElemInPiramid];
//    piramid[numOfElemInPiramid+1]=(struct piramidElem){0,0};
//    int id=0;
//    while(getPirChild(id)<numOfElemInPiramid && (piramid[getPirChild(id)].size > piramid[id].size || piramid[getPirChild(id)+1].size > piramid[id].size)){
//        int idSwap = getPirChild(id);
//        if(piramid[getPirChild(id)].size <= piramid[id].size ||
//           piramid[getPirChild(id)].size<piramid[getPirChild(id)+1].size) idSwap++;
//        struct piramidElem var = piramid[idSwap];
//        piramid[idSwap]=piramid[id];
//        piramid[id] = var;
//        id = idSwap;
//    }
//}
//
//int addBucket(int size){
//    if(numOfElemInPiramid+1==sizeOfPiramid)resizePiramid();
//    printf("%d %d\n",piramid[0].size, size);
//    if(size>=piramid[0].size) {
//        addPiramidElem(sizeOfAlloc, sizeOfAlloc);
//        sizeOfAlloc *= 2;
//    }
//    int result = piramid[0].place;
//    int elseSize = piramid[0].size-size;
//    int elsePlace = piramid[0].place+size;
//    removePiramidElem();
//    addPiramidElem(elseSize,elsePlace);
//    return result;
//}
//
//void saveAlloc(){
////    if(rootSegment[0]%0x4000) {
////        addPiramidElem(0x4000 - rootSegment[0] % 0x4000, rootSegment[0]);
////        rootSegment[0] = (((int) (rootSegment[0] / 0x4000)) + 1) * 0x4000;
////    }
//    for(int i=0; i<sizeOfPiramid; i++){
//        int newAddr = getAddr(rootSegment[0]);
//        *(((struct piramidElem*)(addr+ newAddr))+i)=piramid[i];
//    }
//    rootSegment[1]=sizeOfPiramid;
//    rootSegment[2]=numOfElemInPiramid;
//    free(piramid);
//}
//
//void printPiramid(struct answer* a){
//    for(int i=0; i<numOfElemInPiramid; i++){
//        char* result = "Bucket , size , place in alloc - \n";
//        int n=0;
//        while(result[n]!='\n')a->sentence[a->sizeOfAnswer++]=result[n++];
//        intInAnswer(i, a);
//        a->sentence[a->sizeOfAnswer++]=' ';
//        intInAnswer(piramid[i].size, a);
//        a->sentence[a->sizeOfAnswer++]=' ';
//        intInAnswer(piramid[i].place, a);
//        a->sentence[a->sizeOfAnswer++]='\n';
//    }
//}
//
int top=0;

void initPiramid(){
    rootSegment[1]=top;
}

int addBucket(int size){
    top+=size;
    return top-size;
}