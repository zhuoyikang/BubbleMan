#include "MsgBin.hpp"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// #define string std::string
// #define vector std::vector


namespace msgbin
{

    void show_byte(void *buff, size_t size)
    {
        char *buffer=(char *)buff;
        printf("show: ");
        for (size_t i =0 ; i< size ;i++) {
            printf("%x ", (byte_t)buffer[i]);
        }
        printf("\n");
    }

    //读取一个无符号16位整数.
    int BzReaduint16(byte_t **pbyte, uint16 *u16) {
        byte_t *byte = *pbyte;
        *u16 = (uint16)(byte[0] << 8 | byte[1]);

        (*pbyte) += 2;
        return 0;
    }


    int BzWriteuint16(byte_t **pbyte, uint16 *u16) {
        byte_t *byte = *pbyte;
        byte[0] = byte_t(*u16 >> 8);
        byte[1] = byte_t(*u16);

        (*pbyte) += 2;
        return 0;
    }

    //读取一个有符号16位整数.
    int BzReadint16(byte_t **pbyte, int16 *u16) {
        byte_t *byte = *pbyte;
        *u16 = (int16)(byte[0] << 8 | byte[1]);

        (*pbyte) += 2;
        return 0;
    }


    int BzWriteint16(byte_t **pbyte, int16 *i16) {
        byte_t *byte = *pbyte;
        byte[0] = byte_t(*i16 >> 8);
        byte[1] = byte_t(*i16);

        (*pbyte) += 2;
        return 0;
    }

    //读取一个有符号32位整数.
    int BzReadint32(byte_t **pbyte, int32 *i32) {
        byte_t *byte = *pbyte;
        *i32 = (int32)(byte[0] << 24 | byte[1] << 16 | byte[2] << 8 | byte[3]);

        (*pbyte) += 4;
        return 0;
    }


    int BzWriteint32(byte_t **pbyte, int32 *i32) {
        byte_t *byte = *pbyte;
        byte[0] = byte_t(*i32 >> 24);
        byte[1] = byte_t(*i32 >> 16);
        byte[2] = byte_t(*i32 >> 8);
        byte[3] = byte_t(*i32);

        (*pbyte) += 4;
        return 0;
    }

    //读取一个无符号32位整数.
    int BzReaduint32(byte_t **pbyte, uint32 *i32) {
        byte_t *byte = *pbyte;
        *i32 = (uint32)(byte[0] << 24 | byte[1] << 16 | byte[2] << 8 | byte[3]);

        (*pbyte) += 4;
        return 0;
    }

    int BzWriteuint32(byte_t **pbyte, uint32 *i32) {
        byte_t *byte = *pbyte;
        byte[0] = byte_t(*i32 >> 24);
        byte[1] = byte_t(*i32 >> 16);
        byte[2] = byte_t(*i32 >> 8);
        byte[3] = byte_t(*i32);

        (*pbyte) += 4;
        return 0;
    }

    //读取字符串 #
    int BzReadstring(byte_t **pbyte, std::string *str) {
        uint16 size;
        BzReaduint16(pbyte, &size);
        char *bstr = (char *)malloc(size+1);
        bzero(bstr,size+1);
        memcpy(bstr,*pbyte, size);
        *str = bstr;
        free(bstr);
        (*pbyte) += size;
        return 0;
    }

    int BzWritestring(byte_t **pbyte, std::string *str)
    {
        uint16 size = str->length();
        BzWriteuint16(pbyte, &size);
        memcpy(*pbyte,str->c_str(), size);
        *pbyte+=size;
        return 0;
    }

    void bz_test_u16()
    {
        byte_t *pbuff = (byte_t *)malloc(2);
        uint16 u16 = 14;
        uint16 u16_2;

        byte_t *backp = pbuff;

        BzWriteuint16(&pbuff, &u16);
        //show_byte(backp, 2);
        BzReaduint16(&backp, &u16_2);

        assert(u16_2==u16);
    }

    void bz_test_string()
    {
        byte_t *pbuff = (byte_t *)malloc(100);
        byte_t *backp = pbuff;
        std::string in("this is a test");
        std::string out;
        BzWritestring(&pbuff, &in);
        BzReadstring(&backp,&out);
        assert(in == out);
    }

    void bz_test_int32()
    {
        byte_t *pbuff = (byte_t *)malloc(4);
        int32 i32 = 14323;
        int32 i32_2;

        byte_t *backp = pbuff;

        BzWriteint32(&pbuff, &i32);
        //show_byte(backp, 4);
        BzReadint32(&backp, &i32_2);

        assert(i32_2==i32);

    }

    void bz_test_2_int32()
    {
        byte_t *pbuff = (byte_t *)malloc(8);
    //    byte_t *backp = pbuff;
        int32 i32_1_1,i32_1_2;
      //  int32 i32_2_1,i32_2_2;

        i32_1_1 = 1;
        i32_1_2 = 2;
        BzWriteint32(&pbuff, &i32_1_1);
        BzWriteint32(&pbuff, &i32_1_2);
        //show_byte(backp, 8);

    }

}
