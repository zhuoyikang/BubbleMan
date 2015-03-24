//
//  Rocker.h
//  T1
//
//  Created by zhuoyikang on 15-2-25.
//
//

#ifndef __T1__Rocker__
#define __T1__Rocker__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;


typedef enum {
    tag_rocker,
    tag_rockerBG,
}tagForBRocker;


typedef enum {
    rocker_left,
    rocker_right,
    rocker_up,
    rocker_down,
    rocker_stay,
}tagDirection;

class BRocker : public Layer
{
 public:
    virtual bool init();
    virtual ~BRocker();
    CREATE_FUNC(BRocker);

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static BRocker* createRocker(const char *bg, const char *bc, Point);

    //启动摇杆(显示摇杆、监听摇杆触屏事件)
    void startRocker(bool _isStopOther);
    //停止摇杆(隐藏摇杆，取消摇杆的触屏监听)
    void stopRocker();
    //获取方向
    int GetDirection();

 private:
    //判断控制杆方向，用来判断精灵上、下、左、右运动
    int rocketDirection;
    //当前人物行走方向,用来判断精灵的朝向，精灵脸朝右还是朝左
    bool rocketRun;
    //是否可操作摇杆
    bool isCanMove;
    //摇杆背景的坐标
    Point rockerBGPosition;
    //摇杆背景的半径
    float rockerBGR;


    //自定义初始化函数
    void rockerInit(const char* rockerImageName,const char* rockerBGImageName,
                    Point position);

    //获取当前摇杆与用户触屏点的角度
    float getRad(Point pos1,Point pos2);
    //触屏事件
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);

};


#endif /* defined(__T1__Rocker__) */
