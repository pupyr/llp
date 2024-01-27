//
// Created by olega on 14.01.2024.
//

#ifndef LLP_CLIENT_H
#define LLP_CLIENT_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "transport.h"
#ifdef _WIN32
#include <winsock2.h>
#include <math.h>
#include "xml.h"
#else
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif
#define SA struct sockaddr
SOCKET sock;
int func(const char* in, int size, int parent, int value);
#endif //LLP_CLIENT_H
