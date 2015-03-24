//
//  BPlayer.cpp
//  T4
//
//  Created by zhuoyikang on 15-3-17.
//
//

#include "BPlayer.hpp"
#include "cocostudio/CocoStudio.h"
#include "CocosGUI.h"


USING_NS_CC;


BPlayer::BPlayer()
{
    _animationnNodes[animation_front] =
        CSLoader::createNode("Animation/animation_front.csb");
    _animationnNodes[animation_left] =
        CSLoader::createNode("Animation/animation_left.csb");
    _animationnNodes[animation_right] =
        CSLoader::createNode("Animation/animation_right.csb");
    _animationnNodes[animation_back] =
        CSLoader::createNode("Animation/animation_back.csb");
    _animationnActions[animation_front] =
        CSLoader::createTimeline("Animation/animation_front.csb");
    _animationnActions[animation_left] =
        CSLoader::createTimeline("Animation/animation_left.csb");
    _animationnActions[animation_right] =
        CSLoader::createTimeline("Animation/animation_right.csb");
    _animationnActions[animation_back] =
        CSLoader::createTimeline("Animation/animation_back.csb");

    directionInit(animation_front);
    directionInit(animation_left);
    directionInit(animation_right);
    directionInit(animation_back);

    _currentDirection=-1;

    SetDirection(animation_front);
}

void BPlayer::directionInit(int direction)
{
    this->addChild(_animationnNodes[direction]);
    _animationnNodes[direction]->runAction(_animationnActions[direction]);
    _animationnActions[direction]->gotoFrameAndPlay(0, true);
    _animationnNodes[direction]->setVisible(false);
}

int BPlayer::CurrentDirection()
{
    return _currentDirection;
}

void BPlayer::SetDirection(int direction)
{
    if(direction< 0 || direction >= animation_max){
        return;
    }
    if(_currentDirection==direction){
        return;
    }
    if(_currentDirection!=-1){
        _animationnNodes[_currentDirection]->setVisible(false);
    }
    _currentDirection=direction;
    _animationnNodes[_currentDirection]->setVisible(true);
}
