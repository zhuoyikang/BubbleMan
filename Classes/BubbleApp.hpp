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

class BubbleApp:public SockApp
{
public:
    BubbleApp();
    void Dispatch(unsigned char *buff);
    void Run();
};

#endif /* defined(__T4__BubbleApp__) */
