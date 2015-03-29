//
//  MsgDispatch.h
//  TileTest
//
//  Created by zhuoyikang on 15-2-16.
//
//

#ifndef __TileTest__MsgDispatch__
#define __TileTest__MsgDispatch__

#include <stdio.h>
#include "const.hpp"
#include "MsgBin.hpp"
#include "SockNet.hpp"


class SockBubbleDispatch: public SockDispatch
{
public:
    void Dispatch(unsigned char *buff) ;
    void Close();
};

#endif /* defined(__TileTest__MsgDispatch__) */
