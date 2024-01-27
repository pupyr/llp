#include "server.h"

int b=0;

void func(int connfd)
{
    int exitStatus=1;
    while(exitStatus) {
        struct question q=(struct question){{0},0,{0},0,{0},{0}};
        recv(connfd, (char*)&q, MAX,0);
        char str[MAX]={0};
        int n=0;
        for(int i=0; i<100; i++){
            if((int)q.target[i]==0)break;
            str[n++]=q.target[i];
        }
        str[n++]=' ';
        str[n++]='(';
        for(int i=0; i<q.argsCount; i++){
            for(int j=0; j<60; j++){
                if((int)q.args[i][j]==0)break;
                str[n++]=q.args[i][j];
            }
            if(i!=q.argsCount-1)str[n++]=',';
        }
        str[n++]=')';
        str[n++]=' ';
        if((int)q.value[0]==0)str[n++]=' ';
        for(int i=0; i<36; i++){
            if((int)q.value[i]==0)break;
            str[n++]=q.value[i];
        }
        str[n]=';';
        char fileName[9]={0};
        for(int i=0; i<8; i++){
            fileName[i]=q.nameFile[i];
        }
        getAddressOfMapping(fileName);
        if(!addr[0])
            init();
        initPiramid();
        initMap();
        struct answer a = (struct answer){0,0,0,0,{0}};
        switch (q.type) {
            case 0:{
                addObj(str, &a,0);
//                struct answer a2 = (struct answer){0,0,0,0,{0}};
//                printer(&a2);
//                //printPiramid(&a2);
//                printMap(&a2);
//                for(int i=0; i<a2.sizeOfAnswer; i++){
//                    printf("%c",a2.sentence[i]);
//                }
                break;
            }
            case 1:{
                addObj(str, &a,1);
                break;
            }
            case 2: {
                for(int i=0; i<q.argsCount; i++){
                    int s=0;
                    for(int j=0; j<60; j++){
                        if((int)q.args[i][j]==0)break;
                        s = s*10+(int)q.args[i][j]-48;
                    }
                    removeChild(s, &a);
                }
                break;
            }
            case 3:{
                findInformation(str, &a);
                break;
            }
            case 4:{
                printer(&a);
                //printPiramid(&a);
                printMap(&a);
                break;
            }
            case 5:{
//              struct answer a2 = (struct answer){0,0,0,0,{0}};
                //printer(&a2);
                //printMap(&a2);
                findInformation("<xs:restrictionbase=\"xs:noFixedFacet\"> (1=341);",&a);
//                for(int i=0; i<a2.sizeOfAnswer; i++){
//                    printf("%c",a2.sentence[i]);
//                }
                exitStatus=0;
                break;
            }
            default:
                printf("UNKNOWN COMAND\n");
        }

//        saveAlloc();
        saveMap();
        saveFile();
        send(connfd, (char*)&a, MAX,0);
    }
}

// Driver function
int main()
{

    int connfd, len;
    struct WSAData wsaData;
    WORD word = MAKEWORD(2,2);
    struct sockaddr_in servaddr, cli;
    if(WSAStartup(word, &wsaData) ){
        printf("WSA error...\n");
        exit(0);
    }
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sock, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    if ((listen(sock, 2)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    connfd = accept(sock, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

    func(connfd);
    remove("text.txt");
    closesocket(sock);
    WSACleanup();
    return 0;
}