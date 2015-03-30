#include "MsgGen.hpp"

namespace msgbin
{

int BzReadUserLoginReq(byte_t **pbyte, UserLoginReq *ret)
{
	BzReadstring(pbyte, &ret->udid);
	return 0;
}
int BzWriteUserLoginReq(byte_t **pbyte, UserLoginReq *ret)
{
	BzWritestring(pbyte, &ret->udid);
	return 0;
}
int BzReadUserLoginAck(byte_t **pbyte, UserLoginAck *ret)
{
	BzReadstring(pbyte, &ret->udid);
	BzReadstring(pbyte, &ret->name);
	BzReadint32(pbyte, &ret->level);
	return 0;
}
int BzWriteUserLoginAck(byte_t **pbyte, UserLoginAck *ret)
{
	BzWritestring(pbyte, &ret->udid);
	BzWritestring(pbyte, &ret->name);
	BzWriteint32(pbyte, &ret->level);
	return 0;
}
int BzReadUserJoinReq(byte_t **pbyte, UserJoinReq *ret)
{
	BzReadstring(pbyte, &ret->udid);
	return 0;
}
int BzWriteUserJoinReq(byte_t **pbyte, UserJoinReq *ret)
{
	BzWritestring(pbyte, &ret->udid);
	return 0;
}
int BzReadUserJoinAck(byte_t **pbyte, UserJoinAck *ret)
{
	BzReadstring(pbyte, &ret->udid);
	BzReadstring(pbyte, &ret->name);
	BzReadint32(pbyte, &ret->level);
	return 0;
}
int BzWriteUserJoinAck(byte_t **pbyte, UserJoinAck *ret)
{
	BzWritestring(pbyte, &ret->udid);
	BzWritestring(pbyte, &ret->name);
	BzWriteint32(pbyte, &ret->level);
	return 0;
}
int BzReadRoomReadyNtf(byte_t **pbyte, RoomReadyNtf *ret)
{
	BzReadint32(pbyte, &ret->t);
	return 0;
}
int BzWriteRoomReadyNtf(byte_t **pbyte, RoomReadyNtf *ret)
{
	BzWriteint32(pbyte, &ret->t);
	return 0;
}
int BzReadRoomCloseNtf(byte_t **pbyte, RoomCloseNtf *ret)
{
	BzReadint32(pbyte, &ret->t);
	return 0;
}
int BzWriteRoomCloseNtf(byte_t **pbyte, RoomCloseNtf *ret)
{
	BzWriteint32(pbyte, &ret->t);
	return 0;
}
}

