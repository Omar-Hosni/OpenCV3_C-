#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void histogram() {
    Mat img = imread("Resources/dog.jpg", IMREAD_GRAYSCALE);
    Mat histo;
    histo = Mat::zeros(256, 1, CV_32S);

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            int v = img.at<uchar>(i, j);
            histo.at<int>(v, 0)++;
        }
    }
    cout << histo << endl;
    waitKey();
}

void imageEnhanceFirst() {
    Mat img = imread("Resources/dark_img.jpg",IMREAD_GRAYSCALE);
    double min, max;
    minMaxLoc(img, &min, &max);
    Mat dest = (img - min) * (255 / (max - min));
    imshow("img", img);
    imshow("dest", dest);
    waitKey();
}

void medianBoxGaussianFilters() {
    Mat orange = imread("Resources/orange.jpg", IMREAD_COLOR);
    Mat median_filtered;
    medianBlur(orange, median_filtered, 5);

    Mat box_filtered;
    boxFilter(orange, box_filtered, -1, Size(15, 15));

    Mat gaussian_filtered;
    GaussianBlur(orange, gaussian_filtered, Size(11, 11), 2.0);
    imshow("original", orange);
    imshow("Median filtered", median_filtered);
    imshow("Box filtered", box_filtered);
    imshow("Gaussian filtered", gaussian_filtered);
    waitKey(0);
}

void helaFolder() {
    for (int i = 0; i <= 20; i++) {
        // Read the image in grayscale
        Mat img = imread("Resources/hela/" + to_string(i) + ".png", IMREAD_GRAYSCALE);
        if (img.empty()) {
            cerr << "Could not read image: " << i << endl;
            continue;
        }

        // Apply a large median filter
        Mat bg;
        medianBlur(img, bg, 51);

        // Show the result
        imshow("Background", bg);
        waitKey();

        // Remove the background using the minus operator
        Mat result1 = img - bg;

        // Show the result
        imshow("Result 1", result1);
        waitKey();

        // Remove the background using absdiff
        Mat result2;
        absdiff(img, bg, result2);

        // Show the result
        imshow("Result 2", result2);
        waitKey();
    }
}

int main() {
    helaFolder();
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void simpleEdgeDetector() {
    Mat img = imread("Resources/flower.jpg", IMREAD_GRAYSCALE);

    // Create a black image with the same size
    Mat edges = Mat::zeros(img.size(), CV_8UC1);

    // Compute the gradient and strength of edges
    for (int x = 1; x < img.rows; x++) {
        for (int y = 1; y < img.cols; y++) {
            int Gx = img.at<uchar>(x, y) - img.at<uchar>(x - 1, y);
            int Gy = img.at<uchar>(x, y) - img.at<uchar>(x, y - 1);
            int G = abs(Gx) + abs(Gy);
            edges.at<uchar>(x, y) = static_cast<uchar>(G);
        }
    }

    // Apply a threshold to reduce noise
    threshold(edges, edges, 50, 255, THRESH_BINARY);

    // Display the result
    namedWindow("Original", WINDOW_NORMAL);
    imshow("Original", img);

    namedWindow("Edges", WINDOW_NORMAL);
    imshow("Edges", edges);

    waitKey(0);
}

void findTheEdges1() {
    // Load image in grayscale
    Mat src = imread("Resources/flower.jpg", IMREAD_GRAYSCALE);

    // Check if image is loaded successfully
    if (src.empty()) {
        std::cerr << "Failed to open the image" << std::endl;
    }

    // Apply Sobel operator to get the horizontal and vertical gradient
    Mat dx, dy;
    Sobel(src, dx, CV_16S, 1, 0);
    Sobel(src, dy, CV_16S, 0, 1);

    // Approximate the strength of the edge
    Mat G = abs(dx) + abs(dy);

    // Convert G matrix to the CV_8UC1 image
    Mat dest;
    convertScaleAbs(G, dest);

    // Apply threshold to get the edges
    threshold(dest, dest, 50, 255, THRESH_BINARY);

    // Display the image
    imshow("Edges", dest);
    waitKey(0);
}

void findTheEdges2() {
    // Load an image (grayscale)
    Mat src = imread("Resources/flower.jpg", IMREAD_GRAYSCALE);

    // Use the Sobel operator to get the horizontal and vertical gradient
    Mat gx, gy;
    Sobel(src, gx, CV_32F, 1, 0); // each item is a float number
    Sobel(src, gy, CV_32F, 0, 1);
   
    // Compute the strength of the edges: G
    Mat G;
    magnitude(gx, gy, G);

    // Convert the G matrix to the CV_8UC1 image
    Mat dest;
    convertScaleAbs(G, dest);

    // Use threshold to decide which one is the edge, and which one is not
    threshold(dest, dest, 50, 255, THRESH_BINARY);

    // Display the image
    imshow("Edges", dest);
    waitKey(0);

}
void cannyMethod() {
    // Load image in grayscale
    Mat img = imread("Resources/go2.png", IMREAD_GRAYSCALE);

    if (img.empty())
    {
        cout << "Could not read the image." << endl;
    }

    // Apply Gaussian blur to smooth the image
    Mat blurred;
    GaussianBlur(img, blurred, Size(5, 5), 0);

    // Compute gradient using Sobel operator
    Mat grad_x, grad_y;
    Sobel(blurred, grad_x, CV_32F, 1, 0);
    Sobel(blurred, grad_y, CV_32F, 0, 1);

    // Compute gradient magnitude and direction
    Mat grad_mag, grad_dir;
    magnitude(grad_x, grad_y, grad_mag);
    phase(grad_x, grad_y, grad_dir, true);

    // Non-maximum suppression to thin the edges
    Mat edges = Mat::zeros(img.size(), CV_8UC1);
    for (int y = 1; y < img.rows - 1; y++)
    {
        for (int x = 1; x < img.cols - 1; x++)
        {
            float direction = grad_dir.at<float>(y, x);

            float mag1, mag2;
            if ((direction >= -22.5 && direction < 22.5) || (direction >= 157.5 && direction < -157.5))
            {
                // Horizontal direction
                mag1 = grad_mag.at<float>(y, x - 1);
                mag2 = grad_mag.at<float>(y, x + 1);
            }
            else if ((direction >= 22.5 && direction < 67.5) || (direction >= -157.5 && direction < -112.5))
            {
                // Diagonal direction (bottom left to top right)
                mag1 = grad_mag.at<float>(y - 1, x + 1);
                mag2 = grad_mag.at<float>(y + 1, x - 1);
            }
            else if ((direction >= 67.5 && direction < 112.5) || (direction >= -112.5 && direction < -67.5))
            {
                // Vertical direction
                mag1 = grad_mag.at<float>(y - 1, x);
                mag2 = grad_mag.at<float>(y + 1, x);
            }
            else
            {
                // Diagonal direction (top left to bottom right)
                mag1 = grad_mag.at<float>(y - 1, x - 1);
                mag2 = grad_mag.at<float>(y + 1, x + 1);
            }

            float mag = grad_mag.at<float>(y, x);
            if (mag >= mag1 && mag >= mag2)
            {
                edges.at<uchar>(y, x) = 255;
            }
        }
    }

    // Hysteresis thresholding
    Mat canny;
    Canny(blurred, canny, 100, 200, 3, true);

    // Display results
    namedWindow("Original Image", WINDOW_NORMAL);
    namedWindow("Canny Edges", WINDOW_NORMAL);
    namedWindow("My Canny Edges", WINDOW_NORMAL);
    imshow("Original Image", img);
    imshow("Canny Edges", canny);
    imshow("My Canny Edges", edges);
    waitKey(0);
}

void detectStonesOfGoBoard() {
    Mat img = imread("Resources/go2.png", IMREAD_GRAYSCALE);
    imshow("img", img);
    vector<Vec3f> circles;
    HoughCircles(img, circles, HOUGH_GRADIENT, 2, 10.0, 80, 40, 22, 24);
    Mat canvas;
    cvtColor(img, canvas, COLOR_GRAY2BGR);
    Scalar for_black_stone(255, 0, 0);
    Scalar for_white_stone(0, 0, 255);
    for (Vec3f c : circles) {
        if (img.at<uchar>(Point(c[0], c[1])) > 128) {
            circle(canvas, Point(c[0], c[1]), c[2], for_white_stone, 2);
        }
        else {
            circle(canvas, Point(c[0], c[1]), c[2], for_black_stone, 2);
        }
    }
    imshow("circles", canvas);
    waitKey();
}

void detectTheRoundCells() {
    Mat img;

    for (int i = 0; i <= 20; ++i) {
        img = imread("Resources/hela/" + to_string(i) + ".png", IMREAD_GRAYSCALE);
        vector<Vec3f> circles;
        HoughCircles(img, circles, HOUGH_GRADIENT_ALT, 2, 10.0, 80, 0.7, 18, 33);
        Mat canvas;
        cvtColor(img, canvas, COLOR_GRAY2BGR);

        Scalar round_cells(0, 0, 255);
        for (Vec3f c : circles) {
            circle(canvas, Point(c[0], c[1]), c[2], round_cells, 2);
        }
        imshow("canvas", canvas);
        waitKey(0);
    }
    waitKey();
}
int main() {
    detectTheRoundCells();
	return 0;
}



