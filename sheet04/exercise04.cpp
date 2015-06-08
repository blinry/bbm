/**
 * Vorlesung: Bildbasierte Modellierung SS 2015
 * Professor: Prof. Dr.-Ing. Marcus Magnor
 * Betreuer: Felix Klose (klose@cg.cs.tu-bs.de)
 * Datum: 23.05.2015
 * Blatt: 4
 * Abgabe: Präsentation der bearbeiteten Aufgaben in der Übung am 5.6.2015.
 * URL: http://www.cg.cs.tu-bs.de/teaching/lectures/ss15/bbm/
 */

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <math.h>
#include <algorithm>
#include <cv.h>
#include <highgui.h>

using namespace cv;

Point2f operator*(Mat M, const Point2f& p){
    Mat src(3, 1, CV_32FC1);

    src.at<float>(0,0)=p.x;
    src.at<float>(1,0)=p.y;
    src.at<float>(2,0)=1.0;

    Mat dst = M*src;
    return Point2f(dst.at<float>(0,0)/dst.at<float>(2,0),dst.at<float>(1,0)/dst.at<float>(2,0));
}

Mat get_translation_matrix(int dx, int dy) {
    Mat translation = Mat::zeros(3, 3, CV_32FC1);
    translation.at<float>(0, 0) = 1;
    translation.at<float>(1, 1) = 1;
    translation.at<float>(0, 2) = dx;
    translation.at<float>(1, 2) = dy;
    translation.at<float>(2, 2) = 1;
    return translation;
}

Mat translate(Mat img, int dx, int dy) {
    Mat translation = get_translation_matrix(dx, dy);
    Mat imgt(img.rows, img.cols, CV_32F);
    warpPerspective(img, imgt, translation, imgt.size());
    return imgt;
}


int main(int argc, char *argv[]) {
	if (argc < 3){
		printf("Usage: %s <image-file-name1> <image-file-name2>\n", argv[0]);
		exit(1);
	}

    Mat img1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    img1.convertTo(img1, CV_32FC1, 1.0/255);

    Mat img2 = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
    img2.convertTo(img2, CV_32FC1, 1.0/255);


    /* 4.1.1 Save trees!
     *
     * | 1 0 a |   | x |   | x + f*a |
     * | 0 1 b | * | y | = | y + f*b |
     * | 0 0 1 |   | f |   |   f   |
     *
     * 4.1.3: Four.
     *
     */

/*
	 * Aufgabe: Homographien (5 Punkte)
	 * 
	 * Unter der Annahme, dass Bilder mit einer verzerrungsfreien Lochbildkamera
	 * aufgenommen werden, kann man Aufnahmen mit verschiedenen Bildebenen und
	 * gleichem Projektionszentren durch projektive Abbildungen, sogenannte
	 * Homographien, beschreiben.
	 * 
	 * - Schreibe Translation als Homographie auf (auf Papier!).
	 * - Verschiebe die Bildebene eines Testbildes um 20 Pixel nach rechts, ohne
	 *   das Projektionszentrum zu ändern. Benutze dafür \code{cvWarpPerspective}.
	 * - Wieviele Punktkorrespondenzen benötigt man mindestens, um eine projektive
	 *   Abbildung zwischen zwei Bildern bis auf eine Skalierung eindeutig zu
	 *   bestimmen? Warum? (Schriftlich beantworten!)
	 */

    Mat img1t = translate(img1, 20, 0);

    namedWindow("Original image");
    imshow("Original image", img1);
    waitKey(0);

    namedWindow("Translated image");
    imshow("Translated image", img1t);
    waitKey(0);


	/**
	 * Aufgabe: Panorama (15 Punkte)
	 *
	 * Ziel dieser Aufgabe ist es, aus zwei gegebenen Bildern ein Panorama zu konstruieren.
	 * \begin{center}
	 * \includegraphics[width = 0.3\linewidth]{left.png}
	 * \includegraphics[width = 0.3\linewidth]{right.png}
	 * \end{center}
	 * 
	 * Dafür muss zunächst aus den gegeben Punktkorrespondenzen
	 * \begin{center}
	 * \begin{tabular}{|c|c|}
	 * \hline
	 * linkes Bild & rechtes Bild \\
	 * $(x, y)$ & $(x, y)$ \\ \hline \hline
	 * (463, 164) & (225, 179)\\ \hline
	 * (530, 357) & (294, 370)\\ \hline
	 * (618, 357) & (379, 367)\\ \hline
	 * (610, 153) & (369, 168)\\ \hline
	 * \end{tabular}
	 * \end{center}
	 * eine perspektivische Transformation bestimmt werden, mit der die Bilder auf eine gemeinsame Bildebene transformiert werden können.
	 * 
	 * - Berechne die Transformation aus den gegebenen Punktkorrespondenzen.
	 *   Benutze die Funktion \code{cvGetPerspectiveTransform}. Was ist die
	 *   zentrale Idee des DLT-Algorithmus, wie er in der Vorlesung vorgestellt
	 *   wurde?
	*/

    Point2f points2[4] = {Point2f(463, 164), Point2f(530, 357), Point2f(618, 357), Point2f(610, 153)};
    Point2f points1[4] = {Point2f(225, 179), Point2f(294, 370), Point2f(379, 367), Point2f(369, 168)};

    Mat transformation = getPerspectiveTransform(points1, points2);
    transformation.convertTo(transformation, CV_32FC1, 1);

    /**
     * - Bestimme die notwendige Bildgröße für das Panoramabild.
     */

    Point2f topleft(0, 0);
    Point2f topright(img2.cols-1, 0);
    Point2f bottomleft(0, img2.rows-1);
    Point2f bottomright(img2.cols-1, img2.rows-1);

    Point2f topleft_t = transformation*topleft;
    Point2f topright_t = transformation*topright;
    Point2f bottomleft_t = transformation*bottomleft;
    Point2f bottomright_t = transformation*bottomright;

    int xmax = max(topright_t.x, bottomright_t.x);
    int xmin = min(topleft_t.x, bottomleft_t.x);
    int ymax = max(bottomright_t.y, bottomleft_t.y);
    int ymin = min(topright_t.y, topleft_t.y);

    /**
     * - Projiziere das linke Bild in die Bildebene des rechten Bildes. Beachte
     *   dabei, dass auch der linke Bildrand in das Panoramabild projiziert
     *   wird.
     */

    Mat both1(ymax-ymin, xmax, CV_32F);
    Mat both2 = both1.clone();

    Mat translation = get_translation_matrix(0, -ymin);

    warpPerspective(img1, both1, translation, both1.size());
    warpPerspective(img2, both2, transformation*translation, both2.size());

    /**
     * - Bilde das Panoramabild, so dass Pixel, für die zwei Werte vorhanden sind,
     *   den Mittelwert zugeordnet bekommen.
     */

    Mat both = both1.clone();

    for (int x=0; x < both1.cols; x++) {
        for (int y=0; y < both1.rows; y++) {
            if (both1.at<float>(y,x) != 0) {
                if (both2.at<float>(y,x) != 0) {
                    both.at<float>(y,x) = (both1.at<float>(y,x)+both2.at<float>(y,x))/2.0;
                } else {
                    both.at<float>(y,x) = both1.at<float>(y,x);
                }
            } else {
                both.at<float>(y,x) = both2.at<float>(y,x);
            }
        }
    }

	/**
	 * - Zeige das Panoramabild an.
	 */

    namedWindow("Full image");
    imshow("Full image", both);
    waitKey(0);
}

