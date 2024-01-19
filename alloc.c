#include "alloc.h"

int numOfElemInPiramid=0;
int sizeOfPiramid=16;
int sizeOfAlloc = 1024;

int sizeofPiramidStruct(){
    return sizeof(struct piramidElem)/sizeof(int);
}

void resizePiramid(){
    struct piramidElem* nw = (struct piramidElem*) malloc(sizeof(int)*sizeOfPiramid*2);
    for(int i=0; i<sizeOfPiramid; i++){
        nw[i]=piramid[i];
    }
    sizeOfPiramid*=2;
    free(piramid);
    piramid=nw;
}

void initPiramid(){
    numOfElemInPiramid=rootSegment[2];
    int size = rootSegment[1];
    if(size==0)size=16;
    sizeOfPiramid=size;
    piramid = (struct piramidElem*) malloc(sizeof(struct piramidElem)*size);
    for(int i=0; i < size; i++){
        piramid[i]=*(((struct piramidElem*)(addr+ getAddr(rootSegment[0])))+i);
        *(((struct piramidElem*)(addr+ getAddr(rootSegment[0])))+i)=(struct piramidElem){0,0};
    }
    if(!rootSegment[2]) {
        piramid[numOfElemInPiramid++] = (struct piramidElem) {sizeOfAlloc, 0};
        rootSegment[2]++;
    }
    else numOfElemInPiramid=rootSegment[2];
    rootSegment[1]=size;
}

//void initAllocator(){
//    int size=rootSegment[1];
//    if(size==0)size=1024;
//    alloc = (char*) malloc(sizeof(char)*size);
//    for(int i=0; i < size; i++){
//        alloc[i]=*((char*)(addr+ getAddr(rootSegment[0]))+i);
//        *((char*)(addr+ getAddr(rootSegment[0]))+i)=0;
//    }
//    sizeOfAlloc=size;
//    initPiramid(rootSegment[2]);
//}

int getPirParent(int id){return (int)((id-1)/2);}
int getPirChild(int id){return id*2+1;}

void addPiramidElem(int size, int place){
    if(numOfElemInPiramid+1==sizeOfPiramid)resizePiramid();
    piramid[numOfElemInPiramid]=(struct piramidElem){size,place};
    int id=numOfElemInPiramid++;
    while( id!=0 && piramid[getPirParent(id)].size < piramid[id].size){
        struct piramidElem var = piramid[getPirParent(id)];
        piramid[getPirParent(id)]=piramid[id];
        piramid[id] = var;
        id=getPirParent(id);
    }
}

//void resizeAlloc(){
//    char* nw = (char*) malloc(sizeof(char)*sizeOfAlloc*2);
//    for(int i=0; i<sizeOfAlloc; i++){
//        nw[i]=alloc[i];
//    }
//    for(int i=sizeOfAlloc; i<2*sizeOfAlloc; i++){
//        nw[i]=0;
//    }
//    if(numOfElemInPiramid+1==sizeOfPiramid)resizePiramid();
//    addPiramidElem(sizeOfAlloc,sizeOfAlloc);
//    sizeOfAlloc*=2;
//    free(alloc);
//    alloc=nw;
//}

void removePiramidElem(){
    piramid[0]=piramid[--numOfElemInPiramid];
    piramid[numOfElemInPiramid+1]=(struct piramidElem){0,0};
    int id=0;
    while(getPirChild(id)<numOfElemInPiramid && (piramid[getPirChild(id)].size > piramid[id].size || piramid[getPirChild(id)+1].size > piramid[id].size)){
        int idSwap = getPirChild(id);
        if(piramid[getPirChild(id)].size <= piramid[id].size ||
        piramid[getPirChild(id)].size<piramid[getPirChild(id)+1].size) idSwap++;
        struct piramidElem var = piramid[idSwap];
        piramid[idSwap]=piramid[id];
        piramid[id] = var;
        id = idSwap;
    }
}

int addBucket(int size){
//    while(piramid[0].size<=size)resizeAlloc();
//    alloc[piramid[0].place]=2;
//    for(int i=1; i<size; i++){
//        alloc[piramid[0].place+i]=1;
//    }
    int result = piramid[0].place;
    int elseSize = piramid[0].size-size;
    int elsePlace = piramid[0].place+size;
    removePiramidElem();
    addPiramidElem(elseSize,elsePlace);
    return result;
}

//void dropBucket(int place){
//    int i=1;
//    if(alloc[place]!=2){
//        printf("DROP ALLOC BUCKET: THIS PLACE ISN'T START OF THE BUCKET\n");
//        return;
//    }
//    alloc[place++]=0;
//    while(alloc[place]==1){
//        alloc[place++]=0;
//        i++;
//    }
//    addPiramidElem(i,place);
//}

void saveAlloc(){
//    for(int i=0; i<sizeOfAlloc; i++){
//        *((char*)(addr+ getAddr(rootSegment[0]))+i)=alloc[i];
//    }
//    rootSegment[1]=sizeOfAlloc;
    for(int i=0; i<sizeOfPiramid; i++){
        *(((struct piramidElem*)(addr+ getAddr(rootSegment[0])))+i)=piramid[i];
    }
    rootSegment[1]=sizeOfPiramid;
    rootSegment[2]=numOfElemInPiramid;
//    free(alloc);
    free(piramid);
}

//void printAlloc(){
//    int s=0;
//    int i=0;
//    while(i<sizeOfAlloc){
//        while(alloc[i]==0){
//            s++;
//            i++;
//        }
//        if(s){
//            printf("Free block of size %d\n", s);
//            s=0;
//        }
//        while(alloc[++i]==1)s++;
//        if(s){
//            printf("Busy block of size %d\n", s+1);
//            s=0;
//        }
//    }
//}

void printPiramid(struct answer* a){
    for(int i=0; i<numOfElemInPiramid; i++){
        char* result = "Bucket , size , place in alloc - \n";
        int n=0;
        while(result[n]!='\n')a->sentence[a->sizeOfAnswer++]=result[n++];
        intInAnswer(i, a);
        a->sentence[a->sizeOfAnswer++]=' ';
        intInAnswer(piramid[i].size, a);
        a->sentence[a->sizeOfAnswer++]=' ';
        intInAnswer(piramid[i].place, a);
        a->sentence[a->sizeOfAnswer++]='\n';
    }
}

