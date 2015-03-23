//
//  Bubble.h
//  T4
//
//  Created by zhuoyikang on 15-3-20.
//
//

#ifndef __T4__Bubble__
#define __T4__Bubble__

#include <stdio.h>
#include <string>
#include "cocos2d.h"
#include "const.hpp"

using namespace std;

enum {
    wave_power_left,
    wave_power_right,
    wave_power_top,
    wave_power_Below,
    wave_power_max
};

enum {
    bubble_default,
    wave_center_h,
    wave_center_v,
    wave_left_end,
    wave_right_end,
    wave_top_end,
    wave_below_end,
    bubble_max
};

enum {
    bubble_sts_def,
    bubble_sts_expose,
    bubble_sts_end
};


struct CocosStudioRes {
    cocos2d::Node* Node;
    cocostudio::timeline::ActionTimeline* Action;
};


class Bubble: public cocos2d::Node
{
public:
    Bubble();
    Bubble(int time);

    //状态变化
    void StateTest();

    //泡泡爆炸
    void initExplose();
    void releaseExplose();

    void initDefault();
    void releaseDefault();

    void SetStatus(int status);
    int GetStatus();

    // 检查状态
    int CheckStatus(int time);

private:
    void setStatusEnd();

    int _status;
    int _power;  //破坏范围.
    CocosStudioRes* _powerRes[wave_power_max];
    CocosStudioRes _waveRes[bubble_max];

    //爆炸时间.
    int _exposeTime;
};



#endif /* defined(__T4__Bubble__) */
