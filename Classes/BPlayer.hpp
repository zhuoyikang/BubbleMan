//
//  BPlayer.h
//  T4
//
//  Created by zhuoyikang on 15-3-17.
//
//

#ifndef __T4__BPlayer__
#define __T4__BPlayer__

#include <stdio.h>
#include "cocos2d.h"
#include "const.hpp"



/**
 * 玩家三种状态
 */
enum {
    player_sts_free,
    player_sts_stuck,
    player_sts_die
};


enum {
    animation_left = 0,
    animation_right = 1,
    animation_back = 2,
    animation_front = 3,
    animation_max = 4
};

class BPlayer: public cocos2d::Node
{
public:
    BPlayer();
    void SetDirection(int direction);
    int CurrentDirection();

private:
    void directionInit(int direction);

    int _currentDirection;
    cocostudio::timeline::ActionTimeline* _animationnActions[animation_max];
    cocos2d::Node * _animationnNodes[animation_max];
};

#endif /* defined(__T4__BPlayer__) */
