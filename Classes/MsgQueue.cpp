//
//  MsgQueue.cpp
//  T1
//
//  Created by zhuoyikang on 15-2-24.
//
//

#include "MsgQueue.hpp"
#include <assert.h>



QueueMsg::~QueueMsg(){
}


MsgQueue::MsgQueue()
{
    int ret = pthread_mutex_init(&this->mutex, NULL);
    assert(ret==0);
}

bool MsgQueue::Empty() const
{
    bool ret;
    ret = this->qs.empty();
    return ret;
}

QueueMsg* MsgQueue::Front()
{
    QueueMsg* msg;
    msg =  this->qs.front();
    return msg;
}

void MsgQueue::Pop()
{
    this->qs.pop();
}

void MsgQueue::Push(QueueMsg* msg)
{
    this->qs.push(msg);
}

void MsgQueue::Lock()
{
    pthread_mutex_lock(&this->mutex);
}

void MsgQueue::Unlock()
{
    pthread_mutex_unlock(&this->mutex);
}

