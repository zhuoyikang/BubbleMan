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
    auto sockDispatch = new(SockBubbleDispatch);
    sockNet.Init(sockDispatch);
}

void BubbleApp::Run(){
}
