#pragma once
#include <windows.h>
#include "zoom_sdk.h"
#include "meeting_service_components\meeting_recording_interface.h"

using namespace ZOOMSDK;

class MeetingRecordingCtrlEventListener :public IMeetingRecordingCtrlEvent
{

	void (*onIsGivenRecordingPermission_)();
public:
	MeetingRecordingCtrlEventListener(void (*onIsGivenRecordingPermission)());

	virtual void onRecordingStatus(RecordingStatus status);

	virtual void onCloudRecordingStatus(RecordingStatus status);

	virtual void onRecordPrivilegeChanged(bool bCanRec);

	virtual void onLocalRecordingPrivilegeRequestStatus(RequestLocalRecordingStatus status);

	virtual void onRequestCloudRecordingResponse(RequestStartCloudRecordingStatus status);

	virtual void onLocalRecordingPrivilegeRequested(IRequestLocalRecordingPrivilegeHandler* handler);

	virtual void onStartCloudRecordingRequested(IRequestStartCloudRecordingHandler* handler);

	virtual void onRecording2MP4Done(bool bsuccess, int iResult, const zchar_t* szPath);

	virtual void onRecording2MP4Processing(int iPercentage);

	virtual void onCustomizedLocalRecordingSourceNotification(ICustomizedLocalRecordingLayoutHelper* layout_helper);

	virtual void onCloudRecordingStorageFull(time_t gracePeriodDate);

};

