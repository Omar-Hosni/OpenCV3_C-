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
	
	string objectType;

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

			int objCorner = (int)contourPoly[i].size();

			if (objCorner == 3) objectType = "Triangle";
			
			else if (objCorner == 4) {
				float aspectRatio = boundRect[i].width / boundRect[i].height;
				
				if (aspectRatio >= 0.9 && aspectRatio <= 1.1) objectType = "Square";
				else objectType = "Rectangle";
			}
			
			else if (objCorner > 4) objectType = "Circle";

			putText(dest, objectType, { boundRect[i].x, boundRect[i].y - 5 }, FONT_HERSHEY_DUPLEX, 0.75, Scalar(0, 69, 255), 2);

		}
	}

}

int main()
{
	Mat img = imread("shapes2.png");
	Mat processedImg;

	//preprocessing
	cvtColor(img, processedImg, COLOR_BGR2GRAY);
	GaussianBlur(processedImg, processedImg, Size(3, 3), 3, 0);
	Canny(processedImg, processedImg, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
	dilate(processedImg, processedImg, kernel);

	Mat finalImg;
	//get contour
	getContours(processedImg, img);


	imshow("img", img);
	waitKey();

	
	

}
