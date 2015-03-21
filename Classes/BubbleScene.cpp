//
//  BubbleSceneScene.cpp
//  T4
//
//  Created by zhuoyikang on 15-3-16.
//
//

#include "BubbleScene.hpp"


USING_NS_CC;


Scene* BubbleScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = BubbleScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


void BubbleScene::update(float)
{
}


// on "init" you need to initialize your instance
bool BubbleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    _bubble = new Bubble();
    addChild(_bubble);
    _bubble->setPosition(Point(visibleSize.width/2, visibleSize.height/2));

    scheduleUpdate();

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch *, Event *)->bool { return true; };
    listener->onTouchEnded = CC_CALLBACK_2(BubbleScene::onTouchEnded, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}


void BubbleScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *)
{
    auto touchLocation = touch->getLocation();
    log("current position x %f y %f", touchLocation.x, touchLocation.y);
    _bubble->StateTest();
}
