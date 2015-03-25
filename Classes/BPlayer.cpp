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
    _id=0;
}


BPlayer::BPlayer(int id)
{
    _id=id;
    _animationNodes[animation_front] =
        CSLoader::createNode("Animation/animation_front.csb");
    _animationNodes[animation_left] =
        CSLoader::createNode("Animation/animation_left.csb");
    _animationNodes[animation_right] =
        CSLoader::createNode("Animation/animation_right.csb");
    _animationNodes[animation_back] =
        CSLoader::createNode("Animation/animation_back.csb");
    _animationNodes[animation_stuck] =
        CSLoader::createNode("Animation/animation_stuck.csb");
    _animationNodes[animation_die] =
        CSLoader::createNode("Animation/animation_die.csb");

    _animationActions[animation_front] =
        CSLoader::createTimeline("Animation/animation_front.csb");
    _animationActions[animation_left] =
        CSLoader::createTimeline("Animation/animation_left.csb");
    _animationActions[animation_right] =
        CSLoader::createTimeline("Animation/animation_right.csb");
    _animationActions[animation_back] =
        CSLoader::createTimeline("Animation/animation_back.csb");
    _animationActions[animation_stuck] =
        CSLoader::createTimeline("Animation/animation_stuck.csb");
    _animationActions[animation_die] =
        CSLoader::createTimeline("Animation/animation_die.csb");

    animationInit(animation_front);
    animationInit(animation_left);
    animationInit(animation_right);
    animationInit(animation_back);
    animationInit(animation_stuck);
    animationInit(animation_die);

    _currentDirection=-1;
    _status=player_sts_free;

    SetAnimation(animation_front);
}

void BPlayer::animationInit(int direction)
{
    this->addChild(_animationNodes[direction]);
    _animationNodes[direction]->runAction(_animationActions[direction]);
    _animationActions[direction]->gotoFrameAndPlay(0, true);
    _animationNodes[direction]->setVisible(false);
}

int BPlayer::CurrentDirection()
{
    return _currentDirection;
}

void BPlayer::SetDirection(int direction)
{
    if(_status!=player_sts_free){
        return;
    }
    SetAnimation(direction);
}

void BPlayer::SetAnimation(int direction)
{
    if(direction< 0 || direction >= animation_max){
        return;
    }
    if(_currentDirection==direction){
        return;
    }
    if(_currentDirection!=-1){
        _animationNodes[_currentDirection]->setVisible(false);
    }
    _currentDirection=direction;
    _animationNodes[_currentDirection]->setVisible(true);
}

void BPlayer::SetStatus(int status)
{
    this->_status=status;
    if(status==player_sts_stuck){
        SetAnimation(animation_stuck);
    }else if(status==player_sts_die){
        SetAnimation(animation_die);
    }
}

void BPlayer::setPosition(const cocos2d::Vec2& position)
{
    //只有自由状态才能移动.
    if(this->_status!=player_sts_free){
        return;
    }

    Node::setPosition(position);
}
