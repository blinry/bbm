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

/* TODO */


	/**
	 * - die Höhe, Breite, Anzahl der Farbkanäle dieses Bildes ausgegeben wird.
	 */

/* TODO */


	/**
	 * - dieses Bild in einem \code{cvNamedWindow} angezeigt wird, bis eine Tastatureingabe erfolgt.
	 */

/* TODO */


	/**
	 * - die drei Farbkanäle des Bildes nebeneinander angezeigt werden.
	 */

/* TODO */


	/**
	 * - das Bild zusammen mit einem roten $10 \times 10$ Rechteck um die Bildmitte angezeigt wird.
	 */

/* TODO */


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

/* TODO */


	/**
	 * - das entzerrte Bild in einer Datei gespeichert wird. 
	 */

/* TODO */


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

