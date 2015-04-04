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
int BzReadBVector2(byte_t **pbyte, BVector2 *ret)
{
	BzReadint32(pbyte, &ret->x);
	BzReadint32(pbyte, &ret->y);
	return 0;
}
int BzWriteBVector2(byte_t **pbyte, BVector2 *ret)
{
	BzWriteint32(pbyte, &ret->x);
	BzWriteint32(pbyte, &ret->y);
	return 0;
}
int BzReadRoomUser(byte_t **pbyte, RoomUser *ret)
{
	BzReadBVector2(pbyte, &ret->pos);
	BzReadint32(pbyte, &ret->direction);
	BzReadint32(pbyte, &ret->status);
	return 0;
}
int BzWriteRoomUser(byte_t **pbyte, RoomUser *ret)
{
	BzWriteBVector2(pbyte, &ret->pos);
	BzWriteint32(pbyte, &ret->direction);
	BzWriteint32(pbyte, &ret->status);
	return 0;
}
int BzReadRoomReadyNtf(byte_t **pbyte, RoomReadyNtf *ret)
{
	BzReadint32(pbyte, &ret->roomId);
	uint16 uPosAll_size;
	BzReaduint16(pbyte,&uPosAll_size);
	for(uint16 i=0; i<uPosAll_size; i++ ) {
		RoomUser val;
		BzReadRoomUser(pbyte, &val);
		ret->uPosAll.push_back(val);
	}
	BzReadint32(pbyte, &ret->uIdx);
	return 0;
}
int BzWriteRoomReadyNtf(byte_t **pbyte, RoomReadyNtf *ret)
{
	BzWriteint32(pbyte, &ret->roomId);
	uint16 uPosAll_size=ret->uPosAll.size();
	BzWriteuint16(pbyte,&uPosAll_size);
	for(uint16 i=0; i<uPosAll_size; i++ ) {
		RoomUser val = ret->uPosAll.at(i);
		BzWriteRoomUser(pbyte, &val);
	}
	BzWriteint32(pbyte, &ret->uIdx);
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
int BzReadRoomUserChg(byte_t **pbyte, RoomUserChg *ret)
{
	BzReadint32(pbyte, &ret->uIdx);
	BzReadRoomUser(pbyte, &ret->user);
	return 0;
}
int BzWriteRoomUserChg(byte_t **pbyte, RoomUserChg *ret)
{
	BzWriteint32(pbyte, &ret->uIdx);
	BzWriteRoomUser(pbyte, &ret->user);
	return 0;
}
int BzReadBubble(byte_t **pbyte, Bubble *ret)
{
	BzReadint32(pbyte, &ret->id);
	BzReadBVector2(pbyte, &ret->pos);
	BzReadint32(pbyte, &ret->power);
	BzReadint32(pbyte, &ret->keeptime);
	return 0;
}
int BzWriteBubble(byte_t **pbyte, Bubble *ret)
{
	BzWriteint32(pbyte, &ret->id);
	BzWriteBVector2(pbyte, &ret->pos);
	BzWriteint32(pbyte, &ret->power);
	BzWriteint32(pbyte, &ret->keeptime);
	return 0;
}
int BzReadSetBubble(byte_t **pbyte, SetBubble *ret)
{
	BzReadBubble(pbyte, &ret->b);
	BzReadint32(pbyte, &ret->uIdx);
	return 0;
}
int BzWriteSetBubble(byte_t **pbyte, SetBubble *ret)
{
	BzWriteBubble(pbyte, &ret->b);
	BzWriteint32(pbyte, &ret->uIdx);
	return 0;
}
int BzReadBubbleBomb(byte_t **pbyte, BubbleBomb *ret)
{
	BzReadint32(pbyte, &ret->id);
	uint16 destroyTiles_size;
	BzReaduint16(pbyte,&destroyTiles_size);
	for(uint16 i=0; i<destroyTiles_size; i++ ) {
		BVector2 val;
		BzReadBVector2(pbyte, &val);
		ret->destroyTiles.push_back(val);
	}
	uint16 destroyUsers_size;
	BzReaduint16(pbyte,&destroyUsers_size);
	for(uint16 i=0; i<destroyUsers_size; i++ ) {
		int32 val;
		BzReadint32(pbyte, &val);
		ret->destroyUsers.push_back(val);
	}
	return 0;
}
int BzWriteBubbleBomb(byte_t **pbyte, BubbleBomb *ret)
{
	BzWriteint32(pbyte, &ret->id);
	uint16 destroyTiles_size=ret->destroyTiles.size();
	BzWriteuint16(pbyte,&destroyTiles_size);
	for(uint16 i=0; i<destroyTiles_size; i++ ) {
		BVector2 val = ret->destroyTiles.at(i);
		BzWriteBVector2(pbyte, &val);
	}
	uint16 destroyUsers_size=ret->destroyUsers.size();
	BzWriteuint16(pbyte,&destroyUsers_size);
	for(uint16 i=0; i<destroyUsers_size; i++ ) {
		int32 val = ret->destroyUsers.at(i);
		BzWriteint32(pbyte, &val);
	}
	return 0;
}
}

