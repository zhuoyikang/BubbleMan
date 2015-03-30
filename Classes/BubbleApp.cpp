//
//  BubbleApp.cpp
//  T4
//
//  Created by zhuoyikang on 15-3-29.
//
//

#include <unistd.h>
#include "const.hpp"
#include "MsgBin.hpp"
#include "BubbleApp.hpp"
#include "MsgGen.hpp"

BubbleApp::BubbleApp()
{
    //初始化网络部分.
    Connect("127.0.0.1", 3004);
    Work();
    LOG("I am begin to work");
}

void BubbleApp::Run() {
}


void BubbleApp::Dispatch(unsigned char *p)
{
    unsigned short api;
    msgbin::BzReaduint16(&p, &api);
    LOG("type %d", api);
}

#ifdef SELF_BUBBLE_MAIN


int main(int, char *[])
{

    BubbleApp app;
    msgbin::uint16 api;
    msgbin::byte_t *b1,*b2;

    //发送登录包
    api = 1;
    msgbin::UserLoginReq ulr;
    ulr.udid = "udid";

    b1 = (msgbin::byte_t *)app.Wb;
    b2 = b1;

    msgbin::BzWriteUserLoginReq(&b1, &ulr);

    app.WriteAPI(1, b1-b2);
    app.SendBytes(app.Wbh, b1-b2+4);

    //发送加入房间包
    api = 3;
    msgbin::UserJoinReq jin;

    b1 = (msgbin::byte_t *)app.Wb;
    b2 = b1;

    msgbin::BzWriteUserJoinReq(&b1, &jin);

    app.WriteAPI(3, b1-b2);
    app.SendBytes(app.Wbh, b1-b2+4);

    sleep(120);
}

#endif
