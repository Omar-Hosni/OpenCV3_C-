#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace std;
using namespace cv;

void copyImgAndColor(const Mat img, Mat& background)
{
	resize(background, background, img.size());

	Vec3b c;

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			c = img.at<Vec3b>(i, j);
			 
			if (c[0] < 255 || c[1] < 255 || c[2] < 255) {
				background.at<Vec3b>(i, j) = c;
			}
		}
	}
}

void copyImgAndColor2(const Mat img, Mat& background)
{
	Mat mask;
	inRange(img, Scalar(0, 0, 0), Scalar(100, 100, 100), mask);
	img.copyTo(background, mask);

}

void copyOrange(const Mat img, Mat& background)
{
	resize(background, background, img.size());

	Vec3b c;

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			c = img.at<Vec3b>(i, j);
			if (c[2] > c[0] && c[2] > c[1])
			{
				background.at<Vec3b>(i, j) = c;
			}
		}
	}
}

void copyGreenRedStain(const Mat img, Mat& background)
{
	background = Mat::zeros(img.size(), img.type());
	Mat hsv;
	cvtColor(img, hsv, COLOR_BGR2HSV);

	Vec3b c, c2;

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			c = img.at<Vec3b>(i, j);
			c2 = hsv.at<Vec3b>(i, j);

			if (c2[0] > 60 && c2[0] < 120)
			{
				background.at<Vec3b>(i, j) = c;
			}
		}
	}
}

////////////////////////////////////////////////

string category(double diff)
{
	if (diff <= 1.0)
		return "Not perceptible";
	if (diff <= 2.0)
		return "Perceptible (close obs.)";
	if (diff <= 10.0)
		return "Perceptible at a glance";
	return "Big differences"; // for the last two categories. diff >10
}

void convert_labf(const Mat img, Mat &dest)
{
	Mat rgbf;
	img.convertTo(rgbf, CV_32FC3, 1 / 255.0);
	cvtColor(rgbf, dest, COLOR_BGR2Lab); //luminus, a and b to detect its value(chromatic information)
}

double compare_img(const Mat etalon_lab, const Mat img_lab)
{
	Mat diff = etalon_lab - img_lab;
	Mat diff2 = diff.mul(diff);

	vector<Mat> channels;
	split(diff2, channels);

	Mat dist;
	//sqrt(delta L^2, delta A^2, delta H^2)
	cv::sqrt(channels[0] + channels[1] + channels[2], dist);


	//mean of difference
	return mean(dist)[0];
}


void histogram(const Mat img, Mat& histo)
{
	histo = Mat::zeros(256, 1, CV_32S);

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			int v = img.at<uchar>(i, j);
			histo.at<int>(v)++;
		}
	}
}

int main()
{
	Mat img = imread("floor_tiles/floor_tiles/benchmark.png");

	Mat lab, lab2;

	convert_labf(img, lab);

	for (int i = 1; i <= 7; i++)
	{
		Mat img2 = imread("floor_tiles/floor_tiles/floor_tile" + to_string(i) + ".png");
		convert_labf(img2, lab2);
		cout << compare_img(lab, lab2) << endl;
		cout << category(compare_img(lab, lab2)) << endl;

		imshow("res"+to_string(i), img2);

	}
	
	Mat img = imread("orange1.jpg");
	Mat eqHistImg;


	vector<Mat> chs;
	split(img, chs);

	equalizeHist(chs[0], chs[0]);
	equalizeHist(chs[1], chs[1]);
	equalizeHist(chs[2], chs[2]);

	Mat dest;
	merge(chs, dest);

	Histo::showHisto(img);
	Histo::showHisto(dest);

	waitKey();
}
