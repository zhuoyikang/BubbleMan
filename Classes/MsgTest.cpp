#include "MsgBin.hpp"
#include "MsgGen.hpp"
#include <stdlib.h>
#include <stdio.h>


int main(int, char *[])
{
    msgbin::bz_test_u16();
    msgbin::bz_test_string();
    msgbin::bz_test_int32();
    msgbin::bz_test_2_int32();
}
