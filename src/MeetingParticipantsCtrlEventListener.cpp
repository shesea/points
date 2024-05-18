#include "MeetingParticipantsCtrlEventListener.h"
#include <iostream>

using namespace std;

MeetingParticipantsCtrlEventListener::MeetingParticipantsCtrlEventListener(void (*onIsHost)(), void (*onIsCoHost)(), void (*onIsGivenRecordingPermission)())
{
	onIsHost_ = onIsHost;
	onIsCoHost_ = onIsCoHost;
	onIsGivenRecordingPermission_ = onIsGivenRecordingPermission;
}

void MeetingParticipantsCtrlEventListener::onUserJoin(IList<unsigned int >* lstUserID, const wchar_t* strUserList) {}

void MeetingParticipantsCtrlEventListener::onUserLeft(IList<unsigned int >* lstUserID, const wchar_t* strUserList) {}

void MeetingParticipantsCtrlEventListener::onHostChangeNotification(unsigned int userId) {
	if (onIsHost_)onIsHost_();
}

void MeetingParticipantsCtrlEventListener::onLowOrRaiseHandStatusChanged(bool bLow, unsigned int userid) {}

void MeetingParticipantsCtrlEventListener::onUserNamesChanged(IList<unsigned int>* lstUserID) {}

void MeetingParticipantsCtrlEventListener::onCoHostChangeNotification(unsigned int userId, bool isCoHost) {
	if (onIsCoHost_)onIsCoHost_();
}

void MeetingParticipantsCtrlEventListener::onInvalidReclaimHostkey() {}

void MeetingParticipantsCtrlEventListener::onAllHandsLowered() {}

void MeetingParticipantsCtrlEventListener::onLocalRecordingStatusChanged(unsigned int user_id, RecordingStatus status) {}

void MeetingParticipantsCtrlEventListener::onAllowParticipantsRenameNotification(bool bAllow) {}

void MeetingParticipantsCtrlEventListener::onAllowParticipantsUnmuteSelfNotification(bool bAllow) {}

void MeetingParticipantsCtrlEventListener::onAllowParticipantsStartVideoNotification(bool bAllow) {}

void MeetingParticipantsCtrlEventListener::onAllowParticipantsShareWhiteBoardNotification(bool bAllow) {}

void MeetingParticipantsCtrlEventListener::onRequestLocalRecordingPrivilegeChanged(LocalRecordingRequestPrivilegeStatus status) {}

void MeetingParticipantsCtrlEventListener::onAllowParticipantsRequestCloudRecording(bool bAllow) {}

void MeetingParticipantsCtrlEventListener::onInMeetingUserAvatarPathUpdated(unsigned int userID) {}

void MeetingParticipantsCtrlEventListener::onParticipantProfilePictureStatusChange(bool bHidden) {}

void MeetingParticipantsCtrlEventListener::onFocusModeStateChanged(bool bEnabled) {}

void MeetingParticipantsCtrlEventListener::onFocusModeShareTypeChanged(FocusModeShareType type) {}

