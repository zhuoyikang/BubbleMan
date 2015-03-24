//
//  AminationScene.cpp
//  T4
//
//  Created by zhuoyikang on 15-3-16.
//
//

#include "AminationScene.hpp"


USING_NS_CC;


Scene* Amination::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = Amination::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


void Amination::update(float dt)
{
}


// on "init" you need to initialize your instance
bool Amination::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    _bplayer = new BPlayer();
    addChild(_bplayer);
    _bplayer->setPosition(Point(visibleSize.width/2, visibleSize.height/2));

    scheduleUpdate();

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch *, Event *)->bool { return true; };
    listener->onTouchEnded = CC_CALLBACK_2(Amination::onTouchEnded, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}


void Amination::onTouchEnded(cocos2d::Touch *, cocos2d::Event *)
{
//    auto touchLocation = touch->getLocation();
//    log("current position x %f y %f", touchLocation.x, touchLocation.y);
    int currentDirection = ( _bplayer->CurrentDirection()+1) % animation_max;
    log("current direction %d %d", currentDirection, _bplayer->CurrentDirection());
    _bplayer->SetDirection(currentDirection);
}
