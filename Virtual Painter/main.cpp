#include <iostream>
#include <set>
#include <queue>
#include <climits>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>

using namespace std;
using namespace cv;

int hmin, hmax, smin, smax, vmin, vmax;
VideoCapture cap(0);
Mat img;
vector<vector<int>> newPoints;

// smin, vmin, hmax, smax, vmax

vector<vector<int>> Colors = {
	{128,48,117,143,170,255}, //purple
	{68, 72, 156, 126, 255}, //green
	{0, 62, 0, 35, 255, 255}, //orange

};

vector<Scalar> myColorValues{
	{255,0,255},//purple
	{0,255,0}, //green
	{51,153,255} //orange
};

Point getContours(Mat src)
{
	vector<vector<Point>> contours;
	vector<Vec4i> heirarchy;

	findContours(src, contours, heirarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	// drawContours(dest, contours, -1, Scalar(255, 0, 255), 2);

	//if area is above a certain value then consider it a shape
	vector<vector<Point>> contourPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	Point myPoint(0, 0);

	string objectType;

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;

		if (area > 1000)
		{
			float perimeter = arcLength(contours[i], true);
			approxPolyDP(contours[i], contourPoly[i], 0.02 * perimeter, true);
			boundRect[i] = boundingRect(contourPoly[i]);
			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;
			
			
			drawContours(src, contourPoly, i, Scalar(51, 153, 255), 2); //orange
			rectangle(src, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
		}
	}
	return myPoint;
}

vector<vector<int>> findColor(const Mat img)
{
	Mat imgHSV, mask;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i = 0; i < Colors.size(); i++)
	{

		Scalar lower(Colors[i][0], Colors[i][1], Colors[i][2]);
		Scalar upper(Colors[i][3], Colors[i][4], Colors[i][5]);
		inRange(imgHSV, lower, upper, mask);
		//imshow(to_string(i), mask);
		Point myPoint = getContours(mask);

		if(myPoint.x != 0 && myPoint.y != 0)
		newPoints.push_back({ myPoint.x, myPoint.y, i });
	}
	return newPoints;
}

void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues)
{
	for (int i = 0; i < newPoints.size(); i++)
	{
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorValues[newPoints[i][2]], FILLED);;

	}
}

int main()
{
	
	



	while (true)
	{
		cap.read(img);
		vector<vector<int>> newPoints = findColor(img);
		drawOnCanvas(newPoints, myColorValues);
		imshow("video", img);
		waitKey(1);
	}
	
	

}