/**
 * 玩家
 */

#ifndef __T4__BIdGen__
#define __T4__BIdGen__


class BIdGen
{
protected:
    BIdGen(){}
    /**
     * @return 生成一个新的泡泡ID.
     */
    int newId(){return ++_id;}

    /**
     * 玩家的唯一id，由服务器生成，这里做一个方便客户端调试.
     */
    int _id;
};


class BIdentity
{
public:
    virtual int GetID() = 0;
protected:
    int _id;
};


#endif
