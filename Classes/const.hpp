#ifndef __T4__Const__
#define __T4__Const__


namespace cocostudio {
    namespace timeline {
        class ActionTimeline;
    }
}


#ifdef COCOS
#include "cocos2d.h"
#define LOG(fmt,args...)  log(fmt,##args)
USING_NS_CC;
#else
#include <stdio.h>
#define LOG(fmt,args...)  do{ printf(fmt "\n",##args);fflush(stdout);} while(0)
#endif


#define IP  "192.168.43.141"
#define PORT 3004


#include "MsgBin.hpp"
#include "MsgGen.hpp"
#include "BubbleApp.hpp"

// Sorrry，全局变量
extern msgbin::RoomReadyNtf gRoomReadNtf;
extern BubbleApp gBubbleApp;


#endif
