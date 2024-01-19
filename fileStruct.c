#include "fileStruct.h"


int sizeofBody(){
    return sizeof(struct body)/sizeof(int);
}

int sizeofHeader(){
    return sizeof(struct header)/sizeof(int);
}

int dataSizeInt(int sizeOfData){
    return (int)((sizeOfData)/sizeof(int))+(sizeOfData%sizeof(int)>0);
}

int getAddr(int elem){
    if (curSegment != (int)(elem/0x00010000)){
        if(curSegment!=0)UnmapViewOfFile(addr);
        readSegment(elem, &addr);
    }
    return elem%0x00010000;
}

int getParAddr(int elem){
    if (curParSegment != (int)(elem/0x00010000)){
        if(curParSegment!=0)UnmapViewOfFile(addrParent);
        readSegment(elem, &addrParent);
    }
    return elem%0x00010000;
}

int getChildAddr(int elem){
    if (curChildSegment != (int)(elem/0x00010000)){
        if(curChildSegment!=0)UnmapViewOfFile(addrChild);
        readSegment(elem, &addrChild);
    }
    return elem%0x00010000;
}

void serialize(int* addrTo,struct body* elem){
    int mass[7]={elem->sizeOfElem,elem->type,elem->numOfChildren,elem->sizeOfElem,elem->parent,elem->data ,elem->dataSize};
    for(int i=0; i<7; i++) {
        *(addrTo + i) = mass[i];
    }
}

void deserialize(const int* addrOut, struct body* elem){
    elem->sizeOfElem=addrOut[0];
    elem->type=addrOut[1];
    elem->numOfChildren=addrOut[2];
    elem->sizeOfElem=addrOut[3];
    elem->parent=addrOut[4];
    elem->data=addrOut[5];
    elem->dataSize=addrOut[6];
}

void init(){
    struct body root = (struct body){sizeofBody()+NCOC,4,0,16,0,sizeofBody()+NCOC, 0};
    *rootSegment = sizeofHeader()+sizeofBody()+NCOC;
    serialize(addr+sizeofHeader(), &root);
    addBucket(sizeofHeader());
    addBucket(sizeofBody()+NCOC);
}

int addChild(int parent, int numOfChildren, int sizeOfData,int type, const char data[sizeOfData], struct answer* a){
    if(type == 4/*root element*/){
        char* result = "ADDING OBJECT: DEPRECATED TO CREATE NEW ROOT ELEMENT\n";
        int n=0;
        while(result[n]!='\n')a->sentence[a->sizeOfAnswer++]=result[n++];
        return -1;
    }
    int* addrOfStartParent=addr + getParAddr(parent);
    if(((struct body*) addrOfStartParent)->sizeOfElem<=numOfChildren){
        char* result = "ADDING OBJECT: PARENT HASN'T SO MANY CHILDREN\n";
        int n=0;
        while(result[n]!='\n')a->sentence[a->sizeOfAnswer++]=result[n++];
        return -1;
    }

    if(*(addrOfStartParent+sizeofBody()+numOfChildren))removeChild(*(addrOfStartParent+sizeofBody()+numOfChildren), a);
    ((struct body*)addrOfStartParent)->numOfChildren++;
    int sizeOfDataInInt = dataSizeInt(sizeOfData);
    int addrOfStartElem= addBucket(sizeofBody()+sizeOfDataInInt+NCOC);
    *(addrOfStartParent+sizeofBody()+numOfChildren)=addrOfStartElem;


    serialize(addr+getAddr(addrOfStartElem),&(struct body)
            {sizeofBody()+sizeOfDataInInt+NCOC,type,0,
                    16, parent, sizeofBody()+NCOC, sizeOfData});

    char* startData = (char*)(addr+ getAddr(addrOfStartElem)+sizeofBody()+NCOC);
    for(char i=0; i<(char)sizeOfData; i++){
        *(startData+i)=*(data+i);
    }
    *rootSegment+=sizeofBody()+sizeOfDataInInt+NCOC;
    return addrOfStartElem;
}


int addChildPlace(int parent){
    struct body* addrOfStartParent= (struct body *) (addr + getParAddr(parent));
    return addrOfStartParent->numOfChildren;
}


void dropChild(int elem, struct answer* a){
    struct body* obj= (struct body *) (addr + getAddr(elem));
    removeChild(elem, a);
    struct body* prn = (struct body*) (addr+getParAddr(obj->parent));
    for(int i=0; i<prn->numOfChildren; i++){
        if(*(addr+getParAddr(obj->parent)+sizeofBody()+i)==elem){
            prn->numOfChildren--;
            *(addr+getParAddr(obj->parent)+sizeofBody()+i)=*(addr+getParAddr(obj->parent)+sizeofBody()+prn->numOfChildren);
            *(addr+getParAddr(obj->parent)+sizeofBody()+prn->numOfChildren)=0;
            break;
        }
    }
}



void removeChild(int elem, struct answer* a){

    struct body* obj= (struct body *) (addr + getAddr(elem));
    for(int i=0; i<obj->numOfChildren; i++){
        removeChild(*(addr+getAddr(elem)+sizeofBody()+i), a);
        *(addr+getAddr(elem)+sizeofBody()+i)=0;
    }
    for(int i=obj->data; i<obj->sizeOfElem; i++){
        *(addr + getAddr(elem) + i) = 0;
    }
    addPiramidElem( obj->sizeOfElem,elem);
    *obj=(struct body){0,0,0,0,0, 0};
    a->success=1;
    a->index=elem;
}

void printOneElem(struct body elem, int i, struct answer* a){
    deserialize(addr+getAddr(i), &elem);
    char* result = "element , sizeoOfElem , type , numOfChild , parent , data , dataSize\n";
    int n=0;
    while(result[n]!='\n')a->sentence[a->sizeOfAnswer++]=result[n++];
    a->sentence[a->sizeOfAnswer++]='\n';
    intInAnswer(i, a);
    a->sentence[a->sizeOfAnswer++]='|';
    intInAnswer(elem.sizeOfElem,a);
    a->sentence[a->sizeOfAnswer++]='|';
    intInAnswer(elem.type,a);
    a->sentence[a->sizeOfAnswer++]='|';
    intInAnswer(elem.numOfChildren,a);
    a->sentence[a->sizeOfAnswer++]='|';
    intInAnswer(elem.sizeOfElem,a);
    a->sentence[a->sizeOfAnswer++]='|';
    intInAnswer(elem.parent,a);
    a->sentence[a->sizeOfAnswer++]='|';
    intInAnswer(elem.data,a);
    a->sentence[a->sizeOfAnswer++]='|';
    intInAnswer(elem.dataSize,a);
    a->sentence[a->sizeOfAnswer++]='\n';
    result = "children:\n";
    n=0;
    while(result[n]!='\n')a->sentence[a->sizeOfAnswer++]=result[n++];
    for(int j=0;j<elem.numOfChildren; j++ ){
        if(*(addr+getAddr(i)+sizeofBody()+j)){
            intInAnswer(*(addr+getAddr(i)+sizeofBody()+j),a);
            a->sentence[a->sizeOfAnswer++]='\n';
        }
        else {
            result ="NULL\n";
            n=0;
            while(result[n]!='\n')a->sentence[a->sizeOfAnswer++]=result[n++];
            a->sentence[a->sizeOfAnswer++]='\n';
        }
    }
    result = "data:\n";
    n=0;
    while(result[n]!='\n')a->sentence[a->sizeOfAnswer++]=result[n++];
    a->sentence[a->sizeOfAnswer++]='\n';
    int* startData = addr+getAddr(i);
    for(char* j= (char *) (startData + elem.data); j < (char*) (startData + elem.sizeOfElem); j++) {
        a->sentence[a->sizeOfAnswer++]=*j;
    }
    a->sentence[a->sizeOfAnswer++]='\n';
}

void printer(struct answer* a){
    int sizeOfFile = rootSegment[0];
    int i=sizeofHeader();
    while(i<sizeOfFile){
        while(!*(addr+getAddr(i))){
            i++;
            if(i>=sizeOfFile) return;
        }
        struct body elem={0};
        deserialize(addr+getAddr(i), &elem);
        printOneElem(elem,i, a);
        i+=elem.sizeOfElem;
    }
}