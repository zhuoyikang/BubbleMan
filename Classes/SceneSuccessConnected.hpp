#ifndef __T4__SceneSuccessConnected__
#define __T4__SceneSuccessConnected__

#include "cocos2d.h"

class QueueMsg;

USING_NS_CC;

class SceneSuccessConnected : public cocos2d::Layer
{
    typedef void (SceneSuccessConnected::*SceneSuccessConnectedFptr)(QueueMsg *m);

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    void update(float dt) override;
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(SceneSuccessConnected);


public:
    void RoomReadyNtf(QueueMsg *msg);
    void RoomCloseNtf(QueueMsg *msg);

private:
    void loopMsg();

    SceneSuccessConnectedFptr HandleMap[100];
};

#endif
