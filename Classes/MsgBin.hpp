#ifndef __msgbin__header__
#define __msgbin__header__

#include <string>
#include <vector>

namespace msgbin
{
    typedef unsigned char* bytes;
    typedef unsigned char byte_t;
    typedef unsigned short uint16;
    typedef short int16;
    typedef int int32;
    typedef unsigned int uint32;

    //测试函数
    void bz_test_u16();
    void bz_test_int32();
    void bz_test_string();
    void bz_test_2_int32();

    void show_byte(void *buff, size_t size);

    //读取一个无符号16位整数.
    int BzReaduint16(byte_t **pbyte, uint16 *u16);
    int BzWriteuint16(byte_t **pbyte, uint16 *u16);

    //读取一个有符号16位整数.
    int BzReadint16(byte_t **pbyte, int16 *u16);
    int BzWriteint16(byte_t **pbyte, int16 *i16);

    //读取一个有符号32位整数.
    int BzReadint32(byte_t **pbyte, int32 *i32);
    int BzWriteint32(byte_t **pbyte, int32 *i32);

    //读取一个无符号32位整数.
    int BzReaduint32(byte_t **pbyte, uint32 *i32);
    int BzWriteuint32(byte_t **pbyte, uint32 *i32);

    //字符串
    int BzReadstring(byte_t **pbyte, std::string *str);
    int BzWritestring(byte_t **pbyte, std::string *str);

}

#endif
