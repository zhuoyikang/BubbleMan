//
//  MsgDispatch.cpp
//  TileTest
//
//  Created by zhuoyikang on 15-2-16.
//
//
#include "MsgDispatch.hpp"


void SockBubbleDispatch::Dispatch(unsigned char *buff)
{
    unsigned short api;
    msgbin::BzReaduint16(&buff, &api);
    LOG("type %d", api); //dispatch
}

void SockBubbleDispatch::Close()
{
}
