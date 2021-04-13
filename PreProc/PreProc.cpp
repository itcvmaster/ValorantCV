// PreProc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream> 
#include "../inc/common.h"
#include <opencv2/highgui.hpp>  // OpenCV window I/O
#include <windows.h>

using namespace std;
using namespace cv;

#define PREPROC_INI ".\\preproc.ini"

const char* WINDOW_BMP = "Bitmap";
FILE *fp;

void MakeWhitePattern(Mat &img, Rect rect, unsigned char diff);
void MakeColorPattern(Mat &img, Rect rect);

int main()
{
	namedWindow(WINDOW_BMP, WINDOW_NORMAL);
	resizeWindow(WINDOW_BMP, 300, 50);

	int i, j;
	unsigned char diff;
	Mat img;
	Rect rect;

	char strApp[MAX_PATH], strKey[MAX_PATH], fn[MAX_PATH], ptnFn[MAX_PATH];
	int resolutionCnt = GetPrivateProfileInt(_T("Resolution"), _T("Cnt"), 0, PREPROC_INI);
	for (j = 0; j < resolutionCnt; j++)
	{
		sprintf_s(strKey, "Height%d", j);
		int WinHeight = GetPrivateProfileInt(_T("Resolution"), strKey, 0, PREPROC_INI);
		sprintf_s(strApp, "%d", WinHeight);

		sprintf_s(fn, "ptn%d.dat", WinHeight);
		fopen_s(&fp, fn, "wb");

		//////////////////////////////////////////////////////score pattern
		sprintf_s(ptnFn, "pattern\\%d\\score.bmp", WinHeight);
		img = imread(ptnFn); if (img.empty()) { cout << " no image file ";	return 0; }
		rect.x = 0; rect.y = 0; rect.width = img.cols / 10; rect.height = img.rows;
		diff = GetPrivateProfileInt(strApp, _T("score"), 0, PREPROC_INI);
		for (i = 0; i < 10; i++)
		{
			rect.x = i * rect.width;
			MakeWhitePattern(img, rect, diff);
		}

		//////////////////////////////////////////////////////time pattern
		sprintf_s(ptnFn, "pattern\\%d\\time.bmp", WinHeight);
		img = imread(ptnFn); if (img.empty()) { cout << " no image file ";	return 0; }
		rect.x = 0; rect.y = 0; rect.width = img.cols / 10; rect.height = img.rows;
		diff = GetPrivateProfileInt(strApp, _T("time"), 0, PREPROC_INI);
		for (i = 0; i < 10; i++)
		{
			rect.x = i * rect.width;
			MakeWhitePattern(img, rect, diff);
		}

		//////////////////////////////////////////////////////ammo pattern
		sprintf_s(ptnFn, "pattern\\%d\\ammo.bmp", WinHeight);
		img = imread(ptnFn); if (img.empty()) { cout << " no image file ";	return 0; }
		rect.x = 0; rect.y = 0; rect.width = img.cols / 10; rect.height = img.rows;
		diff = GetPrivateProfileInt(strApp, _T("ammo"), 0, PREPROC_INI);
		for (i = 0; i < 10; i++)
		{
			rect.x = i * rect.width;
			MakeWhitePattern(img, rect, diff);
		}

		////////////////////////////////////////////////////armor pattern
		sprintf_s(ptnFn, "pattern\\%d\\armor.bmp", WinHeight);
		img = imread(ptnFn); if (img.empty()) { cout << " no image file ";	return 0; }
		rect.x = 0; rect.y = 0; rect.width = img.cols / 10; rect.height = img.rows;
		diff = GetPrivateProfileInt(strApp, _T("armor"), 0, PREPROC_INI);
		for (i = 0; i < 10; i++)
		{
			rect.x = i * rect.width;
			MakeWhitePattern(img, rect, diff);
		}

		////////////////////////////////////////////////////credits pattern
		sprintf_s(ptnFn, "pattern\\%d\\credits.bmp", WinHeight);
		img = imread(ptnFn); if (img.empty()) { cout << " no image file ";	return 0; }
		rect.x = 0; rect.y = 0; rect.width = img.cols / 10; rect.height = img.rows;
		diff = GetPrivateProfileInt(strApp, _T("credits"), 0, PREPROC_INI);
		for (i = 0; i < 10; i++)
		{
			rect.x = i * rect.width;
			MakeWhitePattern(img, rect, diff);
		}

		////////////////////////////////////////////////////agents pattern
		sprintf_s(ptnFn, "pattern\\%d\\agents.bmp", WinHeight);
		img = imread(ptnFn); if (img.empty()) { cout << " no image file ";	return 0; }
		rect.x = GetPrivateProfileInt(strApp, _T("agentsX"), 0, PREPROC_INI);
		rect.y = GetPrivateProfileInt(strApp, _T("agentsY"), 0, PREPROC_INI);
		rect.width = GetPrivateProfileInt(strApp, _T("agentsWid"), 0, PREPROC_INI);
		rect.height = GetPrivateProfileInt(strApp, _T("agentsHei"), 0, PREPROC_INI);
		for (i = 0; i < N_AGENTS; i++)
		{
			MakeColorPattern(img, rect);
			rect.x += img.cols / N_AGENTS;
		}

		/////////////////////////////////////////////////weaponchange
		sprintf_s(ptnFn, "pattern\\%d\\weaponchange.bmp", WinHeight);
		img = imread(ptnFn); if (img.empty()) { cout << " no image file ";	return 0; }
		rect.x = 0; rect.y = 0; rect.width = img.cols; rect.height = img.rows;
		diff = GetPrivateProfileInt(strApp, _T("weaponchange"), 0, PREPROC_INI);
		MakeWhitePattern(img, rect, diff);

		/////////////////////////////////////////////////spike pattern
		sprintf_s(ptnFn, "pattern\\%d\\spike.bmp", WinHeight);
		img = imread(ptnFn); if (img.empty()) { cout << " no image file ";	return 0; }
		rect.x = 0; rect.y = 0; rect.width = img.cols; rect.height = img.rows;
		diff = GetPrivateProfileInt(strApp, _T("spike"), 0, PREPROC_INI);
		MakeWhitePattern(img, rect, diff);

		/////////////////////////////////////////////////hit,headshot pattern
		sprintf_s(ptnFn, "pattern\\%d\\hit.bmp", WinHeight);
		img = imread(ptnFn); if (img.empty()) { cout << " no image file ";	return 0; }
		rect.x = 0; rect.y = 0; rect.width = img.cols; rect.height = img.rows;
		MakeColorPattern(img, rect);

		sprintf_s(ptnFn, "pattern\\%d\\headshot.bmp", WinHeight);
		img = imread(ptnFn); if (img.empty()) { cout << " no image file ";	return 0; }
		rect.x = 0; rect.y = 0; rect.width = img.cols; rect.height = img.rows;
		MakeColorPattern(img, rect);

		/////////////////////////////////////////////////other player pattern
		sprintf_s(ptnFn, "pattern\\%d\\other.bmp", WinHeight);
		img = imread(ptnFn); if (img.empty()) { cout << " no image file ";	return 0; }
		rect.x = 0; rect.y = 0; rect.width = img.cols; rect.height = img.rows;
		diff = GetPrivateProfileInt(strApp, _T("other"), 0, PREPROC_INI);
		MakeWhitePattern(img, rect, diff);

		//////////////////////////////////////////////////map agents icon pattern
		sprintf_s(ptnFn, "pattern\\%d\\mapagents.bmp", WinHeight);
		img = imread(ptnFn); if (img.empty()) { cout << " no image file ";	return 0; }
		rect.x = GetPrivateProfileInt(strApp, _T("mapagentsX"), 0, PREPROC_INI);
		rect.y = GetPrivateProfileInt(strApp, _T("mapagentsY"), 0, PREPROC_INI);
		rect.width = GetPrivateProfileInt(strApp, _T("mapagentsWid"), 0, PREPROC_INI);
		rect.height = GetPrivateProfileInt(strApp, _T("mapagentsHei"), 0, PREPROC_INI);
		for (i = 0; i < N_MAPAGENTS; i++)
		{
			MakeColorPattern(img, rect);
			rect.x += img.cols / N_MAPAGENTS;
		}

		/////////////////////////////////////////////////map red circle
		sprintf_s(ptnFn, "pattern\\%d\\mapredcircle.bmp", WinHeight);
		img = imread(ptnFn); if (img.empty()) { cout << " no image file ";	return 0; }
		rect.x = 0; rect.y = 0; rect.width = img.cols; rect.height = img.rows;
		MakeWhitePattern(img, rect, 150);

		////////////////////////////////////////////////////weapon name
		sprintf_s(ptnFn, "pattern\\%d\\weapon.bmp", WinHeight);
		img = imread(ptnFn); if (img.empty()) { cout << " no image file ";	return 0; }
		rect.x = 0; rect.y = 0; rect.width = img.cols / 18; rect.height = img.rows;
		diff = GetPrivateProfileInt(strApp, _T("weapon"), 0, PREPROC_INI);
		for (i = 0; i < 18; i++)
		{
			rect.x = i * rect.width;
			MakeWhitePattern(img, rect, diff);
		}

		fclose(fp);

	}

	
	
	


    return 0;
}

void MakeWhitePattern(Mat &img, Rect rect, unsigned char diff)
{
	WHITE_PATTERN *ptn = new WHITE_PATTERN;
	ptn->width = rect.width; ptn->height = rect.height;

	imshow(WINDOW_BMP, img);
	
	int x = rect.x, y = rect.y;
	int imgWidth = img.cols, imgHeight = img.rows;

	uchar * imgPtr = img.ptr(0),*p;
	memset(ptn->mask, 0, MASK_LEN);
	for (int j = 0; j < rect.height;j++)
	{
		if (y + j >= imgHeight) { cout << "image size error"; break; }
		for (int i = 0; i < rect.width; i++)
		{
			if (x + i >= imgWidth) { cout << "image size error"; break; }
			p = imgPtr + (imgWidth*(y + j) + (x + i)) * 3;
			if ((*p) < diff)continue;
			p++; if ((*p) < diff)continue;
			p++; if ((*p) < diff)continue;
			ptn->mask[j*rect.width + i] = 1;
		}
	}

	fwrite(ptn, 1, sizeof(WHITE_PATTERN), fp);
	waitKey(1);
	delete ptn;
}

void MakeColorPattern(Mat &img, Rect rect)
{
	COLOR_PATTERN *ptn = new COLOR_PATTERN;
	ptn->width = rect.width; ptn->height = rect.height;

	imshow(WINDOW_BMP, img);

	int x = rect.x, y = rect.y;
	int imgWidth = img.cols, imgHeight = img.rows;

	uchar * imgPtr = img.ptr(0), *p;
	int idx = 0;
	memset(ptn->R, 0, MASK_LEN*3);
	for (int j = 0; j < rect.height; j++)
	{
		if (y + j >= imgHeight) { cout << "image size error"; break; }
		for (int i = 0; i < rect.width; i++)
		{
			if (x + i >= imgWidth) { cout << "image size error"; break; }
			p = imgPtr + (imgWidth*(y + j) + (x + i)) * 3;
			idx = j*rect.width + i;
			ptn->B[idx] = *p++;
			ptn->G[idx] = *p++;
			ptn->R[idx] = *p;
		}
	}

	fwrite(ptn, 1, sizeof(COLOR_PATTERN), fp);
	waitKey(1);
	delete ptn;
}