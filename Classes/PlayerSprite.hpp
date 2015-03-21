//
//  PlayerSprite.h
//  T4
//
//  Created by zhuoyikang on 15-3-17.
//
//

#ifndef __T4__PlayerSprite__
#define __T4__PlayerSprite__

#include <stdio.h>
#include "cocos2d.h"
#include "const.hpp"

enum {
    animation_left = 0,
    animation_right = 1,
    animation_back = 2,
    animation_front = 3,
    animation_max = 4
};

class PlayerSprite: public cocos2d::Node
{
public:
    PlayerSprite();
    void SetDirection(int direction);
    int CurrentDirection();

private:
    void directionInit(int direction);

    int _currentDirection;
    cocostudio::timeline::ActionTimeline* _animationnActions[animation_max];
    cocos2d::Node * _animationnNodes[animation_max];
};

#endif /* defined(__T4__PlayerSprite__) */
