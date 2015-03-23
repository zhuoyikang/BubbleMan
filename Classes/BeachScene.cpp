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
                                    Point(110,100));
    this->addChild(_rocker);
    _rocker->startRocker(true);


    _playerSprite = new PlayerSprite();
    addChild(_playerSprite,1);
    //_playerSprite->setAnchorPoint(Vec2(0.5,0));
    //_playerSprite->setPosition(Point(visibleSize.width/3*1, visibleSize.height/2));
    _playerSprite->setPosition(Point(64, 132));

    scheduleUpdate();

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch *, Event *)->bool { return true; };
    listener->onTouchEnded = CC_CALLBACK_2(BeachScene::onTouchEnded, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    listener->setSwallowTouches(false);

    _bubbleManager = BubbleManager::create();
    addChild(_bubbleManager);
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

#define MOVE_STEP 16

struct PlayerMoveMap
{
    int direct;
    int x;
    int y;
};


struct PlayerMoveMap MoveMap[]={
    {rocker_left,-MOVE_STEP,0},
    {rocker_right,MOVE_STEP,0},
    {rocker_up,0,MOVE_STEP},
    {rocker_down,0,-MOVE_STEP},
    {rocker_stay,0,0},
};

Point BeachScene::playerNextPosition(Point old_pos, int direct)
{
    if(direct<0){
        return old_pos;
    }
    auto MoveItem=MoveMap[direct];
    assert(MoveItem.direct==direct);
    auto new_pos = old_pos + Point(MoveItem.x,MoveItem.y);
    if (new_pos.x +32 <=
        (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
        new_pos.y +16 <=
        (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
        new_pos.y -64 >= 0 &&
        new_pos.x -32 >= 0)
    {
        return new_pos;
    }else{
        return old_pos;
    }
}


bool BeachScene::doesPositionBlock(Point position,int direct)
{
    //return false;
    position.y = position.y-32;

    if(direct==rocker_up){
        position.y+=16;
    }else if(direct==rocker_down){
        position.y-=32;
    }else if(direct==rocker_left) {
        position.x-=32;
    }else if(direct==rocker_right){
        position.x+=32;
    }

    Point tileCoord = this->tileCoordForPosition(position);
    log("xx tile x %f y %f positon %f %f", tileCoord.x,tileCoord.y,
        position.x, position.y);
    int tiledGid = _meta->getTileGIDAt(tileCoord);
    if (tiledGid) {
        auto properties = _tileMap->getPropertiesForGID(tiledGid).asValueMap();
        if(!properties.empty()) {
            auto collision = properties["Collidable"].asString();
            if ("True" == collision) {
                //SimpleAudioEngine::getInstance()->playEffect("hit.mp3");
                return true;
            }
        }
    }
    return false;
}


void BeachScene::update(float)
{
    auto direct = _rocker->GetDirection();
    _playerSprite->SetDirection(direct);
    auto pos=playerNextPosition(_playerSprite->getPosition(),direct);
    if(!doesPositionBlock(pos,direct)) {
        _playerSprite->setPosition(pos);
    }
}

void BeachScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *)
{
    auto touchLocation = touch->getLocation();
    auto position = _playerSprite->getPosition();
    //log("current position x %f y %f", touchLocation.x, touchLocation.y);
    _bubbleManager->MakeBubble(3,position);
}


cocos2d::Point BeachScene::tileCoordForPosition(cocos2d::Point position)
{
    int x = position.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height *
              _tileMap->getTileSize().height) - position.y) /
        _tileMap->getTileSize().height;

    y =  MIN(MAX(0, y), _tileMap->getMapSize().height-1);
    x =  MIN(MAX(0, x), _tileMap->getMapSize().width-1);

    log("tileCoordForPosition point x %d y %d %f %f", x, y,
        _tileMap->getMapSize().width, _tileMap->getMapSize().height);
    return Point(x, y);
}
