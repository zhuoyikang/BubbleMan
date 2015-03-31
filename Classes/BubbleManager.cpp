/**
 * 泡泡管理
 */

#include "BubbleManager.hpp"
#include "BTime.hpp"

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

void BubbleManager::MakeBubble(int power, cocos2d::Point position)
{
    MakeBubble(newId(),power,position);
}

void BubbleManager::MakeBubble(int id,int power, Point position)
{
    int now=BTime::GetCurrentUtcSecond();
    auto _bubble = new Bubble(id,power,now+2);
    _bubble->setPosition(position);
    this->addChild(_bubble);
    this->_bubbleList.pushBack(_bubble);
}

void BubbleManager::SetStatus(int id,int status)
{
    for(cocos2d::Vector<Bubble*>::iterator iter=_bubbleList.begin();
        iter!=_bubbleList.end(); ) {
        if((*iter)->GetID()==id){
            (*iter)->SetStatus(status);
        }else{
            iter++;
        }
    }
}

void BubbleManager::Explose(int id)
{
    this->SetStatus(id,bubble_sts_expose);
}

void BubbleManager::update(float)
{
    int now=BTime::GetCurrentUtcSecond();
    //log("BubbleManager::update %d",now);
    cocos2d::Vector<Bubble*> deleteList;
    for(cocos2d::Vector<Bubble*>::iterator iter=_bubbleList.begin();
        iter!=_bubbleList.end(); ) {
        if((*iter)->UpdateStatus(now) == bubble_sts_end){
            this->removeChild(*iter);
            //log("delete");
            iter=_bubbleList.erase(iter);
        }else{
            iter++;
        }
    }
}
