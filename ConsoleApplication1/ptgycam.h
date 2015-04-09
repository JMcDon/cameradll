#ifndef ptgycam
#define ptgycam
#include "FlyCapture2_C.h"

int _setup_camera(fc2Error* error, fc2Context* context, fc2PGRGuid* guid)
{
	unsigned int cameras_found = 0;
	*error = fc2CreateContext(context);
	if (*error != FC2_ERROR_OK)
	{
		printf("Error in fc2CreateContext: %d\n", error);
		return -1;
	}

	*error = fc2GetNumOfCameras(*context, &cameras_found);
	if (*error != FC2_ERROR_OK)
	{
		printf("Error in fc2GetNumOfCameras: %d\n", error);
		return -1;
	}

	if (cameras_found == 0)
	{
		printf("No cameras detected.\n");
		return -1;
	}

	*error = fc2GetCameraFromIndex(*context, 0, guid);
	if (*error != FC2_ERROR_OK)
	{
		printf("Error in fc2GetCameraFromIndex: %d\n", error);
		return -1;
	}
	return 0;
}

int Save_Video(fc2Context context, int frames = 1000)
{
	fc2Error error;
	fc2Image rawImage;
	fc2AVIContext aviContext;
	fc2H264Option h264Option;
	int i;

	error = fc2CreateAVI(&aviContext);
	if (error != FC2_ERROR_OK)
	{
		printf("Error in fc2CreateAVI: %d\n", error);
		return -1;
	}

	error = fc2CreateImage(&rawImage);
	if (error != FC2_ERROR_OK)
	{
		printf("Error in fc2CreateImage: %d\n", error);
		fc2DestroyAVI(aviContext);
		return -1;
	}

	for (i = 0; i < frames; i++)
	{
		// Retrieve the image
		error = fc2RetrieveBuffer(context, &rawImage);
		if (error != FC2_ERROR_OK)
		{
			printf("Error in retrieveBuffer: %d\n", error);
			continue;
		}

		// Perform some initialization for the first time
		if (i == 0)
		{
			h264Option.frameRate = 15;
			h264Option.bitrate = 1000000;
			h264Option.width = rawImage.cols;
			h264Option.height = rawImage.rows;
			error = fc2H264Open(aviContext, "C:\\Users\\jason.mcdonald\\Desktop\\SaveImageToAviEx_C-H264", &h264Option);
			if (error != FC2_ERROR_OK)
			{
				printf("Error opening AVI: %d\n", error);
			}
		}

		error = fc2AVIAppend(aviContext, &rawImage);
		if (error != FC2_ERROR_OK)
		{
			printf("Error appending to AVI: %d\n", error);
		}

		printf("Appended image %d\n", i);
	}

	error = fc2DestroyImage(&rawImage);
	if (error != FC2_ERROR_OK)
	{
		printf("Error in fc2CreateImaged: %d\n", error);
		fc2DestroyAVI(aviContext);
		return -1;
	}

	error = fc2DestroyAVI(aviContext);
	if (error != FC2_ERROR_OK)
	{
		printf("Error in fc2DestroyAVI: %d\n", error);
		return -1;
	}

	return 0;
}


int RunCamera(fc2Context context, fc2PGRGuid guid)
{
	fc2Error error;

	error = fc2Connect(context, &guid);
	if (error != FC2_ERROR_OK)
	{
		printf("Error in fc2Connect: %d\n", error);
		return -1;
	}

	error = fc2StartCapture(context);
	if (error != FC2_ERROR_OK)
	{
		printf("Error in fc2StartCapture: %d\n", error);
		return -1;
	}

	Save_Video(context);

	error = fc2StopCapture(context);
	if (error != FC2_ERROR_OK)
	{
		printf("Error in fc2StopCapture: %d\n", error);
		return -1;
	}

	return 0;
}

int RecordVideo(){
	fc2Error error;
	fc2Context context;
	fc2PGRGuid guid;
	int success = 0;

	success = _setup_camera(&error, &context, &guid);
	if (success != 0)
	{
		printf("Error setting up camera\n");
		return -1;
	}
	success = RunCamera(context, guid);
	if (success != 0)
	{
		printf("Error running camera\n");
		return -1;
	}

	error = fc2DestroyContext(context);
	if (error != FC2_ERROR_OK)
	{
		printf("Error in fc2DestroyContext: %d\n", error);
		return -1;
	}

	return 0;
}



#endif