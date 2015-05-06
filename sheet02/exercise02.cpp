/**
 * Vorlesung: Bildbasierte Modellierung SS 2015
 * Professor: Prof. Dr.-Ing. Marcus Magnor
 * Betreuer: Felix Klose (klose@cg.cs.tu-bs.de)
 * Datum: 1.05.2015
 * Blatt: 2
 * Abgabe: Präsentation der bearbeiteten Aufgaben in der Übung am 8.5.2015.
 * URL: http://www.cg.cs.tu-bs.de/teaching/lectures/ss15/bbm/
 */

#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
    /**
     * Aufgabe: 2D-Operationen auf Bildern (5 Punkte)
     *
     * In der Vorlesung wurde gezeigt, das große Bereiche natürlicher Bilder
     * homogen sind, die meisten Informationen über ein Bild jedoch in den
     * Kanten und Diskontinuitäten zu finden sind.  In disem Übungsblatt soll
     * es darum gehen, Kanten und herausragende Punkte in Bildern zu finden.
     * 
     * Häufige Grundlage der Verarbeitung von Bildern ist das Anwenden von
     * Filtern.  Es entspricht der Faltung (engl. \emph{convolution}) des
     * Bildes mit einem Filterkern.  Filterkerne werden zu sehr verschiedenen
     * Zwecken eingesetzt.
     * 
     * - Skizziere (auf Papier) eine eindimensionale Gaußfunktion mit
     *   Mittelwert $\mu$ und Varianz $\sigma^2$.  Was ist die
     *   Fourier-Transformation einer Gaußfunktion?
     * - Lade ein Testbild und wandle es in ein Grauwertbild mit
     *   float-Werten in $[0, 1]$ um.
     */

    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <image>" << std::endl;
        exit(1);
    }

    Mat image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
    image.convertTo(image, CV_32FC1, 1.0/255);

    namedWindow("Grayscale");
    imshow("Grayscale", image);
    waitKey(0);

    /**
     * - Falte ein verrauschtes Testbild mit Gaußfunktionen verschiedener
     *   Varianzen. Was passiert? Welchen Einfluss hat die Kernelgröße?
     */

    Mat lgauss;
    Mat sgauss;

    GaussianBlur(image, lgauss, Size(0,0), 10);
    GaussianBlur(image, sgauss, Size(0,0), 3);

    namedWindow("Large Gaussian");
    imshow("Large Gaussian", lgauss);
    waitKey(0);

    namedWindow("Small Gaussian");
    imshow("Small Gaussian", sgauss);
    waitKey(0);

    /**
     * - Betrachte die Differenzen zweier gaußgefilterter Bilder (evt.
     *   skalieren).
     */

    Mat dog;
    absdiff(lgauss, sgauss, dog);

    namedWindow("Difference of Gaussians");
    imshow("Difference of Gaussians", dog);
    waitKey(0);

    /**
     * Aufgabe: Diskrete Ableitungen (5 Punkte)
     *
     * Mathematisch sind Ableitungen nur für stetige Funktionen definiert.  Da
     * ein Bild nur pixelweise, d.h. an diskreten Stellen, Informationen
     * liefert, kann man Ableitungen von Bildern nicht direkt bestimmen.  Eine
     * naheliegene Approximation der Ableitung ist der Differenzenquotient.
     * Sei $f:\Omega \subset \mathbb{R} \to \mathbb{R}$ eine Funktion.  Dann
     * ist der Differenzenquotient $D_h(x) = \frac{f(x+h) - f(x)}{h}$ eine
     * Approximation an $f'(x)$ für hinreichend kleines h.  Für
     * differenzierbare Funktionen liefert allerdings die zentrale Differenz
     * \begin{equation}
     * D(x) = \frac{f(x+h) - f(x-h)}{2h}
     * \end{equation}
     * eine deutlich bessere Approximation (siehe auch \emph{Numerical Recipes
     * in C} von Press et al., Chapter 5).
     *
     * - Bestimme einen diskreten Faltungskern, der die zentrale Differenz
     *   approximiert.
     */

    Mat kernelx = Mat::zeros(1, 3, CV_32F);
    kernelx.at<float>(0,0) = -1;
    kernelx.at<float>(0,2) = 1;

    Mat kernely = Mat::zeros(3, 1, CV_32F);
    kernely.at<float>(0,0) = -1;
    kernely.at<float>(2,0) = 1;


    /**
     * - Implementiere diskretes Differenzieren als Faltung mit diesem Kern und
     *   wende es auf ein glattes Testbild an.  Was passiert, wenn du ein
     *   verrauschtes Testbild verwendest?
     */

    Mat dx = image.clone();
    Mat dy = image.clone();

    filter2D(image, dx, -1, kernelx);
    filter2D(image, dy, -1, kernely);

    namedWindow("DX");
    imshow("DX", dx);
    waitKey(0);

    namedWindow("DY");
    imshow("DY", dy);
    waitKey(0);

    /**
     * - Verwende in der Implementierung nun Faltung mit dem Sobel-Operator
     *   (\code{cvSobel}) und beobachte die Ergebnisse auf dem verrauschten
     *   Testbild.
     */

    Mat sobel;
    Sobel(image, sobel, -1, 1, 1);

    namedWindow("sobel");
    imshow("sobel", sobel);
    waitKey(0);

    /**
     * Aufgabe: Features (10 Punkte)
     *
     * Kanten in Bildern werden häufig als Intensitätssprünge beschrieben.
     *
     * - Berechne den Betrag des Gradienten eines Testbildes und bestimme
     *   Schwellwerte des Gradienten, um möglichst alle Kanten zu entdecken
     *   oder möglichst nur Kanten zu entdecken.
     */

    multiply(dx, dx, dx);
    multiply(dy, dy, dy);

    Mat gradient = dx + dy;

    imshow("Gradient", gradient);
    waitKey(0);

    Mat edge1 = image.clone();
    Mat edge2 = image.clone();
    threshold(gradient, edge1, 0.005, 1, THRESH_BINARY);
    threshold(gradient, edge2, 0.1, 1, THRESH_BINARY);

    imshow("All edges", edge1);
    waitKey(0);
    imshow("Only edges", edge2);
    waitKey(0);

    /*
     * - Vergleiche mit dem Ergebnis des Canny-Kantendetektors
     *   (\code{cvCanny}), wenn er mit diesen Parametern aufgerufen wird.
     */

    Mat image_8u;
    image.convertTo(image_8u, CV_8UC1, 255);
    Mat edge = image_8u.clone();
    Canny(image_8u, edge, 100, 200);

    imshow("Canny", edge);
    waitKey(0);

    /**
     * Einzelne herausragende Punkte werden auch als Featurepunkte oder Ecken
     * bezeichnet, selbst wenn sie nicht auf einer Kante liegen.
     *
     * - Implementiere die Harris-Corner Detektion.  Verwende dabei nicht die
     *   OpenCV Methode cvCornerHarris, sondern implementiere selbst eine
     *   Funktion, die ein Grauwertbild, einen Parameter $k$ für die Berechnung
     *   des Featureindikators und einen Schwellwert für ausreichend großen
     *   Ausschlag des Indikators entgegennimmt und die Featurepunkte
     *   zurückgibt.
     */

    vector<Point> harris_pts;

    /* TODO */


    /**
     * - Zeichne ein $3\times3$ Rechteck um jede gefundene Harris-Corner.
     */

    /* TODO */

    //cvShowImage("Harris Corners", image);
    //cvWaitKey(0);
}

