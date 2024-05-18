#include "GetAudioRawData.h"
#include <iostream>
#include <windows.h>
#include <zoom_sdk.h>
#include <meeting_service_interface.h>
#include <rawdata/zoom_rawdata_api.h>
#include <rawdata/rawdata_video_source_helper_interface.h>
#include <setting_service_interface.h>
#include "network_connection_handler_interface.h"
#include "AuthServiceEventListener.h"
#include "NetworkConnectionHandler.h"
#include "MeetingServiceEventListener.h"
#include "MeetingParticipantsCtrlEventListener.h"
#include <setting_service_interface.h>
#include <fstream>
#include "json\json.h"
#include <sstream>
#include "ZoomSDKAudioRawDataDelegate.h"
#include <meeting_service_components/meeting_recording_interface.h>
#include <meeting_service_components/meeting_participants_ctrl_interface.h>
#include <thread>
#include <chrono>
#include <codecvt>
#include <locale>
#include <string>

#include "MeetingRecordingCtrlEventListener.h"
#include "WebService.h"

using namespace std;
using namespace Json;
using namespace ZOOMSDK;

bool g_exit = false;
IMeetingService* meetingService;
IAuthService* authService;
INetworkConnectionHelper* network_connection_helper;
ISettingService* settingService;

wstring sdk_jwt;
unsigned long long meeting_number;
wstring passcode;

bool isJWTWebService = false;

ZoomSDKAudioRawDataDelegate* audio_source = new ZoomSDKAudioRawDataDelegate();
IZoomSDKAudioRawDataHelper* audioHelper;

IMeetingRecordingController* m_pRecordController;
IMeetingParticipantsController* m_pParticipantsController;

inline bool IsInMeeting(ZOOM_SDK_NAMESPACE::MeetingStatus status)
{
	bool bInMeeting(false);
	if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_INMEETING)
	{
		bInMeeting = true;
	}

	return bInMeeting;
}

void attemptToStartLocalRecording() {

	m_pRecordController = meetingService->GetMeetingRecordingController();

	SDKError err1 = m_pRecordController->StartRawRecording();
	if (err1 != SDKERR_SUCCESS) {
		std::cout << "Error occurred";
	}

	audioHelper = GetAudioRawdataHelper();
	SDKError err = audioHelper->subscribe(audio_source);
	if (err != SDKERR_SUCCESS) {
		std::cout << "Error occurred";
	}

}

bool CanIStartLocalRecording()
{
	IMeetingRecordingController* m_pRecordController = meetingService->GetMeetingRecordingController();
	if (m_pRecordController)
	{
		SDKError err = m_pRecordController->CanStartRecording(false, 0);
		if (err != SDKERR_SUCCESS) {

			m_pRecordController->RequestLocalRecordingPrivilege();
			return false;
		}
		else {
			return true;
		}
	}
}

void ShowErrorAndExit(SDKError err) {
	g_exit = true;
	string message = "";
	switch (err) {
	case SDKERR_INTERNAL_ERROR:
		message = ", check your SDK JWT.";
		break;
	case SDKERR_INVALID_PARAMETER:
		message = ", invalid parametersk, check your Meeting Number.";
	}
	printf("SDK Error: %d%s\n", err, message.c_str());
};

void onInMeeting() {
	if (meetingService->GetMeetingStatus() == ZOOM_SDK_NAMESPACE::MEETING_STATUS_INMEETING) {
		auto users = meetingService->GetMeetingParticipantsController()->GetParticipantsList();
		unsigned int me = 0;
		for (int i = 0; i < users->GetCount(); i++) {
			auto curUser = meetingService->GetMeetingParticipantsController()->GetUserByUserID(users->GetItem(i));
			if (curUser->IsMySelf()) {
				me = curUser->GetUserID();
			}
		}
		if (me != 0) {
			meetingService->GetMeetingAudioController()->MuteAudio(me, true);
		}

		if (CanIStartLocalRecording()) {
			attemptToStartLocalRecording();
		}
	}
}

void onMeetingEndsQuitApp() {
	g_exit = true;
}

void onMeetingJoined() {
}

void onIsHost() {

	if (CanIStartLocalRecording()) {
		attemptToStartLocalRecording();
	}
}

void onIsCoHost() {
	if (CanIStartLocalRecording()) {
		attemptToStartLocalRecording();
	}
}

void onIsGivenRecordingPermission() {
	if (CanIStartLocalRecording()) {
		attemptToStartLocalRecording();
	}
}

wstring StringToWString(string input)
{
	wstring output;
	output.assign(input.begin(), input.end());
	return output;
}

string WStringToString(wstring input)
{
	string output;
	output.assign(input.begin(), input.end());
	return output;
}

wstring QuestionInput(string qustion)
{
	wstring input;
	std::cout << qustion;
	getline(wcin, input);
	return input;
}

void JoinMeeting()
{
	SDKError err(SDKError::SDKERR_SUCCESS);

	if ((err = CreateMeetingService(&meetingService)) != SDKError::SDKERR_SUCCESS) ShowErrorAndExit(err);
	m_pParticipantsController = meetingService->GetMeetingParticipantsController();

	JoinParam joinMeetingParam;
	JoinParam4WithoutLogin joinMeetingWithoutLoginParam;
	joinMeetingParam.userType = SDK_UT_WITHOUT_LOGIN;
	joinMeetingWithoutLoginParam.meetingNumber = meeting_number;
	joinMeetingWithoutLoginParam.psw = passcode.c_str();
	joinMeetingWithoutLoginParam.userName = L"Points bot";
	joinMeetingWithoutLoginParam.userZAK = L"";
	joinMeetingWithoutLoginParam.join_token = NULL;
	joinMeetingWithoutLoginParam.vanityID = NULL;
	joinMeetingWithoutLoginParam.customer_key = NULL;
	joinMeetingWithoutLoginParam.webinarToken = NULL;
	joinMeetingWithoutLoginParam.app_privilege_token = NULL;
	joinMeetingWithoutLoginParam.hDirectShareAppWnd = NULL;
	joinMeetingWithoutLoginParam.isAudioOff = false;
	joinMeetingWithoutLoginParam.isVideoOff = true;
	joinMeetingWithoutLoginParam.isDirectShareDesktop = false;
	joinMeetingParam.param.withoutloginuserJoin = joinMeetingWithoutLoginParam;

	meetingService->SetEvent(new MeetingServiceEventListener(&onMeetingJoined, &onMeetingEndsQuitApp, &onInMeeting));
	m_pParticipantsController->SetEvent(new MeetingParticipantsCtrlEventListener(&onIsHost, &onIsCoHost, &onIsGivenRecordingPermission));

	m_pRecordController = meetingService->GetMeetingRecordingController();
	m_pRecordController->SetEvent(new MeetingRecordingCtrlEventListener(&onIsGivenRecordingPermission));

	CreateSettingService(&settingService);

	ZOOM_SDK_NAMESPACE::IAudioSettingContext* pAudioContext = settingService->GetAudioSettings();
	if (pAudioContext)
	{
		
		pAudioContext->EnableAutoJoinAudio(true);
	}

	if ((err = meetingService->Join(joinMeetingParam)) != SDKError::SDKERR_SUCCESS) ShowErrorAndExit(err);
}

void SDKAuth()
{
	SDKError err(SDKError::SDKERR_SUCCESS);

	if ((err = CreateAuthService(&authService)) != SDKError::SDKERR_SUCCESS) ShowErrorAndExit(err);
	AuthContext authContext;
	if ((err = authService->SetEvent(new AuthServiceEventListener(JoinMeeting))) != SDKError::SDKERR_SUCCESS) ShowErrorAndExit(err);
	if (isJWTWebService) {
		authContext.jwt_token = GetSignatureFromWebService();
	}
	else {
		authContext.jwt_token = sdk_jwt.c_str();
	}
	if ((err = authService->SDKAuth(authContext)) != SDKError::SDKERR_SUCCESS) ShowErrorAndExit(err);
}

void InitSDK()
{
	SDKError err(SDKError::SDKERR_SUCCESS);

	InitParam initParam;
	initParam.strWebDomain = L"https://zoom.us";
	initParam.enableLogByDefault = true;
	initParam.obConfigOpts.optionalFeatures = (1 << 5);
	if ((err = InitSDK(initParam)) != SDKError::SDKERR_SUCCESS) ShowErrorAndExit(err);
	if ((err = CreateNetworkConnectionHelper(&network_connection_helper)) != SDKError::SDKERR_SUCCESS) ShowErrorAndExit(err);
	if ((err = network_connection_helper->RegisterNetworkConnectionHandler(new NetworkConnectionHandler(&SDKAuth))) != SDKError::SDKERR_SUCCESS) ShowErrorAndExit(err);
}

int GetAudioRawData::LeaveMeeting()
{
	if (meetingService == NULL)
	{
		if (IsInMeeting(meetingService->GetMeetingStatus()))
		{
			return meetingService->Leave(ZOOM_SDK_NAMESPACE::LEAVE_MEETING);
		}
	}
	return ZOOM_SDK_NAMESPACE::SDKERR_UNINITIALIZE;
}

int GetAudioRawData::startInMeetingProcess(std::string jwt, std::string meetingNumber, std::string pwd)
{	
	sdk_jwt = StringToWString(jwt);
	meeting_number = std::stoull(meetingNumber);
	passcode = StringToWString(pwd);

	if (std::ifstream{ "audio.pcm" }) {
		std::remove("audio.pcm");
	}

	InitSDK();

	int bRet = false;
	MSG msg;
	while (!g_exit && (bRet = GetMessage(&msg, nullptr, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	if (meetingService) DestroyMeetingService(meetingService);
	if (authService) DestroyAuthService(authService);
	if (network_connection_helper) DestroyNetworkConnectionHelper(network_connection_helper);
	CleanUPSDK();
	return 0;
}

