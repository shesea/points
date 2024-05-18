#include "windows.h"
#include <cstdint>
#include <iostream>
#include <fstream>
#include "rawdata/rawdata_audio_helper_interface.h"
#include "ZoomSDKAudioRawDataDelegate.h"
#include "zoom_sdk_def.h" 




using namespace std;
using namespace ZOOM_SDK_NAMESPACE;
void ZoomSDKAudioRawDataDelegate::onOneWayAudioRawDataReceived(AudioRawData* audioRawData, uint32_t node_id)
{
}
void ZoomSDKAudioRawDataDelegate::onShareAudioRawDataReceived(AudioRawData* data_)
{
}
void ZoomSDKAudioRawDataDelegate::onMixedAudioRawDataReceived(AudioRawData* audioRawData)
{
	std::cout << "Received onMixedAudioRawDataReceived" << std::endl;

	static ofstream pcmFile;
	pcmFile.open("audio.pcm", ios::out | ios::binary | ios::app);

	if (!pcmFile.is_open()) {
		std::cout << "Failed to open wave file" << std::endl;
		return;
	}
	try {
		pcmFile.write((char*)audioRawData->GetBuffer(), audioRawData->GetBufferLen());

		pcmFile.close();
		pcmFile.flush();
	}
	catch (exception e)
	{
	}


}