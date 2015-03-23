//
//  BubbleManager.h
//  T4
//
//  Created by zhuoyikang on 15-3-23.
//
//

#ifndef __T4__BubbleManager__
#define __T4__BubbleManager__

#include "Bubble.hpp"
#include "cocos2d.h"


//管理场景中出现的所有Bubble.
class BubbleManager: public cocos2d::Layer
{
public:
    virtual bool init();

    void MakeBubble(int, cocos2d::Point);
    void update(float dt) override;

    CREATE_FUNC(BubbleManager);

private:
    int getCurrentUtcSecond();
    cocos2d::Vector<Bubble*> _bubbleList;
};


#endif /* defined(__T4__BubbleManager__) */
