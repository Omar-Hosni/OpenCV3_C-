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

using namespace std;
using namespace cv;

void getContours(const Mat &src, Mat &dest)
{
	vector<vector<Point>> contours;
	vector<Vec4i> heirarchy;

	findContours(src, contours, heirarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	// drawContours(dest, contours, -1, Scalar(255, 0, 255), 2);

	//if area is above a certain value then consider it a shape
	vector<vector<Point>> contourPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;

		if (area > 1000)
		{
			float perimeter = arcLength(contours[i], true);
			approxPolyDP(contours[i], contourPoly[i], 0.02*perimeter, true);
			drawContours(dest, contourPoly, i, Scalar(255, 0, 255), 2);
			
			cout << "size of contour poly shape" << i << " is " << contourPoly[i].size() << endl;
			
			boundRect[i] = boundingRect(contourPoly[i]);
			cout << "the bounded rect of shape " << i << " is " << boundRect[i] << endl;
			rectangle(dest, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);

		}
	}

}

int main()
{
	Mat img = imread("shapes2.png");
	Mat grayImg, blurImg, cannyImg, dilImg;

	//preprocessing
	cvtColor(img, grayImg, COLOR_BGR2GRAY);
	GaussianBlur(grayImg, blurImg, Size(3, 3), 3, 0);
	Canny(blurImg, cannyImg, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
	dilate(cannyImg, dilImg, kernel);

	Mat finalImg;
	//get contour
	getContours(dilImg, img);


	imshow("img", img);
	waitKey();

	
	

}