#pragma once
#include <windows.h>
#include "zoom_sdk.h"
#include <meeting_service_interface.h>

using namespace ZOOMSDK;

class MeetingServiceEventListener :
    public IMeetingServiceEvent
{
	void (*onMeetingEnds_)();
	void (*onMeetingStarts_)();
	void (*onInMeeting_)();
public:
	MeetingServiceEventListener(void (*onMeetingStarts_)(), void (*onMeetingEnds_)(), void (*onInMeeting_)());

	virtual void onMeetingStatusChanged(MeetingStatus status, int iResult = 0);

	virtual void onMeetingStatisticsWarningNotification(StatisticsWarningType type);

	virtual void onMeetingParameterNotification(const MeetingParameter* meeting_param);

	virtual void onSuspendParticipantsActivities();

	virtual void onAICompanionActiveChangeNotice(bool bActive);
};

