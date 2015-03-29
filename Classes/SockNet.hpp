//
//  SockNet.h
//  TileTest
//
//  Created by zhuoyikang on 15-2-12.
//
//

#ifndef __TileTest__SockNet__
#define __TileTest__SockNet__

#include <pthread.h>


//接口
class SockApp
{
public:
    virtual ~SockApp();
    virtual void Dispatch(unsigned char *buff) = 0;
};


class SockNet
{
public:
    SockNet();
    SockNet(SockApp *);
    virtual ~SockNet();
    void Init(SockApp *);
    static void ShowByte(void *buff, size_t size);
    int Connect(const char *host, int port);
    int SendBytes(void *byte, size_t length);
    int Readn(void *byte, size_t length);
    int Work(); //单独的监听线程.

    //设置为friend可以访问内部的dispatch。
    friend void* _sockNetwork(void *p);

private:
    int Sock;
    void *rb;
    void *wb;
    SockApp *d;
    pthread_t p;
};

#endif /* defined(__TileTest__SockNet__) */
