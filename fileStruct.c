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
    if (curSegment != (int)(elem/(sizeOfMapping/4))){
        if(curSegment!=0)UnmapViewOfFile(addr);
        readSegment(elem, &addr);
    }
    curSegment=(int)(elem/(sizeOfMapping/4));
    return elem%(sizeOfMapping/4);
}

int getParAddr(int elem){
    if (curParSegment != (int)(elem/(sizeOfMapping/4))){
        if(curParSegment!=0)UnmapViewOfFile(addrParent);
        readSegment(elem, &addrParent);
    }
    curParSegment=(int)(elem/(sizeOfMapping/4));
    return elem%(sizeOfMapping/4);
}

int getChildAddr(int elem){
    if (curChildSegment != (int)(elem/(sizeOfMapping/4))){
        if(curChildSegment!=0)UnmapViewOfFile(addrChild);
        readSegment(elem, &addrChild);
    }
    curChildSegment=(int)(elem/(sizeOfMapping/4));
    return elem%(sizeOfMapping/4);
}

void serialize(int* addrTo,struct body* elem){
    int mass[7]={elem->sizeOfElem,elem->type,elem->numOfChildren,elem->maxSizeChildren,elem->parent,elem->data ,elem->dataSize};
    for(int i=0; i<7; i++) {
        *(addrTo + i) = mass[i];
    }
}

void deserialize(const int* addrOut, struct body* elem){
    elem->sizeOfElem=addrOut[0];
    elem->type=addrOut[1];
    elem->numOfChildren=addrOut[2];
    elem->maxSizeChildren=addrOut[3];
    elem->parent=addrOut[4];
    elem->data=addrOut[5];
    elem->dataSize=addrOut[6];
}

void init(){
    struct body root = (struct body){sizeofBody()+256,4,0,64,0,sizeofBody()+256, 0};
    *rootSegment = sizeofHeader()+sizeofBody()+256;
    serialize(addr+sizeofHeader(), &root);
    addBucket(sizeofHeader());
    addBucket(sizeofBody()+256);
}

int resizeChildrenSize(int elem){
    int newAddr = getAddr(elem);
    struct body* this = (struct body*)(addr+ newAddr);
    int sizeOfDataInInt = dataSizeInt(this->dataSize);
    int addrOfStartElem= addBucket(sizeofBody()+sizeOfDataInInt+this->maxSizeChildren*2);
    int newParAddr = getParAddr(this->parent);
    struct body* par = (struct body*)(addr+ newParAddr);
    for(int i=0; i<par->numOfChildren; i++){
        if(*(addrParent + newParAddr+sizeofBody()+i)==elem){
            *(addrParent + newParAddr+sizeofBody()+i) = addrOfStartElem;
            break;
        }
    }

    for(int i=0; i<this->numOfChildren; i++){
        int newChildAddr =getChildAddr(*(((int*)this)+sizeofBody()+i));
        ((struct body*)(addrChild+newChildAddr))->parent= addrOfStartElem;
    }
    newAddr = getAddr(addrOfStartElem);
    struct body* new = (struct body*) (addr+ newAddr);
    *new = *this;
    new->data+=this->maxSizeChildren;
    new->sizeOfElem+=this->maxSizeChildren;
    new->maxSizeChildren*=2;
    for(int i=0; i<this->numOfChildren; i++){
        *(((int*)new)+sizeofBody()+i) = *(((int*)this)+sizeofBody()+i);
        *(((int*)this)+sizeofBody()+i)=0;
    }
    for(int i=0; i<this->sizeOfElem-this->data; i++){
        *(((int*)new)+sizeofBody()+new->maxSizeChildren+i)=*(((int*)this)+sizeofBody()+this->maxSizeChildren+i);
        *(((int*)this)+sizeofBody()+this->maxSizeChildren+i)=0;
    }
//    addPiramidElem(this->sizeOfElem, elem);
    *this = (struct body){0};
    *rootSegment+=new->sizeOfElem;
    return addrOfStartElem;
}

int addChild(int parent, int numOfChildren, int sizeOfData,int type, const char data[sizeOfData], struct answer* a){
    if(type == 4/*root element*/){
        char* result = "ADDING OBJECT: DEPRECATED TO CREATE NEW ROOT ELEMENT\n";
        int n=0;
        while(result[n]!='\n')a->sentence[a->sizeOfAnswer++]=result[n++];
        return -1;
    }
    int newParAddr = getParAddr(parent);
    int* addrOfStartParent=addrParent + newParAddr;
    struct body* par = ((struct body*)addrOfStartParent);
//    if(par->maxSizeChildren<=numOfChildren){
//        parent = resizeChildrenSize(parent);
//        a->reserveIndex=parent;
//        addrOfStartParent = addrParent + newParAddr;
//        par = ((struct body*)addrOfStartParent);
//    }

//    if(*(addrOfStartParent+sizeofBody()+numOfChildren)) {
//        dropChild(*(addrOfStartParent + sizeofBody() + numOfChildren), a);
//    }
    par->numOfChildren++;
    int sizeOfDataInInt = dataSizeInt(sizeOfData);
    int addrOfStartElem= addBucket(sizeofBody()+sizeOfDataInInt+NCOC);

    *(addrOfStartParent+sizeofBody()+numOfChildren)=addrOfStartElem;

    int newAddr = getAddr(addrOfStartElem);
    serialize(addr+newAddr,&(struct body)
            {sizeofBody()+sizeOfDataInInt+NCOC,type,0,
             NCOC, parent, sizeofBody()+NCOC, sizeOfData});

    char* startData = (char*)(addr+ newAddr+sizeofBody()+NCOC);
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
    int newAddr = getAddr(elem);
    struct body obj= *((struct body* ) (addr + newAddr));
    int newParAddr = getParAddr(obj.parent);
    removeChild(elem, a);
    struct body* prn = (struct body*) (addr+getParAddr(obj.parent));
    for(int i=0; i<prn->numOfChildren; i++){
        if(*(addrParent + newParAddr+sizeofBody()+i)==elem){
            prn->numOfChildren--;
            *(addrParent + newParAddr+sizeofBody()+i)=*(addrParent + newParAddr+sizeofBody()+prn->numOfChildren);
            *(addrParent + newParAddr+sizeofBody()+prn->numOfChildren)=0;
            break;
        }
    }
}



void removeChild(int elem, struct answer* a){
    int newAddr=getAddr(elem);
    struct body* obj= (struct body *) (addr + newAddr);
    for(int i=0; i<obj->numOfChildren; i++){
        removeChild(*(addr+newAddr+sizeofBody()+i), a);
        *(addr+newAddr+sizeofBody()+i)=0;
    }
    for(int i=obj->data; i<obj->sizeOfElem; i++){
        *(addr + newAddr + i) = 0;
    }
//    addPiramidElem( obj->sizeOfElem,elem);
    *obj=(struct body){0,0,0,0,0, 0};
    a->success=1;
    a->index=elem;
}

void printOneElem(struct body elem, int i, struct answer* a){
    int newAddr = getAddr(i);
    deserialize(addr+newAddr, &elem);
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
    intInAnswer(elem.maxSizeChildren,a);
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
        if(*(addr+newAddr+sizeofBody()+j)){
            intInAnswer(j,a);
            a->sentence[a->sizeOfAnswer++]=':';
            intInAnswer(*(addr+newAddr+sizeofBody()+j),a);
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
    int* startData = addr+newAddr;
    for(char* j= (char *) (startData + elem.data); j < (char*) (startData + elem.sizeOfElem); j++) {
        a->sentence[a->sizeOfAnswer++]=*j;
    }
    a->sentence[a->sizeOfAnswer++]='\n';
}

void printer(struct answer* a){
    int sizeOfFile = rootSegment[0];
    int i=sizeofHeader();
    while(i<sizeOfFile){
        int newAddr=getAddr(i);
        while(!*(addr+newAddr)){
            i++;
            newAddr=getAddr(i);
            if(i>=sizeOfFile) return;
        }
        struct body elem={0};
        deserialize(addr+newAddr, &elem);
        printOneElem(elem,i, a);
        i+=elem.sizeOfElem;
    }
}