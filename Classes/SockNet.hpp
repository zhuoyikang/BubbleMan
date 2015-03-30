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


class SockApp
{
public:
    SockApp();
    virtual ~SockApp();

    static void ShowByte(void *buff, size_t size);

    int Connect(const char *host, int port);
    int SendBytes(void *byte, size_t length);
    int Readn(void *byte, size_t length);
    int Work(); //单独的监听线程.

    //你必须实现
    virtual void Dispatch(unsigned char *buff) = 0;

    //设置为friend可以访问内部的dispatch。
    friend void* _sockNetwork(void *p);

    void WriteAPI(unsigned short t, unsigned short len);

public:
    unsigned char *Wb;
    unsigned char *Wbh;

    unsigned char *Rb;
    unsigned char *Rbh;


private:
    int s;
    pthread_t p;


};

#endif /* defined(__TileTest__SockApp__) */
