#include "BeachScene.hpp"
#include "BubbleApp.hpp"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;

msgbin::RoomReadyNtf gRoomReadNtf;
msgbin::RoomReadyNtf gRoomReadNtfBak;

Scene* BeachScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene=Scene::create();

    // 'layer' is an autorelease object
    auto layer=BeachScene::create();

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

    string file="Beach/Beach.tmx";
    auto str=String::createWithContentsOfFile (FileUtils::getInstance()->
                                               fullPathForFilename(file.c_str()).
                                               c_str());
    this->_tileMap=TMXTiledMap::createWithXML(str->getCString(),"");
    addChild(_tileMap, -1);

    this->_meta=_tileMap->getLayer("Meta");
    this->_meta->setVisible(false);
    this->_foreground=_tileMap->getLayer("Foreground");
    this->_background=_tileMap->getLayer("Background");

    _rocker=BRocker::createRocker("Rocker/joystick_bg.png",
                                  "Rocker/joystick_center.png",
                                  Point(150,150));
    this->addChild(_rocker);
    _rocker->startRocker(true);

    //单点触摸
    /*
    auto listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=[&](Touch *, Event *)->bool { return true; };
    listener->onTouchEnded=CC_CALLBACK_2(BeachScene::onTouchEnded, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    listener->setSwallowTouches(false);
    */

    //多点触摸
    ///*
    auto listener=EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded=CC_CALLBACK_2(BeachScene::onTouchesEnded, this);
    listener->onTouchesBegan=CC_CALLBACK_2(BeachScene::onTouchesBegan, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //*/

    _bubbleManager=BubbleManager::create();
    addChild(_bubbleManager);


    _playerManager=BPlayerManager::create();
    addChild(_playerManager);

    initAllPlayer();

    HandlerMap[6] = &BeachScene::RoomCloseNtf;
    HandlerMap[7] = &BeachScene::RoomUserChgNtf;
    HandlerMap[8] = &BeachScene::RoomSetBubbleNtf;
    HandlerMap[9] = &BeachScene::BubbleBombNtf;
    HandlerMap[10] = &BeachScene::RoomUserStatusChgNtf;

    scheduleUpdate();

    SimpleAudioEngine::getInstance()->preloadEffect("music/bomb_break.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("music/bomb_on.mp3");
    SimpleAudioEngine::getInstance()->playEffect("music/readygo_music.mp3");
    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bg.mp3", true);

    return true;
}

void BeachScene::initAllPlayer()
{
    LOG("uidx %d", gRoomReadNtf.uIdx);
    for(size_t i=0; i<gRoomReadNtf.uPosAll.size(); i++) {
        msgbin::RoomUser u = gRoomReadNtf.uPosAll.at(i);
        cocos2d::Point pos;
        pos.x = u.pos.x;
        pos.y = u.pos.y;
        LOG("init player src %ld %f %f", i, pos.x ,pos.y);
        pos = positionForTileCoord(pos);
        LOG("init player chg %ld %f %f", i, pos.x ,pos.y);
        u.pos.x= pos.x;
        u.pos.y= pos.y;
        gRoomReadNtf.uPosAll[i] = u;
        _playerManager->MakePlayer(i, pos);
    }
}

//同步其他玩家的位置和方向动画和状态。
void BeachScene::updateAllPlayer()
{
    for(size_t i=0; i<gRoomReadNtf.uPosAll.size(); i++) {
        if( i == (size_t)gRoomReadNtf.uIdx) {
            //不更新自己
            continue;
        }
        msgbin::RoomUser u = gRoomReadNtf.uPosAll.at(i);
        cocos2d::Point pos;
        pos.x = u.pos.x;
        pos.y = u.pos.y;
        // LOG("%ld pos x %f  y %f status %d dir %d", i, pos.x, pos.y,
        //     u.status, u.direction);
        // _playerManager->SetDirection(i, u.direction);
        _playerManager->SetStatus(i, u.status);
        _playerManager->SetPosition(i, pos);
    }
}

void BeachScene::setViewPointCenter(Point position)
{
    auto winSize=Director::getInstance()->getWinSize();
    int x=MAX(position.x, winSize.width/2);
    int y=MAX(position.y, winSize.height/2);
    x=MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width)
          - winSize.width / 2);
    y=MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height)
          - winSize.height / 2);
    auto actualPosition=Point(x, y);

    auto centerOfView=Point(winSize.width/2, winSize.height/2);
    auto viewPoint=centerOfView - actualPosition;
    this->setPosition(viewPoint);
}


void BeachScene::updateMainPlayerStatus(int direct)
{
    BPlayer* p ;
    int uid = gRoomReadNtf.uIdx;
    //LOG("uid %d", uid);
    if((p = this->_playerManager->FindPlayer(uid)) == NULL) {
        return;
    }

    auto status = this->rockerToStatus(direct);
    auto currentPos = p->getPosition();
    auto pos=playerNextPosition(currentPos,direct);
    auto currentStatus = p->GetStatus();

    if(currentStatus >= animation_stuck) {
        return;
    }

    if(status == currentStatus && pos == currentPos){
        return;
    }

    if(!doesPositionBlock(currentPos,pos)) {
        p->setPosition(pos);
        p->SetStatus(status);
    }else{
        p->SetStatus(status);
    }
    syncMainPlayerInfo(pos,status);
}

void BeachScene::syncMainPlayerInfo(cocos2d::Point pos, int status)
{
    msgbin::byte_t *b1,*b2;
    msgbin::RoomUser u = gRoomReadNtf.uPosAll.at(gRoomReadNtf.uIdx);
    u.direction=0;
    u.status=status;
    u.pos.x = pos.x;
    u.pos.y = pos.y;

    msgbin::RoomUserChg chg;
    chg.uIdx = gRoomReadNtf.uIdx;
    chg.user = u;

    //发送登录包
    b1 = (msgbin::byte_t *)gBubbleApp.Wb;
    b2 = b1;

    msgbin::BzWriteRoomUserChg(&b1, &chg);

    gBubbleApp.WriteAPI(7, b1-b2);
    gBubbleApp.SendBytes(gBubbleApp.Wbh, b1-b2+4);
}

void BeachScene::setMainPlayerPosition(Point position)
{
    int uid = gRoomReadNtf.uIdx;
    this->_playerManager->SetPosition(uid,position);
}

void BeachScene::setMainPlayerStatus(int d)
{
    int uid = gRoomReadNtf.uIdx;
    this->_playerManager->SetStatus(uid,d);
}


#define MOVE_STEP 4

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
    if(direct<0) {
        return old_pos;
    }
    auto MoveItem=MoveMap[direct];
    assert(MoveItem.direct==direct);
    auto new_pos=old_pos + Point(MoveItem.x,MoveItem.y);
    if (new_pos.x +38 <=
        (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
        new_pos.y +44 <=
        (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
        new_pos.y -44 >= 0 &&
        new_pos.x -38 >= 0)
    {
        return new_pos;
    }else{
        return old_pos;
    }
}

bool BeachScene::doesPositionBlock(Point pos)
{
    Point tileCoord=this->tileCoordForPosition(pos);
    //log("xx tile x %f y %f positon %f %f", tileCoord.x,tileCoord.y,
    //    position.x, position.y);
    int tiledGid=_meta->getTileGIDAt(tileCoord);
    if (tiledGid) {
        auto properties=_tileMap->getPropertiesForGID(tiledGid).asValueMap();
        if(!properties.empty()) {
            auto collision=properties["Collidable"].asString();
            if ("True" == collision) {
                //SimpleAudioEngine::getInstance()->playEffect("hit.mp3");
                return true;
            }
        }
    }

    return false;
}

#define BLOCK_RADIO (12)

//获取到玩家4个角的坐标，分别进行冲突检查。
bool BeachScene::doesPositionBlock(Point cpos,Point npos)
{
    Point p00,p01,p11,p10;


    p00 = npos + Point(-BLOCK_RADIO,-BLOCK_RADIO);
    p01 = npos + Point(-BLOCK_RADIO,BLOCK_RADIO);
    p11 = npos + Point(BLOCK_RADIO,BLOCK_RADIO);
    p10 = npos + Point(BLOCK_RADIO,-BLOCK_RADIO);

    if(doesPositionBlock(p00)==true){
        return true;
    }
    if(doesPositionBlock(p01)==true ){
        return true;
    }
    if(doesPositionBlock(p10)==true  ){
        return true;
    }
    if(doesPositionBlock(p11)==true ){
        return true;
    }

    auto tileN = tileCoordForPosition(npos);
    if(tileCoordForPosition(cpos) != tileN
       && _bubbleManager->doesBlock(tileN.x, tileN.y)) {
        return true;
    }

    return false;
}


void BeachScene::update(float)
{
    auto direct=_rocker->GetDirection();
    //更新主角位置
    updateMainPlayerStatus(direct);
    loopMsg();
    updateAllPlayer();
}


void BeachScene::onTouchesEnded(const std::vector<Touch*>& touches, Event *e)
{
    for (auto &item: touches)       //遍历容器中的各个成员！！！多点触摸时将显示同时显示多个精灵
    {
        onTouchEnded(item, e);
    }
}


void BeachScene::onTouchesBegan(const std::vector<Touch*>& touches, Event *)
{
    for (auto &item: touches)       //遍历容器中的各个成员！！！多点触摸时将显示同时显示多个精灵
    {
        auto touch = item;
        auto location = touch->getLocation();
        //加载一个精灵
        LOG("touch  begin %f %f", location.x, location.y);
    }
}


void BeachScene::onTouchEnded(Touch *touch, Event *)
{
    auto touchLocation=touch->getLocation();
    //将点击的tail删除掉.| 有用。
    //Point tileCoord=this->tileCoordForPosition(touchLocation);

    if(touchLocation.x < 500 &&  touchLocation.y < 500){
        return;
    }

    // tileExpolsed(tileCoord);
    BPlayer* p ;
    if((p = this->_playerManager->FindPlayer(gRoomReadNtf.uIdx)) == NULL) {
        return;
    }

    if(p->GetStatus() >= animation_stuck) {
        return;
    }

    //客户端自己设置
    auto uPos = p->getPosition();
    auto pos = this->tileCoordForPosition(uPos);
    // pos.x = fixToMiddleTilePos(uPos.x);
    // pos.y = fixToMiddleTilePos(uPos.y);
    // LOG("touch origin x %f y %f", uPos.x, uPos.y);
    // LOG("touch fixed x %f y %f", pos.x, pos.y);
    // // _bubbleManager->MakeBubble(2,pos);

    SimpleAudioEngine::getInstance()->playEffect("music/bomb_on.mp3");

    //向服务器设置
    msgbin::Bubble bubble;
    bubble.pos.x = pos.x;
    bubble.pos.y = pos.y;
    bubble.power = 1;
    bubble.keeptime = 3;
    this->setBubble(bubble);
}


Point BeachScene::tileCoordForPosition(Point position)
{
    int x=position.x / _tileMap->getTileSize().width;
    int y=((_tileMap->getMapSize().height *
            _tileMap->getTileSize().height) - position.y) /
        _tileMap->getTileSize().height;

    y=MIN(MAX(0, y), _tileMap->getMapSize().height-1);
    x=MIN(MAX(0, x), _tileMap->getMapSize().width-1);

    //log("tileCoordForPosition point x %d y %d %f %f", x, y,
    //    _tileMap->getMapSize().width, _tileMap->getMapSize().height);
    return Point(x, y);
}

Point BeachScene::positionForTileCoord(cocos2d::Point position)
{
    LOG("map height %f", _tileMap->getMapSize().height);
    position.y = _tileMap->getMapSize().height - position.y -1;

    int x = (position.x) * _tileMap->getTileSize().width +
        _tileMap->getTileSize().width/2;
    int y = (position.y) * _tileMap->getTileSize().height +
        _tileMap->getTileSize().height/2;
    //LOG("PositionFortileCoord point x %d y %d", x, y);
    return Point(x, y);
}

void BeachScene::tileExpolsed(Point tileCoord)
{
    //log("expose x %f y %f", tileCoord.x, tileCoord.y);
    int tiledGid = _meta->getTileGIDAt(tileCoord);
    if(tiledGid==0) {
        return;
    }
    auto properties = _tileMap->getPropertiesForGID(tiledGid).asValueMap();
    if(!properties.empty()) {
        auto collectable = properties["CanBeBomb"].asString();
        if ("True" == collectable) {
            _meta->removeTileAt(tileCoord);
            _foreground->removeTileAt(tileCoord);
        }
    }
}


void BeachScene::RoomCloseNtf(QueueMsg *)
{
    if( gBubbleApp.Status == BUBBLE_APP_STS_FIGHT ){
        gBubbleApp.Status=BUBBLE_APP_STS_SUCCESS;
        gBubbleApp.SceneSuccess();
        gRoomReadNtf = gRoomReadNtfBak;
        gBubbleApp.Join();
    }
    LOG("RoomCloseNtf");
}


void BeachScene::RoomSetBubbleNtf(QueueMsg *msg)
{
    msgbin::SetBubble setBubbleNtf;
    msgbin::byte_t *buffer = (msgbin::byte_t *)msg->D();
    msgbin::BzReadSetBubble(&buffer, &setBubbleNtf);

    Point pos1,pos2;

    //设置客户端显示
    pos1.x = setBubbleNtf.b.pos.x;
    pos1.y = setBubbleNtf.b.pos.y;

    pos2 = this->positionForTileCoord(pos1);
    LOG("idb %d ", setBubbleNtf.b.id);
    LOG("pos1 x %f y %f ", pos1.x, pos1.y);
    LOG("pos2 x %f y %f ", pos2.x, pos2.y);
    auto bubble = _bubbleManager->MakeBubble(setBubbleNtf.b.id,
                                             setBubbleNtf.b.power,
                                             pos2);
    bubble->TileX = pos1.x;
    bubble->TileY = pos1.y;
}

void BeachScene::setBubble(msgbin::Bubble bubble)
{
    msgbin::byte_t *b1,*b2;

    //发送登录包
    b1 = (msgbin::byte_t *)gBubbleApp.Wb;
    b2 = b1;

    msgbin::SetBubble setBubble;
    setBubble.uIdx = gRoomReadNtf.uIdx;
    setBubble.b = bubble;
    msgbin::BzWriteSetBubble(&b1, &setBubble);

    gBubbleApp.WriteAPI(8, b1-b2);
    gBubbleApp.SendBytes(gBubbleApp.Wbh, b1-b2+4);
}

void BeachScene::RoomUserChgNtf(QueueMsg *msg)
{
    msgbin::RoomUserChg roomUserChg;
    msgbin::byte_t *buffer = (msgbin::byte_t *)msg->D();
    msgbin::BzReadRoomUserChg(&buffer, &roomUserChg);

    //msgbin::RoomUser u = gRoomReadNtf.uPosAll.at(roomUserChg.uid);
    gRoomReadNtf.uPosAll[roomUserChg.uIdx] = roomUserChg.user;
}


void BeachScene::RoomUserStatusChgNtf(QueueMsg *msg)
{
    msgbin::RoomUserStatusChg chg;
    msgbin::byte_t *buffer = (msgbin::byte_t *)msg->D();
    msgbin::BzReadRoomUserStatusChg(&buffer, &chg);

    gRoomReadNtf.uPosAll[chg.id].status = chg.status;
    _playerManager->SetStatus(chg.id, chg.status);
    LOG("RoomUserStatusChgNtf %d sts %d", chg.id, chg.status);
}

void BeachScene::BubbleBombNtf(QueueMsg *msg)
{
    msgbin::BubbleBomb bubbleBomb;
    msgbin::byte_t *buffer = (msgbin::byte_t *)msg->D();
    msgbin::BzReadBubbleBomb(&buffer, &bubbleBomb);


    _bubbleManager->SetStatus(bubbleBomb.id, bubble_sts_expose);
    auto destroyTiles = bubbleBomb.destroyTiles;
    auto destroyUses = bubbleBomb.destroyUsers;

    LOG("bomb id %d size %ld", bubbleBomb.id, destroyTiles.size());

    for(size_t i=0; i< destroyTiles.size(); i++) {
        auto tile = bubbleBomb.destroyTiles[i];
        Point tileCoord;
        tileCoord.x =tile.x;
        tileCoord.y =tile.y;
        LOG("destroy %f %f", tileCoord.x, tileCoord.y);
        this->tileExpolsed(tileCoord);
    }

    SimpleAudioEngine::getInstance()->playEffect("music/bomb_break.mp3");

    //玩家被爆掉。
    // for(size_t i=0; i< destroyUses.size(); i++) {
    //     LOG("destroy_use %ld user %d",i, destroyUses[i]);
    //     _playerManager->SetStatus(destroyUses[i], animation_stuck);
    //     gRoomReadNtf.uPosAll[destroyUses[i]].status = animation_stuck;
    // }

}


void BeachScene::loopMsg()
{
    QueueMsg *msg;
    while (( msg = gBubbleApp.Mq.Pick()) ) {
        //LOG("msg tt %d", msg->T());
        BeachSceneFptr h = this->HandlerMap[msg->T()];
        if (h!=NULL) {
            (this->*h)(msg);
        }
    }
}


int BeachScene::rockerToStatus(int d)
{
    switch(d){
    case -1:
        return animation_free;
    case rocker_left:
        return animation_left;
    case rocker_right:
        return animation_right;
    case rocker_up:
        return animation_back;
    case rocker_down:
        return animation_front;
    }
    return animation_free;
}


float BeachScene::fixToMiddleTilePos(float pos)
{
    pos = pos >= 32 ? pos : 32;
    float f = (pos - 32) / 64;
    float b =  (f - int(f / 1));
    LOG("f %f b %f", f, b);
    if( b  <= 0.5 ) {
        f = int(f);
    }else{
        f = int(f) + 1;
    }
    return f*64 +32;
}
