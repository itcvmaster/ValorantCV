#include "stdafx.h"
#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion

#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O

#include "Stats.h"
#include "CheckEvent.h"
#include <Windows.h>

using namespace std;
using namespace cv;

void Recog(Mat &frames);

CStats m_Stats;
CCheckEvent m_CheckEvent;
int main(int argc, char *argv[])
{

	const char* WIN_RF = "Reference";

	// Windows
	namedWindow(WIN_RF, WINDOW_NORMAL);
	resizeWindow(WIN_RF, 1280, 720);
	moveWindow(WIN_RF, 400, 0);

	Mat frames;

	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA findFileData;
// 	hFind = FindFirstFile(_T("D:\\Dev\\com\\0323\\ValorantCV2_0322\\ValorantCV2_0322\\ErrorBmps\\*.bmp"), &findFileData);
// 	string sourceReference, sourcePath = "D:\\Dev\\com\\0323\\ValorantCV2_0322\\ValorantCV2_0322\\ErrorBmps\\";
// 	hFind = FindFirstFile(_T("D:\\Dev\\img\\3\\1024-768\\*.bmp"), &findFileData);
// 	string sourceReference, sourcePath = "D:\\Dev\\img\\3\\1024-768\\";
	hFind = FindFirstFile(_T(".\\*.bmp"), &findFileData);
	string sourceReference, sourcePath = ".\\";
	if (hFind != INVALID_HANDLE_VALUE)
		while (true)
		{
			cout << endl << findFileData.cFileName;
			sourceReference = sourcePath + findFileData.cFileName;
			frames = imread(sourceReference);
			if (!frames.data)
			{
				cout << "Could not open or find the image" << std::endl;
				break;
			}

			Recog(frames);

			imshow(WIN_RF, frames);

			char c = waitKey(0);
			if (!FindNextFile(hFind, &findFileData))break;
		}
	return 0;
}

void Recog(Mat &frames)
{
	m_Stats.GetStats(frames.ptr(0), frames.cols, frames.rows);
	m_CheckEvent.Check(&m_Stats);
	cout << m_CheckEvent.m_strEvent;
	cout << m_CheckEvent.m_strState << endl;


	//draw enemy rect
	Rect rect(0, 0, 0, 0);
	for (int i = 0; i < m_Stats.m_nEnemy; i++)
	{
		rect.x = m_Stats.m_Enemy[i].left;
		rect.y = m_Stats.m_Enemy[i].top;
		rect.width = m_Stats.m_Enemy[i].right-m_Stats.m_Enemy[i].left;
		rect.height = m_Stats.m_Enemy[i].bottom- m_Stats.m_Enemy[i].top;
		rectangle(frames, rect, Scalar(0, 0, 255), 2);
	}

	int oxy = 227 * m_Stats.m_ImgHeight / 1080;
	for (int i = 0; i < m_Stats.m_nMapEnemy; i++)
	{
		rect.x = oxy + m_Stats.m_MapEnemy[i].mapX - 8;
		rect.y = oxy - m_Stats.m_MapEnemy[i].mapY - 8;
		rect.width = 17;
		rect.height = 17;
		rectangle(frames, rect, Scalar(255, 255, 0), 2);

// 		int x = m_Stats.m_ImgWidth / 2 + (m_Stats.m_ImgWidth * 2 / 5) * m_Stats.m_MapEnemy[i].mapX / m_Stats.m_MapEnemy[i].mapY;
// 		rect.x = x;
// 		rect.y = 100;
// 		rect.width = 3;
// 		rect.height = 800;
// 		rectangle(frames, rect, Scalar(255, 0, 0), 2);


// 		if (m_Stats.m_MapEnemy[i].right == 0)continue;
// 		rect.x = m_Stats.m_MapEnemy[i].left;
// 		rect.y = m_Stats.m_MapEnemy[i].top;
// 		rect.width = m_Stats.m_MapEnemy[i].right - m_Stats.m_MapEnemy[i].left;
// 		rect.height = m_Stats.m_MapEnemy[i].bottom - m_Stats.m_MapEnemy[i].top;
// 		rectangle(frames, rect, Scalar(0,0,255), 2);

	}
}
