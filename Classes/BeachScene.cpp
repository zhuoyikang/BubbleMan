#include "BeachScene.hpp"
#include "BubbleApp.hpp"

USING_NS_CC;

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
                                    Point(110,100));
    this->addChild(_rocker);
    _rocker->startRocker(true);

    auto listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=[&](Touch *, Event *)->bool { return true; };
    listener->onTouchEnded=CC_CALLBACK_2(BeachScene::onTouchEnded, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    listener->setSwallowTouches(false);

    _bubbleManager=BubbleManager::create();
    addChild(_bubbleManager);


    _playerManager=BPlayerManager::create();
    addChild(_playerManager);

    initAllPlayer();


    HandlerMap[6] = &BeachScene::RoomCloseNtf;
    HandlerMap[7] = &BeachScene::RoomUserChgNtf;

    scheduleUpdate();

    return true;
}

void BeachScene::initAllPlayer()
{
    LOG("uidx %d", gRoomReadNtf.uIdx);
    for(size_t i=0; i<gRoomReadNtf.uPosAll.size(); i++){
        msgbin::RoomUser u = gRoomReadNtf.uPosAll.at(i);
        cocos2d::Point pos;
        pos.x = u.pos.x;
        pos.y = u.pos.y;
        pos = positionForTileCoord(pos);
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
    if((p = this->_playerManager->FindPlayer(uid)) == NULL){
        return;
    }

    auto status = this->rockerToStatus(direct);
    auto currentPos = p->getPosition();
    auto pos=playerNextPosition(currentPos,direct);

    if(status == p->GetStatus() && pos == currentPos){
        return;
    }

    if(!doesPositionBlock(pos,direct)) {
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
    chg.uid = gRoomReadNtf.uIdx;
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
    position.y=position.y-32;

    if(direct==rocker_up){
        position.y+=16;
    }else if(direct==rocker_down){
        position.y-=32;
    }else if(direct==rocker_left) {
        position.x-=32;
    }else if(direct==rocker_right){
        position.x+=32;
    }

    Point tileCoord=this->tileCoordForPosition(position);
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


void BeachScene::update(float)
{
    auto direct=_rocker->GetDirection();
    //更新主角位置
    updateMainPlayerStatus(direct);
    loopMsg();
    updateAllPlayer();
}

void BeachScene::onTouchEnded(Touch *touch, Event *)
{
    //auto touchLocation=touch->getLocation();
    //将点击的tail删除掉.| 有用。
    // Point tileCoord=this->tileCoordForPosition(touchLocation);
    // tileExpolsed(tileCoord);
    BPlayer* p ;
    if((p = this->_playerManager->FindPlayer(gRoomReadNtf.uIdx)) == NULL){
        return;
    }

    auto pos = p->getPosition();
    _bubbleManager->MakeBubble(2,pos);

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
    int x = (position.x-1) * _tileMap->getTileSize().width +  _tileMap->getTileSize().width/2;
    int y = (position.y-1) * _tileMap->getTileSize().height +  _tileMap->getTileSize().height/2;
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


void BeachScene::RoomCloseNtf(QueueMsg *msg)
{
    if( gBubbleApp.Status == BUBBLE_APP_STS_FIGHT ){
        gBubbleApp.Status=BUBBLE_APP_STS_SUCCESS;
        gBubbleApp.SceneSuccess();
        gRoomReadNtf = gRoomReadNtfBak;
        gBubbleApp.Join();
    }
    LOG("RoomCloseNtf");
}



void BeachScene::RoomUserChgNtf(QueueMsg *msg)
{
    msgbin::RoomUserChg roomUserChg;
    msgbin::byte_t *buffer = (msgbin::byte_t *)msg->D();
    msgbin::BzReadRoomUserChg(&buffer, &roomUserChg);

    //msgbin::RoomUser u = gRoomReadNtf.uPosAll.at(roomUserChg.uid);
    gRoomReadNtf.uPosAll[roomUserChg.uid] = roomUserChg.user;
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
