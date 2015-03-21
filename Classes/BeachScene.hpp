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


class BeachScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void update(float dt) override;

    // implement the "static create()" method manually
    CREATE_FUNC(BeachScene);
private:
    void setViewPointCenter(cocos2d::Point position);
    void setPlayerPosition(cocos2d::Point position);
    cocos2d::Point tileCoordForPosition(cocos2d::Point position);

    cocos2d::TMXTiledMap *_tileMap;
    cocos2d::Sprite * _player;
    cocos2d::TMXLayer *_background;
    cocos2d::TMXLayer *_meta;
    cocos2d::TMXLayer *_foreground;

    HRocker *_rocker;
    PlayerSprite *_playerSprite;

};


#endif /* defined(__T4__BeachScene__) */
