//
// Created by olega on 15.01.2024.
//

#ifndef LLP_TRANSPORT_H
#define LLP_TRANSPORT_H

#define MAX sizeof(struct question)
#define PORT 8086

struct question{
    char nameFile[8];
    int type;
    char target[12];
    int argsCount;
    char args[16][60];
    char value[36];
};

struct answer{
    int success;
    int index;
    int sizeOfAnswer;
    char sentence[sizeof(struct question)-3*sizeof(int)];
};


#endif //LLP_TRANSPORT_H
