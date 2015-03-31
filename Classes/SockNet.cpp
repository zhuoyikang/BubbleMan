//
//  SockApp.cpp
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

#include "const.hpp"
#include "SockNet.hpp"
#include "MsgBin.hpp"
#include "MsgGen.hpp"

#define BUFF_SIZE (1 << 16)


SockApp::SockApp()
{
    this->Rbh = (unsigned char *)malloc(4+BUFF_SIZE);
    assert(this->Rbh!=NULL);

    this->Wbh = (unsigned char *)malloc(4+BUFF_SIZE);
    assert(this->Wbh!=NULL);

    this->Rb = this->Rbh+4;
    this->Wb = this->Wbh+4;
}

SockApp::~SockApp() {}


// 尝试连接服务器.
int SockApp::Connect(const char *host, int port)
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
    pin.sin_port=htons(port);

    this->s = socket(AF_INET,SOCK_STREAM,0);

    if(this->s == -1){
        LOG("failed to socket %d", this->s);
        return -1;
    }

    if(connect(this->s,(struct sockaddr*)&pin,sizeof(pin)) == -1){
        LOG("failed to connected %d", this->s);
        return -1;
    } else {
        LOG("successes to connected");
        return 0;
    }
}



void SockApp::ShowByte(string title, void *buff, size_t size)
{
    char *buffer=(char *)buff;
    LOG("%s show: ", title.c_str());
    for(size_t i =0 ; i< size ;i++) {
        printf("%x ", (unsigned char)buffer[i]);
    }
    LOG("\n");

}

void* _sockNetwork(void *p)
{
    SockApp *sock = (SockApp*)p;
    int ret;
    unsigned short api;
    while(1) {
        ret = sock->Readn(sock->Rbh, 2);
        if(ret < 0) {
            LOG("continue %d\n", ret);
            return NULL;
        }
        unsigned char *buff = (unsigned char*)sock->Rbh;
        unsigned short size = (buff[0]<< 8 | buff[1]);
        buff = sock->Rbh+2;
        //读取整个包size字节.
        ret = sock->Readn(buff, size-2);
        msgbin::BzReaduint16(&buff, &api);
        SockApp::ShowByte("Read",buff, size-4);

        //sleep(1);
        sock->Dispatch(api,buff,size-4);
    }
    return NULL;
}


// 向服务器发送字节
int SockApp::SendBytes(void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = (const char *)vptr;

    // 注释我吧
    ShowByte("Send",(void*)ptr, n);

    nleft = n;
    while (nleft > 0) {
        if ((nwritten = send(this->s, ptr, nleft,0)) <= 0) {
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

int SockApp::Readn(void* vptr, size_t n)
{
    size_t  nleft;
    ssize_t nread;
    char   *ptr;

    ptr = (char*)vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nread = recv(this->s, ptr, nleft,0)) < 0) {
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

int SockApp::Work()
{
    int n;
    if ((n=pthread_create(&this->p, NULL, _sockNetwork, this))!=0) {
        return n;
    }
    return 0;
}

//设置API类型.
void SockApp::WriteAPI(unsigned short t, unsigned short len)
{
    len+=4;
    unsigned char *p = this->Wbh;
    msgbin::BzWriteuint16(&p, &len);
    msgbin::BzWriteuint16(&p, &t);
}


#ifdef SELF_MAIN


class SockAppDemo: public SockApp
{
public:
    void Dispatch(int t, unsigned char *buff,int);
};

void SockAppDemo::Dispatch(int api,unsigned char *buff,int)
{
    LOG("type %d", api);
}

int main(int, char *[])
{
    SockApp sock();
    sock.Connect("127.0.0.1", 3004);
    sock.Work();
    sleep(120);
}

#endif
