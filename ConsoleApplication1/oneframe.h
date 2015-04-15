#define CHKERROR(err,msg) 	if (err != FC2_ERROR_OK){ printf("%s: %d\n", msg, err); return -1; }


typedef struct {
	fc2Error error;
	fc2Image rawImage;
	fc2Image saveImage;
	fc2Context context;
	fc2PGRGuid guid;
}camera;

camera* g_cam;


// This fucntion creates the fc2 context and gets the camera GUID. From there it turns 
// the camera on and everything should be set up for captureing images
int _setup(){

	unsigned int cameras_found = 0;

	g_cam->error = fc2CreateContext(&g_cam->context);
	CHKERROR(g_cam->error, "Error in fc2CreateContext");

	g_cam->error = fc2GetNumOfCameras(g_cam->context, &cameras_found);
	CHKERROR(g_cam->error, "Error in fc2GetNumOfcameras");

	if (cameras_found == 0)
	{
		printf("No cameras detected.\n");
		return -1;
	}

	g_cam->error = fc2GetCameraFromIndex(g_cam->context, 0, &g_cam->guid);
	CHKERROR(g_cam->error, "Error in fc2GetcameraFromIndex");

	g_cam->error = fc2Connect(g_cam->context, &g_cam->guid);
	CHKERROR(g_cam->error, "Error in fc2Connect");

	g_cam->error = fc2StartCapture(g_cam->context);
	CHKERROR(g_cam->error, "fc2StartCapture");

	g_cam->error = fc2CreateImage(&g_cam->rawImage);
	g_cam->error = fc2CreateImage(&g_cam->saveImage);
	CHKERROR(g_cam->error, "Error in fc2CreateImage");

	return 0;
}

//This fucntion creates a frame and stores it
int _save_frame()
{
	g_cam->error = fc2RetrieveBuffer(g_cam->context, &g_cam->rawImage);
	CHKERROR(g_cam->error, "Error in retrieveBuffer");

	fc2ConvertImageTo(FC2_PIXEL_FORMAT_RGB8, &g_cam->rawImage, &g_cam->saveImage);
	return 0;
}


int Create(){
	g_cam = malloc(sizeof(camera)); //deallocated in CleanUp()
	int success = 0;

	success = _setup(); 
	if (success != 0)
	{
		printf("Error setting up camera\n");
		return -1;
	}

	_save_frame();

	return 0;
}

fc2Image GrabImage(){
	unsigned char** picture = NULL;

	fc2Image return_image = g_cam->saveImage;
	char * ima = g_cam->saveImage.pData;
	//fc2GetImageData(&g_cam->saveImage, &picture);
	printf("%s", ima);
	_save_frame();
	return return_image;

}


int CleanUp(){
	g_cam->error = fc2DestroyImage(&g_cam->rawImage);
	g_cam->error = fc2DestroyImage(&g_cam->saveImage);
	CHKERROR(g_cam->error, "Error in fc2DestroyImaged");

	g_cam->error = fc2StopCapture(g_cam->context);
	CHKERROR(g_cam->error, "Error in fc2StopCapture");

	g_cam->error = fc2DestroyContext(g_cam->context);
	CHKERROR(g_cam->error, "Error in Error in fc2DestroyContext");

	free(g_cam);
	return 0;
}