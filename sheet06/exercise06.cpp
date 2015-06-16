/**
 * Vorlesung: Bildbasierte Modellierung SS 2015
 * Professor: Prof. Dr.-Ing. Marcus Magnor
 * Betreuer: Felix Klose (klose@cg.cs.tu-bs.de)
 * Datum: 12.06.2015
 * Blatt: 6
 * Abgabe: Präsentation der bearbeiteten Aufgaben in der Übung am 19.06.2012.
 * URL: http://www.cg.cs.tu-bs.de/teaching/lectures/ss15/bbm/
 */

#include <ctime>
#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>

#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
    if(argc<3){
        cout << "Usage: bbm6 <image-file-name1> <image-file-name2>" << endl;
        exit(0);
    }

    Mat img1 = imread(argv[1]);
    img1.convertTo(img1, CV_32FC3, 1.0/255);
    Mat img2 = imread(argv[2]);
    img2.convertTo(img2, CV_32FC3, 1.0/255);

    Mat depth = Mat::zeros(img1.rows, img1.cols, CV_32FC1);

    /**
     * Aufgabe: Erzeugen einer Tiefenkarte (10 Punkte)
     *
     * Es soll für ein Stereo-Bildpaar eine Tiefenkarte erzeugt werden.  Die beiden
     * Bilder können als bereits rektifiziert angenommen werden.  Es ist also eine
     * Disparität für jeden Pixel zu schätzen.  Der Suchradius soll dabei
     * vorgegeben werden können.  Der Pixel in Bild 1 an der Position $\vec{x}_1$
     * ist beschrieben durch einen Deskriptor $d_1(\vec{x}_1)$.  Für jeden Pixel
     * $\vec{x}_2$ innerhalb des Suchradius muss nun der Deskriptor
     * $d_2(\vec{x}_2)$ mit $d_1$ verglichen werden.  Verwende als Deskriptor
     * zunächst einfach die Farbe des Pixels.  Zeige die erzeugte Tiefenkarte in
     * einem Fenster an.
     */

    float min_dx = +999999999999; // dirty hack
    float max_dx = -999999999999;

    for (int x = 0; x < img1.cols; x++) {
        for (int y = 0; y < img1.rows; y++) {
            float best_dx = 0;
            float best_diff = 999999999999;
            for (int dx = -50; dx <= 50; dx++) {
                if (x+dx >= 0 && x+dx < img2.cols) {
                    Vec3f d1 = img1.at<Vec3f>(y,x);
                    Vec3f d2 = img2.at<Vec3f>(y,x+dx);
                    float diff = abs(d1[0]-d2[0])+abs(d1[1]-d2[1])+abs(d1[2]-d2[2]);
                    if (diff < best_diff) {
                        best_diff = diff;
                        best_dx = dx;
                    }
                }
            }

            depth.at<float>(y,x) = best_dx;

            if (best_dx < min_dx) {
                min_dx = best_dx;
            }

            if (best_dx > max_dx) {
                max_dx = best_dx;
            }
        }
    }

    for (int x = 0; x < depth.cols; x++) {
        for (int y = 0; y < depth.rows; y++) {
            depth.at<float>(y,x) = 1-(depth.at<float>(y,x)-min_dx)/(max_dx-min_dx);
        }
    }

    namedWindow("Depth");
    imshow("Depth", depth);
    waitKey(0);


    /**
     * Aufgabe: Robustere Methoden (20 Punkte)
     *
     * In dieser Aufgabe soll die Tiefenschätzung robuster gegenüber Fehlern
     * gemacht werden.  Hier ist Eure Kreativität gefragt.  Überlegt Euch wie die
     * Disparität zuverlässiger bestimmt werden kann und implementiert Euren
     * Ansatz.  Möglich wären zum Beispiel:
     * - bessere Deskriptoren, etwa ein Fenster von mehreren Pixeln Breite
     * - Regularisierung, d.h. benachbarte Pixel sollten ähnliche Tiefenwerte
     *   haben, auch wenn dadurch die Deskriptor etwas weniger gut passen; dazu
     *   könnte man beispielsweise mit der Lösung der ersten Aufgabe beginnen und
     *   in einem zweiten Schritt die Disparitäten der Nachbarpixel in den
     *   Deskriptor mit einbauen. Das Ganze würde man dann solange wiederholen, bis
     *   sich nichts mehr ändert.
     */

    min_dx = +999999999999;
    max_dx = -999999999999;

    for (int x = 0; x < img1.cols; x++) {
        for (int y = 0; y < img1.rows; y++) {
            float best_dx = 0;
            float best_diff = 999999999999;
            for (int dx = -50; dx <= 50; dx++) {
                if (x+dx >= 0 && x+dx < img2.cols) {
                    float diff = 0;
                    int count = 0;
                    for (int rectx = -1; rectx <= 1; rectx ++) {
                        for (int recty = -1; recty <= 1; recty ++) {
                            if (x+dx+rectx >= 0 && x+dx+rectx < img2.cols && y+recty >= 0 && y+recty <= img2.rows) {
                                Vec3f d1 = img1.at<Vec3f>(y,x);
                                Vec3f d2 = img2.at<Vec3f>(y+recty,x+dx+rectx);
                                diff += abs(d1[0]-d2[0])+abs(d1[1]-d2[1])+abs(d1[2]-d2[2]);
                                count++;
                            }
                        }
                    }
                    if (diff/count < best_diff) {
                        best_diff = diff/count;
                        best_dx = dx;
                    }
                }
            }

            depth.at<float>(y,x) = best_dx;

            if (best_dx < min_dx) {
                min_dx = best_dx;
            }

            if (best_dx > max_dx) {
                max_dx = best_dx;
            }
        }
    }

    for (int x = 0; x < depth.cols; x++) {
        for (int y = 0; y < depth.rows; y++) {
            depth.at<float>(y,x) = 1-(depth.at<float>(y,x)-min_dx)/(max_dx-min_dx);
        }
    }

    namedWindow("Depth");
    imshow("Depth", depth);
    waitKey(0);

    return 0;
}

