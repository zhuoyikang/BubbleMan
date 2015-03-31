#include "SceneSuccessConnected.hpp"
#include "const.hpp"
#include "BubbleApp.hpp"

USING_NS_CC;

void SceneSuccessConnected::update(float dt)
{
    //LOG("up");
    loopMsg();
    // if( gBubbleApp.Status == BUBBLE_APP_STS_SUCCESS ){
    //     gBubbleApp.Status=BUBBLE_APP_STS_FIGHT;
    //     gBubbleApp.SceneRoom();
    // }
}


Scene* SceneSuccessConnected::createScene()
{
    auto scene = Scene::create();
    auto layer = SceneSuccessConnected::create();

    scene->addChild(layer);
    return scene;
}

bool SceneSuccessConnected::init()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto label = Label::createWithTTF("Success to Connected; Wait other player",
                                      "fonts/Marker Felt.ttf", 24);
    LOG("origin x %f y %f",origin.x,origin.y);
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height/2));

    // add the label as a child to this layer
    this->addChild(label, 1);

    scheduleUpdate();

    HandleMap[5]= &SceneSuccessConnected::RoomReadyNtf;
    HandleMap[6]= &SceneSuccessConnected::RoomCloseNtf;
    return true;
}


void SceneSuccessConnected::loopMsg()
{
    QueueMsg *msg;
    while (( msg = gBubbleApp.Mq.Pick()) ) {
        LOG("msg tt %d", msg->T());
        SceneSuccessConnectedFptr h = this->HandleMap[msg->T()];
        if (h!=NULL) {
            (this->*h)(msg);
        }
    }
}

void SceneSuccessConnected::RoomReadyNtf(QueueMsg *msg)
{
    if( gBubbleApp.Status == BUBBLE_APP_STS_SUCCESS ){
        gBubbleApp.Status=BUBBLE_APP_STS_FIGHT;
        gBubbleApp.SceneRoom();
    }
    LOG("RoomReadyNtf");
}

void SceneSuccessConnected::RoomCloseNtf(QueueMsg *msg)
{

}
