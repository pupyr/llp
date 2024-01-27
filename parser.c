//
// Created by olega on 15.12.2023.
//

#include "parser.h"

int sizeOfMapInInt = 0;

void resizeMap(){
    struct schema* nw = malloc(sizeof(struct schema)*sizeOfMap*2);
    for(int i=0; i<sizeOfMap; i++){
        nw[i]=map[i];
    }
    sizeOfMap*=2;
    free(map);
    map=nw;
}

void initMap(){
    mainTypes=6;
    if(!rootSegment[3])sizeOfMap=sizeof(struct schema)*8;
    else sizeOfMap=rootSegment[3];
    if(!rootSegment[4])numOfSchemas=6;
    else numOfSchemas=rootSegment[4];
    if(!rootSegment[5])sizeOfMapInInt=19;
    else sizeOfMapInInt=rootSegment[5];
    map = malloc(sizeof(struct schema)*sizeOfMap);
    map[0]=(struct schema){3,"Int"};
    map[1]=(struct schema){5,"Float"};
    map[2]=(struct schema){3, "Str"};
    map[3]=(struct schema){4, "Bool"};
    map[4]=(struct schema){4, "ROOT"};
    map[5]=(struct schema){4, "NODE"};
    int i=0, ind=mainTypes;
    while(ind<numOfSchemas){
        int* addrOfMap = addr+ getAddr(rootSegment[0]/*+rootSegment[1]*sizeofPiramidStruct()*/)+i;
        map[ind].s=addrOfMap[0];
        addrOfMap[0]=0;
        map[ind].name = malloc(sizeof(char)*map[ind].s);
        for(int j=0; j<map[ind].s; j++){
            map[ind].name[j]=*((char*)(addrOfMap+sizeof(int))+j);
            *((char*)(addrOfMap+sizeof(int))+j)=0;
        }
        i+=sizeof(int)+dataSizeInt(map[ind].s);
        ind++;
    }
    rootSegment[3]=sizeOfMap;
    rootSegment[4]=numOfSchemas;
}

void addType(const char* str, int type, struct answer* a){
    int i=0;
    int j=i;
    while(str[++j]!=' ');
    int nameSize=j-i;
    j=i;
    char* name = malloc(sizeof(char)*nameSize);
    while(str[i]!=' '){name[i-j]=str[i]; i++; }
    for(int l=mainTypes; l<numOfSchemas; l++){
        if(map[l].s==nameSize){
            int b=1;
            for(int l2=0; l2<nameSize; l2++){
                if(*(((char*)map[l].name)+l2) != *(((char*)name)+l2))b=0;
            }
            if(b){
                char* result = "ADDING TYPE: Type already exists with the number %d\n";
                int n=0;
                while(result[n]!='\n'){
                    a->sentence[n]=result[n];
                    n++;
                }
                free(name);
                return;
            }
        }
    }
    if(numOfSchemas==sizeOfMap)resizeMap();
    map[numOfSchemas++]=(struct schema){nameSize, name};
    sizeOfMapInInt+=2 + dataSizeInt(nameSize);
    a->success=1;
    a->index=numOfSchemas-1;
}

//void addObjByType(const char* in, int parent, int type){
//    int j=0;
//    int linkMap[map[type].size];
//    int dataSize=0;
//    int linkSize=0;
//    char* data = malloc(sizeof(char)*64); // 64=4*16 благодаря ограничению на кол-во детей
//    int j2 = j;
//    for(int i1=0; i1<map[type].size; i1++){
//        if(map[type].types[i1]<=3) {
//            while(in[j2++]!=':');
//            j2++;
//            while(in[++j2]!='\"')dataSize++;
//        }
//        else{
//            int b=1;
//            for(int j1=0; j1<map[type].nameSizes[i1]; j1++){
//                char string[] = "String";
//                if(map[type].typesName[i1][j1]!=string[j1])b=0;
//            }
//            if(b)dataSize+=map[type].size;
//            else linkMap[linkSize++]=j;
//
//        }
//    }
//
//    int dataCounter=0;
//    for(int l=0; l<map[type].size; l++){
//
//
//
//        while(in[j++]!=':');
//        j++;
//        if(in[j++]=='{'){
//            int count=1;
//            while(count){
//                if(in[j]=='{')count++;
//                if(in[j]=='}')count--;
//                j++;
//            }
//        }
//
//        int b=1;
//
//        for(int j1=0; j1<map[type].nameSizes[l]; j1++){
//            char string[] = "String";
//            if(map[type].typesName[l][j1]!=string[j1])b=0;
//        }
//
//        if(map[type].types[l]<4 || b){
//            while(in[j]!='\"'){
//                data[dataCounter++]=in[j++];
//            }
//        }
//    }
//
//    addChild(parent,addr,dataSize,type,data,start);
//    int thisObj = *(addr+parent+sizeofBody()+((struct body*)addr+parent)->numOfChildren);
//    for(int l=0; l<linkSize; l++){
//        addObj(addr,map,mapSize,in+linkMap[l],thisObj,start);
//    }
//}

int checkType(int type, const char* in, int size, struct answer* a){
    switch (type) {
        case 0:{
            int64_t s=0;
            for(int i=0; i<size; i++){
                if((int)in[i]<48 || (int)in[i]>57){
                    char* result = "CHECK TYPE: WRONG LETTER IN INT\n";
                    int n=0;
                    while(result[n]!='\n'){
                        a->sentence[n]=result[n];
                        n++;
                    }
                    return 0;
                }
                s=s*10+(int)in[i]-48;
                if(s>=4294967296/*максимальное 4-х байтовое число*/) {
                    char* result = "CHECK TYPE: NUMBER IS VERY BIG\n";
                    int n=0;
                    while(result[n]!='\n'){
                        a->sentence[n]=result[n];
                        n++;
                    }
                    return 0;
                }
            }
            break;
        }
        case 1:{
            int num = 0;
            int b=1;
            for(int i=0; i<size;i++){
                if(!(((int)in[i]>=48 && (int)in[i]<=57)||in[i]=='.')){
                    char* result = "CHECK TYPE: WRONG LETTER IN FLOAT\n";
                    int n=0;
                    while(result[n]!='\n'){
                        a->sentence[n]=result[n];
                        n++;
                    }
                    return 0;
                }
                if(in[i]=='.')b=0;
                if(b)num++;
                if(num>38){
                    char* result = "CHECK TYPE: NUMBER IS VERY BIG\n";
                    int n=0;
                    while(result[n]!='\n'){
                        a->sentence[n]=result[n];
                        n++;
                    }
                    return 0;
                }
            }
            if(b){
                char* result = "CHECK TYPE: FLOAT NEEDS DOT\n";
                int n=0;
                while(result[n]!='\n'){
                    a->sentence[n]=result[n];
                    n++;
                }
                return 0;
            }
            break;
        }
        case 3: {
            if (size != 1 || ((int) in[0] - 48 != 0 && (int) in[0] - 48 != 1)) {
                char* result = "CHECK TYPE: BOOL CAN BE 0 OR 1 ONLY\n";
                int n=0;
                while(result[n]!='\n'){
                    a->sentence[n]=result[n];
                    n++;
                }
                return 0;
            }
            break;
        }
        //case 2://В строке может быть что угодно
    }
    return 1;
}

//void addObj(const char* in, struct answer* a){
//    int i=0;
//    int j=i;
//    while(in[++j]!=' ');
//    int type = -1;
//    for(int l=0; l<numOfSchemas; l++){
//        if(map[l].s == j-i) {
//            int b=1;
//            for (int l2 = 0; l2 < j - i; l2++) {
//                if (map[l].name[l2] != in[i + l2])b=0;
//            }
//            if(b){
//                type=l;
//                break;
//            }
//        }
//    }
//    if(type==-1){
//        addType(in, a);
//    }
//    while(in[i++]!='(');
//    if((int)in[i]<48 || (int)in[i]>=58){
//        char* result = "ADDING OBJECT: WRONG PARENT NUMBER\n";
//        int n=0;
//        while(result[n]!='\n'){
//            a->sentence[n]=result[n];
//            n++;
//        }
//        return;
//    }
//    int s=(int)in[i]-48;
//    while(in[++i]!=',')s=s*10+(int)in[i]-48;
//    int num=0;
//    while(in[++i]!=')')num=num*10+(int)in[i]-48;
//    j=++i;
//    while(in[++j]!=';');
//    int ind;
//    if(type<4){
//        if(! checkType(type, &in[i+1],j-i-1, a))return;
//        ind = addChild(s,num,j-i-1,type,&in[i+1], a);
//    }
//    else ind = addChild(s,num,0,  type,"", a);
//    if(ind!=-1)a->success=1;
//    a->index=ind;
//}
void addObj(const char* in, struct answer* a, int updOrIns){
    int i=0;
    int j=i;
    while(in[++j]!=' ');
    int type = -1;
    for(int l=0; l<numOfSchemas; l++){
        if(map[l].s == j-i) {
            int b=1;
            for (int l2 = 0; l2 < j - i; l2++) {
                if (map[l].name[l2] != in[i + l2])b=0;
            }
            if(b){
                type=l;
                break;
            }
        }
    }
    while(in[i++]!='(');
    if((int)in[i]<48 || (int)in[i]>=58){
        char* result = "ADDING OBJECT: WRONG PARENT NUMBER\n";
        int n=0;
        while(result[n]!='\n'){
            a->sentence[n]=result[n];
            n++;
        }
        return;
    }
    int s=(int)in[i]-48;
    while(in[++i]!=',')s=s*10+(int)in[i]-48;

    int num=0;
    if(updOrIns){
        while(in[++i]!=',')num=num*10+(int)in[i]-48;
    }
    else{
        struct body* addrOfStartParent= (struct body *) (addr + s);
        num =  addrOfStartParent->numOfChildren;
    }
    int typeVal=0;
    while(in[++i]!=')')typeVal=typeVal*10+(int)in[i]-48;
    j=++i;
    while(in[++j]!=';');
    int ind;
    if(! checkType(typeVal, &in[i+1],j-i-1, a))return;
    if(type==-1){
        addType(in, typeVal, a);
        type=a->index;
    }
    ind = addChild(s,num,j-i-1,type,&in[i+1], a);
    if(ind!=-1)a->success=1;
    a->index=ind;
}

int compare(const char* in, int size, int* elem){
    struct body* child = (struct body*)elem;
    int objSize = child->dataSize;
    int min = objSize*(objSize<=size) + size*(objSize>size);
    for(int l=0; l<min; l++) {
        if(in[l]<*(((char*)(elem+child->data))+l))return -1;
        else if (in[l]>*(((char*)(elem+child->data))+l))return 1;
    }
    if(objSize==size)return 0;
    else if(objSize>size && child->type==3)return -1;
    else if(objSize<size && child->type==3) return 1;
    else if(objSize>size){
        for(int i=size; i<objSize; i++){
            if(*(((char*)(elem+child->data))+i)!='0')return -1;
        }
        return 0;
    }
    else if(objSize<size){
        for(int i=objSize; i<size; i++){
            if(in[i]!='0')return 1;
        }
        return 0;
    }
    return 0;
}

void findInformation(const char* in, struct answer* a){
    int i=0;
    int j=i;
    while(in[++j]!=' ');
    int type = -1;
    for(int l=0; l<sizeOfMap; l++){
        if(map[l].s == j-i) {
            int b=1;
            for (int l2 = 0; l2 < j - i; l2++) {
                if (map[l].name[l2] != in[i + l2])b=0;
            }
            if(b){
                type=l;
                break;
            }
        }
    }
    if(type==-1){
        char* result = "FINDING INFO: UNKNOWN TYPE NAME\n";
        int n=0;
        while(result[n]!='\n'){
            a->sentence[n]=result[n];
            n++;
        }
        return;
    }
    i=sizeofHeader();
    j+=2;// пропускаем " ("
    while(i<rootSegment[0]) {
        while (!*(addr + getAddr(i))) {
            i++;
            if (i >= rootSegment[0]) return;
        }
        struct body elem={0};
        deserialize(addr+getAddr(i), &elem);

        if(elem.type==type){
            int b=1;
            int j2 = j;
            while(in[j2-1]!=')'){
                int s=0;
                while(in[j2]!='=' && in[j2]!='>' && in[j2]!='<'){
                    if((int)in[j2]>=58 || (int)in[j2]<48){
                        char* result = "FINDING INFO: WRONG ATTRIBUTE NUMBER\n";
                        int n=0;
                        while(result[n]!='\n'){
                            a->sentence[n]=result[n];
                            n++;
                        }
                        return;
                    }
                    s=s*10+(int)in[j2]-48;
                    j2++;
                }
                if(s>=elem.numOfChildren){
                    b=0;
                    break;
//                    char* result = "FINDING INFO: WRONG ATTRIBUTE NUMBER\n";
//                    int n=0;
//                    while(result[n]!='\n'){
//                        a->sentence[n]=result[n];
//                        n++;
//                    }
//                    return;
                }
                int sym = j2;
                while(in[j2]!=')' && in[j2]!=',')j2++;
                struct body* child = (struct body*)(addr+getChildAddr(*(addr+getAddr(i)+sizeofBody()+s)));
                if(!checkType(child->type, &in[sym+1], j2-sym-1, a))return;
                int result=compare(&in[sym+1], j2-sym-1,addr+getChildAddr(*(addr+getAddr(i)+sizeofBody()+s)));
                if((in[sym]=='>' && result!=-1) || (in[sym]=='<' && result!=1) || (in[sym]=='=' && result!=0)){
                    b=0;
                    break;
                }
                j2++;
            }
            if(b)printOneElem(elem,i,a);
        }
        i += elem.sizeOfElem;
    }
}

void saveMap(){
    rootSegment[3]=sizeOfMap;
    rootSegment[4]=numOfSchemas;
    int i=0, ind=mainTypes;
    while(ind<rootSegment[4]){
        int* addrOfMap = addr+ getAddr(rootSegment[0]/*+rootSegment[1]*sizeofPiramidStruct()*/)+i;
        addrOfMap[0]=map[ind].s;
        for(int j=0; j<map[ind].s; j++){
            *((char*)(addrOfMap+sizeof(int))+j)=*((char*)map[ind].name+j);
        }
        free(map[ind].name);
        i+=sizeof(int)+dataSizeInt(map[ind].s);
        ind++;
    }
    free(map);
}

void printMap(struct answer* a){
    for(int i=0; i<numOfSchemas; i++) {
        intInAnswer(i,a);
        a->sentence[a->sizeOfAnswer++]='-';
        for (int j = 0; j < map[i].s; j++) {
            a->sentence[a->sizeOfAnswer++]=map[i].name[j];
        }
        a->sentence[a->sizeOfAnswer++]='\n';
    }
}

void intInAnswer(int num, struct answer* a){
    int copy=num;
    int n=1;
    while((int)copy/10){
        n++;
        copy=(int)copy/10;
    }
    for(int i=0; i<n; i++){
        a->sentence[a->sizeOfAnswer++]=(char)((int)(num/pow(10,n-i-1)) % 10+48);
    }

}