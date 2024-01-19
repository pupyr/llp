#include "client.h"

char types[6][4]={"addO", "updO", "drop", "find","info", "exit"};

void func()
{
    char string[MAX];
    int n;
    int exitStatus=1;
    while(exitStatus) {
        for(int i=0; i<MAX; i++){
            string[i]=0;
        }
        struct question q = (struct question){{0},0,{0},0,{0},{0}};

        printf("Enter the string : ");
        n=0;
        while ((string[n++] = getchar()) != '\n');
        n=0;
        while(string[n]!=' '){
            if(string[n]=='\n'){
                printf("NOT ENOUGH WORDS\n");
                break;
            }
            q.nameFile[n]=string[n];
            n++;
        }
        n++;
        int type=-1;
        for(int i=0; i<6; i++){
            int b=1;
            for(int j=0; j<4; j++){
                if(types[i][j]!=string[n+j])b=0;
            }
            if(b){
                type=i;
                break;
            }
        }
        if(type==-1){
            printf("UNKNOWN COMAND\n");
            continue;
        }
        if(type==5){
            exitStatus=0;
            q.type=5;
            send(sock, (char*)&q, MAX,0);
            continue;
        }
        q.type=type;
        n+=5;
        int startName=n;
        if(type!=4) {
            if(string[n]=='(')n--;
            while (string[n] != ' ' && startName - n < 12) {
                q.target[n - startName] = string[n];
                n++;
            }
            if (string[++n] != '(') {
                printf("bracket ISN'T IN SENTENCE\n");
                continue;
            }
            n++;
            int count = 0;
            int c = 0;
            while (string[n] != ')' && n < MAX) {
                if (string[n] == ',') {
                    count++;
                    c = 0;
                } else q.args[count][c++] = string[n];
                n++;
            }
            if (n >= MAX) {
                printf("bracket ISN'T IN SENTENCE\n");
                continue;
            }
            q.argsCount = count + 1;
            n++;
            if (string[n] != ';') {
                int startValue = ++n;
                while (string[n] != ';') {
                    q.value[n - startValue] = string[n];
                    n++;
                }
            }
        }
        send(sock, (char*)&q, MAX,0);
        struct answer a = (struct answer){0,0,0,{0}};
        recv(sock, (char*)&a, MAX,0);
        if(q.type<3){
            if(a.success){
                printf("index - %d\n",a.index);
            }
            else{
                printf("%s\n", a.sentence);
            }
        }
        else if(q.type<5){
            for(int i=0; i<a.sizeOfAnswer; i++){
                printf("%c", a.sentence[i]);
            }
        }
        //for(int i=0; i<MAX; i++){
        //    buff[i]=0;
        //}
        //recv(sock, buff, MAX,0);
        //printf("From Server : %s", buff);
        //if ((strncmp(buff, "exit", 4)) == 0) {
        //    printf("Client Exit...\n");
        //    break;
        //}
    }
}

int main()
{
    struct WSAData wsaData;
    WORD word = MAKEWORD(2,2);
    struct sockaddr_in servaddr;
    if(WSAStartup(word, &wsaData) ){
        printf("WSA error...\n");
        exit(0);
    }
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    int conn=connect(sock, (SA*)&servaddr, sizeof(servaddr));
    if (conn!= 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    func();

    closesocket(sock);
}