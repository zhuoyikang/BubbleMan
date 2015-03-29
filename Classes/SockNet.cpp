//
//  SockNet.cpp
//  TileTest
//
//  Created by zhuoyikang on 15-2-12.
//
//



#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string>

#include "const.hpp"
#include "SockNet.hpp"
#include "MsgBin.hpp"
#include "MsgGen.hpp"

#define BUFF_SIZE 1 << 16


SockNet::SockNet() {}

SockNet::~SockNet() {}

SockNet::SockNet(SockApp * dis)
{
    Init(dis);
}

void SockNet::Init(SockApp * dis)
{
    this->rb = malloc(BUFF_SIZE);
    assert(this->rb!=NULL);

    this->wb = malloc(BUFF_SIZE);
    assert(this->wb!=NULL);

    d = dis;
}

// 尝试连接服务器.
int SockNet::Connect(const char *host, int)
{
    struct sockaddr_in pin;

    struct hostent *nlp_host;
    char host_name[256];

    strcpy(host_name,host);

    while ((nlp_host=gethostbyname(host_name))==0){
        LOG("Resolve Error!\n");
    }

    bzero(&pin,sizeof(pin));
    pin.sin_family=AF_INET;                 //AF_INET表示使用IPv4
    pin.sin_addr.s_addr=htonl(INADDR_ANY);
    pin.sin_addr.s_addr=((struct in_addr *)(nlp_host->h_addr))->s_addr;
    pin.sin_port=htons(8080);

    this->Sock = socket(AF_INET,SOCK_STREAM,0);

    if(this->Sock == -1){
        LOG("failed to socket %d", this->Sock);
        return -1;
    }
    /*
    int sockopt=1;
    if(setsockopt(this->Sock, SOL_SOCKET, SO_USELOOPBACK, (char*)&sockopt, sizeof(sockopt)))
        LOG("error on setsockopt SO_USELOOPBACK: %s", strerror(errno));
    */

    msgbin::show_byte(&pin, sizeof(pin));

    if(connect(this->Sock,(struct sockaddr*)&pin,sizeof(pin)) == -1){
        LOG("failed to connected %d", this->Sock);
        return -1;
    } else {
        LOG("successes to connected");
        return 0;
    }
}


typedef unsigned char byte_t;
typedef unsigned short ushort_t;
typedef short short_t;
typedef int int32_t;
typedef unsigned int uint32_t;


void show_byte(void *buff, size_t size)
{
    char *buffer=(char *)buff;
    LOG("show: ");
    for(size_t i =0 ; i< size ;i++) {
        printf("%x ", (byte_t)buffer[i]);
    }
    LOG("\n");
}


void* _sockNetwork(void *p)
{
    SockNet *sock = (SockNet*)p;
    int ret;
    while(1) {
        ret = sock->Readn(sock->rb, 2);
        if(ret < 0) {
            LOG("continue %d\n", ret);
            return NULL;
        }
        unsigned char *buff = (unsigned char*)sock->rb;
        unsigned short size = (buff[0]<< 8 | buff[1]);
        //读取整个包size字节.
        ret = sock->Readn(sock->rb, size-2);
        show_byte(buff, size-2);
        sock->d->Dispatch(buff);
    }
    return NULL;
}


// 向服务器发送字节
int SockNet::SendBytes(void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = (const char *)vptr;
    nleft = n;
    while (nleft > 0) {
        LOG("send %d", this->Sock);
        if ((nwritten = send(this->Sock, ptr, nleft,0)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;   /* and call write() again */
            else
                return (-1);    /* error */
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n);
}

int SockNet::Readn(void* vptr, size_t n)
{
    size_t  nleft;
    ssize_t nread;
    char   *ptr;

    ptr = (char*)vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nread = recv(this->Sock, ptr, nleft,0)) < 0) {
            if (errno == EINTR) {
                nread=0;
            } else {
                return (-1);
            }
        } else if (nread == 0)
            break;              /* EOF */

        nleft -= nread;
        ptr += nread;
    }
    if(nleft == 0) {
        return 1;
    }else {
        return -1;
    }
}

int SockNet::Work()
{
    int n;
    if ((n=pthread_create(&this->p, NULL, _sockNetwork, this))!=0) {
        return n;
    }
    return 0;
}



#ifndef COCOS

int main(int, char *[])
{
    SockNet sock;
    sock.Connect("127.0.0.1", 8080);
    sock.Work();
    sleep(120);
}

#endif
