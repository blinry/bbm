/**
 * Vorlesung: Bildbasierte Modellierung SS 2015
 * Professor: Prof. Dr.-Ing. Marcus Magnor
 * Betreuer: Felix Klose (klose@cg.cs.tu-bs.de)
 * Datum: 10.05.2015
 * Blatt: 3
 * Abgabe: Präsentation der bearbeiteten Aufgaben in der Übung am 23.5.2015.
 * URL: http://www.cg.cs.tu-bs.de/teaching/lectures/ss15/bbm/
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cv.h>
#include <highgui.h>

#include <math.h>

using namespace cv;
using namespace std;

#define WINDOW_SIZE 10
#define PI 180

/**
 * Aufgabe: Median-Filter (10 Punkte)
 *
 * Der Median-Filter ist ein einfacher nichtlinearer Filter, der sich gut eignet, um additives Bildrauschen zu entfernen.
 *
 * - Implementiere einen Median-Filter, ohne cvSmooth zu verwenden.
 */

/* TODO */

Mat median_filter(Mat img, int windowSize){
	
	//Mat *originalImage = new Mat(img);
	Mat transformedImage(img.rows, img.cols, CV_32FC1);
	vector<float> window;
	
	for(int i = 0; i < img.cols; i++){
		for(int j = 0; j < img.rows; j++){
			// 2.1
			window.clear();
			for(int x = (-windowSize/2); x < windowSize/2; x++){
				for(int y = (-windowSize/2); y < windowSize/2; y++){
					if (i+x >= 0 && i+x < img.cols && j+y >= 0 && j+y < img.rows) {
						window.push_back( img.at<float>(Point2i(i+x,j+y)) );
					}
				}
			}
			// 2.2
			sort(window.begin(), window.end());
// 			// 2.3
			transformedImage.at<float>(Point2i(i,j)) = window[window.size()/2];
		}
	}
	// Steps:
	/* 
	 * 1 - Create a Window with size windowSize
	 * 2- Slide with the window over the original image.
	 * 	2.1 - get all the pixels in that area
	 * 	2.2 - calculate the median
	 * 	2.3 - set the median pixel as the value in the corresponding position in the transformed image
	 */
	
	/*
	 * transformedImage.at(Point pt) = median
	 */
	
	return transformedImage;
}

Mat hough_tranformation(Mat img){
	
// 	int max_d = sqrt(exp2(img.rows/2) + exp2(img.cols/2));
	int max_d = sqrt(pow(img.rows/2, 2) + pow(img.cols/2, 2));
// 	int min_d = -max_d;
	
// 	cout << img.rows << "\n";
	cout << "max d: " << max_d << "\n";
	
	Mat hough = Mat::zeros(PI, 2*max_d, CV_8UC1);
// 	Mat hough = Mat::zeros(42, 42, CV_8UC1);
	
	for(int i = 0; i < img.cols; i++){
		for(int j = 0; j < img.rows; j++){
			if(img.at<float>(Point2i(i,j)) != 0){
				for(int alpha = 0; alpha < PI; alpha++){
					float alpha_radian = alpha*M_PI/PI;
					int d = i*cos(alpha_radian) + j*sin(alpha_radian) + max_d - 1;
					cout << "d: " << d << " ; ";
					cout << "alpha: " << alpha << "\n";
					hough.at<uint>(Point2i(alpha, d))++;
				}
			}
		}
	}
	
	return hough;
// 	max_d := sqrt((1/2 * bildhöhe)^2 + (1/2 * bildbreite)^2)
// 	min_d := max_d * -1
// 	houghRaum[0...\pi][min_d...max_d] := 0
// 	foreach pixel != 0 do
// 		for \alpha := 0 to \pi do
// 		d := pixelx * cos(\alpha) + pixely * sin(\alpha)
// 		houghRaum[\alpha][d]++
// 		end
// 	end
}


int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <image>" << std::endl;
        exit(1);
    }

    // load the input image
//     IplImage *img = cvLoadImage(argv[1]);

    /**
     * - Wende den Median-Filter auf ein Graustufenbild an.
     */

	Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
// 	img.convertTo(img, CV_32FC1, 1.0/255);
// 	Mat medianImg = median_filter(img, WINDOW_SIZE);
// 	namedWindow("Median Filter");
// 	imshow("Median Filter", medianImg);
// 	waitKey(0);


    /**
     * - Wende den Median-Filter auf die einzelnen Kanäle eines Farbbilds an
     */

// 	Mat channels[3];
// 	Mat median_channels[3];
// 	img = imread(argv[1]);
// 	img.convertTo(img, CV_32FC3, 1.0/255);
// 	split(img, channels);
// 	for (int i = 0; i < 3; i++){
// 		median_channels[i] = median_filter(channels[i], WINDOW_SIZE);
// 	}
// 	namedWindow("Median Filter Colour 1");
// 	imshow("Median Filter Colour 1", median_channels[0]);
// 	waitKey(0);
// 	imshow("Median Filter Colour 1", median_channels[1]);
// 	waitKey(0);
// 	imshow("Median Filter Colour 1", median_channels[2]);
// 	waitKey(0);
// 	
// 	Mat merged_channels(img.rows, img.cols, CV_32FC3);
// 	merge(median_channels, 3, merged_channels);
// 	imshow("Median Filter Colour 1", merged_channels);
// 	waitKey(0);


    /**
     * - Wie kann man ungewollte Farbverschiebungen vermeiden?
     */

    /**
     * Aufgabe: Hough-Transformation (10 Punkte)
     *
     * Die Hough-Transformation kann für das Finden von Linien in Bildern verwendet werden.
     *
     * In dieser Aufgabe sollst du die Hough-Transformation implementieren.
     * - Lade ein Testbild und wandle es in Graustufen um.
     */

    /* TODO */
	img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	img.convertTo(img, CV_32FC1, 1.0/255);
	Mat houghImg = hough_tranformation(img);
	namedWindow("Hough Filter");
	imshow("Hough Filter", houghImg);
	waitKey(0);


    /**
     * - Erzeuge ein Gradientenbild. Verwende dazu einen Filter deiner Wahl. Begründe die Wahl des Kantendetektors.
     */

    /* TODO */


    /**
     * - Transformiere das Gradientenbild in den Hough-Raum und zeige das Bild an.
     */
//     IplImage *hough = cvCreateImage(cvSize(400, 400), IPL_DEPTH_32F, 1);

    /* TODO */

//     cvNamedWindow("Hough Space"); 
//     cvShowImage("Hough Space", hough);
//     cvWaitKey(0);

    /**
     * - Finde die markantesten Linien und zeichne diese in das Originalbild ein.
     */

    /* TODO */

//     cvNamedWindow("Hough Lines");
//     cvShowImage("Hough Lines", img);
//     cvWaitKey(0); 
}

