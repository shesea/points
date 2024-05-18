#include <iostream>
#include "NetworkConnectionHandler.h"

using namespace std;

NetworkConnectionHandler::NetworkConnectionHandler(void (*postToDo)())
{
	postToDo_ = postToDo;
}

void NetworkConnectionHandler::onProxyDetectComplete()
{
	if(postToDo_) postToDo_();
}

void NetworkConnectionHandler::onProxySettingNotification(IProxySettingHandler* handler)
{
}

void NetworkConnectionHandler::onSSLCertVerifyNotification(ISSLCertVerificationHandler* handler)
{
}