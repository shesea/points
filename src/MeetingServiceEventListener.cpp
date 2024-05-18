#include "MeetingServiceEventListener.h"
#include <rawdata/zoom_rawdata_api.h>
#include <iostream>

using namespace std;

MeetingServiceEventListener::MeetingServiceEventListener(void (*onMeetingStarts)(), void (*onMeetingEnds)(), void (*onInMeeting)())
{
	onMeetingEnds_ = onMeetingEnds;
	onMeetingStarts_ = onMeetingStarts;
	onInMeeting_ = onInMeeting;
}

void MeetingServiceEventListener::onMeetingStatusChanged(MeetingStatus status, int iResult)
{
	std::cout << "onMeetingStatusChanged: " << status << ", iResult: " << iResult << endl;
	switch (status)
	{
	case MEETING_FAIL_MEETING_NOT_EXIST:
		break;

	case MEETING_STATUS_INMEETING:
		if (onInMeeting_) onInMeeting_();

		break;
	}

	if (status == MEETING_STATUS_ENDED && onMeetingEnds_) onMeetingEnds_();
}

void MeetingServiceEventListener::onMeetingStatisticsWarningNotification(StatisticsWarningType type) {}

void MeetingServiceEventListener::onMeetingParameterNotification(const MeetingParameter* meeting_param)
{
	if (onMeetingStarts_) onMeetingStarts_();
}


void MeetingServiceEventListener::onSuspendParticipantsActivities()
{
}

void MeetingServiceEventListener::onAICompanionActiveChangeNotice(bool bActive)
{
}

