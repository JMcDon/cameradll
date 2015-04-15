// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ptgycam.h"
#include <stdio.h>
#include "oneframe.h"
#include <Windows.h>

#if defined(WIN32) || defined(WIN64)
#define _CRT_SECURE_NO_WARNINGS		
#endif


int main(int argc, char** argv)
{

	Create();
	char buf[70];
	for (int i = 0; i < 100; i++)
	{
		sprintf_s(buf, 60, "%s%d%s", "C:\\Users\\jason.mcdonald\\Desktop\\Pictures\\Frame", i,".jpeg");
		fc2Image picture = GrabImage();
		g_cam->error = fc2SaveImage(&picture, buf, FC2_JPEG);
		CHKERROR(g_cam->error, "Error in saving image");
		Sleep(100);
	}
	CleanUp();
	printf("done!");
	getchar();

	return 0;
}

