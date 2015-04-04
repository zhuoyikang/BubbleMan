//
//  BPlayer.cpp
//  T4/

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
    _animationNodes[animation_free] =
        CSLoader::createNode("Animation/animation_front.csb");
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

    _animationActions[animation_free] =
        CSLoader::createTimeline("Animation/animation_front.csb");
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

    animationInit(animation_free);  // free== front
    animationInit(animation_front);
    animationInit(animation_left);
    animationInit(animation_right);
    animationInit(animation_back);
    animationInit(animation_stuck);
    animationInit(animation_die);

    _status = -1;
    SetStatus(animation_free);
}

void BPlayer::animationInit(int direction)
{
    this->addChild(_animationNodes[direction]);
    _animationNodes[direction]->runAction(_animationActions[direction]);
    _animationActions[direction]->gotoFrameAndPlay(0, true);
    _animationNodes[direction]->setVisible(false);
}

int BPlayer::GetStatus()
{
    return _status;
}

void BPlayer::SetStatus(int status)
{
    if(status< 0 || status >= animation_max){
        return;
    }
    if(_status==status){
        return;
    }
    if(_status!=-1){
        _animationNodes[_status]->setVisible(false);
    }
    _status=status;
    _animationNodes[_status]->setVisible(true);

}

void BPlayer::setPosition(const cocos2d::Vec2& position)
{
    //只有自由状态才能移动.
    if(this->_status==animation_stuck ||this->_status==animation_die){
        return;
    }

    Node::setPosition(position);
}
