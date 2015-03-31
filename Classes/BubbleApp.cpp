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
#include "SceneFailedConnected.hpp"
#include "SceneSuccessConnected.hpp"
#include "BeachScene.hpp"

#include "cocos2d.h"

USING_NS_CC;

BubbleApp gBubbleApp;

BubbleApp::BubbleApp(){
    bzero(&this->Map,  sizeof(Map));

    Map[5]=&BubbleApp::RoomReadyNtfPktHander;
    Map[6]=&BubbleApp::RoomCloseNtfPktHander;

    Status = BUBBLE_APP_STS_SUCCESS;
}

static void directorInit()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("泡泡超人");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
}


void BubbleApp::Run() {
    auto director = Director::getInstance();
    if(Connect(IP, PORT) == -1){
        auto scene = SceneFailedConnected::createScene();
        director->runWithScene(scene);
    }else{
        auto scene = SceneSuccessConnected::createScene();
        director->runWithScene(scene);
        Work();
        Login();
        Join();
    }
    LOG("I am begin to work");
}


void BubbleApp::Dispatch(int api, unsigned char *p, int size)
{
    QueueMsg *msg = new SockNetMsg(api, p, size);
    LOG("msg t %d", msg->T());
    Mq.Push(msg);
}

void BubbleApp::Loop()
{
    /*
    while(1) {
        sleep(1);

        if(Mq.Empty()==true){
            continue;
        }
        QueueMsg *msg = Mq.Front();
        Mq.Pop();
        LOG("t1 %d", msg->T());
        delete msg; // 用完删除.
    }
     */
}

//切换到房间场景.
void BubbleApp::SceneRoom()
{
    auto scene = BeachScene::createScene();
    auto transitions = TransitionMoveInL::create(0.2f, scene);
    Director::getInstance()->replaceScene(transitions);
}

void BubbleApp::SceneClose()
{
    auto scene = SceneFailedConnected::createScene();
    auto transitions = TransitionMoveInL::create(0.2f, scene);
    Director::getInstance()->replaceScene(transitions);
}


void BubbleApp::SceneSuccess()
{
    auto scene = SceneSuccessConnected::createScene();
    auto transitions = TransitionMoveInL::create(0.2f, scene);
    Director::getInstance()->replaceScene(transitions);
}

void BubbleApp::Login()
{
    msgbin::uint16 api;
    msgbin::byte_t *b1,*b2;

    //发送登录包
    api = 1;
    msgbin::UserLoginReq ulr;
    ulr.udid = "udid";

    b1 = (msgbin::byte_t *)this->Wb;
    b2 = b1;

    msgbin::BzWriteUserLoginReq(&b1, &ulr);

    this->WriteAPI(1, b1-b2);
    this->SendBytes(this->Wbh, b1-b2+4);
}

void BubbleApp::Join()
{
    msgbin::uint16 api;
    msgbin::byte_t *b1,*b2;

    //发送加入房间包
    api = 3;
    msgbin::UserJoinReq jin;

    b1 = (msgbin::byte_t *)this->Wb;
    b2 = b1;

    msgbin::BzWriteUserJoinReq(&b1, &jin);

    this->WriteAPI(3, b1-b2);
    this->SendBytes(this->Wbh, b1-b2+4);

}




void BubbleApp::RoomReadyNtfPktHander(unsigned char *p)
{
    LOG("RoomReadyNtfPktHander");
}
void BubbleApp::RoomCloseNtfPktHander(unsigned char *p)
{
    LOG("RoomCloseNtfPktHander");
}


#ifdef SELF_BUBBLE_MAIN


int main(int, char *[])
{

    BubbleApp app;
    app.Run();
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

    app.Loop();

}

#endif
