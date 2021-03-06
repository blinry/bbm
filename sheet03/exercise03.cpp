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
#define HOUGH_SIZE 600

/**
 * Aufgabe: Median-Filter (10 Punkte)
 *
 * Der Median-Filter ist ein einfacher nichtlinearer Filter, der sich gut eignet, um additives Bildrauschen zu entfernen.
 *
 * - Implementiere einen Median-Filter, ohne cvSmooth zu verwenden.
 */

Mat median_filter(Mat img, int windowSize){
	Mat transformedImage(img.rows, img.cols, CV_32FC1);
	vector<float> window;
	
	for(int i = 0; i < img.cols; i++){
		for(int j = 0; j < img.rows; j++){
			// 2.1
			window.clear();
			for(int x = (-windowSize/2); x < windowSize/2; x++){
				for(int y = (-windowSize/2); y < windowSize/2; y++){
					if (i+x >= 0 && i+x < img.cols && j+y >= 0 && j+y < img.rows) {
						window.push_back( img.at<float>(j+y, i+x) );
					}
				}
			}
			// 2.2
			sort(window.begin(), window.end());
			// 2.3
			transformedImage.at<float>(j, i) = window[window.size()/2];
		}
	}
	
	return transformedImage;
}

/**
 * Aufgabe: Hough-Transformation (10 Punkte)
 *
 * Die Hough-Transformation kann für das Finden von Linien in Bildern verwendet werden.
 *
 * In dieser Aufgabe sollst du die Hough-Transformation implementieren.
 * - Lade ein Testbild und wandle es in Graustufen um.
 */

Mat hough_tranformation(Mat img){
    // Maximum possible distance of a pixel to the origin at (0,0):
    int max_d = sqrt(img.cols*img.cols + img.rows*img.rows);

    // Distance can also be negative:
    Mat hough(HOUGH_SIZE, 2*max_d, CV_32FC1);

    for (int x=0; x < img.rows; x++) {
        for (int y=0; y < img.cols; y++) {
            if (img.at<unsigned char>(x,y) > 0) {
                for (int a=0; a < HOUGH_SIZE; a++) {
                    float alpha = 1.0*a/HOUGH_SIZE*M_PI;
                    // Shift d by max_d to make it always positive:
                    float d = x*cos(alpha) + y*sin(alpha) + max_d;
                    hough.at<float>(a,d) += 0.01;
                }
            }
        }
    }

    return hough;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <image>" << std::endl;
        exit(1);
    }

    /**
     * - Wende den Median-Filter auf ein Graustufenbild an.
     */

	Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
 	img.convertTo(img, CV_32FC1, 1.0/255);
 	Mat medianImg = median_filter(img, WINDOW_SIZE);
 	namedWindow("Median Filter");
 	imshow("Median Filter", medianImg);
 	waitKey(0);


    /**
     * - Wende den Median-Filter auf die einzelnen Kanäle eines Farbbilds an
     */

 	Mat channels[3];
 	Mat median_channels[3];
 	img = imread(argv[1]);
 	img.convertTo(img, CV_32FC3, 1.0/255);
 	split(img, channels);

 	for (int i = 0; i < 3; i++){
 		median_channels[i] = median_filter(channels[i], WINDOW_SIZE);
 	}

 	namedWindow("Median Filter single channel");
 	imshow("Median Filter single channel", median_channels[0]);
 	waitKey(0);
 	imshow("Median Filter single channel", median_channels[1]);
 	waitKey(0);
 	imshow("Median Filter single channel", median_channels[2]);
 	waitKey(0);
 	
 	Mat merged_channels(img.rows, img.cols, CV_32FC3);
 	merge(median_channels, 3, merged_channels);
    namedWindow("Median Filter merged channels");
 	imshow("Median Filter merged channels", merged_channels);
 	waitKey(0);


    /**
     * - Wie kann man ungewollte Farbverschiebungen vermeiden?
     */

    // Für die drei Channels jeweils den gleichen Pixel nehmen.

    /**
     * - Erzeuge ein Gradientenbild. Verwende dazu einen Filter deiner Wahl. Begründe die Wahl des Kantendetektors.
     */

    img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    Mat edge = img.clone();
    Canny(img, edge, 200, 500);

    namedWindow("Hough Filter");
    imshow("Hough Filter", edge);
    waitKey(0);

    /**
     * - Transformiere das Gradientenbild in den Hough-Raum und zeige das Bild an.
     */

    Mat houghImg = hough_tranformation(edge);

    namedWindow("Hough Filter");
    imshow("Hough Filter", houghImg);
    waitKey(0);

    Mat filteredHough;
    threshold(houghImg, filteredHough, 1.0, 1, THRESH_BINARY);

    namedWindow("Hough Filter");
    imshow("Hough Filter", filteredHough);
    waitKey(0);


    /**
     * - Finde die markantesten Linien und zeichne diese in das Originalbild ein.
     */

    img = imread(argv[1]);
    int max_d = sqrt(img.rows*img.rows + img.cols*img.cols);

    for(int d = 0; d < filteredHough.cols; d++){
        for(int a = 0; a < filteredHough.rows; a++){
            if (filteredHough.at<float>(a,d) == 1) {
                float alpha = 1.0*a/HOUGH_SIZE*M_PI;

                // Shift the d back
                int real_d = d - max_d;

                int x = 1.0*real_d/cos(M_PI/2-alpha);
                int y = 0;
                int x2 = 0;
                int y2 = 1.0*real_d/cos(alpha);

                line(img, Point(x2,y2), Point(x,y), Scalar(0, 0, 255));
            }
        }
    }

    namedWindow("Hough Filter");
    imshow("Hough Filter", img);
    waitKey(0);
}

