// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ptgycam.h"
#include <stdio.h>
#include "oneframe.h"

#if defined(WIN32) || defined(WIN64)
#define _CRT_SECURE_NO_WARNINGS		
#endif


int main(int argc, char** argv)
{
	OneFrame test = OneFrame();
	test.TakePicture("C:\\Users\\jason.mcdonald\\Desktop\\Pictures\\TestFrame");

	//RecordVideo();
	//getchar();

	return 0;
}

