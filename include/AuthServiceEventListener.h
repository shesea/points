#pragma once
#include <windows.h>
#include <auth_service_interface.h>

using namespace ZOOMSDK;
class AuthServiceEventListener :
    public IAuthServiceEvent
{
	void (*postToDo_)();
public:
	AuthServiceEventListener(void (*postToDo)());

	virtual void onAuthenticationReturn(AuthResult ret);

	virtual void onLoginReturnWithReason(LOGINSTATUS ret, IAccountInfo* pAccountInfo, LoginFailReason reason);

	virtual void onLogout();

	virtual void onZoomIdentityExpired();

	virtual void onZoomAuthIdentityExpired();

	virtual void onNotificationServiceStatus(SDKNotificationServiceStatus status);

	virtual void onNotificationServiceStatus(SDKNotificationServiceStatus status, SDKNotificationServiceError error);

};

