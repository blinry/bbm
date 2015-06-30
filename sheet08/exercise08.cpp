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

using namespace cv;
using namespace std;

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
 *
 *             ^
 *            /.\ <--- rechter Winkel!
 *           /   \
 *          /     \
 *         /   x   \
 *
 *
 *
 *             ^
 *             |
 *          Kamera
 *
 * - Beschreibe $z$ als Funktion von $x$ jeweils für die linke und rechte
 *   Bildhälfte. Diese Funktion wird später verwendet, um 3D-Koordinaten aus
 *   den 2D-Bildkoordinaten zu berechnen.
 *
 *
 *   Rechte Bildhälfte:
 *
 *   z(x) = -x
 *
 *   Linke Bildhälfte:
 *
 *   z(x) = x
 *
 */

pair<Point, Point> find_line(Mat img, int dx=0, int dy=0) {
    vector<Vec2f> lines;
    HoughLines(img, lines, 1, CV_PI/180, 40);

    pair<Point, Point> p;

    if (lines.size() > 0) {
        float r = lines[0][0];
        float alpha = lines[0][1];

        float y = r/cos(alpha-CV_PI/2);
        float x = -r/cos(CV_PI-alpha);

        p.first = Point(x+dx, dy);
        p.second = Point(dx, y+dy);
    }

    return p;
}

int main(int argc, char** argv) {
    if (argc < 3 or argc > 4) {
        std::cerr << "Usage: " << argv[0] << " infile.avi outfile.asc [frame_index_skip=1]" << std::endl;
        return 1;
    }

    VideoCapture cap(argv[1]);
    ofstream outfile(argv[2]);
    int frame_index_skip = argc > 3 ? atoi(argv[3]) : 1;

    /**
     * Aufgabe: Laserscanner (15 Punkte)
     *
     * Führe folgende Schritte für jeden Frame des Videos durch:
     */

    Mat frame;
    Mat bin_frame;
    int frame_index = 0;
    while (true) {
        cap >> frame;
        frame_index++;

        if (frame_index % frame_index_skip != 0) continue;
        std::cout << "processing frame " << frame_index << std::endl;

        Mat channels[3];
        split(frame, channels);

        /**
         * - Binarisiere das Bild, so dass nur die Laserlinie erhalten bleibt.
         */

        threshold(channels[0], bin_frame, 40, 255, THRESH_BINARY);

        Rect left_rect(0, 0, bin_frame.cols/2, bin_frame.rows/2);
        Rect right_rect(frame.cols/2, 0, bin_frame.cols/2, bin_frame.rows/2);
        Mat left = bin_frame(left_rect);
        Mat right = bin_frame(right_rect);

        /**
         * - Finde die Kalibrierlinie im linken Bild. Berechne die
         *   $z$-Koordinaten gemäß deiner theoretischen Überlegungen.
         */

        pair<Point, Point> left_points = find_line(left, -frame.cols/2, -frame.rows/2);

        /**
         * - Finde die Kalibrierlinie im rechten Bild. Berechne die
         *   $z$-Koordinaten gemäß deiner theoretischen Überlegungen. Beachte
         *   dabei, dass die von \code{find\_line} zurückgegebenen Koordinaten
         *   relativ zum Ursprung der rechten Bildhälfte sind.
         */

        pair<Point, Point> right_points = find_line(right, 0, -frame.rows/2);

        /**
         * - Finde eine Ebene, die durch beide Kalibrierlinien geht. Für jeden
         *   Punkt $\vec{x}$ auf der Ebene soll gelten: $\vec{n} \cdot \vec{x}
         *   = d$.
         */

        if ((left_points.first.x == 0 && left_points.second.y == 0) ||
                (right_points.first.x == 0 && right_points.second.y == 0)) {
            // we didn't find two lines
            continue;
        }

        Point3f p1(left_points.first.x, left_points.first.y, left_points.first.x);
        Point3f p2(left_points.second.x, left_points.second.y, left_points.second.x);
        Point3f p3(right_points.first.x, right_points.first.y, -right_points.first.x);

        cout << "p1 " << p1 << endl;
        cout << "p2 " << p2 << endl;
        cout << "p3 " << p3 << endl;

        Point3f n = (p1-p2).cross(p1-p3);
        n = 1/(sqrt(n.x*n.x + n.y*n.y + n.z*n.z))*n;

        cout << n << endl;

        /**
         * - Projiziere jeden hellen Punkt des Binärbildes auf die Ebene und
         *   schreibe die 3D-Koordinaten in der Form $x, y, z$ nach
         *   \code{outfile}. Da im OpenCV-Koordinatensystem $y$ nach unten hin
         *   wächst, solltest du die $y$-Koordinate invertieren, um die
         *   korrekte Darstellung im Renderer zu erreichen. Abhängig von der
         *   Wahl deines Koordinatensystems musst du möglicherweise auch $z$
         *   invertieren.
         */

        for (int x = 0; x < frame.cols; x++) {
            for (int y = 0; y < frame.rows; y++) {
                if (bin_frame.at<int>(Point(x,y)) > 0) {
                    Point3f l(0, 0, 1);
                    Point3f l0(x-frame.cols/2, y-frame.rows/2, 0);

                    int z = (p1 - l0).dot(n)/(l.dot(n));

                    Point3f p(x-frame.cols/2, y-frame.rows/2, z);
                    outfile << p.x << " " << -p.y << " " << p.z << endl;
                }
            }
        }

        /**
         * - Stelle dein Ergebnis mit Meshlab dar. Entspricht das Ergebnis
         *   deinen Erwartungen? Warum?
         */
    }

    return 0;
}

