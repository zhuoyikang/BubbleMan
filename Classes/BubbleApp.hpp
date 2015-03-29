//
//  BubbleApp.h
//  T4
//
//  Created by zhuoyikang on 15-3-29.
//
//

#ifndef __T4__BubbleApp__
#define __T4__BubbleApp__

#include <stdio.h>
#include "SockNet.hpp"
#include "MsgGen.hpp"
#include "MsgDispatch.hpp"

class BubbleApp
{
public:
    BubbleApp();
    void Run();

    SockNet sockNet;
};


#endif /* defined(__T4__BubbleApp__) */
