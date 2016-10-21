﻿#pragma once

#include <functional>

struct ISocketStream 
{
	virtual void Send(const void* pvData, size_t uDataLen) = 0;
	virtual void SetDataCallback(const std::function<void(BYTE*, size_t)>& callback) = 0;
	virtual void SetErrorCallback(const std::function<void()>& callback) = 0;
	virtual const char* GetRemoteAddress() = 0;
	virtual const char* GetError(int* pnError = nullptr) = 0;
	virtual void Close() = 0;
};

struct ISocketListener
{
	virtual void SetStreamCallback(const std::function<void(ISocketStream* pSocket)>& callback) = 0;
	virtual void SetStreamBufferSize(size_t uRecvBufferSize, size_t uSendBufferSize) = 0;
	virtual void Close() = 0;
};

// 每个异步connect返回一个连接器,在回调后(on_connect或on_error),用户应该close.
// 在等待connect完成期间,可以主动close,从而取消connect.
struct connector_t
{
	virtual void on_connect(const std::function<void(ISocketStream*)>& cb) = 0;
	virtual void on_error(const std::function<void(const char*)>& cb) = 0;
	virtual void close() = 0;
};

using connecting_callback_t = std::function<void(ISocketStream* pStream, const char* pszErr)>;

struct ISocketManager
{
	virtual ISocketListener* Listen(const char szIP[], int nPort) = 0;

	// node: 可以为域名或者ip
	// service: 可以为数字端口如"80"或者为服务名如"http", "ftp"
	// timeout: 单位ms,-1表示永远
	virtual connector_t* connect(const char node[], const char service[], int timeout = -1) = 0;

	// nTimeout: 单位ms,-1表示无限
	// uSendBufferSize,指网络库内部使用的发送缓冲区大小
	// uRecvBufferSize,指网络库内部使用的缓冲区大小,至少要能容纳一个最大网络包
	// 无论成败,(在稍后的Wait中)回调一定会发生
	virtual void ConnectAsync(const char szIP[], int nPort, const connecting_callback_t& callback, int nTimeout = 2000, size_t uRecvBufferSize = 4096, size_t uSendBufferSize = 4096) = 0;

	// nTimeout: 单位ms,-1表示无限
	virtual void Wait(int nTimeout = 50) = 0;

	virtual void Release() = 0;
};

ISocketManager* create_socket_mgr(int max_connection);