/**
 * 泡泡
 */

#ifndef __T4__Bubble__
#define __T4__Bubble__

#include <stdio.h>
#include <string>
#include "cocos2d.h"
#include "const.hpp"
#include "BIdGen.hpp"

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

/**
 * 泡泡爆炸后停留时间
 */
#define BUBBLE_EXPOSE_KEEP_TIME 2


struct CocosStudioRes {
    cocos2d::Node* Node;
    cocostudio::timeline::ActionTimeline* Action;
};


class Bubble: public cocos2d::Node, public BIdentity
{
public:
    Bubble(int id,int power,int time);

    /**
     * 自动状态变化，每调用一次状态变化一次
     */
    void StateTest();

    void initExplose();
    void releaseExplose();

    void initDefault();
    void releaseDefault();

    /**
     * 设置泡泡的各种状态，设置后有各种效果
     * @param status @see bubble_sts_def
     */
    void SetStatus(int status);
    int GetStatus();

    inline int GetID() {
        return _id;
    }

    /**
     * 根据爆炸时间检查状态，并执行变化
     * @param time 泡泡是根据当前时间time来检查变化的
     * @return 返回泡泡的最新状态
     */
    int UpdateStatus(int time);

    /* 类型.
     */
    int Type;

private:
    void setStatusEnd();
    int _status;

    CocosStudioRes* _powerRes[wave_power_max];
    CocosStudioRes _waveRes[bubble_max];

    /**
     * 十字破坏范围.
     */
    int _power;


    /**
     * 爆炸时间.
     */
    int _exposeTime;
};



#endif /* defined(__T4__Bubble__) */
