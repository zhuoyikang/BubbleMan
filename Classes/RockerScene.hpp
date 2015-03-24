//
//  RockerScene.h
//  T4
//
//  Created by zhuoyikang on 15-3-18.
//
//

#ifndef __T4__RockerScene__
#define __T4__RockerScene__

#include <stdio.h>
#include "cocos2d.h"
#include "Rocker.hpp"

class RockerScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(RockerScene);
private:
    BRocker *_rocker;
};


#endif /* defined(__T4__RockerScene__) */
