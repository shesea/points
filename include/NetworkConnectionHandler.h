#pragma once
#include <windows.h>
#include <zoom_sdk.h>
#include <network_connection_handler_interface.h>

using namespace ZOOMSDK;

class NetworkConnectionHandler :
    public INetworkConnectionHandler
{
	void (*postToDo_)();
public:
	NetworkConnectionHandler(void (*postToDo)());

	virtual void onProxyDetectComplete();

	virtual void onProxySettingNotification(IProxySettingHandler* handler);

	virtual void onSSLCertVerifyNotification(ISSLCertVerificationHandler* handler);
};

