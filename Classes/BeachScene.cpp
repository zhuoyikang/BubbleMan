//
//  BeachScene.cpp
//  T4
//
//  Created by zhuoyikang on 15-3-17.
//
//

#include "BeachScene.hpp"

USING_NS_CC;


Scene* BeachScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = BeachScene::create();

    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BeachScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    std::string file = "Beach/Beach.tmx";
    auto str = String::createWithContentsOfFile (FileUtils::getInstance()->
                                                 fullPathForFilename(file.c_str()).
                                                 c_str());
    this->_tileMap = TMXTiledMap::createWithXML(str->getCString(),"");
    addChild(_tileMap, -1);

    this->_meta = _tileMap->getLayer("Meta");
    this->_meta->setVisible(false);

    _rocker = HRocker::createRocker("Rocker/joystick_bg.png",
                                    "Rocker/joystick_center.png",
                                    Point(110,60));
    this->addChild(_rocker);
    _rocker->startRocker(true);


    _playerSprite = new PlayerSprite();
    addChild(_playerSprite);
    _playerSprite->setPosition(Point(visibleSize.width/2, visibleSize.height/2));

    scheduleUpdate();
    return true;
}



void BeachScene::setViewPointCenter(cocos2d::Point position)
{
    auto winSize = Director::getInstance()->getWinSize();
    int x = MAX(position.x, winSize.width/2);
    int y = MAX(position.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width)
            - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height)
            - winSize.height / 2);
    auto actualPosition = Point(x, y);

    auto centerOfView = Point(winSize.width/2, winSize.height/2);
    auto viewPoint = centerOfView - actualPosition;
    this->setPosition(viewPoint);
}

void BeachScene::setPlayerPosition(cocos2d::Point position)
{
    _player->setPosition(position);
}

void  BeachScene::update(float )
{
    switch(_rocker->GetDirection())
    {
    case rocker_left:
        _playerSprite->SetDirection(animation_left);
        break;
    case rocker_right:
        _playerSprite->SetDirection(animation_right);
        break;
    case rocker_up:
        _playerSprite->SetDirection(animation_back);
        break;
    case rocker_down:
        _playerSprite->SetDirection(animation_front);
        break;
    default:
        break;
    }
}
