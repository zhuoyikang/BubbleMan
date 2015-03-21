//
//  Bubble.cpp
//  T4
//
//  Created by zhuoyikang on 15-3-20.
//
//

#include "Bubble.hpp"
#include "cocostudio/CocoStudio.h"
#include "CocosGUI.h"
#include <assert.h>

USING_NS_CC;

struct BubbleActionItem
{
    int action;
    string name;  //csb name
};

#define TILE_SIZE  (64)
#define BUBBLE_EXPOSE_TIME (0.4)

struct WaveDirectFactor {
    int direct;
    int x;
    int y;
};

static struct BubbleActionItem WavePowerMap[] = {
    {wave_power_left,"Bubble/WaveLeft.csb"},
    {wave_power_right,"Bubble/WaveRight.csb"},
    {wave_power_top,"Bubble/WaveTop.csb"},
    {wave_power_Below,"Bubble/WaveBelow.csb"},
};

static struct WaveDirectFactor WavePowerFactorMap[]={
    {wave_power_left,-1,0},
    {wave_power_right,1,0},
    {wave_power_top,0,1},
    {wave_power_Below,0,-1},
};


//动作表.
static struct BubbleActionItem WaveActionsMap[] = { //
    {bubble_default,"Bubble/BubbleDefault.csb"},
    {wave_center_h,"Bubble/WaveMiddleH.csb"},
    {wave_center_v,"Bubble/WaveMiddleV.csb"},
    {wave_left_end,"Bubble/WaveLeftEnd.csb"},
    {wave_right_end,"Bubble/WaveRightEnd.csb"},
    {wave_top_end,"Bubble/WaveTopEnd.csb"},
    {wave_below_end,"Bubble/WaveBelowEnd.csb"}
};


//启动默认泡泡/
void Bubble::initDefault()
{
    _waveRes[bubble_default].Node=
        CSLoader::createNode(WaveActionsMap[bubble_default].name);
    _waveRes[bubble_default].Action=
        CSLoader::createTimeline(WaveActionsMap[bubble_default].name);
    this->_waveRes[bubble_default].Node->setPosition(Point(0,0));
    this->_waveRes[bubble_default].Node->runAction(_waveRes[bubble_default].Action);
    this->_waveRes[bubble_default].Action->gotoFrameAndPlay(0,true);
    this->addChild(_waveRes[bubble_default].Node);

}

//删除默认泡泡资源
void Bubble::releaseDefault()
{
    this->removeChild(_waveRes[bubble_default].Node);
}

//启动爆炸资源
void Bubble::initExplose()
{
    //初始化end资源。
    for(int i=1;i<bubble_max;i++) {
        assert(i==(WaveActionsMap[i].action));
        this->_waveRes[i].Node=CSLoader::createNode(WaveActionsMap[i].name);
        this->_waveRes[i].Action=CSLoader::createTimeline(WaveActionsMap[i].name);

        _waveRes[i].Action->gotoFrameAndPlay(1,false);
        this->_waveRes[i].Node->runAction(this->_waveRes[i].Action);

        this->addChild(this->_waveRes[i].Node);
    }

    // 放置各种资源的位置:边界
    _waveRes[wave_center_h].Node->setPosition(Point(0,0));
    _waveRes[wave_center_v].Node->setPosition(Point(0,0));
    _waveRes[wave_left_end].Node->setPosition(Point((-TILE_SIZE)*(_power+1),0));
    _waveRes[wave_right_end].Node->setPosition(Point((TILE_SIZE)*(_power+1),0));
    _waveRes[wave_top_end].Node->setPosition(Point(0,(TILE_SIZE)*(_power+1)));
    _waveRes[wave_below_end].Node->setPosition(Point(0,(-TILE_SIZE)*(_power+1)));

    // 初始化Power资源.
    for(int i=0;i<wave_power_max;i++) {
        assert(i==(WavePowerMap[i].action));
        this->_powerRes[i] = new CocosStudioRes[_power];
        auto factor = WavePowerFactorMap[i];
        assert(i==factor.direct);

        for(int j=0;j<_power;j++)
        {
            auto node=CSLoader::createNode(WavePowerMap[i].name);
            auto action=CSLoader::createTimeline(WavePowerMap[i].name);
            node->runAction(action);
            action->gotoFrameAndPlay(1,false);

            struct CocosStudioRes res={node,action};
            this->_powerRes[i][j]= res;

            int x = TILE_SIZE * (j+1) * factor.x;
            int y = TILE_SIZE * (j+1) * factor.y;

            this->addChild(node);
            node->setPosition(Point(x,y));
        }
    }

    //给自己设置一个延时动作，1s后把自己的状态打成end.
    MoveTo * moveto = MoveTo::create(BUBBLE_EXPOSE_TIME, this->getPosition());
    CallFunc * func = CallFunc::create(CC_CALLBACK_0(Bubble::setStatusEnd,this));
    Sequence * sequenece = Sequence::create(moveto, func, NULL);
    this->runAction(sequenece);
}

void Bubble::setStatusEnd()
{
    this->SetStatus(bubble_sts_end);
}

//取消爆炸资源
void Bubble::releaseExplose()
{
    for(int i=1;i<bubble_max;i++) {
        assert(i==(WaveActionsMap[i].action));
        this->removeChild(this->_waveRes[i].Node);
    }
    for(int i=0;i<wave_power_max;i++) {
        CocosStudioRes* res=_powerRes[i];
        for(int j=0;j<_power;j++)
        {
            this->removeChild(res[j].Node);
        }
    }
}


Bubble::Bubble():_power(1)
{
    _status = bubble_sts_def;
    initDefault();
}


void Bubble::SetStatus(int status)
{
    if(this->_status==status){
        return;
    }

    if(_status==bubble_sts_def){
        releaseDefault();
    }

    if(_status==bubble_sts_expose){
        releaseExplose();
    }

    if(status==bubble_sts_def){
        initDefault();
    }

    if(status==bubble_sts_expose){
        initExplose();
    }

    this->_status=status;
}

int Bubble::GetStatus()
{
    return this->_status;
}

//状态变化
void Bubble::StateTest()
{
    if(this->GetStatus()==bubble_sts_def) {
        this->SetStatus(bubble_sts_expose);
    }else if(this->GetStatus()==bubble_sts_expose){
        this->SetStatus(bubble_sts_end);
    }else if(this->GetStatus()==bubble_sts_end){
        this->SetStatus(bubble_sts_def);
    }
}
