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

template<typename T> const T &min(const T &a, const T &b) { return a < b ? a : b; }
template<typename T> const T &max(const T &a, const T &b) { return a > b ? a : b; }

int main(int argc, char *argv[]) {
	if (argc < 3){
		printf("Usage: %s <image-file-name1> <image-file-name2>\n", argv[0]);
		exit(1);
	}

	IplImage* img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if (!img1) {
		printf("Could not load image file: %s\n", argv[1]);
		exit(1);
	}
	IplImage* img1f = cvCreateImage(cvGetSize(img1), IPL_DEPTH_32F, 1);
	cvConvertScale(img1, img1f, 1.0 / 255.0);

	IplImage* img2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
	if (!img2) {
		printf("Could not load image file: %s\n", argv[2]);
		exit(1);
	}
	IplImage* img2f = cvCreateImage(cvGetSize(img1), IPL_DEPTH_32F, 1);
	cvConvertScale(img2, img2f, 1.0 / 255.0);

	/**
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

/* TODO */


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
	 * (618, 357) &(379, 367)\\ \hline
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

/* TODO */

	
	/**
	 * - Bestimme die notwendige Bildgröße für das Panoramabild.
	 */

/* TODO */


	/**
	 * - Projiziere das linke Bild in die Bildebene des rechten Bildes. Beachte
	 *   dabei, dass auch der linke Bildrand in das Panoramabild projiziert
	 *   wird.
	 */

/* TODO */


	/**
	 * - Bilde das Panoramabild, so dass Pixel, für die zwei Werte vorhanden sind,
	 *   den Mittelwert zugeordnet bekommen.
	 */

/* TODO */


	/**
	 * - Zeige das Panoramabild an.
	 */

/* TODO */

}

