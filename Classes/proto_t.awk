#! /usr/local/bin/gawk -f
# 类型别名.

@include "proto_r.awk"

BEGIN {
    PayLoadCount = 1
    LoadType()
    LoadPayLoad()
}


END {
    OutPutPayLoadHeader();
    OutPutPayLoadBody();
}


# 输出所有的PayLoadStruct.
function OutPutAllPayLoadStruct() {
    for(PayLoadi =1; PayLoadi< PayLoadCount; PayLoadi ++) {
        name = PayLoadNames[PayLoadi]
        OutPutPayLoadStruct(name)
    }
}

function OutPutErrCheck(prefix) {
    # print prefix, "\tif err != nil {"  > "demo.go"
    # print prefix, "\t\treturn"  > "demo.go"
    # print prefix, "\t}"  > "demo.go"
}

# 输出所有的序列化函数.
function OutPutAllSerialize() {
    for (i = 1; i< PayLoadCount; i++) {
        Name = PayLoadNames[i]
        OutPutSerializeRead(Name)
        OutPutSerializeWrite(Name)
    }
}


# 输出所有的序列化函数.
function OutPutDeclareSerialize() {
    for (i = 1; i< PayLoadCount; i++) {
        Name = PayLoadNames[i]
        printf("\tint BzRead%s(byte_t **pbyte, %s *ret);\n",
               Name,Name) > "MsgGen.hpp"
        printf("\tint BzWrite%s(byte_t **pbyte, %s *ret);\n\n",
               Name,Name) > "MsgGen.hpp"
    }
}



# 实现类型函数
function LoadType()
{
    TypeMap["byte"] = "byte"
    TypeMap["uint32"] = "uint32"
    TypeMap["int32"] = "int32"

    TypeMap["uint16"] = "uint16"
    TypeMap["int16"] = "int16"

    TypeMap["string"] = "string"
    TypeMap["int"] = "int32"
    TypeMap["integer"] = "int32"

    TypeMap["std::string"] = "string"
    TypeMap["uint32_t"] = "uint32"
    TypeMap["int32_t"] = "int32"
    TypeMap["ushort_t"] = "uint16"
    TypeMap["short_t"] = "int16"
}

# 实现输出函数
# function OutPutPayLoad() {
#     for (i = 1; i< PayLoadCount; i++) {
#         name = PayLoadNames[i]
#         print i,name
#     }
# }


# 输出c++的struct结构.
function OutPutPayLoadStruct(Name) {
    count=PayLoadList[Name,"count"]
    printf("\tstruct %s\n\t{\n", Name) > "MsgGen.hpp"
    for(StructI=1; StructI<= count; StructI++) {
        name = PayLoadList[Name,StructI,"name"]
        type = PayLoadList[Name,StructI,"type"]
        addtion = PayLoadList[Name,StructI,"addtion"]
        if(type == "array") {
            if(TypeMap[addtion]=="") {
                addtion=addtion
            }
            printf("\t\tvector<%s> %s;\n", addtion, name) > "MsgGen.hpp"
        } else {
            if(TypeMap[type]=="") {
                type=type
            }else{
                type=TypeMap[type]
            }
            printf("\t\t%s %s;\n", type, name) > "MsgGen.hpp"
        }
    }
    print "\t};\n" > "MsgGen.hpp"
}


function OutPutPayLoadHeader() {
    print "/*this file is generated by awk*/\n" > "MsgGen.hpp"
    print "#ifndef __msgbin__gen__" > "MsgGen.hpp"
    print "#define __msgbin__gen__\n" > "MsgGen.hpp"

    print "#include <string>" > "MsgGen.hpp"
    print "#include <vector>"> "MsgGen.hpp"
    print "#include \"MsgBin.hpp\"\n" > "MsgGen.hpp"


    print "namespace msgbin" > "MsgGen.hpp"
    print "{\n" > "MsgGen.hpp"

    OutPutAllPayLoadStruct()
    OutPutDeclareSerialize()

    print "}\n" > "MsgGen.hpp"

    print "#endif" > "MsgGen.hpp"
}


function OutPutPayLoadBody() {
    print "#include \"MsgGen.hpp\"\n"  > "MsgGen.cpp"
    print "namespace msgbin\n{\n"  > "MsgGen.cpp"
    OutPutAllSerialize()
    print "}\n"  > "MsgGen.cpp"
}

# 输出序列化:read
function OutPutSerializeRead(Name) {
    count=PayLoadList[Name,"count"]
    printf("int BzRead%s(byte_t **pbyte, %s *ret)\n{\n",
           Name,Name) > "MsgGen.cpp"
    for(Filedi = 1 ;Filedi<= count; Filedi++) {
        type = PayLoadList[Name,Filedi,"type"]
        addtion = PayLoadList[Name,Filedi,"addtion"]
        name = PayLoadList[Name,Filedi,"name"]

        if (type == "array") {

            if(TypeMap[addtion]=="") {
                addtion1="*"addtion
            }else{
                addtion1=addtion
            }
            printf("\tuint16 %s_size;\n", name) > "MsgGen.cpp"
            printf("\tBzReaduint16(pbyte,&%s_size);\n", name) > "MsgGen.cpp"
            printf("\tfor(uint16 i=0; i<%s_size; i++ ) {\n", name)  > "MsgGen.cpp"
            printf("\t\t%s val;\n",addtion) > "MsgGen.cpp"
            printf("\t\tBzRead%s(pbyte, &val);\n",addtion,name) > "MsgGen.cpp"
            OutPutErrCheck("\t")
            printf("\t\tret->%s.push_back(val);\n",name) > "MsgGen.cpp"
            printf("\t}\n") > "MsgGen.cpp"

        } else {
            printf("\tBzRead%s(pbyte, &ret->%s);\n",type, name) > "MsgGen.cpp"
        }
        OutPutErrCheck()
    }
    print "\treturn 0;" > "MsgGen.cpp"
    printf("}\n") > "MsgGen.cpp"
}

function OutPutSerializeWrite(Name) {
    count=PayLoadList[Name,"count"]
    printf("int BzWrite%s(byte_t **pbyte, %s *ret)\n{\n",
           Name,Name) > "MsgGen.cpp"
    for(Filedi = 1 ;Filedi<= count; Filedi++) {
        type = PayLoadList[Name,Filedi,"type"]
        addtion = PayLoadList[Name,Filedi,"addtion"]
        name = PayLoadList[Name,Filedi,"name"]

        if (type == "array") {

            if(TypeMap[addtion]=="") {
                addtion1="*"addtion
            }else{
                addtion1=addtion
            }
            printf("\tuint16 %s_size=ret->%s.size();\n", name, name) > "MsgGen.cpp"
            printf("\tBzWriteuint16(pbyte,&%s_size);\n", name) > "MsgGen.cpp"
            printf("\tfor(uint16 i=0; i<%s_size; i++ ) {\n", name)  > "MsgGen.cpp"
            printf("\t\t%s val = ret->%s.at(i);\n",addtion, name) > "MsgGen.cpp"
            printf("\t\tBzWrite%s(pbyte, &val);\n",addtion,name) > "MsgGen.cpp"
            OutPutErrCheck("\t")
            printf("\t}\n") > "MsgGen.cpp"

        } else {
            printf("\tBzWrite%s(pbyte, &ret->%s);\n",type, name) > "MsgGen.cpp"
        }
        OutPutErrCheck()
    }
    print "\treturn 0;" > "MsgGen.cpp"
    printf("}\n") > "MsgGen.cpp"
}
