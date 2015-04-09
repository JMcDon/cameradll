#include <stdio.h>
#include "FlyCapture2_C.h"
extern "C"
{
	__declspec(dllexport) void DisplayHelloFromMyDLL()
	{
		printf("Hello DLL.\n");
	}


__declspec(dllexport) int _setup(fc2Error* error, fc2Context* context, fc2PGRGuid* guid){

	unsigned int cameras_found = 0;
	*error = fc2CreateContext(context);
	if (*error != FC2_ERROR_OK)
	{
		return -1;
	}

	*error = fc2GetNumOfCameras(*context, &cameras_found);
	if (*error != FC2_ERROR_OK)
	{
		return -1;
	}

	if (cameras_found == 0)
	{
		return -1;
	}

	*error = fc2GetCameraFromIndex(*context, 0, guid);
	if (*error != FC2_ERROR_OK)
	{
		return -1;
	}
	return 0;
}

__declspec(dllexport) int _save_frame(fc2Context context, char file_location[])
{
	fc2Error error;
	fc2Image rawImage;
	fc2Image saveImage;
	char buf[256];

	error = fc2CreateImage(&rawImage);
	error = fc2CreateImage(&saveImage);
	if (error != FC2_ERROR_OK)
	{
		return -1;
	}

	error = fc2RetrieveBuffer(context, &rawImage);
	if (error != FC2_ERROR_OK)
	{
		return -1;
	}

	sprintf_s(buf, "%s%s", file_location, ".jpeg"); // concatinate file extension
	fc2ConvertImageTo(FC2_PIXEL_FORMAT_RGBU, &rawImage, &saveImage);
	error = fc2SaveImage(&saveImage, buf, FC2_JPEG);
	if (error != FC2_ERROR_OK)
	{
		return -1;
	}


	if (error != FC2_ERROR_OK)
	{
		return -1;
	}

	error = fc2DestroyImage(&rawImage);
	if (error != FC2_ERROR_OK)
	{
		return -1;
	}
	return 0;
}


__declspec(dllexport) int _connect_camera(fc2Context context, fc2PGRGuid guid, char file_location[])
{
	fc2Error error;

	error = fc2Connect(context, &guid);
	if (error != FC2_ERROR_OK)
	{
		return -1;
	}

	error = fc2StartCapture(context);
	if (error != FC2_ERROR_OK)
	{
		return -1;
	}

	_save_frame(context, file_location);

	error = fc2StopCapture(context);
	if (error != FC2_ERROR_OK)
	{
		return -1;
	}

	return 0;
}
__declspec(dllexport) int TakePicture(char file_location[]){
	fc2Error error;
	fc2Context context;
	fc2PGRGuid guid;
	int success = 0;

	success = _setup(&error, &context, &guid);
	if (success != 0)
	{
		return -1;
	}
	success = _connect_camera(context, guid, file_location);
	if (success != 0)
	{
		return -1;
	}

	error = fc2DestroyContext(context);
	if (error != FC2_ERROR_OK)
	{
		return -1;
	}
	return 0;
}
}