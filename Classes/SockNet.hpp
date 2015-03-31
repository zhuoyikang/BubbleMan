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
#include <string>
#include "MsgQueue.hpp"
#include <assert.h>

using namespace std;


//用于多线程通信.
class SockNetMsg :public QueueMsg
{
public:
    SockNetMsg(int t, void *data ,int size) {
        d = malloc(size);
        assert(d != NULL);
        memcpy(d, data, size);
        this->t=t;
    }

    ~SockNetMsg() {
        free(this->d);
    }

    int T() {return t;}
    void *D () {return d;}

private:
    int t;
    void *d;
};



class SockApp
{
public:
    SockApp();
    virtual ~SockApp();

    static void ShowByte(string title, void *buff, size_t size);

    int Connect(const char *host, int port);
    int SendBytes(void *byte, size_t length);
    int Readn(void *byte, size_t length);
    int Work(); //单独的监听线程.

    //你必须实现
    virtual void Dispatch(int t, unsigned char *buff, int) = 0;

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
