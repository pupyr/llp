//
// Created by olega on 15.01.2024.
//

#ifndef LLP_TRANSPORT_H
#define LLP_TRANSPORT_H

#define MAX sizeof(struct answer)
#define PORT 8091


struct question{
    char nameFile[8];
    int type;
    char target[300];
    int argsCount;
    char args[16][60];
    char value[36];
};

struct answer{
    int success;
    int index;
    int reserveIndex;
    int sizeOfAnswer;
    char sentence[100000];
};


#endif //LLP_TRANSPORT_H
