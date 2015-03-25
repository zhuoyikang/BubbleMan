/**
 * 玩家管理，多人玩家.
 */


#ifndef __T4__BPlayerManager__
#define __T4__BPlayerManager__

#include "BidGen.hpp"
#include "cocos2d.h"
#include "BPlayer.hpp"

class BPlayerManager : public cocos2d::Layer, private BIdGen
{
public:
    virtual bool init();

    void MakePlayer(int id, cocos2d::Point pos);
    void MakePlayer(cocos2d::Point pos);

    /**
     * 设置某一个玩家的状态.
     */
    void SetStatus(int id, int direction);

    CREATE_FUNC(BPlayerManager);
private:
    /**
     * 所有的玩家列表
     */
    cocos2d::Vector<BPlayer*> _playerList;
};



#endif
