//
//  Rocker.cpp
//  T1
//
//  Created by zhuoyikang on 15-2-25.
//
//
#include "Rocker.hpp"

const double PI=3.1415;

BRocker* BRocker::createRocker(const char *bg, const char *bc, Point position)
{
    BRocker *layer = BRocker::create();
    if(layer){
        layer->rockerInit(bc, bg, position);
    }

   // CC_SAFE_DELETE(layer);
    return layer;
}


bool BRocker::init()
{
    if(!Layer::init()){
        return false;
    }

    rocketRun=false;
    return true;
}

BRocker::~BRocker()
{
}


//启动摇杆(显示摇杆、监听摇杆触屏事件)
void BRocker::startRocker(bool )
{
    Sprite * rocker = (Sprite*)this->getChildByTag(tag_rocker);
    rocker->setVisible(true);

    Sprite * rockerBG = (Sprite*)this->getChildByTag(tag_rockerBG);
    rockerBG->setVisible(true);

    auto touchListener = EventListenerTouchOneByOne::create();
    //设置是否想下传递触摸
    touchListener->setSwallowTouches(false);

    touchListener->onTouchBegan =
        CC_CALLBACK_2(BRocker::onTouchBegan, this);
    touchListener->onTouchMoved =
        CC_CALLBACK_2(BRocker::onTouchMoved, this);
    touchListener->onTouchEnded =
        CC_CALLBACK_2(BRocker::onTouchEnded, this);

    this->_eventDispatcher->
        addEventListenerWithSceneGraphPriority(touchListener,this);
}

//停止摇杆(隐藏摇杆，取消摇杆的触屏监听)
void BRocker::stopRocker()
{
    Sprite * rocker = (Sprite*)this->getChildByTag(tag_rocker);
    rocker->setVisible(false);

    Sprite * rockerBG = (Sprite*)this->getChildByTag(tag_rockerBG);
    rockerBG->setVisible(false);

    _eventDispatcher->removeEventListenersForTarget(this);
}


//自定义初始化函数
void BRocker::rockerInit(const char* rockerImageName,const char* rockerBGImageName,
                         Point position)
{
    Sprite *spRockerBG = Sprite::create(rockerBGImageName);
    spRockerBG->setPosition(position);
    spRockerBG->setVisible(false);
    addChild(spRockerBG,0,tag_rockerBG);

    Sprite *spRocker = Sprite::create(rockerImageName);
    spRocker->setPosition(position);
    spRocker->setVisible(false);
    addChild(spRocker,0,tag_rocker);

    rockerBGPosition = position;
    rockerBGR = spRockerBG->getContentSize().width * 0.5;
    log("rocker x %f y %f", spRockerBG->getContentSize().width,
        spRockerBG->getContentSize().height);
    rocketDirection = -1;
}

//获取当前摇杆与用户触屏点的角度
float BRocker::getRad(Point pos1,Point pos2)
{
    float px1 = pos1.x;
    float py1 = pos1.y;
    float px2 = pos2.x;
    float py2 = pos2.y;

    float x = px2- px1;
    float y = py1 - py2;

    float xie = sqrt(pow(x,2) + pow(y,2));
    float cosAngle = x / xie;
    float rad = acos(cosAngle);

    if(py2 < py1){
        rad = - rad;
    }
    return rad;
}

Point getAngelePosition(float r,float angle){
    return Point(r*cos(angle),r*sin(angle));
}


//触屏事件
bool BRocker::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
    Sprite * rocker = (Sprite*)this->getChildByTag(tag_rocker);
    if(rocker->boundingBox().containsPoint(point)){
        isCanMove = true;
        //log("begin");
    }

    return true;
}

void BRocker::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if(!isCanMove){
        return;
    }

    Point point = pTouch->getLocation();
    Sprite * rocker = (Sprite*)this->getChildByTag(tag_rocker);
    //得到摇杆与触屏点所形成的角度
    float angle = getRad(rockerBGPosition,point);

    if (sqrt(pow((rockerBGPosition.x - point.x),2) +
             pow((rockerBGPosition.y - point.y),2)) >= rockerBGR)
    {
        //保证内部小圆运动的长度限制
        rocker->setPosition(getAngelePosition(rockerBGR,angle) +
                            Point(rockerBGPosition.x,rockerBGPosition.y));
    }
    else
    {
        //当没有超过，让摇杆跟随用户触屏点移动即可
        rocker->setPosition(point);
    }


    //判断方向
    if(angle>=-PI/4&&angle<PI/4)
    {
        rocketDirection=rocker_right;
        rocketRun=false;
        //log("%d",rocketDirection);
    }
    else if(angle>=PI/4&&angle<3*PI/4)
    {
        rocketDirection=rocker_up;
        //log("%d",rocketDirection);
    }
    else if((angle>=3*PI/4&&angle<=PI)||(angle>=-PI&&angle<-3*PI/4))
    {
        rocketDirection=rocker_left;
        rocketRun=true;
        //log("%d",rocketDirection);
    }
    else if(angle>=-3*PI/4&&angle<-PI/4)
    {
        rocketDirection=rocker_down;
        //log("%d",rocketDirection);
    }
}

void BRocker::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!isCanMove)
    {
        return;
    }
    Sprite *rockerBG = (Sprite*)this->getChildByTag(tag_rockerBG);
    Sprite *rocker = (Sprite*)this->getChildByTag(tag_rocker);
    rocker->stopAllActions();
    rocker->runAction(MoveTo::create(0.08f,rockerBG->getPosition()));
    isCanMove = false;
    rocketDirection=rocker_stay;
}

//获取方向
int BRocker::GetDirection()
{
    return this->rocketDirection;
}
