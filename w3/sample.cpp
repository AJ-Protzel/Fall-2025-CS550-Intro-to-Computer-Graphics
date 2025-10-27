#include <stdio.h> // Include header file // Preprocessor directive
#include <stdlib.h> // Include header file // Preprocessor directive
#define _USE_MATH_DEFINES // Preprocessor directive
#include <math.h> // Include header file // Preprocessor directive
#include <ctype.h> // Include header file // Preprocessor directive
#include <time.h> // Include header file // Preprocessor directive

#ifndef F_PI // Preprocessor directive
#define F_PI		((float)(M_PI)) // Preprocessor directive
#define F_2_PI		((float)(2.f*F_PI)) // Preprocessor directive
#define F_PI_2		((float)(F_PI/2.f)) // Preprocessor directive
#endif // Preprocessor directive

#ifdef WIN32 // Preprocessor directive
#include <windows.h> // Include header file // Preprocessor directive
#pragma warning(disable:4996) // Preprocessor directive
#endif // Preprocessor directive
#ifdef __APPLE__ // Preprocessor directive
#include <OpenGL/gl.h> // Include header file // Preprocessor directive
#include <OpenGL/glu.h> // Include header file // Preprocessor directive
#else // Preprocessor directive
#include "glew.h" // Include header file // Preprocessor directive
#include <GL/gl.h> // Include header file // Preprocessor directive
#include <GL/glu.h> // Include header file // Preprocessor directive
#endif // Preprocessor directive
#include "glut.h" // Include header file // Preprocessor directive
#include <cstdlib> // Include header file // Preprocessor directive
#include <ctime> // Include header file // Preprocessor directive

const char* WINDOWTITLE = "OpenGL / GLUT Sample -- Adrien Protzel"; // Title for main window // Constant definition
const char* GLUITITLE = "User Interface Window"; // Title for UI window // Constant definition
const int GLUITRUE = true; // Constant definition
const int GLUIFALSE = false; // Constant definition
const int ESCAPE = 0x1b; // Escape key ASCII code // Constant definition
const int INIT_WINDOW_SIZE = 1000; // Initial window size // Constant definition
const float BOXSIZE = 2.f; // Size of the 3D box // Constant definition
const float ANGFACT = 1.f; // Angle factor for rotation // Constant definition
const float SCLFACT = 0.005f; // Scale factor for scaling // Constant definition
const float MINSCALE = 0.05f; // Minimum scale limit // Constant definition
const int SCROLL_WHEEL_UP = 3; // Scroll wheel up button // Constant definition
const int SCROLL_WHEEL_DOWN = 4; // Scroll wheel down button // Constant definition
const float SCROLL_WHEEL_CLICK_FACTOR = 5.f; // Scroll wheel click sensitivity // Constant definition
const int LEFT = 4; // Left mouse button // Constant definition
const int MIDDLE = 2; // Middle mouse button // Constant definition
const int RIGHT = 1; // Right mouse button // Constant definition

enum Projections // Projection modes // Enum declaration
{
	ORTHO, // Code line
	PERSP // Code line
};

enum ButtonVals // Menu button values // Enum declaration
{
	RESET, // Code line
	QUIT // Code line
};

const GLfloat BACKCOLOR[] = { 0., 0., 0., 1. }; // Background color // Constant definition
const GLfloat AXES_WIDTH = 3.; // Width of axes lines // Constant definition

enum Colors // Color options // Enum declaration
{
	RED, // Code line
	YELLOW, // Code line
	GREEN, // Code line
	CYAN, // Code line
	BLUE, // Code line
	MAGENTA // Code line
};

char* ColorNames[] = // Names of colors // Function or helper declaration
{
(char*)"Red", // Code line
(char*)"Yellow", // Code line
(char*)"Green", // Code line
(char*)"Cyan", // Code line
(char*)"Blue", // Code line
(char*)"Magenta" // Code line
};

const GLfloat Colors[][3] = // RGB values for colors // Constant definition
{
{ 1., 0., 0. },
{ 1., 1., 0. },
{ 0., 1., 0. },
{ 0., 1., 1. },
{ 0., 0., 1. },
{ 1., 0., 1. },
};

const GLfloat FOGCOLOR[4] = { .0f, .0f, .0f, 1.f }; // Fog color // Constant definition
const GLenum  FOGMODE = GL_LINEAR; // Fog mode // Constant definition
const GLfloat FOGDENSITY = 0.30f; // Fog density // Constant definition
const GLfloat FOGSTART = 1.5f; // Fog start distance // Constant definition
const GLfloat FOGEND = 4.f; // Fog end distance // Constant definition

const float	WHITE[] = { 1.,1.,1.,1. }; // Constant definition
const int MS_PER_CYCLE = 10000; // Animation cycle duration // Constant definition

enum LightMode // Light mode options // Enum declaration
{
	LIGHT_POINT, // Code line
	LIGHT_SPOT // Code line
};
int CurrentLightMode = LIGHT_POINT; // Variable declaration

const float ROOM_XSIDE = 10.0f; // Constant definition
const float ROOM_ZSIDE = 10.0f; // Constant definition
const float ROOM_X0 = -ROOM_XSIDE / 2.0f; // Constant definition
const float ROOM_Z0 = -ROOM_ZSIDE / 2.0f; // Constant definition

enum LightColor // Light color options // Enum declaration
{
	LS_WHITE, // Code line
	LS_RED, // Code line
	LS_ORANGE, // Code line
	LS_YELLOW, // Code line
	LS_GREEN, // Code line
	LS_CYAN, // Code line
	LS_MAGENTA // Code line
};

int CurrentLightColor = LS_WHITE; // Variable declaration

const GLfloat LightColors[][4] = // RGBA values for light colors // Constant definition
{
{ 1.0f, 1.0f, 1.0f, 1.f },
{ 1.0f, 0.0f, 0.0f, 1.f },
{ 1.0f, 0.5f, 0.0f, 1.f },
{ 1.0f, 1.0f, 0.0f, 1.f },
{ 0.0f, 1.0f, 0.0f, 1.f },
{ 0.0f, 1.0f, 1.0f, 1.f },
{ 1.0f, 0.0f, 1.0f, 1.f }
};

int		ActiveButton; // Variable declaration
int		AxesOn; // Variable declaration
int		DebugOn; // Variable declaration
int		DepthCueOn; // Variable declaration
int		DepthBufferOn; // Variable declaration
int		DepthFightingOn; // Variable declaration
int		MainWindow; // Variable declaration
int		NowColor; // Variable declaration
int		NowProjection; // Variable declaration
float	Scale; // Variable declaration
int		ShadowsOn; // Variable declaration
float	Time; // Variable declaration
int		Xmouse, Ymouse; // Variable declaration
float	Xrot, Yrot; // Variable declaration

GLuint SolidHorseList; // Variable declaration
GLuint GridDL; // Variable declaration
GLuint	CircleList; // Variable declaration
GLuint	AxesList; // Variable declaration

float gLightRadius = 3.0f; // Variable declaration
float gLightBaseY = 1.2f; // Variable declaration
float gLightAmpY = 1.0f; // Variable declaration

void	Animate(); // Function or helper declaration
void	Display(); // Function or helper declaration
void	DoAxesMenu(int); // Function or helper declaration
void	DoColorMenu(int); // Function or helper declaration
void	DoDepthBufferMenu(int); // Function or helper declaration
void	DoDepthFightingMenu(int); // Function or helper declaration
void	DoDepthMenu(int); // Function or helper declaration
void	DoDebugMenu(int); // Function or helper declaration
void DoViewMenu(int); // Function or helper declaration
void	DoMainMenu(int); // Function or helper declaration
void	DoProjectMenu(int); // Function or helper declaration
void	DoRasterString(float, float, float, char*); // Function or helper declaration
void	DoStrokeString(float, float, float, float, char*); // Function or helper declaration
float	ElapsedSeconds(); // Variable declaration
void	InitGraphics(); // Function or helper declaration
void	InitLists(); // Function or helper declaration
void	InitMenus(); // Function or helper declaration
void	Keyboard(unsigned char, int, int); // Function or helper declaration
void	MouseButton(int, int, int, int); // Function or helper declaration
void	MouseMotion(int, int); // Function or helper declaration
void	Reset(); // Function or helper declaration
void	Resize(int, int); // Function or helper declaration
void	Visibility(int); // Function or helper declaration
void			Axes(float); // Function or helper declaration
void			HsvRgb(float[3], float[3]); // Function or helper declaration
void			Cross(float[3], float[3], float[3]); // Function or helper declaration
float			Dot(float[3], float[3]); // Variable declaration
float			Unit(float[3], float[3]); // Variable declaration
float			Unit(float[3]); // Variable declaration

float* // Variable declaration
Array3(float a, float b, float c) // Code line
{
	static float array[4]; // Function or helper declaration
	array[0] = a; // Code line
	array[1] = b; // Code line
	array[2] = c; // Code line
	array[3] = 1.; // Code line
	return array; // Return statement
}

float* // Variable declaration
MulArray3(float factor, float array0[]) // Code line
{
	static float array[4]; // Function or helper declaration
	array[0] = factor * array0[0]; // Code line
	array[1] = factor * array0[1]; // Code line
	array[2] = factor * array0[2]; // Code line
	array[3] = 1.; // Code line
	return array; // Return statement
}

float* // Variable declaration
MulArray3(float factor, float a, float b, float c) // Code line
{
	static float array[4]; // Function or helper declaration
	float* abc = Array3(a, b, c); // Variable declaration
	array[0] = factor * abc[0]; // Code line
	array[1] = factor * abc[1]; // Code line
	array[2] = factor * abc[2]; // Code line
	array[3] = 1.; // Code line
	return array; // Return statement
}

float // Variable declaration
Ranf(float low, float high) // Code line
{
	float r = (float)rand(); // Variable declaration
	float t = r / (float)RAND_MAX; // Variable declaration
	return   low + t * (high - low); // Return statement
}

void // Function or helper declaration
TimeOfDaySeed() // Code line
{
	struct tm y2k; // Code line
	y2k.tm_hour = 0;    y2k.tm_min = 0; y2k.tm_sec = 0; // Code line
	y2k.tm_year = 2000; y2k.tm_mon = 0; y2k.tm_mday = 1; // Code line
	time_t  now; // Code line
	time(&now); // Code line
	double seconds = difftime(now, mktime(&y2k)); // Code line
	unsigned int seed = (unsigned int)(1000. * seconds); // Code line
	srand(seed); // Code line
}

#include "setmaterial.cpp" // Include header file // Preprocessor directive
#include "setlight.cpp" // Include header file // Preprocessor directive
#include "osusphere.cpp" // Include header file // Preprocessor directive
#include "osucube.cpp" // Include header file // Preprocessor directive
#include "osucylindercone.cpp" // Include header file // Preprocessor directive
#include "osutorus.cpp" // Include header file // Preprocessor directive
#include "bmptotexture.cpp" // Include header file // Preprocessor directive
#include "loadobjmtlfiles.cpp" // Include header file // Preprocessor directive
#include "keytime.cpp" // Include header file // Preprocessor directive
#include "glslprogram.cpp" // Include header file // Preprocessor directive
#include "vertexbufferobject.cpp" // Include header file // Preprocessor directive

#include "CarouselHorse0.10.550.cpp" // Include header file // Preprocessor directive
const int MSEC = MS_PER_CYCLE; // Constant definition
Keytimes Xpos, Ypos, Zpos; // Code line
Keytimes ThetaX, ThetaY, ThetaZ; // Code line
#define NUM_HORSES 6 // Preprocessor directive
#define RING_RADIUS 2.f // Preprocessor directive
GLuint SolidHorseLists[NUM_HORSES]; // Variable declaration
Keytimes HXpos[NUM_HORSES], HYpos[NUM_HORSES], HZpos[NUM_HORSES]; // Code line
Keytimes HThetaX[NUM_HORSES], HThetaY[NUM_HORSES], HThetaZ[NUM_HORSES]; // Code line

GLuint FaceDL = 0; // Variable declaration
GLuint SkeletonDL = 0; // Variable declaration
GLuint LightSphere; // Variable declaration
Keytimes LightThetaY; // Code line



int // Variable declaration
main(int argc, char* argv[]) // Code line
{
	glutInit(&argc, argv); // OpenGL/GLUT function call
	InitGraphics(); // Code line
	InitLists(); // Code line
	Reset(); // Code line
	InitMenus(); // Code line
	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutMainLoop(); // OpenGL/GLUT function call
	return 0; // Return statement
}


void // Function or helper declaration
Animate() // Code line
{
	int ms = glutGet(GLUT_ELAPSED_TIME); // Variable declaration
	ms %= MS_PER_CYCLE; // Code line
	Time = (float)ms / (float)MS_PER_CYCLE; // Code line
	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutPostRedisplay(); // OpenGL/GLUT function call
}


void ConfigureLight0(float lx, float ly, float lz) // Function or helper declaration
{
	glEnable(GL_LIGHTING); // OpenGL/GLUT function call
	const GLfloat* C = LightColors[CurrentLightColor]; // Constant definition

	if (CurrentLightMode == LIGHT_POINT) // Conditional logic
	{
		SetPointLight(GL_LIGHT0, lx, ly, lz, C[0], C[1], C[2]); // OpenGL/GLUT function call
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f); // OpenGL/GLUT function call
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f); // OpenGL/GLUT function call
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f); // OpenGL/GLUT function call
	}
	else // Conditional logic
	{
		float dir[3] = { ROOM_X0 - lx, 0.0f, 0.0f - lz }; // Variable declaration
		Unit(dir); // Code line
		SetSpotLight(GL_LIGHT0, lx, ly, lz, dir[0], dir[1], dir[2], C[0], C[1], C[2]); // OpenGL/GLUT function call
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 22.5f); // OpenGL/GLUT function call
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0f); // OpenGL/GLUT function call
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f); // OpenGL/GLUT function call
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f); // OpenGL/GLUT function call
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f); // OpenGL/GLUT function call
	}
}


void Display() // Function or helper declaration
{
	if (DebugOn != 0) fprintf(stderr, "Starting Display.\n"); // Conditional logic

	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glDrawBuffer(GL_BACK); // OpenGL/GLUT function call
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // OpenGL/GLUT function call
	glEnable(GL_DEPTH_TEST); // OpenGL/GLUT function call

	glMatrixMode(GL_PROJECTION); // OpenGL/GLUT function call
	glLoadIdentity(); // OpenGL/GLUT function call
	gluPerspective(70.f, 1.f, 0.1f, 1000.f); // OpenGL/GLUT function call

	glMatrixMode(GL_MODELVIEW); // OpenGL/GLUT function call
	glLoadIdentity(); // OpenGL/GLUT function call

	gluLookAt(0.f, 0.f, 3.f, // OpenGL/GLUT function call
		0.f, 0.f, 0.f, // Code line
		0.f, 1.f, 0.f); // Code line

	glRotatef((GLfloat)Yrot, 0.f, 1.f, 0.f); // OpenGL/GLUT function call
	glRotatef((GLfloat)Xrot, 1.f, 0.f, 0.f); // OpenGL/GLUT function call
	if (Scale < MINSCALE) Scale = MINSCALE; // Conditional logic
	glScalef((GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale); // OpenGL/GLUT function call

	if (DepthCueOn != 0) // Conditional logic
	{
		glFogi(GL_FOG_MODE, FOGMODE); // OpenGL/GLUT function call
		glFogfv(GL_FOG_COLOR, FOGCOLOR); // OpenGL/GLUT function call
		glFogf(GL_FOG_DENSITY, FOGDENSITY); // OpenGL/GLUT function call
		glFogf(GL_FOG_START, FOGSTART); // OpenGL/GLUT function call
		glFogf(GL_FOG_END, FOGEND); // OpenGL/GLUT function call
		glEnable(GL_FOG); // OpenGL/GLUT function call
	}
	else glDisable(GL_FOG); // OpenGL/GLUT function call

	int msec = glutGet(GLUT_ELAPSED_TIME) % MSEC; // Variable declaration
	float nowSecs = (float)msec / 1000.f; // Variable declaration

	glEnable(GL_DEPTH_TEST); // OpenGL/GLUT function call
	glEnable(GL_NORMALIZE); // OpenGL/GLUT function call
	glShadeModel(GL_SMOOTH); // OpenGL/GLUT function call
	glDisable(GL_COLOR_MATERIAL); // OpenGL/GLUT function call
	glDisable(GL_TEXTURE_2D); // OpenGL/GLUT function call
	glEnable(GL_LIGHTING); // OpenGL/GLUT function call
	glEnable(GL_LIGHT0); // OpenGL/GLUT function call

	float angleDeg = LightThetaY.GetValue(nowSecs); // Variable declaration
	float angleRad = angleDeg * (M_PI / 180.0f); // Variable declaration
	const float LightRadius = 3.0f; // Constant definition
	float LightZ = 0.0f; // Variable declaration
	float LightY = LightRadius * cos(angleRad); // Variable declaration
	float LightX = LightRadius * sin(angleRad); // Variable declaration

	ConfigureLight0(LightX, LightY, LightZ); // Code line

	glCallList(GridDL); // OpenGL/GLUT function call

	for (int i = 0; i < NUM_HORSES; ++i) { // Loop statement
		glPushMatrix(); // OpenGL/GLUT function call
		glRotatef(HThetaX[i].GetValue(nowSecs), 1.f, 0.f, 0.f); // OpenGL/GLUT function call
		glRotatef(HThetaY[i].GetValue(nowSecs), 0.f, 1.f, 0.f); // OpenGL/GLUT function call
		glRotatef(HThetaZ[i].GetValue(nowSecs), 0.f, 0.f, 1.f); // OpenGL/GLUT function call
		glTranslatef(HXpos[i].GetValue(nowSecs), HYpos[i].GetValue(nowSecs), HZpos[i].GetValue(nowSecs)); // OpenGL/GLUT function call
		glCallList(SolidHorseLists[i]); // OpenGL/GLUT function call
		glPopMatrix(); // OpenGL/GLUT function call
	}

	glCallList(FaceDL); // OpenGL/GLUT function call
	glCallList(SkeletonDL); // OpenGL/GLUT function call

	glDisable(GL_LIGHTING); // OpenGL/GLUT function call

	const GLfloat* C = LightColors[CurrentLightColor]; // Constant definition
	glColor3f(C[0], C[1], C[2]); // OpenGL/GLUT function call

	glPushMatrix(); // OpenGL/GLUT function call
	glTranslatef(LightX, LightY, LightZ); // OpenGL/GLUT function call
	glutSolidSphere(0.08, 20, 20); // OpenGL/GLUT function call
	glPopMatrix(); // OpenGL/GLUT function call

	if (AxesOn) { // Conditional logic
		glCallList(AxesList); // OpenGL/GLUT function call
	}

	glCallList(CircleList); // OpenGL/GLUT function call
	glDisable(GL_DEPTH_TEST); // OpenGL/GLUT function call
	glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0.f, 100.f, 0.f, 100.f); // OpenGL/GLUT function call
	glMatrixMode(GL_MODELVIEW);  glLoadIdentity(); // OpenGL/GLUT function call
	glColor3f(1.f, 1.f, 1.f); // OpenGL/GLUT function call
	glutSwapBuffers(); // OpenGL/GLUT function call
	glFlush(); // OpenGL/GLUT function call
}

void // Function or helper declaration
DoAxesMenu(int id) // Code line
{
	AxesOn = id; // Code line
	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutPostRedisplay(); // OpenGL/GLUT function call
}

void // Function or helper declaration
DoColorMenu(int id) // Code line
{
	NowColor = id - RED; // Code line
	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutPostRedisplay(); // OpenGL/GLUT function call
}

void // Function or helper declaration
DoDebugMenu(int id) // Code line
{
	DebugOn = id; // Code line
	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutPostRedisplay(); // OpenGL/GLUT function call
}

void // Function or helper declaration
DoDepthBufferMenu(int id) // Code line
{
	DepthBufferOn = id; // Code line
	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutPostRedisplay(); // OpenGL/GLUT function call
}

void // Function or helper declaration
DoDepthFightingMenu(int id) // Code line
{
	DepthFightingOn = id; // Code line
	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutPostRedisplay(); // OpenGL/GLUT function call
}

void // Function or helper declaration
DoDepthMenu(int id) // Code line
{
	DepthCueOn = id; // Code line
	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutPostRedisplay(); // OpenGL/GLUT function call
}

void // Function or helper declaration
DoMainMenu(int id) // Code line
{
	switch (id) // Switch-case logic
	{
	case RESET: // Switch-case logic
		Reset(); // Code line
		break; // Switch-case logic

	case QUIT: // Switch-case logic
		glutSetWindow(MainWindow); // OpenGL/GLUT function call
		glFinish(); // OpenGL/GLUT function call
		glutDestroyWindow(MainWindow); // OpenGL/GLUT function call
		exit(0); // Code line
		break; // Switch-case logic

	default: // Switch-case logic
		fprintf(stderr, "Don't know what to do with Main Menu ID %d\n", id); // Code line
	}

	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutPostRedisplay(); // OpenGL/GLUT function call
}

void // Function or helper declaration
DoProjectMenu(int id) // Code line
{
	NowProjection = id; // Code line

	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutPostRedisplay(); // OpenGL/GLUT function call
}

void DoViewMenu(int id) // Function or helper declaration
{
	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutPostRedisplay(); // OpenGL/GLUT function call
}

void // Function or helper declaration
DoRasterString(float x, float y, float z, char* s) // Code line
{
	glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z); // OpenGL/GLUT function call

	char c; // Function or helper declaration
	for (; (c = *s) != '\0'; s++) // Loop statement
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); // OpenGL/GLUT function call
	}
}

void // Function or helper declaration
DoStrokeString(float x, float y, float z, float ht, char* s) // Code line
{
	glPushMatrix(); // OpenGL/GLUT function call
	glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)z); // OpenGL/GLUT function call
	float sf = ht / (119.05f + 33.33f); // Variable declaration
	glScalef((GLfloat)sf, (GLfloat)sf, (GLfloat)sf); // OpenGL/GLUT function call
	char c; // Function or helper declaration
	for (; (c = *s) != '\0'; s++) // Loop statement
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, c); // OpenGL/GLUT function call
	}
	glPopMatrix(); // OpenGL/GLUT function call
}

float // Variable declaration
ElapsedSeconds() // Code line
{
	int ms = glutGet(GLUT_ELAPSED_TIME); // Variable declaration
	return (float)ms / 1000.f; // Return statement
}

void // Function or helper declaration
InitGraphics() // Code line
{
	if (DebugOn != 0) // Conditional logic
		fprintf(stderr, "Starting InitGraphics.\n"); // Code line

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // OpenGL/GLUT function call

	glutInitWindowPosition(0, 0); // OpenGL/GLUT function call
	glutInitWindowSize(INIT_WINDOW_SIZE, INIT_WINDOW_SIZE); // OpenGL/GLUT function call

	MainWindow = glutCreateWindow(WINDOWTITLE); // Code line
	glutSetWindowTitle(WINDOWTITLE); // OpenGL/GLUT function call

	glClearColor(BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3]); // OpenGL/GLUT function call

	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutDisplayFunc(Display); // OpenGL/GLUT function call
	glutReshapeFunc(Resize); // OpenGL/GLUT function call
	glutKeyboardFunc(Keyboard); // OpenGL/GLUT function call
	glutMouseFunc(MouseButton); // OpenGL/GLUT function call
	glutMotionFunc(MouseMotion); // OpenGL/GLUT function call
	glutPassiveMotionFunc(MouseMotion); // OpenGL/GLUT function call
	glutVisibilityFunc(Visibility); // OpenGL/GLUT function call
	glutEntryFunc(NULL); // OpenGL/GLUT function call
	glutSpecialFunc(NULL); // OpenGL/GLUT function call
	glutSpaceballMotionFunc(NULL); // OpenGL/GLUT function call
	glutSpaceballRotateFunc(NULL); // OpenGL/GLUT function call
	glutSpaceballButtonFunc(NULL); // OpenGL/GLUT function call
	glutButtonBoxFunc(NULL); // OpenGL/GLUT function call
	glutDialsFunc(NULL); // OpenGL/GLUT function call
	glutTabletMotionFunc(NULL); // OpenGL/GLUT function call
	glutTabletButtonFunc(NULL); // OpenGL/GLUT function call
	glutMenuStateFunc(NULL); // OpenGL/GLUT function call
	glutTimerFunc(-1, NULL, 0); // OpenGL/GLUT function call
	glutIdleFunc(Animate); // OpenGL/GLUT function call

#ifdef WIN32 // Preprocessor directive
	GLenum err = glewInit(); // Code line
	if (err != GLEW_OK) // Conditional logic
	{
		fprintf(stderr, "glewInit Error\n"); // Code line
	}
	else // Conditional logic
		fprintf(stderr, "GLEW initialized OK\n"); // Code line
	fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION)); // Code line
#endif // Preprocessor directive


	for (int i = 0; i < NUM_HORSES; i++) { // Loop statement
		float t = 10.0f - i * 1.0f; // Variable declaration

		HYpos[i].Init(); // Code line
		HYpos[i].AddTimeValue(0.0f, 0.0f); // Code line
		HYpos[i].AddTimeValue(t * 0.125f, 0.5f); // Code line
		HYpos[i].AddTimeValue(t * 0.25f, 0.0f); // Code line
		HYpos[i].AddTimeValue(t * 0.375f, -0.5f); // Code line
		HYpos[i].AddTimeValue(t * 0.5f, 0.0f); // Code line
		HYpos[i].AddTimeValue(t * 0.625f, 0.5f); // Code line
		HYpos[i].AddTimeValue(t * 0.75f, 0.0f); // Code line
		HYpos[i].AddTimeValue(t * 0.875f, -0.5f); // Code line
		HYpos[i].AddTimeValue(t, 0.0f); // Code line

		HThetaY[i].Init(); // Code line
		HThetaY[i].AddTimeValue(0.0, 0.0); // Code line
		HThetaY[i].AddTimeValue(10.0f, -360); // Code line

	}
	LightThetaY.Init(); // Code line
	LightThetaY.AddTimeValue(0.0f, 0.0f); // Code line
	LightThetaY.AddTimeValue(10.0f, -360.0f); // Code line
}



void BuildHorseList(GLuint listID, float angleDeg) // Function or helper declaration
{
	glNewList(listID, GL_COMPILE); // OpenGL/GLUT function call
	glPushMatrix(); // OpenGL/GLUT function call
	glRotatef(angleDeg, 0.f, 1.f, 0.f); // OpenGL/GLUT function call
	glTranslatef(RING_RADIUS, -1.1f, 0.f); // OpenGL/GLUT function call
	glShadeModel(GL_SMOOTH); // OpenGL/GLUT function call
	SetMaterial(0.8f, 0.2f, 0.2f, 5.f); // Code line

	glBegin(GL_TRIANGLES); // OpenGL/GLUT function call
	for (int i = 0; i < HORSEntris; ++i) // Loop statement
	{
		point p0 = HORSEpoints[HORSEtris[i].p0]; // Code line
		point p1 = HORSEpoints[HORSEtris[i].p1]; // Code line
		point p2 = HORSEpoints[HORSEtris[i].p2]; // Code line

		float p01[3] = { p1.x - p0.x, p1.y - p0.y, p1.z - p0.z }; // Variable declaration
		float p02[3] = { p2.x - p0.x, p2.y - p0.y, p2.z - p0.z }; // Variable declaration
		float n[3]; // Variable declaration
		Cross(p01, p02, n); // Code line
		Unit(n, n); // Code line

		glNormal3f(n[0], n[1], n[2]); // OpenGL/GLUT function call
		glVertex3f(p0.x, p0.y, p0.z); // OpenGL/GLUT function call
		glVertex3f(p1.x, p1.y, p1.z); // OpenGL/GLUT function call
		glVertex3f(p2.x, p2.y, p2.z); // OpenGL/GLUT function call
	}
	glEnd(); // OpenGL/GLUT function call
	glPopMatrix(); // OpenGL/GLUT function call
	glEndList(); // OpenGL/GLUT function call
}

void BuildFaceList() // Function or helper declaration
{
	static GLuint s_FaceGeo = 0; // Function or helper declaration
	if (s_FaceGeo == 0) // Conditional logic
	{
		s_FaceGeo = LoadObjMtlFiles((char*)"Face.obj"); // Code line
		if (s_FaceGeo == 0) // Conditional logic
		{
			fprintf(stderr, "Failed to load Face.obj\n"); // Code line
		}
	}

	FaceDL = glGenLists(1); // Code line
	glNewList(FaceDL, GL_COMPILE); // OpenGL/GLUT function call
	glShadeModel(GL_SMOOTH); // OpenGL/GLUT function call
	glDisable(GL_COLOR_MATERIAL); // OpenGL/GLUT function call
	glDisable(GL_TEXTURE_2D); // OpenGL/GLUT function call
	glPushMatrix(); // OpenGL/GLUT function call
	glTranslatef(1.2f, 1.5f, -2.7f); // OpenGL/GLUT function call
	SetMaterial(0.70f, 0.25f, 0.85f, 100.f); // Code line
	glCallList(s_FaceGeo); // OpenGL/GLUT function call
	glPopMatrix(); // OpenGL/GLUT function call
	glEnable(GL_TEXTURE_2D); // OpenGL/GLUT function call
	glEndList(); // OpenGL/GLUT function call
}

void BuildSkeletonList() // Function or helper declaration
{
	static GLuint s_SkelGeo = 0; // Function or helper declaration
	if (s_SkelGeo == 0) // Conditional logic
	{
		s_SkelGeo = LoadObjMtlFiles((char*)"Skeleton.obj"); // Code line
		if (s_SkelGeo == 0) // Conditional logic
			fprintf(stderr, "Failed to load Skeleton.obj\n"); // Code line
	}

	SkeletonDL = glGenLists(1); // Code line
	glNewList(SkeletonDL, GL_COMPILE); // OpenGL/GLUT function call
	glShadeModel(GL_SMOOTH); // OpenGL/GLUT function call
	glDisable(GL_COLOR_MATERIAL); // OpenGL/GLUT function call
	glDisable(GL_TEXTURE_2D); // OpenGL/GLUT function call
	SetMaterial(0.36f, 0.22f, 0.12f, 8.f); // Code line
	glPushMatrix(); // OpenGL/GLUT function call
	glTranslatef(0.0f, -7.f, 3.7f); // OpenGL/GLUT function call
	glRotatef(180.f, 0.f, 1.f, 0.f); // OpenGL/GLUT function call
	glCallList(s_SkelGeo); // OpenGL/GLUT function call
	glPopMatrix(); // OpenGL/GLUT function call
	glEnable(GL_TEXTURE_2D); // OpenGL/GLUT function call
	glEndList(); // OpenGL/GLUT function call
}

void BuildBackgroundList() // Function or helper declaration
{
	const float XSIDE = 10.0f, ZSIDE = 10.0f, WALL_HEIGHT = 10.0f; // Constant definition

	const int   NX = 20, NZ = 20; // Constant definition
	const int   WX = 20, WY = 20; // Constant definition

	const float DXf = XSIDE / (NX - 1); // Constant definition
	const float DZf = ZSIDE / (NZ - 1); // Constant definition

	const float X0 = -XSIDE / 2.0f; // Constant definition
	const float Z0 = -ZSIDE / 2.0f; // Constant definition

	const float WALL_Y0 = -WALL_HEIGHT / 2.0f; // Constant definition
	const float DYw = WALL_HEIGHT / (WY - 1); // Constant definition
	const float DXw = XSIDE / (WX - 1); // Constant definition
	const float DZw = ZSIDE / (WX - 1); // Constant definition

	GridDL = glGenLists(1); // Code line
	glNewList(GridDL, GL_COMPILE); // OpenGL/GLUT function call

	glEnable(GL_NORMALIZE); // OpenGL/GLUT function call
	glShadeModel(GL_SMOOTH); // OpenGL/GLUT function call

	SetMaterial(0.6f, 0.8f, 0.6f, 20.f); // Code line
	glNormal3f(0.f, 1.f, 0.f); // OpenGL/GLUT function call
	for (int iz = 0; iz < NZ - 1; ++iz) // Loop statement
	{
		glBegin(GL_QUAD_STRIP); // OpenGL/GLUT function call
		for (int ix = 0; ix < NX; ++ix) // Loop statement
		{
			glVertex3f(X0 + DXf * ix, WALL_Y0, Z0 + DZf * iz); // OpenGL/GLUT function call
			glVertex3f(X0 + DXf * ix, WALL_Y0, Z0 + DZf * (iz + 1)); // OpenGL/GLUT function call
		}
		glEnd(); // OpenGL/GLUT function call
	}

	SetMaterial(0.7f, 0.6f, 0.6f, 15.f); // Code line
	glNormal3f(0.f, 0.f, 1.f); // OpenGL/GLUT function call
	for (int jy = 0; jy < WY - 1; ++jy) // Loop statement
	{
		float y0 = WALL_Y0 + DYw * jy; // Variable declaration
		float y1 = WALL_Y0 + DYw * (jy + 1); // Variable declaration
		glBegin(GL_QUAD_STRIP); // OpenGL/GLUT function call
		for (int ix = 0; ix < WX; ++ix) // Loop statement
		{
			float x = X0 + DXw * ix; // Variable declaration
			glVertex3f(x, y0, Z0); // OpenGL/GLUT function call
			glVertex3f(x, y1, Z0); // OpenGL/GLUT function call
		}
		glEnd(); // OpenGL/GLUT function call
	}

	SetMaterial(0.6f, 0.6f, 0.8f, 15.f); // Code line
	glNormal3f(1.f, 0.f, 0.f); // OpenGL/GLUT function call
	for (int jy = 0; jy < WY - 1; ++jy) // Loop statement
	{
		float y0 = WALL_Y0 + DYw * jy; // Variable declaration
		float y1 = WALL_Y0 + DYw * (jy + 1); // Variable declaration
		glBegin(GL_QUAD_STRIP); // OpenGL/GLUT function call
		for (int iz = 0; iz < WX; ++iz) // Loop statement
		{
			float z = Z0 + DZw * iz; // Variable declaration
			glVertex3f(X0, y0, z); // OpenGL/GLUT function call
			glVertex3f(X0, y1, z); // OpenGL/GLUT function call
		}
		glEnd(); // OpenGL/GLUT function call
	}

	glEndList(); // OpenGL/GLUT function call
}

void BuildCircleList() // Function or helper declaration
{
	CircleList = glGenLists(1); // Code line
	glNewList(CircleList, GL_COMPILE); // OpenGL/GLUT function call
	glShadeModel(GL_SMOOTH); // OpenGL/GLUT function call
	glDisable(GL_LIGHTING); // OpenGL/GLUT function call
	glColor3f(1.f, 0.f, 0.f); // OpenGL/GLUT function call
	const int segments = 100; // Constant definition
	const float radius = 2.f; // Constant definition
	glBegin(GL_LINE_LOOP); // OpenGL/GLUT function call
	for (int i = 0; i < segments; ++i) // Loop statement
	{
		float a = 2.f * M_PI * i / segments; // Variable declaration
		glVertex3f(radius * cosf(a), 0.f, radius * sinf(a)); // OpenGL/GLUT function call
	}
	glEnd(); // OpenGL/GLUT function call
	glEndList(); // OpenGL/GLUT function call
}

void BuildAxesList(float length) // Function or helper declaration
{
	AxesList = glGenLists(1); // Code line
	glNewList(AxesList, GL_COMPILE); // OpenGL/GLUT function call
	glShadeModel(GL_SMOOTH); // OpenGL/GLUT function call
	glDisable(GL_LIGHTING); // OpenGL/GLUT function call
	glLineWidth(AXES_WIDTH); // OpenGL/GLUT function call
	Axes(length); // Code line
	glLineWidth(1.f); // OpenGL/GLUT function call
	glEndList(); // OpenGL/GLUT function call
}

void BuildLightSphereList(float r, float g, float b, float lightX, float lightY, float lightZ) // Function or helper declaration
{
	LightSphere = glGenLists(1); // Code line
	glNewList(LightSphere, GL_COMPILE); // OpenGL/GLUT function call
	glShadeModel(GL_SMOOTH); // OpenGL/GLUT function call

	glDisable(GL_LIGHTING); // OpenGL/GLUT function call
	glColor3f(r, g, b); // OpenGL/GLUT function call

	glPushMatrix(); // OpenGL/GLUT function call
	glTranslatef(lightX, lightY, lightZ); // OpenGL/GLUT function call
	glutSolidSphere(0.08, 20, 20); // OpenGL/GLUT function call
	glPopMatrix(); // OpenGL/GLUT function call

	glEndList(); // OpenGL/GLUT function call
}

void InitLists() // Function or helper declaration
{
	if (DebugOn != 0) // Conditional logic
		fprintf(stderr, "Starting InitLists.\n"); // Code line

	glutSetWindow(MainWindow); // OpenGL/GLUT function call

	BuildBackgroundList(); // Code line


	for (int i = 0; i < NUM_HORSES; ++i) // Loop statement
	{
		SolidHorseLists[i] = glGenLists(1); // Code line
		float angleDeg = 360.f / NUM_HORSES * i; // Variable declaration
		BuildHorseList(SolidHorseLists[i], angleDeg); // Code line
	}

	BuildFaceList(); // Code line

	BuildSkeletonList(); // Code line


	BuildLightSphereList(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0); // Code line
}



void // Function or helper declaration
InitMenus() // Code line
{
	if (DebugOn != 0) // Conditional logic
		fprintf(stderr, "Starting InitMenus.\n"); // Code line

	glutSetWindow(MainWindow); // OpenGL/GLUT function call

	int numColors = sizeof(Colors) / (3 * sizeof(float)); // Variable declaration
	int colormenu = glutCreateMenu(DoColorMenu); // Variable declaration
	for (int i = 0; i < numColors; i++) // Loop statement
	{
		glutAddMenuEntry(ColorNames[i], i); // OpenGL/GLUT function call
	}

	int axesmenu = glutCreateMenu(DoAxesMenu); // Variable declaration
	glutAddMenuEntry("Off", 0); // OpenGL/GLUT function call
	glutAddMenuEntry("On", 1); // OpenGL/GLUT function call

	int depthcuemenu = glutCreateMenu(DoDepthMenu); // Variable declaration
	glutAddMenuEntry("Off", 0); // OpenGL/GLUT function call
	glutAddMenuEntry("On", 1); // OpenGL/GLUT function call

	int depthbuffermenu = glutCreateMenu(DoDepthBufferMenu); // Variable declaration
	glutAddMenuEntry("Off", 0); // OpenGL/GLUT function call
	glutAddMenuEntry("On", 1); // OpenGL/GLUT function call

	int depthfightingmenu = glutCreateMenu(DoDepthFightingMenu); // Variable declaration
	glutAddMenuEntry("Off", 0); // OpenGL/GLUT function call
	glutAddMenuEntry("On", 1); // OpenGL/GLUT function call

	int debugmenu = glutCreateMenu(DoDebugMenu); // Variable declaration
	glutAddMenuEntry("Off", 0); // OpenGL/GLUT function call
	glutAddMenuEntry("On", 1); // OpenGL/GLUT function call

	int mainmenu = glutCreateMenu(DoMainMenu); // Variable declaration
	glutAddSubMenu("Axes", axesmenu); // OpenGL/GLUT function call
	glutAddSubMenu("Axis Colors", colormenu); // OpenGL/GLUT function call


#ifdef DEMO_DEPTH_BUFFER // Preprocessor directive
	glutAddSubMenu("Depth Buffer", depthbuffermenu); // OpenGL/GLUT function call
#endif // Preprocessor directive

#ifdef DEMO_Z_FIGHTING // Preprocessor directive
	glutAddSubMenu("Depth Fighting", depthfightingmenu); // OpenGL/GLUT function call
#endif // Preprocessor directive

	glutAddSubMenu("Depth Cue", depthcuemenu); // OpenGL/GLUT function call
	glutAddMenuEntry("Reset", RESET); // OpenGL/GLUT function call
	glutAddSubMenu("Debug", debugmenu); // OpenGL/GLUT function call
	glutAddMenuEntry("Quit", QUIT); // OpenGL/GLUT function call



	glutAttachMenu(GLUT_RIGHT_BUTTON); // OpenGL/GLUT function call
}

void // Function or helper declaration
Keyboard(unsigned char c, int x, int y) // Code line
{
	if (DebugOn != 0) // Conditional logic
		fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c); // Code line

	switch (c) // Switch-case logic
	{
	case 'w': // Switch-case logic
	case 'W': // Switch-case logic
		CurrentLightColor = LS_WHITE; // Code line
		break; // Switch-case logic

	case 'r': // Switch-case logic
	case 'R': // Switch-case logic
		CurrentLightColor = LS_RED; // Code line
		break; // Switch-case logic

	case 'o': // Switch-case logic
	case 'O': // Switch-case logic
		CurrentLightColor = LS_ORANGE; // Code line
		break; // Switch-case logic

	case 'y': // Switch-case logic
	case 'Y': // Switch-case logic
		CurrentLightColor = LS_YELLOW; // Code line
		break; // Switch-case logic

	case 'g': // Switch-case logic
	case 'G': // Switch-case logic
		CurrentLightColor = LS_GREEN; // Code line
		break; // Switch-case logic

	case 'c': // Switch-case logic
	case 'C': // Switch-case logic
		CurrentLightColor = LS_CYAN; // Code line
		break; // Switch-case logic

	case 'm': // Switch-case logic
	case 'M': // Switch-case logic
		CurrentLightColor = LS_MAGENTA; // Code line
		break; // Switch-case logic

	case 'p': // Switch-case logic
	case 'P': // Switch-case logic
		CurrentLightMode = LIGHT_POINT; // Code line
		break; // Switch-case logic

	case 's': // Switch-case logic
	case 'S': // Switch-case logic
		CurrentLightMode = LIGHT_SPOT; // Code line
		break; // Switch-case logic

	case 'q': // Switch-case logic
	case 'Q': // Switch-case logic
	case ESCAPE: // Switch-case logic
		DoMainMenu(QUIT); // Code line
		break; // Switch-case logic

	default: // Switch-case logic
		fprintf(stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c); // Code line
	}


	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutPostRedisplay(); // OpenGL/GLUT function call
}

void // Function or helper declaration
MouseButton(int button, int state, int x, int y) // Code line
{
	int b = 0; // Variable declaration

	if (DebugOn != 0) // Conditional logic
		fprintf(stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y); // Code line

	switch (button) // Switch-case logic
	{
	case GLUT_LEFT_BUTTON: // Switch-case logic
		b = LEFT;		break; // Code line

	case GLUT_MIDDLE_BUTTON: // Switch-case logic
		b = MIDDLE;		break; // Code line

	case GLUT_RIGHT_BUTTON: // Switch-case logic
		b = RIGHT;		break; // Code line

	case SCROLL_WHEEL_UP: // Switch-case logic
		Scale += SCLFACT * SCROLL_WHEEL_CLICK_FACTOR; // Code line
		if (Scale < MINSCALE) // Conditional logic
			Scale = MINSCALE; // Code line
		break; // Switch-case logic

	case SCROLL_WHEEL_DOWN: // Switch-case logic
		Scale -= SCLFACT * SCROLL_WHEEL_CLICK_FACTOR; // Code line
		if (Scale < MINSCALE) // Conditional logic
			Scale = MINSCALE; // Code line
		break; // Switch-case logic

	default: // Switch-case logic
		b = 0; // Code line
		fprintf(stderr, "Unknown mouse button: %d\n", button); // Code line
	}


	if (state == GLUT_DOWN) // Conditional logic
	{
		Xmouse = x; // Code line
		Ymouse = y; // Code line
		ActiveButton |= b; // Code line
	}
	else // Conditional logic
	{
		ActiveButton &= ~b; // Code line
	}

	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutPostRedisplay(); // OpenGL/GLUT function call

}

void // Function or helper declaration
MouseMotion(int x, int y) // Code line
{
	int dx = x - Xmouse; // Variable declaration
	int dy = y - Ymouse; // Variable declaration

	if ((ActiveButton & LEFT) != 0) // Conditional logic
	{
		Xrot += (ANGFACT * dy); // Code line
		Yrot += (ANGFACT * dx); // Code line
	}

	if ((ActiveButton & MIDDLE) != 0) // Conditional logic
	{
		Scale += SCLFACT * (float)(dx - dy); // Code line


		if (Scale < MINSCALE) // Conditional logic
			Scale = MINSCALE; // Code line
	}

	Xmouse = x; // Code line
	Ymouse = y; // Code line

	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutPostRedisplay(); // OpenGL/GLUT function call
}

void // Function or helper declaration
Reset() // Code line
{
	ActiveButton = 0; // Code line
	AxesOn = 1; // Code line
	DebugOn = 0; // Code line
	DepthBufferOn = 1; // Code line
	DepthFightingOn = 0; // Code line
	DepthCueOn = 0; // Code line
	Scale = 1.0; // Code line
	ShadowsOn = 0; // Code line
	NowColor = YELLOW; // Code line
	NowProjection = PERSP; // Code line
	Xrot = Yrot = 0.; // Code line

	NowProjection = PERSP; // Code line
	CurrentLightMode = LIGHT_POINT; // Code line
	Xrot = Yrot = 0.; // Code line
}

void // Function or helper declaration
Resize(int width, int height) // Code line
{

	glutSetWindow(MainWindow); // OpenGL/GLUT function call
	glutPostRedisplay(); // OpenGL/GLUT function call
}

void // Function or helper declaration
Visibility(int state) // Code line
{
	if (DebugOn != 0) // Conditional logic
		fprintf(stderr, "Visibility: %d\n", state); // Code line

	if (state == GLUT_VISIBLE) // Conditional logic
	{
		glutSetWindow(MainWindow); // OpenGL/GLUT function call
		glutPostRedisplay(); // OpenGL/GLUT function call
	}
	else // Conditional logic
	{
	}
}



static float xx[] = { 0.f, 1.f, 0.f, 1.f }; // Function or helper declaration
static float xy[] = { -.5f, .5f, .5f, -.5f }; // Function or helper declaration
static int xorder[] = { 1, 2, -3, 4 }; // Function or helper declaration
static float yx[] = { 0.f, 0.f, -.5f, .5f }; // Function or helper declaration
static float yy[] = { 0.f, .6f, 1.f, 1.f }; // Function or helper declaration
static int yorder[] = { 1, 2, 3, -2, 4 }; // Function or helper declaration
static float zx[] = { 1.f, 0.f, 1.f, 0.f, .25f, .75f }; // Function or helper declaration
static float zy[] = { .5f, .5f, -.5f, -.5f, 0.f, 0.f }; // Function or helper declaration
static int zorder[] = { 1, 2, 3, 4, -5, 6 }; // Function or helper declaration
const float LENFRAC = 0.10f; // Constant definition
const float BASEFRAC = 1.10f; // Constant definition

void // Function or helper declaration
Axes(float length) // Code line
{
	glBegin(GL_LINE_STRIP); // OpenGL/GLUT function call
	glVertex3f(length, 0., 0.); // OpenGL/GLUT function call
	glVertex3f(0., 0., 0.); // OpenGL/GLUT function call
	glVertex3f(0., length, 0.); // OpenGL/GLUT function call
	glEnd(); // OpenGL/GLUT function call
	glBegin(GL_LINE_STRIP); // OpenGL/GLUT function call
	glVertex3f(0., 0., 0.); // OpenGL/GLUT function call
	glVertex3f(0., 0., length); // OpenGL/GLUT function call
	glEnd(); // OpenGL/GLUT function call

	float fact = LENFRAC * length; // Variable declaration
	float base = BASEFRAC * length; // Variable declaration

	glBegin(GL_LINE_STRIP); // OpenGL/GLUT function call
	for (int i = 0; i < 4; i++) // Loop statement
	{
		int j = xorder[i]; // Variable declaration
		if (j < 0) // Conditional logic
		{

			glEnd(); // OpenGL/GLUT function call
			glBegin(GL_LINE_STRIP); // OpenGL/GLUT function call
			j = -j; // Code line
		}
		j--; // Code line
		glVertex3f(base + fact * xx[j], fact * xy[j], 0.0); // OpenGL/GLUT function call
	}
	glEnd(); // OpenGL/GLUT function call

	glBegin(GL_LINE_STRIP); // OpenGL/GLUT function call
	for (int i = 0; i < 5; i++) // Loop statement
	{
		int j = yorder[i]; // Variable declaration
		if (j < 0) // Conditional logic
		{

			glEnd(); // OpenGL/GLUT function call
			glBegin(GL_LINE_STRIP); // OpenGL/GLUT function call
			j = -j; // Code line
		}
		j--; // Code line
		glVertex3f(fact * yx[j], base + fact * yy[j], 0.0); // OpenGL/GLUT function call
	}
	glEnd(); // OpenGL/GLUT function call

	glBegin(GL_LINE_STRIP); // OpenGL/GLUT function call
	for (int i = 0; i < 6; i++) // Loop statement
	{
		int j = zorder[i]; // Variable declaration
		if (j < 0) // Conditional logic
		{

			glEnd(); // OpenGL/GLUT function call
			glBegin(GL_LINE_STRIP); // OpenGL/GLUT function call
			j = -j; // Code line
		}
		j--; // Code line
		glVertex3f(0.0, fact * zy[j], base + fact * zx[j]); // OpenGL/GLUT function call
	}
	glEnd(); // OpenGL/GLUT function call

}

void // Function or helper declaration
HsvRgb(float hsv[3], float rgb[3]) // Code line
{

	float h = hsv[0] / 60.f; // Variable declaration
	while (h >= 6.)	h -= 6.; // Loop statement
	while (h < 0.) 	h += 6.; // Loop statement

	float s = hsv[1]; // Variable declaration
	if (s < 0.) // Conditional logic
		s = 0.; // Code line
	if (s > 1.) // Conditional logic
		s = 1.; // Code line

	float v = hsv[2]; // Variable declaration
	if (v < 0.) // Conditional logic
		v = 0.; // Code line
	if (v > 1.) // Conditional logic
		v = 1.; // Code line


	if (s == 0.0) // Conditional logic
	{
		rgb[0] = rgb[1] = rgb[2] = v; // Code line
		return; // Return statement
	}


	float i = (float)floor(h); // Variable declaration
	float f = h - i; // Variable declaration
	float p = v * (1.f - s); // Variable declaration
	float q = v * (1.f - s * f); // Variable declaration
	float t = v * (1.f - (s * (1.f - f))); // Variable declaration

	float r = 0., g = 0., b = 0.; // Variable declaration
	switch ((int)i) // Switch-case logic
	{
	case 0: // Switch-case logic
		r = v;	g = t;	b = p; // Code line
		break; // Switch-case logic

	case 1: // Switch-case logic
		r = q;	g = v;	b = p; // Code line
		break; // Switch-case logic

	case 2: // Switch-case logic
		r = p;	g = v;	b = t; // Code line
		break; // Switch-case logic

	case 3: // Switch-case logic
		r = p;	g = q;	b = v; // Code line
		break; // Switch-case logic

	case 4: // Switch-case logic
		r = t;	g = p;	b = v; // Code line
		break; // Switch-case logic

	case 5: // Switch-case logic
		r = v;	g = p;	b = q; // Code line
		break; // Switch-case logic
	}


	rgb[0] = r; // Code line
	rgb[1] = g; // Code line
	rgb[2] = b; // Code line
}

void // Function or helper declaration
Cross(float v1[3], float v2[3], float vout[3]) // Code line
{
	float tmp[3]; // Variable declaration
	tmp[0] = v1[1] * v2[2] - v2[1] * v1[2]; // Code line
	tmp[1] = v2[0] * v1[2] - v1[0] * v2[2]; // Code line
	tmp[2] = v1[0] * v2[1] - v2[0] * v1[1]; // Code line
	vout[0] = tmp[0]; // Code line
	vout[1] = tmp[1]; // Code line
	vout[2] = tmp[2]; // Code line
}

float // Variable declaration
Dot(float v1[3], float v2[3]) // Code line
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]; // Return statement
}


float // Variable declaration
Unit(float vin[3], float vout[3]) // Code line
{
	float dist = vin[0] * vin[0] + vin[1] * vin[1] + vin[2] * vin[2]; // Variable declaration
	if (dist > 0.0) // Conditional logic
	{
		dist = sqrtf(dist); // Code line
		vout[0] = vin[0] / dist; // Code line
		vout[1] = vin[1] / dist; // Code line
		vout[2] = vin[2] / dist; // Code line
	}
	else // Conditional logic
	{
		vout[0] = vin[0]; // Code line
		vout[1] = vin[1]; // Code line
		vout[2] = vin[2]; // Code line
	}
	return dist; // Return statement
}

float // Variable declaration
Unit(float v[3]) // Code line
{
	float dist = v[0] * v[0] + v[1] * v[1] + v[2] * v[2]; // Variable declaration
	if (dist > 0.0) // Conditional logic
	{
		dist = sqrtf(dist); // Code line
		v[0] /= dist; // Code line
		v[1] /= dist; // Code line
		v[2] /= dist; // Code line
	}
	return dist; // Return statement
}
