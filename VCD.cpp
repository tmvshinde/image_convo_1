#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>
#include <opencv2/highgui/highgui_c.h>
#include <math.h>

using namespace cv;
using namespace std;
float k = 15.0;
float dt = 0.2 , eps ;

float func_phi(float);
float func_epsi(float );

float func_phi(float v) {
    float X = v / k;
    float power = X * X / 2;
    float phi = (X * exp(-(power)));
    return phi;
}

float func_epsi(float v) {
    float psi = v / (k * sqrt(2));
    float power = psi * psi / 2;
    float epsi = (psi * exp(-(power))/sqrt(2));
    return epsi;
}
int main() {
    //Read image


    Mat image = imread("C:/Users/DELL/Pictures/Saved Pictures/test_image.png");

    cv::Mat nframe(image.rows, image.cols, CV_8UC1);
    Mat img3, newimage, Newimage1;

    if (image.empty()) {
        cout << "can not find image" << endl;
        return 0;
    }
    /*
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (i == j) {
                cout << "Pixel value of - " << i << " " << j << "== " << static_cast<int>(image.at<uchar>(i, j)) << " ";
            }
        }
        cout << endl;
    }
    */
    //image.convertTo(, CV_32FC3, 1.f / 255);
   //normalize(image, tempImage, 100, 150, CV_MINMAX);
image.convertTo(img3, CV_32FC3, 1.f / 255.0, 0);
//imwrite("New_image3.png", img3);
/*
for (int i = 0; i < img3.rows; i++) {
    for (int j = 0; j < img3.cols; j++) {
        if (i == j) {
            cout << "Pixel value of - " << i << " " << j << "== " << static_cast<float>(img3.at<float>(i, j)) << " ";
        }
    }
    cout << endl;
}*/
//  cout << "Image pixel value is this " << static_cast<float>(img3.at<float>(i, j)) << endl;

float* a = new float[img3.rows * img3.cols];

for (int i = 0; i < img3.rows; i++) {
    for (int j = 0; j < img3.cols; j++) {
        a[i * img3.cols + j] = static_cast<float>(img3.at<float>(i, j));
        //cout << a[i * img3.cols + j] << " ";
    }
    //cout << endl;
}

float* newarray = new float[img3.rows * img3.cols];

int N = 0;
while (N <= 10 || eps > 0.005) {
    eps = 0;
    for (int i = 1; i < img3.rows - 1; ++i) {
        for (int j = 1; j < img3.cols - 1; ++j) {

            float dO = func_phi(a[(i + 1) * img3.cols + j] - a[i * img3.cols + j]) - func_phi(a[i * img3.cols + j] - a[(i - 1) * img3.cols + j]);
            float d1 = func_epsi(a[(i + 1) * img3.cols + (j + 1)] - a[i * img3.cols + j]) - func_epsi(a[i * img3.cols + j] - a[(i - 1) * img3.cols + (j - 1)]);
            float d2 = func_phi(a[i * img3.cols + (j + 1)] - a[i * img3.cols + j]) - func_phi(a[i * img3.cols + j] - a[i * img3.cols + (j - 1)]);
            float d3 = func_epsi(a[(i - 1) * img3.cols + (j + 1)] - a[i * img3.cols + j]) - func_epsi(a[i * img3.cols + j] - a[(i + 1) * img3.cols + (j - 1)]);

            float dtij = dO + d1 + d2 + d3;

            newarray[i * img3.cols + j] = a[i * img3.cols + j] + k * dt * dtij;

            if (eps < abs(dtij)) {
                eps = abs(dtij);
            }
        }
    }
    a = newarray;
    N++;
    cout << "Iteration number = " << N << " eps = " << eps << endl;
}

//nframe = cv::Mat(img3.rows, img3.cols, CV_32F, &newarray);

//normalize(nframe, newimage, 0, 255, CV_MINMAX);

for (int i = 0; i < image.rows; ++i) {
    for (int j = 0; j < image.cols; ++j) {
        nframe.data[i * image.cols + j] = newarray[i * image.cols + j] * 255.0;
    }
}


/*

for (int i = 0; i < img3.rows; i++) {
    for (int j = 0; j < img3.cols; j++) {
        if (i == j) {
            cout << "Pixel value of - " << i << " " << j << "== " << static_cast<int>(nframe.at<uchar>(i, j)) << " ";
        }
    }
    cout << endl;
}
*/
// normalize(nframe, newimage, 255, 0, NORM_MINMAX, -1, noArray());
 //image.convertTo(nframe, CV_8UC1, 1.0 * 255, 1);
cout << nframe.cols;

namedWindow("images", WINDOW_NORMAL);
imshow("images", image);

namedWindow("images3", WINDOW_NORMAL);
imshow("images3", img3);


namedWindow("images4new", WINDOW_NORMAL);
imshow("images4new", nframe);
imwrite("New_image23.png", nframe);


delete[] a;
waitKey(0);
}
