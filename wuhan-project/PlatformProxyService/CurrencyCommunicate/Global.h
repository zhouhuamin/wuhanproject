#ifndef SP_GLOBAL_H_H
#define SP_GLOBAL_H_H
#include "../PlatformProxyService/public/BasicConfigInfo.h"

#include <WinSock2.h>
#pragma comment (lib,"ws2_32.lib")

#define DATA_BUFSIZE 2048
enum IOType 
{
	IOInitialize,
	IORead,
	IOWrite,
	IOIdle,
};
typedef struct
{
	OVERLAPPED Overlapped;
	IOType		m_ioType;
	WSABUF DataBuf;
	
    CHAR szClientIp[64];
	int  nClientPort;
	BOOL bIsStart;
	
	SOCKET Socket;
	CHAR szBuffer[DATA_BUFSIZE];
	
	LPVOID   pVoid;
	
} PER_IO_OPERATION_DATA, * LPPER_IO_OPERATION_DATA;

#endif

