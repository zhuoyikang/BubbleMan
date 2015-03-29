//
//  MsgQueue.h
//  T1
//
//  Created by zhuoyikang on 15-2-24.
//
//

#ifndef __T1__MsgQueue__
#define __T1__MsgQueue__

#include <stdio.h>
#include <iostream>
#include <pthread.h>

using namespace std;


//用于多线程通信.
class BombMsg
{
public:
    int type;
    void *data;
    void init(void *data ,int size);
    void destroy();
};

class MsgQueue
{
public:
    MsgQueue();
    bool empty() const;
    BombMsg front();
    void pop();
    void push(const BombMsg msg);
    void lock();
    void unlock();

private:
    pthread_mutex_t mutex;
    queue<BombMsg> qs;
};

#endif /* defined(__T1__MsgQueue__) */
