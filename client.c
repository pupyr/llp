#include "client.h"

char types[6][4]={"addO", "updO", "drop", "find","info", "exit"};

int func(const char *in, int size, int parent,int value)
{
    printf("%s %d %d\n", in,size,parent);
    char string[MAX];
    for(int i=0; i<MAX; i++){
        string[i]=0;
    }
        struct question q = (struct question){"text.txt",0,{0},2,{0},{0}};
        for(int i=0; i<size; i++){
            q.target[i]=in[i];
        }
        int copy=value;
        int n=1;
        while((int)copy/10){
            n++;
            copy=(int)copy/10;
        }
        if(value) {
            for (int i = 0; i < n; i++) {
                q.value[i] = (char) ((int) (parent / pow(10, n - i - 1)) % 10 + 48);
            }
            q.type=0;
        }

        copy=parent;
        n=1;
        while((int)copy/10){
            n++;
            copy=(int)copy/10;
        }
        for(int i=0; i<n; i++){
            q.args[0][i]=(char)((int)(parent/pow(10,n-i-1)) % 10+48);
        }


        (q.args)[1][0] = '5';
        send(sock, (char*)&q, MAX,0);
        struct answer a = (struct answer){0,0,0,0,{0}};
        recv(sock, (char*)&a, MAX,0);
        if(q.type<3){
            if(a.success){
                printf("index - %d\n",a.index);
            }
            else{
                printf("%s\n", a.sentence);
            }
        }
        else if(q.type<5) {
            for (int i = 0; i < a.sizeOfAnswer; i++) {
                printf("%c", a.sentence[i]);
            }
        }
        return a.index;
}

//void func_find(){
//    struct question q = (struct question){"text.txt",4,{0},2,{0},{0}};
//    q.type=4;
//    send(sock, (char*)&q, MAX,0);
//}

void func_exit(){
    struct question q = (struct question){"text.txt",5,{0},0,{0},{0}};
    send(sock, (char*)&q, MAX,0);
    struct answer a = (struct answer){0,0,0,0,{0}};
    recv(sock, (char*)&a, MAX,0);
    for (int i = 0; i < a.sizeOfAnswer; i++) {
        printf("%c", a.sentence[i]);
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

//    int a = func("<?xmlversion=\"1.0\"encoding=\"utf-8\"?>",36,6 );
//    func("<?xmlversion=\"1.0\"encoding=\"utf-8\"?>",36,a );
    parse();
    //func_find();
    func_exit();

    closesocket(sock);
    return 0;
}