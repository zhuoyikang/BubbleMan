//
//  BubbleApp.h
//  T4
//
//  Created by zhuoyikang on 15-3-29.
//
//

#ifndef __T4__BubbleApp__
#define __T4__BubbleApp__

#include <stdio.h>
#include "SockNet.hpp"
#include "MsgQueue.hpp"

enum {
    BUBBLE_APP_STS_SUCCESS = 1,
    BUBBLE_APP_STS_FAILED = 2,
    BUBBLE_APP_STS_FIGHT = 3,
    BUBBLE_APP_STS_ClOSE = 4
};

class BubbleApp:public SockApp
{
    typedef void (BubbleApp::*BubblePktHander)(unsigned char *);

public:
    BubbleApp();
    void Dispatch(int t, unsigned char *buff, int);
    void Run();
    //LoopMq。
    void Loop();

    //切换到房间场景.
    void SceneRoom();

    //切换到关闭场景
    void SceneClose();

    void SceneSuccess();

    void Login();
    void Join();

    //当前状态
    int Status;


    //服务器消息处理
public:
    void RoomReadyNtfPktHander(unsigned char *p);
    void RoomCloseNtfPktHander(unsigned char *p);
    BubblePktHander Map[100];

    MsgQueue Mq;
    //private:
};


// Sorrry，全局变量
extern BubbleApp gBubbleApp;

#endif /* defined(__T4__BubbleApp__) */
