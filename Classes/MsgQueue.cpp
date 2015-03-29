//
//  MsgQueue.cpp
//  T1
//
//  Created by zhuoyikang on 15-2-24.
//
//

#include "MsgQueue.h"
#include <assert.h>

void BombMsg::init(void *data_in ,int size)
{
    data = malloc(size);
    assert(data != NULL);
    memcpy(data, data_in, size);
}

void BombMsg::destroy()
{
    free(this->data);
}



MsgQueue::MsgQueue()
{
    int ret = pthread_mutex_init(&this->mutex, NULL);
    assert(ret==0);
}

bool MsgQueue::empty() const
{
    bool ret;
    ret = this->qs.empty();
    return ret;
}
BombMsg MsgQueue::front()
{
    BombMsg msg;
    msg =  this->qs.front();
    return msg;
}

void MsgQueue::pop()
{
    this->qs.pop();
}

void MsgQueue::push(const BombMsg msg)
{
    this->qs.push(msg);
}

void MsgQueue::lock()
{
    pthread_mutex_lock(&this->mutex);
}

void MsgQueue::unlock()
{
    pthread_mutex_unlock(&this->mutex);

}

