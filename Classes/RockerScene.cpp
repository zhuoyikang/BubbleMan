//
//  RockerScene.cpp
//  T4
//
//  Created by zhuoyikang on 15-3-18.
//
//

#include "RockerScene.hpp"

USING_NS_CC;

Scene* RockerScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = RockerScene::create();

    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

bool RockerScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    _rocker = BRocker::createRocker("Rocker/joystick_bg.png",
                                   "Rocker/joystick_center.png",
                                   Point(110,60));
    this->addChild(_rocker);
    _rocker->startRocker(true);

    return true;
}
