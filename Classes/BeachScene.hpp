//
//  BeachScene.h
//  T4
//
//  Created by zhuoyikang on 15-3-17.
//
//

#ifndef __T4__BeachScene__
#define __T4__BeachScene__

#include <stdio.h>
#include "Rocker.hpp"
#include "PlayerSprite.hpp"
#include "Bubble.hpp"
#include "BubbleManager.hpp"


class BeachScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    virtual bool init();

    void update(float dt) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *);

    //用来计算Player移动坐标.
    Point playerNextPosition(Point old, int direct);
    //检查位置实时被阻塞.
    bool doesPositionBlock(Point old,int direct);

    //根据位置计算tile坐标.
    Point tileCoordForPosition(cocos2d::Point position);

    // implement the "static create()" method manually
    CREATE_FUNC(BeachScene);
private:

    BubbleManager *_bubbleManager;

    void setViewPointCenter(cocos2d::Point position);
    void setPlayerPosition(cocos2d::Point position);

    cocos2d::TMXTiledMap *_tileMap;
    cocos2d::Sprite * _player;
    cocos2d::TMXLayer *_background;
    cocos2d::TMXLayer *_meta;
    cocos2d::TMXLayer *_foreground;

    HRocker *_rocker;
    PlayerSprite *_playerSprite;
    //Bubble *_bubble;
};


#endif /* defined(__T4__BeachScene__) */
