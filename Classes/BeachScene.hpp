/**
 * 游戏战斗场景
 */

#ifndef __T4__BeachScene__
#define __T4__BeachScene__

#include <stdio.h>
#include "Rocker.hpp"
#include "BPlayer.hpp"
#include "Bubble.hpp"
#include "BubbleManager.hpp"
#include "BPlayerManager.hpp"
#include "MsgGen.hpp"


class QueueMsg;

/**
 * 游戏战斗场景
 */
class BeachScene : public cocos2d::Layer
{
    typedef void (BeachScene::*BeachSceneFptr)(QueueMsg *m);

public:
    static cocos2d::Scene* createScene();
    virtual bool init();


    int rockerToStatus(int d);

    /**
     * 定时器刷新
     */
    void update(float dt) override;

    /**
     * 玩家触屏
     */
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *);

    /**
     * 用来计算Player移动坐标.
     */
    Point playerNextPosition(Point old, int direct);

    /**
     * 检查位置实时被阻塞.
     */
    bool doesPositionBlock(Point old,int direct);

    /**
     * 根据位置计算tile坐标.
     */
    Point tileCoordForPosition(cocos2d::Point position);
    Point positionForTileCoord(cocos2d::Point position);

    /**
     * 设置某个瓦片被泡泡炸毁
     */
    void tileExpolsed(cocos2d::Point tileCoord);


    CREATE_FUNC(BeachScene);

public:
    /**
     * 服务器消息处理函数
     */
    void RoomCloseNtf(QueueMsg *msg);

    /**
     * 设置玩家的位置.
     */
    void RoomUserChgNtf(QueueMsg *msg);

    /**
     * 设置泡泡
     */
    void RoomSetBubbleNtf(QueueMsg *msg);

    /**
     * 爆炸
     */
    void BubbleBombNtf(QueueMsg *msg);

private:
    void loopMsg();

    /**
     * 设置视点
     */
    void setViewPointCenter(cocos2d::Point position);


    /**
     * 设置主角的位置
     */
    void updateMainPlayerStatus(int d);
    void setMainPlayerPosition(cocos2d::Point position);
    void setMainPlayerStatus(int d);

    /*
      向服务器发一个泡泡
     */
    void setBubble(msgbin::Bubble b);

    /**
     * 同步主角信息到服务器.
     */
    void syncMainPlayerInfo(cocos2d::Point pos, int d);


    /**
     * 初始化所有的玩家.
     */
    void initAllPlayer();
    void updateAllPlayer();

private:

    /**
     * 泡泡管理
     */
    BubbleManager *_bubbleManager;
    BPlayerManager * _playerManager;

    /**
     * 各种场景元素
     */
    cocos2d::TMXTiledMap *_tileMap;
    cocos2d::Sprite * _player;
    cocos2d::TMXLayer *_background;
    cocos2d::TMXLayer *_meta;
    cocos2d::TMXLayer *_foreground;

    /**
     * 摇杆
     */
    BRocker *_rocker;

    /**
     * 玩家
     */
    //BPlayer *_playerSprite;


    BeachSceneFptr HandlerMap[100];
};

#endif
