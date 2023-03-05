#include <stdio.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <ShlObj.h>
#include "./clarius-cast-v10.2.2-windows/include/cast/cast.h"
#pragma comment( lib, "./clarius-cast-v10.2.2-windows/lib/cast.lib" )


void callbackClariusNewImage(const void* image, const CusProcessedImageInfo* nfo, int npos, const CusPosInfo* pos)
{
	printf("\nclariusNewImage called");
	fflush(stdout);
}

void callbackClariusNewRawImage(const void* image, const CusRawImageInfo* nfo, int npos, const CusPosInfo* pos)
{
	printf("\nclariusNewRawImage called, bitsPerSample: %d, rf: %d, jpeg: %d", nfo->bitsPerSample, nfo->rf, nfo->jpeg);
}

void callbackClariusNewSpectralImage(const void* img, const CusSpectralImageInfo* nfo)
{

}

void callbackClariusFreeze(int freezed)
{
	printf("dll::event_clariusFreeze: %d\n", freezed);
	fflush(stdout);
}

void callbackClariusButton(CusButton btn, int clicks)
{
	printf("dll::event_clariusButton: btn: %d, clicks: %d\n", btn, clicks);
	fflush(stdout);
}

void callbackClariusProgress(int progress)
{
	printf("dll:event_clariusProgress: %d\n", progress);
}

void callbackClariusError(const char* errMessage)
{
	printf("dll::event_error: %s\n", errMessage);
	fflush(stdout);
}

int main(int argc, char** argv)
{
	std::string ipAddress = "192.168.1.1";
	int port = 5828;
	int ret;

	//---------------------------------------
	// retrieve AppData folder path
	//---------------------------------------
	char keyFolder[MAX_PATH];
	HRESULT result = SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, keyFolder);
	if (result == S_OK)
	{
		std::cout << "AppData folder path: " << keyFolder << std::endl;
	}
	else
	{
		std::cerr << "Failed to retrieve AppData folder path" << std::endl;
	}

	//-------------------------------
	// initialize cusCastInit
	//-------------------------------
	printf("Invoking cusCastInit...");
	fflush(stdout);
	{
		ret = cusCastInit(
			argc, argv,
			//0, NULL,
			keyFolder,
			callbackClariusNewImage,
			callbackClariusNewRawImage,
			callbackClariusNewSpectralImage,
			callbackClariusFreeze,
			callbackClariusButton,
			callbackClariusProgress,
			callbackClariusError,
			640,
			480
		);
	}
	printf(", ret = %d\n", ret);

	//-------------------------------
	// initialize cusCastInit
	//-------------------------------
	printf("Invoking cusCastConnect...");
	fflush(stdout);
	{
		ret = cusCastConnect(ipAddress.c_str(), port, "research", NULL);
	}
	printf(", ret = %d\n", ret);

	//-----------------------------------------
	// wait until users press enter to exit
	//-----------------------------------------
	printf("Waiting for cast api to call other callbacks...\n");
	std::string line;
	std::getline(std::cin, line);
	cusCastDestroy();

	return 0;
}
