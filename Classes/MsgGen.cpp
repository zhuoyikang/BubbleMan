#include "MsgGen.hpp"

namespace msgbin
{

int BzReadClientInfo(byte_t **pbyte, ClientInfo *ret)
{
	BzReadstring(pbyte, &ret->udid);
	return 0;
}
int BzWriteClientInfo(byte_t **pbyte, ClientInfo *ret)
{
	BzWritestring(pbyte, &ret->udid);
	return 0;
}
int BzReadUserLoginReq(byte_t **pbyte, UserLoginReq *ret)
{
	BzReadstring(pbyte, &ret->udid);
	BzReadClientInfo(pbyte, &ret->client);
	return 0;
}
int BzWriteUserLoginReq(byte_t **pbyte, UserLoginReq *ret)
{
	BzWritestring(pbyte, &ret->udid);
	BzWriteClientInfo(pbyte, &ret->client);
	return 0;
}
int BzReadUserLoginAck(byte_t **pbyte, UserLoginAck *ret)
{
	BzReadstring(pbyte, &ret->udid);
	BzReadstring(pbyte, &ret->name);
	BzReadint32(pbyte, &ret->level);
	BzReadClientInfo(pbyte, &ret->client);
	return 0;
}
int BzWriteUserLoginAck(byte_t **pbyte, UserLoginAck *ret)
{
	BzWritestring(pbyte, &ret->udid);
	BzWritestring(pbyte, &ret->name);
	BzWriteint32(pbyte, &ret->level);
	BzWriteClientInfo(pbyte, &ret->client);
	return 0;
}
}

