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

/**
 * Aufgabe: Median-Filter (10 Punkte)
 *
 * Der Median-Filter ist ein einfacher nichtlinearer Filter, der sich gut eignet, um additives Bildrauschen zu entfernen.
 *
 * - Implementiere einen Median-Filter, ohne cvSmooth zu verwenden.
 */

/* TODO */


int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <image>" << std::endl;
        exit(1);
    }

    // load the input image
    IplImage *img = cvLoadImage(argv[1]);

    /**
     * - Wende den Median-Filter auf ein Graustufenbild an.
     */

    /* TODO */


    /**
     * - Wende den Median-Filter auf die einzelnen Kanäle eines Farbbilds an
     */

    /* TODO */


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


    /**
     * - Erzeuge ein Gradientenbild. Verwende dazu einen Filter deiner Wahl. Begründe die Wahl des Kantendetektors.
     */

    /* TODO */


    /**
     * - Transformiere das Gradientenbild in den Hough-Raum und zeige das Bild an.
     */
    IplImage *hough = cvCreateImage(cvSize(400, 400), IPL_DEPTH_32F, 1);

    /* TODO */

    cvNamedWindow("Hough Space"); 
    cvShowImage("Hough Space", hough);
    cvWaitKey(0);

    /**
     * - Finde die markantesten Linien und zeichne diese in das Originalbild ein.
     */

    /* TODO */

    cvNamedWindow("Hough Lines");
    cvShowImage("Hough Lines", img);
    cvWaitKey(0); 
}

