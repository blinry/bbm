/**
 * Vorlesung: Bildbasierte Modellierung SS 2015
 * Professor: Prof. Dr.-Ing. Marcus Magnor
 * Betreuer: Felix Klose (klose@cg.cs.tu-bs.de)
 * Datum: 29.06.2015
 * Blatt: 8
 * Abgabe: Präsentation der bearbeiteten Aufgaben in der Übung am 03.07.2015.
 * URL: http://www.cg.cs.tu-bs.de/teaching/lectures/ss15/bbm/
 */

/**
 * In dieser Aufgabe soll ein Algorithmus implementiert werden, um aus
 * Videodaten eines einfachen Laser\-scanners 3D-Punktwolken zu erzeugen. Der
 * Scanner besteht aus einem Linienlaser, wie sie beispielsweise an
 * Scannerkassen Verwendung finden, einer Kamera und zwei Hintergrundebenen.
 * Der Laser überstreicht das Objekt, das vor den in einem Winkel zueinander
 * stehenden Hintergrundebenen platziert ist. Objekt und Ebenen werden dabei
 * von der Kamera aufgenommen. Die Geometrie der Ebenen wird als bekannt
 * vorausgesetzt; sie kann beispielsweise durch ein optisches
 * Kalibrierverfahren bestimmt werden. Das Licht des Lasers bildet zu jedem
 * Zeitpunkt eine weitere Ebene, die die beiden Hintergrundebenen in jeweils
 * einer Linie schneidet. In Verbindung mit der bekannten Geometrie der Ebenen
 * kann aus diesen Linien die durch den Laser aufgespannte Ebene im
 * Koordinatensystem der Kamera bestimmt werden. Die Koordinaten der Punkte, an
 * denen der Laser das Objekt trifft, können auf diese Ebene projiziert werden,
 * um 3D-Koordinaten zu erhalten. Wird nun der Laser über das Objekt bewegt,
 * kann aus den Einzelbildern jeweils ein Linienprofil des Objekts
 * rekonstruiert werden; zusammen ergeben diese Profile eine 3D-Punktwolke des
 * Objekts.
 *
 * Ein Laserscanner, der nach diesem Prinzip funktioniert, wurde am Institut
 * für Robotik und Prozessinformatik der TU Braunschweig entwickelt. Aufbau und
 * Funktionsweise sind auf \url{http://www.david-laserscanner.com} erklärt.
 * Dort findet sich auch eine Beispielaufnahme, die wir im Folgenden verwenden
 * werden.
 *
 * Unser Programm zur 3D-Rekonstruktion erhält als ersten Parameter den Namen
 * des Eingabevideos, als zweiten den der Ausgabedatei (jede Zeile ein
 * 3D-Punkt, Koordinaten durch Komma getrennt, Endung \code{.asc}). Als dritter
 * Parameter kann optional eine Ganzzahl angegeben werden, die bestimmt, jeder
 * wievielte Frame des Videos rekonstruiert wird; dadurch lässt sich die
 * Laufzeit beliebig auf Kosten der Auflösung reduzieren. Die entstehenden
 * Punktwolken können mit Meshlab (\url{http://meshlab.sourceforge.net/})
 * gerendert werden.  Zur Darstellung muss dort unter \emph{Render}
 * $\rightarrow$ \emph{Render Mode} ``Points'' gewählt werden.
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <opencv/cv.h>
#include <opencv/highgui.h>

std::ostream &operator<<(std::ostream &o, const CvPoint3D32f &p) {
	o << "(" << p.x << ", " << p.y << ", " << p.z << ")";
	return o;
}

float dot(const CvPoint3D32f &a, const CvPoint3D32f &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

CvPoint3D32f cross(const CvPoint3D32f &a, const CvPoint3D32f &b) {
	return cvPoint3D32f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

float length(const CvPoint3D32f &p) {
	return sqrt(dot(p, p));
}

CvPoint3D32f normalized(const CvPoint3D32f &p) {
	float l = length(p);
	return cvPoint3D32f(p.x / l, p.y / l, p.z / l);
}

CvPoint3D32f operator+(const CvPoint3D32f &a, const CvPoint3D32f &b) {
	return cvPoint3D32f(a.x + b.x, a.y + b.y, a.z + b.z);
}

CvPoint3D32f operator-(const CvPoint3D32f &a, const CvPoint3D32f &b) {
	return cvPoint3D32f(a.x - b.x, a.y - b.y, a.z - b.z);
}

/**
 * Aufgabe: Vorüberlegungen (10 Punkte)
 *
 * Um für jeden Punkt eine Tiefe berechnen zu können, muss aus den beiden
 * Linien, an denen die Laserebene die Hintergrundebenen schneidet, die
 * Laserebene im Kamerakoordinatensystem rekonstruiert werden. Vereinfachend
 * können wir annehmen, dass die Kamera sehr weit vom Objekt entfernt ist, die
 * Projektion ist also näherungsweise orthographisch. Anstelle einer
 * automatischen Kalibrierung der Hintergrundebenen soll angenommen werden,
 * dass diese senkrecht sowie symmetrisch um die horizontale Bildmitte
 * angeordnet sind und sich dort orthogonal treffen.
 *
 * - Skizziere eine Draufsicht der beiden Hintergrundebenen.
 *
 * - Beschreibe $z$ als Funktion von $x$ jeweils für die linke und rechte
 *   Bildhälfte. Diese Funktion wird später verwendet, um 3D-Koordinaten aus
 *   den 2D-Bildkoordinaten zu berechnen.
 */

std::pair<CvPoint, CvPoint> find_line(IplImage *bimg) {
	/**
	 * Aufgabe: Kalibrierlinien (5 Punkte)
	 *
	 * Zunächst müssen im Eingabebild die beiden Kalibrierlinien gefunden
	 * werden. Dafür wird das Bild binarisiert und in den linken und rechten
	 * Teil separiert. Finde in einem solchen Binärbild die dominanteste Linie
	 * und gib zwei Punkte zurück, die auf dieser Linie liegen.
	 */

/* TODO */

}

int main(int argc, char** argv) {
	if (argc < 3 or argc > 4) {
		std::cerr << "Usage: " << argv[0] << " infile.avi outfile.asc [frame_index_skip=1]" << std::endl;
		return 1;
	}

	CvCapture *capture = cvCaptureFromFile(argv[1]);
	std::ofstream outfile(argv[2]);
	int frame_index_skip = argc > 3 ? atoi(argv[3]) : 1;

	/**
	 * Aufgabe: Laserscanner (15 Punkte)
	 *
	 * Führe folgende Schritte für jeden Frame des Videos durch:
	 */
	for (int frame_index = 0; cvGrabFrame(capture); ++frame_index) {
		if (frame_index % frame_index_skip != 0) continue;
		std::cout << "processing frame " << frame_index << std::endl;
		IplImage *img = cvRetrieveFrame(capture);
		IplImage *gimg = cvCreateImage(cvGetSize(img), 8, 1);
		cvSplit(img, gimg, NULL, NULL, NULL);

		/**
		 * - Binarisiere das Bild, so dass nur die Laserlinie erhalten bleibt.
		 */

/* TODO */


		/**
		 * - Finde die Kalibrierlinie im linken Bild. Berechne die
		 *   $z$-Koordinaten gemäß deiner theoretischen Überlegungen.
		 */

/* TODO */


		/**
		 * - Finde die Kalibrierlinie im rechten Bild. Berechne die
		 *   $z$-Koordinaten gemäß deiner theoretischen Überlegungen. Beachte
		 *   dabei, dass die von \code{find\_line} zurückgegebenen Koordinaten
		 *   relativ zum Ursprung der rechten Bildhälfte sind.
		 */

/* TODO */


		/**
		 * - Finde eine Ebene, die durch beide Kalibrierlinien geht. Für jeden
		 *   Punkt $\vec{x}$ auf der Ebene soll gelten: $\vec{n} \cdot \vec{x}
		 *   = d$.
		 */

/* TODO */


		/**
		 * - Projiziere jeden hellen Punkt des Binärbildes auf die Ebene und
		 *   schreibe die 3D-Koordinaten in der Form $x, y, z$ nach
		 *   \code{outfile}. Da im OpenCV-Koordinatensystem $y$ nach unten hin
		 *   wächst, solltest du die $y$-Koordinate invertieren, um die
		 *   korrekte Darstellung im Renderer zu erreichen. Abhängig von der
		 *   Wahl deines Koordinatensystems musst du möglicherweise auch $z$
		 *   invertieren.
		 */

/* TODO */


		cvReleaseImage(&bimg);
		cvReleaseImage(&gimg);

		/**
		 * - Stelle dein Ergebnis mit Meshlab dar. Entspricht das Ergebnis
		 *   deinen Erwartungen? Warum?
		 */
	}

	cvReleaseCapture(&capture);

	return 0;
}

