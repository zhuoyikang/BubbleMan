#include "BPlayerManager.hpp"


USING_NS_CC;


bool BPlayerManager::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    scheduleUpdate();
    return true;
}

void BPlayerManager::MakePlayer(int id, cocos2d::Point pos)
{
    auto player = new BPlayer(id);
    this->addChild(player);
    player->setPosition(pos);
    _playerList.pushBack(player);
}


void BPlayerManager::MakePlayer(cocos2d::Point pos)
{
    auto player = new BPlayer(newId());
    this->addChild(player);
    player->setPosition(pos);
    _playerList.pushBack(player);
}


void BPlayerManager::SetStatus(int id, int status)
{
    for(cocos2d::Vector<BPlayer*>::iterator iter=_playerList.begin();
        iter!=_playerList.end(); ) {
        if((*iter)->GetID()==id){
            (*iter)->SetStatus(status);
            return;
        }else{
            iter++;
        }
    }
}


// void BPlayerManager::SetDirection(int id, int direction)
// {
//     for(cocos2d::Vector<BPlayer*>::iterator iter=_playerList.begin();
//         iter!=_playerList.end(); ) {
//         if((*iter)->GetID()==id){
//             (*iter)->SetDirection(direction);
//             return;
//         }else{
//             iter++;
//         }
//     }
// }


void BPlayerManager::SetPosition(int id, cocos2d::Point pos)
{
    for(cocos2d::Vector<BPlayer*>::iterator iter=_playerList.begin();
        iter!=_playerList.end(); ) {
        if((*iter)->GetID()==id){
            (*iter)->setPosition(pos);
            return;
        }else{
            iter++;
        }
    }
}

BPlayer* BPlayerManager::FindPlayer(int id)
{
    BPlayer* p = NULL;
    for(cocos2d::Vector<BPlayer*>::iterator iter=_playerList.begin();
        iter!=_playerList.end(); ) {
        if((*iter)->GetID()==id){
            p = *iter;
            break;
        }else{
            iter++;
        }
    }
    return p;
}
