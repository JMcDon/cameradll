

class OneFrame
{
	int _setup(fc2Error* error, fc2Context* context, fc2PGRGuid* guid){

		unsigned int cameras_found = 0;
		*error = fc2CreateContext(context);
		if (*error != FC2_ERROR_OK)
		{
			//printf("Error in fc2CreateContext: %d\n", error);
			return -1;
		}

		*error = fc2GetNumOfCameras(*context, &cameras_found);
		if (*error != FC2_ERROR_OK)
		{
			//printf("Error in fc2GetNumOfCameras: %d\n", error);
			return -1;
		}

		if (cameras_found == 0)
		{
			//printf("No cameras detected.\n");
			return -1;
		}

		*error = fc2GetCameraFromIndex(*context, 0, guid);
		if (*error != FC2_ERROR_OK)
		{
			//printf("Error in fc2GetCameraFromIndex: %d\n", error);
			return -1;
		}
		return 0;
	}

	int _save_frame(fc2Context context, char file_location[], int frames = 1000)
	{
		fc2Error error;
		fc2Image rawImage;
		fc2Image saveImage;
		char buf[256];

		error = fc2CreateImage(&rawImage);
		error = fc2CreateImage(&saveImage);
		if (error != FC2_ERROR_OK)
		{
			//printf("Error in fc2CreateImage: %d\n", error);
			return -1;
		}

		error = fc2RetrieveBuffer(context, &rawImage);
		if (error != FC2_ERROR_OK)
		{
			//printf("Error in retrieveBuffer: %d\n", error);
			return -1;
		}

		sprintf_s(buf, "%s%s", file_location, ".jpeg"); // concatinate file extension
		fc2ConvertImageTo(FC2_PIXEL_FORMAT_RGBU, &rawImage, &saveImage);
		error = fc2SaveImage(&saveImage, buf, FC2_JPEG);
		if (error != FC2_ERROR_OK)
		{
			//printf("Error in saving image: %d\n", error);
			return -1;
		}
		
	
		if (error != FC2_ERROR_OK)
		{
			//printf("Error in saving image: %d\n", error);
			return -1;
		}

		error = fc2DestroyImage(&rawImage);
		if (error != FC2_ERROR_OK)
		{
			//printf("Error in fc2DestroyImaged: %d\n", error);
			return -1;
		}
		return 0;
	}


	int _connect_camera(fc2Context context, fc2PGRGuid guid, char file_location[])
	{
		fc2Error error;

		error = fc2Connect(context, &guid);
		if (error != FC2_ERROR_OK)
		{
			//printf("Error in fc2Connect: %d\n", error);
			return -1;
		}

		error = fc2StartCapture(context);
		if (error != FC2_ERROR_OK)
		{
			//printf("Error in fc2StartCapture: %d\n", error);
			return -1;
		}

		_save_frame(context, file_location);

		error = fc2StopCapture(context);
		if (error != FC2_ERROR_OK)
		{
			//printf("Error in fc2StopCapture: %d\n", error);
			return -1;
		}

		return 0;
	}
	public:
	int TakePicture(char file_location[]){
		fc2Error error;
		fc2Context context;
		fc2PGRGuid guid;
		int success = 0;

		success = _setup(&error, &context, &guid);
		if (success != 0)
		{
			//printf("Error setting up camera\n");
			return -1;
		}
		success = _connect_camera(context, guid, file_location);
		if (success != 0)
		{
			//printf("Error running camera\n");
			return -1;
		}

		error = fc2DestroyContext(context);
		if (error != FC2_ERROR_OK)
		{
			//printf("Error in fc2DestroyContext: %d\n", error);
			return -1;
		}
	}
};