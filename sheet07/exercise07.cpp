/**
 * Vorlesung: Bildbasierte Modellierung SS 2015
 * Professor: Prof. Dr.-Ing. Marcus Magnor
 * Betreuer: Felix Klose (klose@cg.cs.tu-bs.de)
 * Datum: 19.06.2015
 * Blatt: 7
 * Abgabe: Präsentation der bearbeiteten Aufgaben in der Übung am 29.06.2015.
 * URL: http://www.cg.cs.tu-bs.de/teaching/lectures/ss15/bbm/
 */

/**
 * Das Ziel dieses Übungsblatts ist die Implementierung des
 * \emph{Space-Carving}-Algorithmus, um aus Fotografien eines Objekts eine
 * Volumendarstellung zu erhalten. Der Algorithmus wurde ursprünglich von
 * Kutulakos und Seitz in \emph{A Theory of Shape by Space Carving}
 * veröffentlicht.
 * 
 * Zwei Beispielszenen, \emph{scene1} und \emph{scene2}, sind auf der Webseite
 * als Archive verfügbar und müssen zunächst entpackt werden. Das Programm kann
 * mit dem Aufruf \code{./exercise07 <scenename> <color\_threshold>
 * <grid\_resolution>} gestartet werden, wobei \code{<scenename>} der Name der
 * Szene, also \emph{scene1} oder \emph{scene2}, \code{<color\_threshold>} ein
 * Schwellwert zwischen $0$ und $255$ und \code{<grid\_resolution>} die
 * Auflösung des Voxelgitters ist.  Der Befehl \code{./src/BBM scene1 100 50}
 * lädt also alle benötigten Daten aus dem Ordner \emph{scene1}, benutzt einen
 * Farb-Schwellwert von $100$ und arbeitet auf einem $50\times50\times50$
 * Voxelgitter.  Zu Anfang sollte das Programm einen leeren Drahtwürfel und
 * kleine blaue Würfel an den Kamerapositionen anzeigen. Der Sichtpunkt kann
 * mit der Maus verändert werden.
 * 
 * Die Voxelmittelpunkte werden immer in einen Einheitswürfel von $(-0.5, -0.5,
 * -0.5)$ bis zu $(0.5, 0.5, 0.5)$ gesetzt, d.h. der Mittelpunkt von Voxel
 *  $(0,0,0)$ wird an die Stelle $(-0.5, -0.5, -0.5)$, der Mittelpunkt von
 *  Voxel $(\code{grid\_resolution}-1, \code{grid\_resolution}-1,
 *  \code{grid\_resolution}-1)$ an die Stelle $(0.5, 0.5, 0.5)$ gesetzt.
 * 
 * Die Datei \code{algebra.h} enthält eine Reihe von hilfreichen Klassen
 * (\code{vec3}, \code{vec4}, \code{mat3}, \code{mat4}), die sehr intuitiv in
 * der Benutzung sind. So kann ein \code{vec4 p} mit einer $4\times4$ Matrix
 * \code{mat4 m} multiplziert werden, indem \code{vec4 mp = m * p} aufgerufen
 * wird.
 * 
 * \begin{center}
 * \includegraphics[width=0.3\linewidth]{imgConsistent.png}
 * \includegraphics[width=0.3\linewidth]{silConsistent.png}
 * \includegraphics[width=0.3\linewidth]{SpaceCarving.png}
 * \end{center}
 *
 * Von allen möglicherweise sichtbaren Voxeln im Gitter (linkes Bild) gehören
 * nur Voxel zum Objekt, die nicht auf weißen Bildhintergrund projiziert werden
 * (mittleres Bild).  Vergleicht man Farbwerte verschiedener Kameras, lässt
 * sich 3D-Struktur rekonstruieren (rechtes Bild).
 */

#include <iostream>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <set>

#include "algebra.h"
#include <cv.h>
#include <highgui.h>

#if defined(__APPLE_CC__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

struct voxel{
	alg::vec3 pos, col;

	bool operator <(const voxel& v) const { 
		for(int i=0; i<3; ++i){
			if(pos[i] < v.pos[i]) return true;
			else if(pos[i] > v.pos[i]) return false; }    
		return false;
	};
};

typedef std::pair<int, int> pixPos;

bool isImgConsistent(
		const std::vector<pixPos>& pixPositions,
		const int numInput,
		const std::vector<int>& imgWidth,
		const std::vector<int>& imgHeight,
		int& imgRemoval
		);
bool isSilhouetteConsistent(
		const std::vector<pixPos>& pixPositions,
		const int numInput,
		const std::vector<IplImage*>& imgs,
		int& silRemoval
		);
bool isColourConsistent_plane(
		const alg::vec4& voxPosition, 
		const std::vector<pixPos>& pixPositions,
		const std::vector<alg::vec3>& camPos,
		alg::vec3& outputColour, 
		const int axis,
		int& colRemoval,
		const int numInput,
		const std::vector<IplImage*>& imgs,
		const int diffThresh,
		const float voxsize
		);

/**
 * Aufgabe: Bildkonsistenz (5 Punkte)
 * 
 * \label{aufg:ImgConsistent} Iteriere über alle Voxel, indem du zuerst alle
 * Voxel einer Ebene mit gleicher z-Koordinate betrachtest und dann die Ebene
 * entlang der positiven z-Achse verschiebst.  Benutze \code{textureMatrices},
 * um die Voxelmittelpunkte in die einzelnen Bilder zu projizieren.  Ist die
 * berechnete Position innerhalb der Bildes, behalte sie, ansonsten wird dieser
 * Voxel aus dem Gitter entfernt. Dies führt zu einer Menge von Voxeln, die
 * möglicherweise (abhängig vom Objekt in der Szene) in allen Bildern sichtbar
 * sind. Setzt man alle Voxel, die diese Bedingung erfüllen schwarz, so sollte
 * das Ergebnis aussehen wie im linken Bild.
 */
void applySpaceCarving(
		const int numInput, // number of input images
		const std::vector<int>& imgWidth, // imgWidth[i] is the width of the i-th image
		const std::vector<int>& imgHeight, // imgHeight[i] is the height of the i-th image
		const std::vector<IplImage*>& imgs, // the collection of input images (unsigned char!)
		const int gridRes, // resolution of the grid
		const std::vector<alg::mat4> textureMatrices, // matrices projecting from world coordinates into image i:
		// alg::vec4 tpos = textureMatrices.at(i) * pos;
		// alg::vec2 imgPos(tpos[0] / tpos[3], tpos[1] / tpos[3]);
		// int xImg = round(imgPos[0]);
		// int yImg = round(imgPos[1]);
		const std::vector<alg::vec3> campos, // camera position in world coordinates for every camera
		const int diffThresh, // allowed difference in color for a voxel to be color consistent.
		std::vector<voxel>& result // consistent voxels, color scaled to [0,1]
		) {
	int imgRemoval = 0;
	int silRemoval = 0;
	int colRemoval = 0;

	std::set<voxel> consistentVoxels;


	// the midpoints range from [-0.5, -0.5, -0.5] to [0.5, 0.5, 0.5]
	float step = 1.0f / (gridRes-1);

	std::vector<pixPos> pixPositions;

	pixPositions.resize(numInput);


/* TODO */


	// remove doubled elements //
	for(std::set<voxel>::iterator iter = consistentVoxels.begin();
			iter != consistentVoxels.end(); ++iter){
		result.push_back(*iter);
	}

	std::cout << "\n\nTotal amount of consistent voxels: " << result.size() << std::endl << std::endl;
}

bool isImgConsistent(
		const std::vector<pixPos>& pixPositions,
		const int numInput,
		const std::vector<int>& imgWidth,
		const std::vector<int>& imgHeight,
		int& imgRemoval
		) {
	assert(pixPositions.size() == static_cast<unsigned int>(numInput));


/* TODO */

	return true;
}

/**
 * Aufgabe: Silhouettenkonsistenz (5 Punkte)
 * 
 * Es wird eine weitere Bedingung hinzugefügt: alle gegebenen Bilder haben
 * einen weißen Hintergrund. Wenn also ein Voxel in irgendeinem der Bilder auf
 * einen weißen Pixel projiziert wird, wird er aus dem Voxelgitter entfernt.
 * Nimmt man die Bedingungen aus Aufgabe \ref{aufg:ImgConsistent} hinzu, sollte
 * das Ergebnis aussehen wie im mittleren Bild.
 */
bool isSilhouetteConsistent(
		const std::vector<pixPos>& pixPositions,
		const int numInput,
		const std::vector<IplImage*>& imgs,
		int& silRemoval
		) {
	assert(pixPositions.size() == static_cast<unsigned int>(numInput));


/* TODO */


	return true;
}

/**
 * Aufgabe: Space Carving (10 Punkte)
 * 
 * - Erweitere das bisher entstandene Framework um eine weitere Bedingung:
 *   vergleiche die Farbe eines Voxels in allen Eingabebildern. Nur wenn sich
 *   die Farben paarweise um weniger als den Eingabeparameter
 *   \code{<color\_threshold>} (entspricht \code{diffThresh}) pro Farbkanal
 *   unterscheiden werden sie im Gitter belassen. Weise diesen Voxeln die
 *   mittlere Farbe aller Pixelfarben zu. Die 3D-Szene wird mit OpenGL
 *   gerendert.  Farbwerte liegen dort im Bereich $[0,1]$, sie müssen also
 *   entsprechend umgewandelt werden.
 * - Aus numerischen Gründen vergleiche nun nicht die Farbwerte alle
 *   Eingabebilder, sondern nur der $3$ Bilder, die am nächsten am Mittelwert
 *   liegen. Weise dem Voxel den Mittelwert dieser 3 Farbwerte zu.  Eine
 *   \code{priority\_queue} und \code{pair} können die Arbeit hier erleichtern.
 * - Erweitere das Programm so, dass es mehrere Durchläufe (\emph{sweeps} )
 *   benutzt: Betrachte nicht nur eine Ebene, die sich entlang der positiven
 *   z-Achse bewegt, sondern auch Ebenen entlang der positiven $x$- und
 *   $y$-Achse und entlang der negativen Achsen.
 * - Ziehe nun zum Farbvergleich nur Kameras hinzu, die sich in Bezug auf die
 *   Bewegungsrichtung hinter der Ebene befinden.
 */
bool isColourConsistent_plane(
		const alg::vec4& voxPosition, 
		const std::vector<pixPos>& pixPositions,
		const std::vector<alg::vec3>& camPos,
		alg::vec3& outputColour, 
		const int axis,
		int& colRemoval,
		const int numInput,
		const std::vector<IplImage*>& imgs,
		const int diffThresh,
		const float voxsize
		) {
	assert(pixPositions.size() == static_cast<unsigned int>(numInput));
	assert(camPos.size() == static_cast<unsigned int>(numInput));


/* TODO */


	return true;
}



/* * * Code for I/O and GUI follows; don't bother. * * */


// global //
int width  = 512;
int height = 512;

std::string path;


// trackball //
#define ROTATE 1
#define MOVE 2

static int   g_oldx = 0;
static int   g_oldy = 0;
static int   g_MotionState = ROTATE;
static float g_theta = (float)0.5f*M_PI/2.0f;
static float g_phi = 1.0f;
static float g_distance = 2.5f;


// space carving //
int gridRes = 50;

int numInput = 3;
std::vector<int> imgWidth;
std::vector<int> imgHeight;
std::vector<voxel> result;
std::vector<alg::mat4> textureMatrices;
std::vector<alg::mat4> modelView;
std::vector<alg::mat4> projection;
std::vector<IplImage*> imgs;
std::vector<alg::vec3> campos;
int diffThresh = 100;


alg::mat4 createTextureMatrix(const alg::mat4& modelviewmat, const alg::mat4& projectionmat, const IplImage *img) {
	alg::mat4 flip(alg::vec4(1, 0, 0, 0), alg::vec4(0, 1, 0, img->height), alg::vec4(0, 0, 1, 0), alg::vec4(0, 0, 0, 1));
	alg::mat4 imgscale(alg::vec4(img->width, 0, 0, 0), alg::vec4(0, -img->height, 0, 0), alg::vec4(0, 0, 1, 0), alg::vec4(0, 0, 0, 1));
	alg::mat4 scale(alg::vec4(0.5f, 0, 0, 0), alg::vec4(0, 0.5f, 0, 0), alg::vec4(0, 0, 0.5f, 0), alg::vec4(0, 0, 0, 1));
	alg::mat4 translate(alg::vec4(1, 0, 0, 0.5f), alg::vec4(0, 1, 0, 0.5f), alg::vec4(0, 0, 1, 0.5f), alg::vec4(0, 0, 0, 1));
	return (flip * imgscale * translate * scale * projectionmat * modelviewmat);
}

void createTextureMatrices() {
	for(int i=0; i<numInput; ++i){
		alg::mat4 tex = createTextureMatrix(modelView[i], projection[i], imgs.at(i));
		textureMatrices.push_back(tex);
	}
}

void findCameraPositions() {
	for(std::vector<alg::mat4>::iterator iter = modelView.begin(); iter != modelView.end(); ++iter){
		// calculate position //
		alg::mat4 m = (*iter).inverse();

		// position //
		alg::vec4 origin(0,0,0,1);
		alg::vec3 pos = m * origin;

		campos.push_back(pos);
	}
}

void loadData() {
	std::stringstream p;
	p << path << "/num.txt";

	// find out number of images to be loaded //
	std::ifstream file(p.str().c_str());
	if(!file){ throw "Error\n"; }

	file >> numInput;
	std::cout << "Number of Images: " << numInput << std::endl;

	modelView.resize(numInput);
	projection.resize(numInput);

	// load camera data //
	int count = 0;
	for(int i=0; i<numInput; ++i){
		std::stringstream s;
		s << path << "/cam" << count++ << ".txt";
		std::ifstream file(s.str().c_str());

		for(int j=0; j<16; ++j){
			file >> modelView[i][j%4][j/4];
		}
		for(int j=0; j<16; ++j){
			file >> projection[i][j%4][j/4];
		}
	}

	// load image data //
	count = 0;
	for(int i=0; i<numInput; ++i){
		std::stringstream s;
		s << path << "/img" << count++ << ".png";
		IplImage *img = cvLoadImage(s.str().c_str());
		imgs.push_back(img);
		imgWidth.push_back(img->width);
		imgHeight.push_back(img->height);
	}

	createTextureMatrices();

	findCameraPositions();
}


void polar2XYZ(const float radius, const float theta, const float phi, float &posx, float &posy, float &posz) {
	posx = radius * sin(theta) * cos(phi);
	posy = radius * cos(theta);
	posz = radius * sin(theta) * sin(phi);
}



void parseInput(int argc, char** argv) {
	if(argc > 1){ path = argv[1]; }
	else{ path = ""; }
	if(argc > 2){ diffThresh = atoi(argv[2]); }
	if(argc > 3){ gridRes = atoi(argv[3]); }
}

void idle() {
	glutPostRedisplay();
}


void init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// Initialize camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);


	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	// load the data //
	loadData();

	// apply space carving //
	applySpaceCarving(numInput, imgWidth, imgHeight, imgs, gridRes, textureMatrices, campos, diffThresh, result);
}



void drawUnitWireCube() {
	bool lightIsOn = glIsEnabled(GL_LIGHTING);
	bool textureIsUsed = glIsEnabled(GL_TEXTURE_2D);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	glColor3f(1.0, 0.0, 0.0);
	glutWireCube(1.0);

	glPopMatrix();

	if(lightIsOn) glEnable(GL_LIGHTING);
	if(textureIsUsed) glEnable(GL_TEXTURE_2D);
}


void drawGeometry() {
	glPushMatrix();
	for(std::vector<voxel>::const_iterator iter = result.begin(); iter != result.end(); ++iter){
		alg::vec3 pos = iter->pos;
		alg::vec3 col = iter->col;

		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);
		glColor3f(col[0], col[1], col[2]);
		glutSolidCube(1.0f/(gridRes-1) + 0.001);
		glPopMatrix();
	}
	glPopMatrix();


	drawUnitWireCube();
}


void drawCameras() {
	glColor3f(0.0, 0.0, 1.0);
	for(std::vector<alg::vec3>::iterator iter = campos.begin(); iter != campos.end(); ++iter){
		alg::vec3& pos = *iter;
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);
		glutSolidCube(0.1f);
		glPopMatrix();
	}
}

void updateCamera() {
	float x,y,z,upX,upY,upZ;
	polar2XYZ(g_distance, g_theta, g_phi, x, y, z);
	polar2XYZ(g_distance, g_theta - M_PI / 2, g_phi, upX, upY, upZ);

	gluLookAt(x,y,z,0,0.1,0,upX,upY,upZ);
}


void display(void)	 {
	// Clear window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// update camera position //
	updateCamera();

	// draw the scene //
	drawGeometry();

	// draw the cameras //
	drawCameras();

	// Swap display buffers
	glutSwapBuffers();

}




///////////////////////////////////////////////////////////////////
// Interaction functions special, mouse, mousMotion, menu
///////////////////////////////////////////////////////////////////

void mouse(int button, int state, int x, int y) {
	g_oldx = x;
	g_oldy = y;

	// left mouse button pressed //
	if(button == GLUT_LEFT_BUTTON)
		if(state == GLUT_DOWN)
			g_MotionState = ROTATE;


	// right mouse button pressed //
	if(button == GLUT_RIGHT_BUTTON)
		if(state == GLUT_DOWN)
			g_MotionState = MOVE;
}


void mouseMotion(int x, int y) {
	// rotation mode, as left mouse button was pressed //
	if(g_MotionState == ROTATE) {
		g_phi += 0.01 * (x - g_oldx);
		g_theta -= 0.01 * (y - g_oldy);
	}


	// move mode, as right mouse button was pressed //
	if(g_MotionState == MOVE){
		g_distance = g_distance + 0.1 * (y - g_oldy);
	}


	// update values //
	g_oldx = x;
	g_oldy = y;
}



int main(int argc, char **argv) {
	try{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(width, height);
		glutCreateWindow("Space Carving");

		glutDisplayFunc(display);
		glutIdleFunc(idle);
		glutMouseFunc(mouse);
		glutMotionFunc(mouseMotion);

		parseInput(argc, argv);

		init();

		glutMainLoop();
	}
	catch(const char* message){
		std::cerr << message << std::endl;
		exit(1);
	}
	catch(...){
		std::cerr << "An exception was thrown.\n";
		exit(1);
	}
}

