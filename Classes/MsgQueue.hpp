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
#include <queue>

using namespace std;


class QueueMsg
{
public:
    virtual ~QueueMsg() =0;
    virtual int T() = 0;  //类型
    virtual void *D() = 0;  //数据
};


class MsgQueue
{
public:
    MsgQueue();
    bool Empty() const;
    QueueMsg* Front();
    void Pop();
    void Push(QueueMsg* msg);
    void Lock();
    void Unlock();

private:
    pthread_mutex_t mutex;
    queue<QueueMsg*> qs;
};

#endif /* defined(__T1__MsgQueue__) */
