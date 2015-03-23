//
//  BubbleManager.cpp
//  T4
//
//  Created by zhuoyikang on 15-3-23.
//
//

#include "BubbleManager.hpp"

USING_NS_CC;


bool BubbleManager::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    scheduleUpdate();
    return true;
}

void BubbleManager::MakeBubble(int, Point position)
{
    int now=getCurrentUtcSecond();
    auto _bubble = new Bubble(now+2);
    _bubble->setPosition(position);
    this->addChild(_bubble);
    this->_bubbleList.pushBack(_bubble);
}


// 将已经爆炸的泡泡删除.
void BubbleManager::update(float)
{
    int now=getCurrentUtcSecond();
    log("BubbleManager::update %d",now);
    cocos2d::Vector<Bubble*> deleteList;
    for(cocos2d::Vector<Bubble*>::iterator iter=_bubbleList.begin();
        iter!=_bubbleList.end(); ) {
        if((*iter)->CheckStatus(now) == bubble_sts_end){
            this->removeChild(*iter);
            log("delete");
            iter=_bubbleList.erase(iter);
        }else{
            iter++;
        }
    }
}

// 获取当前的utc秒数
int BubbleManager::getCurrentUtcSecond()
{
    struct  timeval    tv;
    struct  timezone   tz;
    gettimeofday(&tv,&tz);
    return tv.tv_sec;
}
