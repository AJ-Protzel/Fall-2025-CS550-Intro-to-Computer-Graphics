#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <ctype.h>
#include <time.h>

#ifndef F_PI
#define F_PI		((float)(M_PI))
#define F_2_PI		((float)(2.f*F_PI))
#define F_PI_2		((float)(F_PI/2.f))
#endif

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include "glut.h"
#include <cstdlib>
#include <ctime>

const char* WINDOWTITLE = "OpenGL / GLUT Sample -- Adrien Protzel";
const char* GLUITITLE = "User Interface Window";
const int GLUITRUE = true;
const int GLUIFALSE = false;
const int ESCAPE = 0x1b;
const int INIT_WINDOW_SIZE = 1000;
const float BOXSIZE = 2.f;
const float ANGFACT = 1.f;
const float SCLFACT = 0.005f;
const float MINSCALE = 0.05f;
const int SCROLL_WHEEL_UP = 3;
const int SCROLL_WHEEL_DOWN = 4;
const float SCROLL_WHEEL_CLICK_FACTOR = 5.f;
const int LEFT = 4;
const int MIDDLE = 2;
const int RIGHT = 1;

enum Projections
{
	ORTHO,
	PERSP
};

enum ButtonVals
{
	RESET,
	QUIT
};

const GLfloat BACKCOLOR[] = { 0., 0., 0., 1. };
const GLfloat AXES_WIDTH = 3.;

enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA
};

char* ColorNames[] =
{
(char*)"Red",
(char*)"Yellow",
(char*)"Green",
(char*)"Cyan",
(char*)"Blue",
(char*)"Magenta"
};

const GLfloat Colors[][3] =
{
{ 1., 0., 0. },
{ 1., 1., 0. },
{ 0., 1., 0. },
{ 0., 1., 1. },
{ 0., 0., 1. },
{ 1., 0., 1. },
};

const GLfloat FOGCOLOR[4] = { .0f, .0f, .0f, 1.f };
const GLenum  FOGMODE = GL_LINEAR;
const GLfloat FOGDENSITY = 0.30f;
const GLfloat FOGSTART = 1.5f;
const GLfloat FOGEND = 4.f;

const float	WHITE[] = { 1.,1.,1.,1. };
const int MS_PER_CYCLE = 10000;

enum LightMode
{
	LIGHT_POINT,
	LIGHT_SPOT
};
int CurrentLightMode = LIGHT_POINT;

const float ROOM_XSIDE = 10.0f;
const float ROOM_ZSIDE = 10.0f;
const float ROOM_X0 = -ROOM_XSIDE / 2.0f;
const float ROOM_Z0 = -ROOM_ZSIDE / 2.0f;

enum LightColor
{
	LS_WHITE,
	LS_RED,
	LS_ORANGE,
	LS_YELLOW,
	LS_GREEN,
	LS_CYAN,
	LS_MAGENTA
};

int CurrentLightColor = LS_WHITE;

const GLfloat LightColors[][4] =
{
{ 1.0f, 1.0f, 1.0f, 1.f },
{ 1.0f, 0.0f, 0.0f, 1.f },
{ 1.0f, 0.5f, 0.0f, 1.f },
{ 1.0f, 1.0f, 0.0f, 1.f },
{ 0.0f, 1.0f, 0.0f, 1.f },
{ 0.0f, 1.0f, 1.0f, 1.f },
{ 1.0f, 0.0f, 1.0f, 1.f }
};

int		ActiveButton;
int		AxesOn;
int		DebugOn;
int		DepthCueOn;
int		DepthBufferOn;
int		DepthFightingOn;
int		MainWindow;
int		NowColor;
int		NowProjection;
float	Scale;
int		ShadowsOn;
float	Time;
int		Xmouse, Ymouse;
float	Xrot, Yrot;

GLuint SolidHorseList;
GLuint GridDL;
GLuint	CircleList;
GLuint	AxesList;
GLuint	TreeList;

GLuint OsuSphereList, OsuCubeList, OsuCylinderList, OsuConeList, OsuTorusList, OsuObjList;
GLuint OsuSphereTex, OsuCubeTex, OsuCylinderTex, OsuConeTex, OsuTorusTex, OsuObjTex;



float gLightBaseY = 1.2f;
float gLightAmpY = 1.0f;

int CurrentObject = 1;
bool TexMode = TRUE;

void	Animate();
void	Display();
void	DoAxesMenu(int);
void	DoColorMenu(int);
void	DoDepthBufferMenu(int);
void	DoDepthFightingMenu(int);
void	DoDepthMenu(int);
void	DoDebugMenu(int);
void DoViewMenu(int);
void	DoMainMenu(int);
void	DoProjectMenu(int);
void	DoRasterString(float, float, float, char*);
void	DoStrokeString(float, float, float, float, char*);
float	ElapsedSeconds();
void	InitGraphics();
void	InitLists();
void	InitMenus();
void	Keyboard(unsigned char, int, int);
void	MouseButton(int, int, int, int);
void	MouseMotion(int, int);
void	Reset();
void	Resize(int, int);
void	Visibility(int);
void			Axes(float);
void			HsvRgb(float[3], float[3]);
void			Cross(float[3], float[3], float[3]);
float			Dot(float[3], float[3]);
float			Unit(float[3], float[3]);
float			Unit(float[3]);

float*
Array3(float a, float b, float c)
{
	static float array[4];
	array[0] = a;
	array[1] = b;
	array[2] = c;
	array[3] = 1.;
	return array;
}

float*
MulArray3(float factor, float array0[])
{
	static float array[4];
	array[0] = factor * array0[0];
	array[1] = factor * array0[1];
	array[2] = factor * array0[2];
	array[3] = 1.;
	return array;
}

float*
MulArray3(float factor, float a, float b, float c)
{
	static float array[4];
	float* abc = Array3(a, b, c);
	array[0] = factor * abc[0];
	array[1] = factor * abc[1];
	array[2] = factor * abc[2];
	array[3] = 1.;
	return array;
}

float
Ranf(float low, float high)
{
	float r = (float)rand();
	float t = r / (float)RAND_MAX;
	return   low + t * (high - low);
}

void
TimeOfDaySeed()
{
	struct tm y2k;
	y2k.tm_hour = 0;    y2k.tm_min = 0; y2k.tm_sec = 0;
	y2k.tm_year = 2000; y2k.tm_mon = 0; y2k.tm_mday = 1;
	time_t  now;
	time(&now);
	double seconds = difftime(now, mktime(&y2k));
	unsigned int seed = (unsigned int)(1000. * seconds);
	srand(seed);
}

#include "setmaterial.cpp"
#include "setlight.cpp"
#include "osusphere.cpp"
#include "osucube.cpp"
#include "osucylindercone.cpp"
#include "osutorus.cpp"
#include "bmptotexture.cpp"
#include "loadobjmtlfiles.cpp"
#include "keytime.cpp"
#include "glslprogram.cpp"

#include "vertexbufferobject.cpp"

#include "CarouselHorse0.10.550.cpp"
const int MSEC = MS_PER_CYCLE;
Keytimes Xpos, Ypos, Zpos;
Keytimes ThetaX, ThetaY, ThetaZ;
#define NUM_HORSES 3
#define RING_RADIUS 3.f
GLuint SolidHorseLists[NUM_HORSES];
Keytimes HXpos[NUM_HORSES], HYpos[NUM_HORSES], HZpos[NUM_HORSES];
Keytimes HThetaX[NUM_HORSES], HThetaY[NUM_HORSES], HThetaZ[NUM_HORSES];

GLuint FaceDL = 0;
GLuint SkeletonDL = 0;
GLuint LightSphere;
Keytimes LightThetaY;

GLuint SnakeList, SnakeTex;
GLSLProgram Snake; 

GLuint DinoList, DinoTex;
GLSLProgram Dino; // your VS+FS shader program name
int gLitOn = 1;         // lighting toggle
int gSilhouetteOn = 1;  // silhouette toggle
float gTol = 0.15f; 
const float kTolMin = 0.00f;
const float kTolMax = 1.00f;
const float kTolStep = 0.02f;



#define NUM_FALLING_LEAVES 60
Keytimes EyeX, EyeY, EyeZ;

struct FallingLeaf
{
	Keytimes X, Y, Z;
	Keytimes Yaw, Pitch, Roll, Scale;
	float    duration;
	float    phase;
};

FallingLeaf gFalling[NUM_FALLING_LEAVES];
GLuint      FallingLeafDL = 0;
int         FallingLeavesOn = 1;


void BuildFallingList();
void drawFalling(float nowSecs);

int
main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	InitGraphics();
	InitLists();
	Reset();
	InitMenus();
	glutSetWindow(MainWindow);
	glutMainLoop();
	return 0;
}

void
Animate()
{
	int ms = glutGet(GLUT_ELAPSED_TIME);
	ms %= MS_PER_CYCLE;
	Time = (float)ms / (float)MS_PER_CYCLE;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void ConfigureLight0(float lx, float ly, float lz)
{
	glEnable(GL_LIGHTING);
	const GLfloat* C = LightColors[CurrentLightColor];

	if (CurrentLightMode == LIGHT_POINT)
	{
		SetPointLight(GL_LIGHT0, lx, ly, lz, C[0], C[1], C[2]);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
	}
	else
	{
		float dir[3] = { ROOM_X0 - lx, 0.0f, 0.0f - lz };
		Unit(dir);
		SetSpotLight(GL_LIGHT0, lx, ly, lz, dir[0], dir[1], dir[2], C[0], C[1], C[2]);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 22.5f);
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0f);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
	}
}






void Display()
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting Display.\n");

	// Set rendering target and clear buffers
	glutSetWindow(MainWindow);
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// === Projection Setup ===
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.f, 1.f, 0.1f, 1000.f);  // Perspective view

	// === Camera Setup ===
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(3.f, 3.f, 9.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);  // Eye position

	// === Scene Transformations ===
	glRotatef((GLfloat)Yrot, 0.f, 1.f, 0.f);  // Y-axis rotation
	glRotatef((GLfloat)Xrot, 1.f, 0.f, 0.f);  // X-axis rotation
	if (Scale < MINSCALE) Scale = MINSCALE;
	glScalef((GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale);  // Zoom

	// === Fog Setup ===
	if (DepthCueOn != 0)
	{
		glFogi(GL_FOG_MODE, FOGMODE);
		glFogfv(GL_FOG_COLOR, FOGCOLOR);
		glFogf(GL_FOG_DENSITY, FOGDENSITY);
		glFogf(GL_FOG_START, FOGSTART);
		glFogf(GL_FOG_END, FOGEND);
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
	}

	// === Lighting Setup ===
	int msec = glutGet(GLUT_ELAPSED_TIME) % MSEC;
	float nowSecs = (float)msec / 1000.f;

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_COLOR_MATERIAL);
	//glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//// === Light Source Position Calculation ===
	//float angleDeg = LightThetaY.GetValue(nowSecs);
	//float angleRad = angleDeg * (M_PI / 180.0f);
	//const float LightRadius = 5.0f;
	//float LightX = LightRadius * sin(angleRad);
	//float LightY = LightRadius * cos(angleRad);
	//float LightZ = 0.0f;


	const float LightX = 15.0f;  // pick a fixed X
	const float LightY = 15.0f;  // pick a fixed Y
	const float LightZ = 15.0f;  // pick a fixed Z

	ConfigureLight0(LightX, LightY, LightZ);

	// === Object Rendering ===
	if (TexMode == TRUE)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);




	Dino.Use();

	// Vertex shader uniforms (unchanged)
	Dino.SetUniformVariable("uLightPos", LightX, LightY, LightZ);

	// Fragment shader uniforms — use homework names:
	Dino.SetUniformVariable("uSurface", (bool)gLitOn);            // solid surface on/off
	Dino.SetUniformVariable("uSilh", (bool)gSilhouetteOn);     // silhouette on/off
	Dino.SetUniformVariable("uTol", 0.15f);                   // silhouette tolerance

	// If you use the core version with uniforms for colors & coefficients:
	Dino.SetUniformVariable("uBaseColor", 0.93f, 0.90f, 0.85f);
	Dino.SetUniformVariable("uSpecularColor", 0.95f, 0.95f, 0.95f);
	Dino.SetUniformVariable("uSilhouetteColor", 0.0f, 0.2f, 1.0f);
	Dino.SetUniformVariable("uKa", 0.15f);
	Dino.SetUniformVariable("uKd", 0.60f);
	Dino.SetUniformVariable("uKs", 0.15f);
	Dino.SetUniformVariable("uShininess", 10.0f);

	// Clamp for safety (in case anything else modifies gTol):
	if (gTol < kTolMin) gTol = kTolMin;
	if (gTol > kTolMax) gTol = kTolMax;

	// Send the tolerance to the fragment shader
	Dino.SetUniformVariable("uTol", gTol);


	glCallList(DinoList);
	Dino.UnUse();










	glDisable(GL_TEXTURE_2D);


	// === Axes Rendering ===
	glDisable(GL_LIGHTING);
	const GLfloat* C = LightColors[CurrentLightColor];
	glColor3f(C[0], C[1], C[2]);

	if (AxesOn)
		glCallList(AxesList);

	//// === Light Sphere Rendering ===
	//glPushMatrix();
	//glTranslatef(LightX, LightY, LightZ);
	////glutSolidSphere(0.08, 20, 20);
	//glPopMatrix();

	// === 2D Overlay Setup ===
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.f, 100.f, 0.f, 100.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1.f, 1.f, 1.f);

	// === Finalize Frame ===
	glutSwapBuffers();
	glFlush();
}





void DoAxesMenu(int id)
{
	AxesOn = id;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void DoColorMenu(int id)
{
	NowColor = id - RED;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void DoDebugMenu(int id)
{
	DebugOn = id;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void DoDepthBufferMenu(int id)
{
	DepthBufferOn = id;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void DoDepthFightingMenu(int id)
{
	DepthFightingOn = id;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void DoDepthMenu(int id)
{
	DepthCueOn = id;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void DoMainMenu(int id)
{
	switch (id)
	{
	case RESET:
		Reset();
		break;

	case QUIT:
		glutSetWindow(MainWindow);
		glFinish();
		glutDestroyWindow(MainWindow);
		exit(0);
		break;

	default:
		fprintf(stderr, "Don't know what to do with Main Menu ID %d\n", id);
	}

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void DoProjectMenu(int id)
{
	NowProjection = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void DoViewMenu(int id)
{
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void DoRasterString(float x, float y, float z, char* s)
{
	glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);

	char c;
	for (; (c = *s) != '\0'; s++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
}

void DoStrokeString(float x, float y, float z, float ht, char* s)
{
	glPushMatrix();
	glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)z);
	float sf = ht / (119.05f + 33.33f);
	glScalef((GLfloat)sf, (GLfloat)sf, (GLfloat)sf);
	char c;
	for (; (c = *s) != '\0'; s++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
	}
	glPopMatrix();
}

float ElapsedSeconds()
{
	int ms = glutGet(GLUT_ELAPSED_TIME);
	return (float)ms / 1000.f;
}

void LoadTexture(const char* filepath, GLuint* textureID) {
	int width, height;
	char* file = (char*)filepath;
	unsigned char* texture = BmpToTexture(file, &width, &height);
	if (texture == NULL)
		fprintf(stderr, "Cannot open texture '%s'\n", file);
	else
		fprintf(stderr, "Opened '%s': width = %d ; height = %d\n", file, width, height);
	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, *textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
}

void
InitGraphics()
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting InitGraphics.\n");

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(INIT_WINDOW_SIZE, INIT_WINDOW_SIZE);

	MainWindow = glutCreateWindow(WINDOWTITLE);
	glutSetWindowTitle(WINDOWTITLE);

	glClearColor(BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3]);

	glutSetWindow(MainWindow);
	glutDisplayFunc(Display);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MouseMotion);
	glutVisibilityFunc(Visibility);
	glutEntryFunc(NULL);
	glutSpecialFunc(NULL);
	glutSpaceballMotionFunc(NULL);
	glutSpaceballRotateFunc(NULL);
	glutSpaceballButtonFunc(NULL);
	glutButtonBoxFunc(NULL);
	glutDialsFunc(NULL);
	glutTabletMotionFunc(NULL);
	glutTabletButtonFunc(NULL);
	glutMenuStateFunc(NULL);
	glutTimerFunc(-1, NULL, 0);
	glutIdleFunc(Animate);

#ifdef WIN32
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "glewInit Error\n");
	}
	else
		fprintf(stderr, "GLEW initialized OK\n");
	fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif



	for (int i = 0; i < NUM_HORSES; i++) {
		HYpos[i].Init();
		HYpos[i].AddTimeValue(0.0f, 0.0f);
		HYpos[i].AddTimeValue(2.0f, 0.5f);
		HYpos[i].AddTimeValue(4.0f, 0.0f);
		HYpos[i].AddTimeValue(6.0f, -0.5f);
		HYpos[i].AddTimeValue(8.0f, 0.0f);
		HYpos[i].AddTimeValue(10.0f, 0.0f);

		HThetaY[i].Init();
		HThetaY[i].AddTimeValue(0.0f, 0.0f);
		HThetaY[i].AddTimeValue(2.0f, -72.0f);
		HThetaY[i].AddTimeValue(4.0f, -144.0f);
		HThetaY[i].AddTimeValue(6.0f, -216.0f);
		HThetaY[i].AddTimeValue(8.0f, -288.0f);
		HThetaY[i].AddTimeValue(10.0f, -360.0f);
		HThetaX[i].Init();
		HThetaX[i].AddTimeValue(0.0f, 0.0f);
		HThetaX[i].AddTimeValue(2.0f, 15.0f);
		HThetaX[i].AddTimeValue(4.0f, -15.0f);
		HThetaX[i].AddTimeValue(6.0f, 15.0f);
		HThetaX[i].AddTimeValue(8.0f, -15.0f);
		HThetaX[i].AddTimeValue(10.0f, 0.0f);
	}



	LightThetaY.Init();
	LightThetaY.AddTimeValue(0.0f, 0.0f);
	LightThetaY.AddTimeValue(10.0f, -360.0f);


	for (int i = 0; i < NUM_FALLING_LEAVES; ++i) {
		auto& L = gFalling[i];
		float startY = Ranf(3.0f, 5.5f), groundY = -5.0f;
		float startX = Ranf(-8.0f, 8.0f), startZ = Ranf(-8.0f, 8.0f);
		float endX = startX + Ranf(-2.0f, 2.0f), endZ = startZ + Ranf(-2.5f, 2.5f);
		float s = Ranf(0.3f, 0.6f), yaw0 = Ranf(0.f, 360.f);

		L.X.Init(); L.Y.Init(); L.Z.Init();
		L.Yaw.Init(); L.Pitch.Init(); L.Roll.Init(); L.Scale.Init();


		L.Y.AddTimeValue(0.0f, startY);
		L.Y.AddTimeValue(2.0f, startY * 0.8f + groundY * 0.2f);
		L.Y.AddTimeValue(4.0f, startY * 0.6f + groundY * 0.4f);
		L.Y.AddTimeValue(6.0f, startY * 0.4f + groundY * 0.6f);
		L.Y.AddTimeValue(8.0f, startY * 0.2f + groundY * 0.8f);
		L.Y.AddTimeValue(10.0f, startY);


		for (int j = 0; j <= 5; ++j) {
			float t = j * 2.0f;
			float fx = (1.0f - j / 5.0f) * startX + (j / 5.0f) * endX + Ranf(-1.0f, 1.0f);
			float fz = (1.0f - j / 5.0f) * startZ + (j / 5.0f) * endZ + Ranf(-1.0f, 1.0f);
			L.X.AddTimeValue(t, fx);
			L.Z.AddTimeValue(t, fz);
		}
		L.X.AddTimeValue(10.0f, startX);
		L.Z.AddTimeValue(10.0f, startZ);


		L.Roll.AddTimeValue(0.0f, 0.0f);
		L.Roll.AddTimeValue(10.0f, 0.0f);

		L.Pitch.AddTimeValue(0.0f, Ranf(-10.f, 10.f));
		L.Pitch.AddTimeValue(2.5f, 25.f);
		L.Pitch.AddTimeValue(5.0f, -25.f);
		L.Pitch.AddTimeValue(7.5f, 18.f);
		L.Pitch.AddTimeValue(10.0f, Ranf(-10.f, 10.f));

		L.Yaw.AddTimeValue(0.0f, yaw0);
		L.Yaw.AddTimeValue(10.0f, yaw0);


		L.Scale.AddTimeValue(0.0f, s);
		L.Scale.AddTimeValue(10.0f, s);

		L.duration = 10.0f;
		L.phase = Ranf(0.f, 10.0f);
	}


	EyeX.Init();
	EyeX.AddTimeValue(0.0f, 0.0f);
	EyeX.AddTimeValue(2.0f, 1.0f);
	EyeX.AddTimeValue(4.0f, -3.0f);
	EyeX.AddTimeValue(6.0f, -7.0f);
	EyeX.AddTimeValue(8.0f, -5.0f);
	EyeX.AddTimeValue(10.0f, 0.0f);

	EyeY.Init();
	EyeY.AddTimeValue(0.0f, 0.0f);
	EyeY.AddTimeValue(2.0f, 1.5f);
	EyeY.AddTimeValue(4.0f, 1.0f);
	EyeY.AddTimeValue(6.0f, 2.5f);
	EyeY.AddTimeValue(8.0f, 7.0f);
	EyeY.AddTimeValue(10.0f, 0.0f);

	EyeZ.Init();
	EyeZ.AddTimeValue(0.0f, 20.0f);
	EyeZ.AddTimeValue(2.0f, 12.5f);
	EyeZ.AddTimeValue(4.0f, 12.0f);
	EyeZ.AddTimeValue(6.0f, 12.5f);
	EyeZ.AddTimeValue(8.0f, 23.0f);
	EyeZ.AddTimeValue(10.0f, 20.0f);




	Dino.Init();
	bool valid = Dino.Create("PigInPython.vert", "PigInPython.frag");
	if (!valid)
	{
		fprintf(stderr, "Yuch!The Dino shader did not compile.\n");
	}
	else
	{
		fprintf(stderr, "Woo - Hoo!The Dino shader compiled.\n");
	}


}











void BuildDinoList()
{
	GLuint s_DinolGeo = LoadObjMtlFiles((char*)"dino.obj");
	DinoList = glGenLists(1);
	glNewList(DinoList, GL_COMPILE);
	SetMaterial(0.93f, 0.90f, 0.85f, 30.0f);
	glBindTexture(GL_TEXTURE_2D, DinoTex);
	glPushMatrix();
	glCallList(s_DinolGeo);
	glPopMatrix();
	glEndList();
}


void InitLists()
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting InitLists.\n");

	glutSetWindow(MainWindow);



	BuildDinoList();


}





























void
InitMenus()
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting InitMenus.\n");

	glutSetWindow(MainWindow);

	int numColors = sizeof(Colors) / (3 * sizeof(float));
	int colormenu = glutCreateMenu(DoColorMenu);
	for (int i = 0; i < numColors; i++)
	{
		glutAddMenuEntry(ColorNames[i], i);
	}

	int axesmenu = glutCreateMenu(DoAxesMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int depthcuemenu = glutCreateMenu(DoDepthMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int depthbuffermenu = glutCreateMenu(DoDepthBufferMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int depthfightingmenu = glutCreateMenu(DoDepthFightingMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int debugmenu = glutCreateMenu(DoDebugMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int mainmenu = glutCreateMenu(DoMainMenu);
	glutAddSubMenu("Axes", axesmenu);
	glutAddSubMenu("Axis Colors", colormenu);


#ifdef DEMO_DEPTH_BUFFER
	glutAddSubMenu("Depth Buffer", depthbuffermenu);
#endif

#ifdef DEMO_Z_FIGHTING
	glutAddSubMenu("Depth Fighting", depthfightingmenu);
#endif

	glutAddSubMenu("Depth Cue", depthcuemenu);
	glutAddMenuEntry("Reset", RESET);
	glutAddSubMenu("Debug", debugmenu);
	glutAddMenuEntry("Quit", QUIT);



	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void
Keyboard(unsigned char c, int x, int y)
{
	if (DebugOn != 0)
		fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);

	switch (c)
	{
	case 'f':
	case 'F':
		FallingLeavesOn = !FallingLeavesOn;
		break;

	case 'w':
	case 'W':
		CurrentLightColor = LS_WHITE;
		break;

	case 'r':
	case 'R':
		CurrentLightColor = LS_RED;
		break;

	case 'o':
	case 'O':
		CurrentLightColor = LS_ORANGE;
		break;

	case 'y':
	case 'Y':
		CurrentLightColor = LS_YELLOW;
		break;

	case 'g':
	case 'G':
		CurrentLightColor = LS_GREEN;
		break;

	case 'c':
	case 'C':
		CurrentLightColor = LS_CYAN;
		break;

	case 'm':
	case 'M':
		CurrentLightColor = LS_MAGENTA;
		break;

	//case 'p':
	//case 'P':
	//	CurrentLightMode = LIGHT_POINT;
	//	break;

	//case 's':
	//case 'S':
	//	CurrentLightMode = LIGHT_SPOT;
	//	break;

	case 'q':
	case 'Q':
	case ESCAPE:
		DoMainMenu(QUIT);
		break;

	//case '1': CurrentObject = 1; break;
	//case '2': CurrentObject = 2; break;
	//case '3': CurrentObject = 3; break;
	//case '4': CurrentObject = 4; break;
	//case '5': CurrentObject = 5; break;
	//case '6': CurrentObject = 6; break;

	//case 't':
	//case 'T':
	//	TexMode = !TexMode;
	//	break;


	case 'l': case 'L': gLitOn = !gLitOn; break;              // toggle lighting
	case 's': case 'S': gSilhouetteOn = !gSilhouetteOn; break; // toggle silhouette


	case '>':    // increase tolerance
		gTol += kTolStep;
		if (gTol > kTolMax) gTol = kTolMax;
		fprintf(stderr, "uTol = %.3f\n", gTol);
		break;

	case '<':    // decrease tolerance
		gTol -= kTolStep;
		if (gTol < kTolMin) gTol = kTolMin;
		fprintf(stderr, "uTol = %.3f\n", gTol);
		break;




	default:
		fprintf(stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c);
	}


	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void
MouseButton(int button, int state, int x, int y)
{
	int b = 0;

	if (DebugOn != 0)
		fprintf(stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y);

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		b = LEFT;		break;

	case GLUT_MIDDLE_BUTTON:
		b = MIDDLE;		break;

	case GLUT_RIGHT_BUTTON:
		b = RIGHT;		break;

	case SCROLL_WHEEL_UP:
		Scale += SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
		if (Scale < MINSCALE)
			Scale = MINSCALE;
		break;

	case SCROLL_WHEEL_DOWN:
		Scale -= SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
		if (Scale < MINSCALE)
			Scale = MINSCALE;
		break;

	default:
		b = 0;
		fprintf(stderr, "Unknown mouse button: %d\n", button);
	}


	if (state == GLUT_DOWN)
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;
	}
	else
	{
		ActiveButton &= ~b;
	}

	glutSetWindow(MainWindow);
	glutPostRedisplay();

}

void
MouseMotion(int x, int y)
{
	int dx = x - Xmouse;
	int dy = y - Ymouse;

	if ((ActiveButton & LEFT) != 0)
	{
		Xrot += (ANGFACT * dy);
		Yrot += (ANGFACT * dx);
	}

	if ((ActiveButton & MIDDLE) != 0)
	{
		Scale += SCLFACT * (float)(dx - dy);


		if (Scale < MINSCALE)
			Scale = MINSCALE;
	}

	Xmouse = x;
	Ymouse = y;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void
Reset()
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	DepthBufferOn = 1;
	DepthFightingOn = 0;
	DepthCueOn = 0;
	Scale = 1.0;
	ShadowsOn = 0;
	NowColor = YELLOW;
	NowProjection = PERSP;
	Xrot = Yrot = 0.;

	NowProjection = PERSP;
	CurrentLightMode = LIGHT_POINT;
	Xrot = Yrot = 0.;
}

void
Resize(int width, int height)
{

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void
Visibility(int state)
{
	if (DebugOn != 0)
		fprintf(stderr, "Visibility: %d\n", state);

	if (state == GLUT_VISIBLE)
	{
		glutSetWindow(MainWindow);
		glutPostRedisplay();
	}
	else
	{
	}
}



static float xx[] = { 0.f, 1.f, 0.f, 1.f };
static float xy[] = { -.5f, .5f, .5f, -.5f };
static int xorder[] = { 1, 2, -3, 4 };
static float yx[] = { 0.f, 0.f, -.5f, .5f };
static float yy[] = { 0.f, .6f, 1.f, 1.f };
static int yorder[] = { 1, 2, 3, -2, 4 };
static float zx[] = { 1.f, 0.f, 1.f, 0.f, .25f, .75f };
static float zy[] = { .5f, .5f, -.5f, -.5f, 0.f, 0.f };
static int zorder[] = { 1, 2, 3, 4, -5, 6 };
const float LENFRAC = 0.10f;
const float BASEFRAC = 1.10f;

void
Axes(float length)
{
	glBegin(GL_LINE_STRIP);
	glVertex3f(length, 0., 0.);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., length, 0.);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., 0., length);
	glEnd();

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 4; i++)
	{
		int j = xorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(base + fact * xx[j], fact * xy[j], 0.0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 5; i++)
	{
		int j = yorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(fact * yx[j], base + fact * yy[j], 0.0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 6; i++)
	{
		int j = zorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(0.0, fact * zy[j], base + fact * zx[j]);
	}
	glEnd();

}

void
HsvRgb(float hsv[3], float rgb[3])
{

	float h = hsv[0] / 60.f;
	while (h >= 6.)	h -= 6.;
	while (h < 0.) 	h += 6.;

	float s = hsv[1];
	if (s < 0.)
		s = 0.;
	if (s > 1.)
		s = 1.;

	float v = hsv[2];
	if (v < 0.)
		v = 0.;
	if (v > 1.)
		v = 1.;


	if (s == 0.0)
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}


	float i = (float)floor(h);
	float f = h - i;
	float p = v * (1.f - s);
	float q = v * (1.f - s * f);
	float t = v * (1.f - (s * (1.f - f)));

	float r = 0., g = 0., b = 0.;
	switch ((int)i)
	{
	case 0:
		r = v;	g = t;	b = p;
		break;

	case 1:
		r = q;	g = v;	b = p;
		break;

	case 2:
		r = p;	g = v;	b = t;
		break;

	case 3:
		r = p;	g = q;	b = v;
		break;

	case 4:
		r = t;	g = p;	b = v;
		break;

	case 5:
		r = v;	g = p;	b = q;
		break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

void
Cross(float v1[3], float v2[3], float vout[3])
{
	float tmp[3];
	tmp[0] = v1[1] * v2[2] - v2[1] * v1[2];
	tmp[1] = v2[0] * v1[2] - v1[0] * v2[2];
	tmp[2] = v1[0] * v2[1] - v2[0] * v1[1];
	vout[0] = tmp[0];
	vout[1] = tmp[1];
	vout[2] = tmp[2];
}

float
Dot(float v1[3], float v2[3])
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}


float
Unit(float vin[3], float vout[3])
{
	float dist = vin[0] * vin[0] + vin[1] * vin[1] + vin[2] * vin[2];
	if (dist > 0.0)
	{
		dist = sqrtf(dist);
		vout[0] = vin[0] / dist;
		vout[1] = vin[1] / dist;
		vout[2] = vin[2] / dist;
	}
	else
	{
		vout[0] = vin[0];
		vout[1] = vin[1];
		vout[2] = vin[2];
	}
	return dist;
}

float
Unit(float v[3])
{
	float dist = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	if (dist > 0.0)
	{
		dist = sqrtf(dist);
		v[0] /= dist;
		v[1] /= dist;
		v[2] /= dist;
	}
	return dist;
}
