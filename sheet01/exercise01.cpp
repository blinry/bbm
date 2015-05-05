/**
 * Vorlesung: Bildbasierte Modellierung SS 2012
 * Professor: Prof. Dr.-Ing. Marcus Magnor
 * Betreuer: Felix Klose (klose@cg.cs.tu-bs.de)
 * Datum: 17.04.2012
 * Blatt: 1
 * Abgabe: Präsentation der bearbeiteten Aufgaben in der Übung am 24.04.2012.
 * URL: http://www.cg.cs.tu-bs.de/teaching/lectures/ss15/bbm/
 */

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

template<typename T> const T &min(const T &a, const T &b) { return a < b ? a : b; }
template<typename T> const T &max(const T &a, const T &b) { return a > b ? a : b; }

/**
 * Ziel der ersten Übung ist das Vertrautwerden mit der OpenCV-Bibliothek. 
 * Diese stellt nicht nur die Grundlagen zum Arbeiten mit Bildern zur Verfügung, 
 * sondern auch viele weitere in der Bilderverarbeitung häufig verwendete Funktionen.
 * Erhältlich ist sie als Open-Source-Projekt unter:
 * \begin{center}\url{http://opencv.orb/}\end{center}
 * Dokumentation findet sich beispielsweise im Buch \emph{Learning OpenCV} von Gary Bradski und Adrian Kaehler oder unter:
 * \begin{center}\url{http://http://docs.opencv.org/modules/refman.html}\end{center}
 */

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: main <image-file-name> <output-file-name> <kappa1> <kappa2>\n\7");
        exit(1);
    }

    /**
     * Aufgabe: OpenCV starten (10 Punkte)
     * 
     * Erweitere die gegebene Programmgrundstruktur so, dass
     * - ein Bild geladen werden kann.
     */

    Mat img = imread(argv[1]);


    /**
     * - die Höhe, Breite, Anzahl der Farbkanäle dieses Bildes ausgegeben wird.
     */

    cout << "Width: " << img.cols << " Height: " << img.rows << " Channels: " << img.channels() << endl;


    /**
     * - dieses Bild in einem \code{cvNamedWindow} angezeigt wird, bis eine Tastatureingabe erfolgt.
     */

    namedWindow("Ahwsum windoh");
    imshow("Ahwsum windoh", img);
    waitKey(0);


    /**
     * - die drei Farbkanäle des Bildes nebeneinander angezeigt werden.
     */

    // Create a Mat of the same size, filled with zeroes
    Mat empty = Mat::zeros(img.rows, img.cols, CV_8UC1);

    // Split the color channels
    Mat channels[3];
    split(img, channels);

    Mat channels_side_by_side;

    // For each channel i...
    for (int i=0; i<3; i++) {
        // Create three channels (two of which are zero)...
        vector<Mat> final_channels;
        for (int j=0; j<3; j++) {
            if (i != j) {
                final_channels.push_back(empty);
            } else {
                final_channels.push_back(channels[j]);
            }
        }

        // ...merge them...
        Mat merged;
        merge(final_channels, merged);

        // ...and append all the results horizontally.
        if (i == 0) {
            channels_side_by_side = merged;
        } else {
            hconcat(channels_side_by_side, merged, channels_side_by_side);
        }

    }
    imshow("Ahwsum windoh", channels_side_by_side);
    waitKey(0);


    /**
     * - das Bild zusammen mit einem roten $10 \times 10$ Rechteck um die Bildmitte angezeigt wird.
     */

    Mat img_copy = img.clone();
    Point2f center = Point2f(img.cols/2, img.rows/2);
    Point2f shift(5,5);
    Rect rect(center-shift, center+shift);
    rectangle(img_copy, rect, Scalar(0, 0, 255));
    imshow("Ahwsum windoh", img_copy);
    waitKey(0);


    /**
     * Aufgabe: Bilder entzerren (10 Punkte)
     *
     * Das Bild \code{distorted.png}, Abb~\ref{fig:distorted}, wurde mit einer
     * Weitwinkelkamera aufgenommen und zeigt starke radiale Verzerrung.  Aus der
     * Vorlesung bekannt ist, dass die radiale Verzerrung oft durch
     * $$ x = x_c + L(r) (x_d-x_c) \quad y = y_c + L(r) (y_d-y_c) $$
     * ausgedrückt wird, wo $(x, y)$ die idealen Koordinaten sind, $({x_d}, {y_d})$
     * die verzerrten Koordinaten und $L({r})$ eine Funktion, die nur von der
     * Entfernung ${r}=\sqrt{(x-x_c)^2 + ({y}-y_c)^2}$ zum Verzerrungszentrum
     * $(x_c,y_c)$ abhängt.
     * Die Funktion $L(r)$ kann durch ihre Taylorentwicklung $L(r) = 1+ \kappa_1 r
     * + \kappa_2 r^2 + \kappa_3 r^3 + \cdots$ beschrieben werden.
     * Verschiedene Möglichkeiten, die Parameter zu bestimmen, sind denkbar und
     * werden beispielsweise in \emph{Multiple View Geometry} von Hartley und
     * Zisserman beschrieben, sollen hier aber nicht zur Anwendung kommen.
     *
     * Erweitere die gegebene Programmgrundstruktur so, dass 
     * - die Funktion $L$ mit Taylorentwicklung 2. Ordnung approximiert wird,
     *   wobei das Verzerrungszentrum der Bildmitte entspricht.
     */

    Mat straightened(img.rows, img.cols, img.type());
    for (int x=0; x<img.cols; x++) {
        for (int y=0; y<img.rows; y++) {
            float r = sqrt(pow(x - center.x, 2)+pow(y - center.y, 2));
            float L = 1 + atof(argv[3])*r + atof(argv[4])*r*r;
            int x2 = center.x + (x - center.x)/L;
            int y2 = center.y + (y - center.y)/L;
            if (x2 >= 0 && y2 >= 0 && x2 < img.cols && y2 < img.rows) {
                straightened.at<Vec3b>(Point2i(x,y)) = img.at<Vec3b>(Point2i(x2, y2));
            }
        }
    }


    /**
     * - das entzerrte Bild in einer Datei gespeichert wird. 
     */

    imshow("Ahwsum windoh", straightened);
    imwrite(argv[2], straightened);
    waitKey(0);


    /**
     * Was passiert, wenn die Größe der Parameter, ihr Vorzeichen etc. verändert wird?
     * Ein Startwert kann z.B. $\kappa_1 = 0.001$, $\kappa_2 = 0.000005$ sein.
     *
     * \begin{figure}[hb]
     * \centering
     * \includegraphics[width=0.3\linewidth]{distorted}
     * \caption{Bild mit starker radialer Verzerrung.}
     * \label{fig:distorted}
     * \end{figure}
     */
}

