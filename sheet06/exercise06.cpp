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

int main(int argc, char *argv[]) {
    if(argc<3){
        cout << "Usage: bbm6 <image-file-name1> <image-file-name2>" << endl;
        exit(0);
    }

    // create a window
    cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE); 
    cvMoveWindow("mainWin", 100, 100);

    // load an image
    IplImage *img1 = cvLoadImage(argv[1]);
    IplImage *img2 = cvLoadImage(argv[2]);

    if(!img1 || !img2) {
        cout << "Could not load image file: " << argv[1] << " or " << argv[2] << endl;
        exit(1);
    }

    //Convert to suitable format
    IplImage* img1f = cvCreateImage(cvGetSize(img1), IPL_DEPTH_32F, img1->nChannels);
    IplImage* img2f = cvCreateImage(cvGetSize(img2), IPL_DEPTH_32F, img2->nChannels);

    cvConvertScale(img1, img1f, 1.0/255.0);
    cvConvertScale(img2, img2f, 1.0/255.0);

    //Create depth map
    IplImage* depth = cvCreateImage(cvGetSize(img2), IPL_DEPTH_32F, 1);

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

    /* TODO */


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

    /* TODO */


    return 0;
}

