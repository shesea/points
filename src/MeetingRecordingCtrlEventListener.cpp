#include "MeetingRecordingCtrlEventListener.h"

using namespace std;


MeetingRecordingCtrlEventListener::MeetingRecordingCtrlEventListener(void(*onIsGivenRecordingPermission)())
{
	onIsGivenRecordingPermission_ = onIsGivenRecordingPermission;
}

void MeetingRecordingCtrlEventListener::onRecordingStatus(RecordingStatus status) {}

void MeetingRecordingCtrlEventListener::onCloudRecordingStatus(RecordingStatus status) {}

void MeetingRecordingCtrlEventListener::onRecordPrivilegeChanged(bool bCanRec) {

	if (bCanRec) {
		if (onIsGivenRecordingPermission_)onIsGivenRecordingPermission_();
	}
}

void MeetingRecordingCtrlEventListener::onLocalRecordingPrivilegeRequestStatus(RequestLocalRecordingStatus status) {}

void MeetingRecordingCtrlEventListener::onRequestCloudRecordingResponse(RequestStartCloudRecordingStatus status) {}

void MeetingRecordingCtrlEventListener::onLocalRecordingPrivilegeRequested(IRequestLocalRecordingPrivilegeHandler* handler) {}

void MeetingRecordingCtrlEventListener::onStartCloudRecordingRequested(IRequestStartCloudRecordingHandler* handler) {}

void MeetingRecordingCtrlEventListener::onRecording2MP4Done(bool bsuccess, int iResult, const zchar_t* szPath) {}

void MeetingRecordingCtrlEventListener::onRecording2MP4Processing(int iPercentage) {}

void MeetingRecordingCtrlEventListener::onCustomizedLocalRecordingSourceNotification(ICustomizedLocalRecordingLayoutHelper* layout_helper) {}

void MeetingRecordingCtrlEventListener::onCloudRecordingStorageFull(time_t gracePeriodDate) {}

