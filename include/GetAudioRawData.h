#pragma once
#include <string>

class GetAudioRawData {
public:
	static int LeaveMeeting();
	static int startInMeetingProcess(std::string jwt, std::string meetingNumber, std::string pwd);
};