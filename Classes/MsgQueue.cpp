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

QueueMsg* MsgQueue::Pick()
{
    QueueMsg* msg = NULL;
    pthread_mutex_lock(&this->mutex);
    if( ! this->qs.empty() ) {
        msg = this->qs.front();
        this->qs.pop();
    }
    pthread_mutex_unlock(&this->mutex);
    return msg;
}

void MsgQueue::Push(QueueMsg* msg)
{
    pthread_mutex_lock(&this->mutex);
    this->qs.push(msg);
    pthread_mutex_unlock(&this->mutex);
}
