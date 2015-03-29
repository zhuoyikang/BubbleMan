//
//  BubbleApp.cpp
//  T4
//
//  Created by zhuoyikang on 15-3-29.
//
//

#include "BubbleApp.hpp"
#include "MsgDispatch.hpp"

BubbleApp::BubbleApp()
{
    //初始化网络部分.
    sockNet.Init(self);
    sock.Connect("127.0.0.1", 8080);
    sock.Work();
}

void BubbleApp::Run() {
}


void BubbleApp::Dispatch(unsigned char *buff)
{
}
