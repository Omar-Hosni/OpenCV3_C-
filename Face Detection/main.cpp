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



int main()
{
	
	Mat img = imread("face.jpg");

	CascadeClassifier faceCascade;
	faceCascade.load("haarcascade_frontalface_default.xml");
	
	if (faceCascade.empty()) {
		cout << "not loaded";
	}

	vector<Rect> faces;
	faceCascade.detectMultiScale(img, faces, 1.1, 10);

	for (int i = 0; i < faces.size(); i++)
	{
		rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 255, 255), 2);
	}

	imshow("img", img);

	waitKey();

	
	

}