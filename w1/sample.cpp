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

#include <cstdlib>   // for rand(), srand()
#include <ctime>     // for time()



//	This is a sample OpenGL / GLUT program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with a glut menu
//
//	The left mouse button does rotation
//	The middle mouse button does scaling
//	The user interface allows:
//		1. The axes to be turned on and off
//		2. The color of the axes to be changed
//		3. Debugging to be turned on and off
//		4. Depth cueing to be turned on and off
//		5. The projection to be changed
//		6. The transformations to be reset
//		7. The program to quit
//
//	Author:			Adrien Protzel

// title of these windows:

const char *WINDOWTITLE = "OpenGL / GLUT Sample -- Adrien Protzel";
const char *GLUITITLE   = "User Interface Window";

// what the glui package defines as true and false:

const int GLUITRUE  = true;
const int GLUIFALSE = false;

// the escape key:

const int ESCAPE = 0x1b;

// initial window size:

const int INIT_WINDOW_SIZE = 1000;

// size of the 3d box to be drawn:

const float BOXSIZE = 2.f;

// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = 1.f;
const float SCLFACT = 0.005f;

// minimum allowable scale factor:

const float MINSCALE = 0.05f;

// scroll wheel button values:

const int SCROLL_WHEEL_UP   = 3;
const int SCROLL_WHEEL_DOWN = 4;

// equivalent mouse movement when we click the scroll wheel:

const float SCROLL_WHEEL_CLICK_FACTOR = 5.f;

// active mouse buttons (or them together):

const int LEFT   = 4;
const int MIDDLE = 2;
const int RIGHT  = 1;

// which projection:

enum Projections
{
	ORTHO,
	PERSP
};

// which button:

enum ButtonVals
{
	RESET,
	QUIT
};

// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };

// line width for the axes:

const GLfloat AXES_WIDTH   = 3.;

// the color numbers:
// this order must match the radio button order, which must match the order of the color names,
// 	which must match the order of the color RGB values

enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA
};

char * ColorNames[ ] =
{
	(char*)"Red",
	(char*)"Yellow",
	(char*)"Green",
	(char*)"Cyan",
	(char*)"Blue",
	(char*)"Magenta"
};

// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] = 
{
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
};

// fog parameters:

const GLfloat FOGCOLOR[4] = { .0f, .0f, .0f, 1.f };
const GLenum  FOGMODE     = GL_LINEAR;
const GLfloat FOGDENSITY  = 0.30f;
const GLfloat FOGSTART    = 1.5f;
const GLfloat FOGEND      = 4.f;

// for lighting:

const float	WHITE[ ] = { 1.,1.,1.,1. };

// for animation:

const int MS_PER_CYCLE = 10000;		// 10000 milliseconds = 10 seconds


// what options should we compile-in?
// in general, you don't need to worry about these
// i compile these in to show class examples of things going wrong
//#define DEMO_Z_FIGHTING
//#define DEMO_DEPTH_BUFFER


// non-constant global variables:

int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
GLuint	BoxList;				// object display list
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int		DepthBufferOn;			// != 0 means to use the z-buffer
int		DepthFightingOn;		// != 0 means to force the creation of z-fighting
int		MainWindow;				// window id for main graphics window
int		NowColor;				// index into Colors[ ]
int		NowProjection;		// ORTHO or PERSP
float	Scale;					// scaling factor
int		ShadowsOn;				// != 0 means to turn shadows on
float	Time;					// used for animation, this has a value between 0. and 1.
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees


// function prototypes:

void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoColorMenu( int );
void	DoDepthBufferMenu( int );
void	DoDepthFightingMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void			Axes( float );
void			HsvRgb( float[3], float [3] );
void			Cross(float[3], float[3], float[3]);
float			Dot(float [3], float [3]);
float			Unit(float [3], float [3]);
float			Unit(float [3]);


// utility to create an array from 3 separate values:

float *
Array3( float a, float b, float c )
{
	static float array[4];

	array[0] = a;
	array[1] = b;
	array[2] = c;
	array[3] = 1.;
	return array;
}

// utility to create an array from a multiplier and an array:

float *
MulArray3( float factor, float array0[ ] )
{
	static float array[4];

	array[0] = factor * array0[0];
	array[1] = factor * array0[1];
	array[2] = factor * array0[2];
	array[3] = 1.;
	return array;
}


float *
MulArray3(float factor, float a, float b, float c )
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
Ranf( float low, float high )
{
        float r = (float) rand();               // 0 - RAND_MAX
        float t = r  /  (float) RAND_MAX;       // 0. - 1.

        return   low  +  t * ( high - low );
}

// call this if you want to force your program to use
// a different random number sequence every time you run it:
void
TimeOfDaySeed( )
{
	struct tm y2k;
	y2k.tm_hour = 0;    y2k.tm_min = 0; y2k.tm_sec = 0;
	y2k.tm_year = 2000; y2k.tm_mon = 0; y2k.tm_mday = 1;

	time_t  now;
	time( &now );
	double seconds = difftime( now, mktime(&y2k) );
	unsigned int seed = (unsigned int)( 1000.*seconds );    // milliseconds
	srand( seed );
}

// these are here for when you need them -- just uncomment the ones you need:

//#include "setmaterial.cpp"
//#include "setlight.cpp"
//#include "osusphere.cpp"
//#include "osucube.cpp"
//#include "osucylindercone.cpp"
//#include "osutorus.cpp"
//#include "bmptotexture.cpp"
//#include "loadobjmtlfiles.cpp"
//#include "keytime.cpp"
//#include "glslprogram.cpp"
//#include "vertexbufferobject.cpp"


// main program:

int
main( int argc, char *argv[ ] )
{
	// seed random with time
	srand(static_cast<unsigned int>(time(NULL)));


	// turn on the glut package:
	// (do this before checking argc and argv since glutInit might
	// pull some command line arguments out)

	glutInit( &argc, argv );

	// setup all the graphics stuff:

	InitGraphics( );

	// create the display lists that **will not change**:

	InitLists( );

	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset( );

	// setup all the user interface stuff:

	InitMenus( );

	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow( MainWindow );
	glutMainLoop( );

	// glutMainLoop( ) never actually returns
	// the following line is here to make the compiler happy:

	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutPostRedisplay( ) do it

void
Animate( )
{
	// put animation stuff in here -- change some global variables for Display( ) to find:

	int ms = glutGet(GLUT_ELAPSED_TIME);
	ms %= MS_PER_CYCLE;							// makes the value of ms between 0 and MS_PER_CYCLE-1
	Time = (float)ms / (float)MS_PER_CYCLE;		// makes the value of Time between 0. and slightly less than 1.

	// for example, if you wanted to spin an object in Display( ), you might call: glRotatef( 360.f*Time,   0., 1., 0. );

	// force a call to Display( ) next time it is convenient:

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// draw the complete scene:

void
Display( )
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting Display.\n");

	// set which window we want to do the graphics into:
	glutSetWindow( MainWindow );

	// erase the background:
	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable( GL_DEPTH_TEST );
#ifdef DEMO_DEPTH_BUFFER
	if( DepthBufferOn == 0 )
		glDisable( GL_DEPTH_TEST );
#endif


	// specify shading to be flat:

	glShadeModel( GL_FLAT );

	// set the viewport to be a square centered in the window:

	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );


	// set the viewing volume:
	// remember that the Z clipping  values are given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( NowProjection == ORTHO )
		glOrtho( -2.f, 2.f,     -2.f, 2.f,     0.1f, 1000.f );
	else
		gluPerspective( 70.f, 1.f,	0.1f, 1000.f );

	// place the objects into the scene:

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	// set the eye position, look-at position, and up-vector:

	gluLookAt( 0.f, 0.f, 3.f,     0.f, 0.f, 0.f,     0.f, 1.f, 0.f );

	// rotate the scene:

	glRotatef( (GLfloat)Yrot, 0.f, 1.f, 0.f );
	glRotatef( (GLfloat)Xrot, 1.f, 0.f, 0.f );

	// uniformly scale the scene:

	if( Scale < MINSCALE )
		Scale = MINSCALE;
	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );

	// set the fog parameters:

	if( DepthCueOn != 0 )
	{
		glFogi( GL_FOG_MODE, FOGMODE );
		glFogfv( GL_FOG_COLOR, FOGCOLOR );
		glFogf( GL_FOG_DENSITY, FOGDENSITY );
		glFogf( GL_FOG_START, FOGSTART );
		glFogf( GL_FOG_END, FOGEND );
		glEnable( GL_FOG );
	}
	else
	{
		glDisable( GL_FOG );
	}

	// possibly draw the axes:

	if( AxesOn != 0 )
	{
		glColor3fv( &Colors[NowColor][0] );
		glCallList( AxesList );
	}

	// since we are using glScalef( ), be sure the normals get unitized:

	glEnable( GL_NORMALIZE );


	// draw the box object by calling up its display list:

	glCallList( BoxList );

#ifdef DEMO_Z_FIGHTING
	if( DepthFightingOn != 0 )
	{
		glPushMatrix( );
			glRotatef( 90.f,   0.f, 1.f, 0.f );
			glCallList( BoxList );
		glPopMatrix( );
	}
#endif


	// draw some gratuitous text that just rotates on top of the scene:
	// i commented out the actual text-drawing calls -- put them back in if you have a use for them
	// a good use for thefirst one might be to have your name on the screen
	// a good use for the second one might be to have vertex numbers on the screen alongside each vertex

	glDisable( GL_DEPTH_TEST );
	glColor3f( 0.f, 1.f, 1.f );
	//DoRasterString( 0.f, 1.f, 0.f, (char *)"Text That Moves" );


	// draw some gratuitous text that is fixed on the screen:
	//
	// the projection matrix is reset to define a scene whose
	// world coordinate system goes from 0-100 in each axis
	//
	// this is called "percent units", and is just a convenience
	//
	// the modelview matrix is reset to identity as we don't
	// want to transform these coordinates

	glDisable( GL_DEPTH_TEST );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluOrtho2D( 0.f, 100.f,     0.f, 100.f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	glColor3f( 1.f, 1.f, 1.f );
	//DoRasterString( 5.f, 5.f, 0.f, (char *)"Text That Doesn't" );

	// swap the double-buffered framebuffers:

	glutSwapBuffers( );

	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush( );
}


void
DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
	NowColor = id - RED;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
	DebugOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthBufferMenu( int id )
{
	DepthBufferOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthFightingMenu( int id )
{
	DepthFightingOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
			break;

		case QUIT:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
	NowProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

	char c;			// one character to print
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05f + 33.33f );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		char c;			// one character to print
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.f;
}



// initialize the glut and OpenGL libraries:
//	also setup callback functions

void
InitGraphics( )
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting InitGraphics.\n");

	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );

	// open the window and set its title:

	MainWindow = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );

	// set the framebuffer clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc(MouseMotion);
	//glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( -1, NULL, 0 );

	// setup glut to call Animate( ) every time it has
	// 	nothing it needs to respond to (which is most of the time)
	// we don't need to do this for this program, and really should set the argument to NULL
	// but, this sets us up nicely for doing animation

	glutIdleFunc( Animate );

	// init the glew package (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit( );
	if( err != GLEW_OK )
	{
		fprintf( stderr, "glewInit Error\n" );
	}
	else
		fprintf( stderr, "GLEW initialized OK\n" );
	fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

	// all other setups go here, such as GLSLProgram and KeyTime setups:

}



void drawTRUNK() {
	const float radius = 1.5f, height = 10.0f; // trunk dimensions
	const int numSides = 33; // number of sides

	glPushMatrix(); // save transform
	glTranslatef(0.0f, 0.0f, 0.0f); // trunk is centered at origin

	glBegin(GL_TRIANGLE_STRIP); // draw trunk
	for (int i = 0; i < numSides; ++i) {
		float t = (float)i / (numSides - 1); // normalized index
		float angle = 2.0f * M_PI * t; // angle from index
		float x = radius * cosf(angle), z = radius * sinf(angle); // x and z position
		float brightness = 0.1f + 0.2f * (1.0f - fabsf(2.0f * t - 1.0f)); // brightness
		glColor3f(0.55f * brightness, 0.27f * brightness, 0.07f * brightness); // color
		glVertex3f(x, -height * 0.5f, z); glVertex3f(x, height * 0.5f, z); // bottom and top vertex
	}
	glEnd(); // end trunk

	glPopMatrix(); // restore transform
}

void drawBARK() {
	const float trunkRadius = 1.5f, trunkHeight = 10.0f; // trunk dimensions
	const int numPieces = 124, numSides = 13; // bark pieces and sides
	const float barkRadius = 0.15f, hMin = 0.2f, hMax = 1.2f, coneHeight = 0.05f; // bark size
	const float offsetR = trunkRadius * 0.95f; // radial offset
	const float yMin = -trunkHeight * 0.5f + 0.3f, yMax = trunkHeight * 0.5f - 0.3f; // vertical bounds
	const float dAngle = 2.0f * M_PI / (numSides - 1); // angle step

	for (int i = 0; i < numPieces; ++i) {
		float angle = Ranf(0.f, 2.0f * M_PI); // random angle
		float x = offsetR * cosf(angle), z = offsetR * sinf(angle); // x and z position
		float y = Ranf(yMin, yMax); // y position
		float h = Ranf(hMin, hMax); // height

		glPushMatrix(); // save transform
		glTranslatef(x, y, z); // move to bark position

		glBegin(GL_TRIANGLE_STRIP); // draw bark body
		for (int j = 0; j < numSides; ++j) {
			float a = dAngle * j; // angle
			float xPos = barkRadius * cosf(a), zPos = barkRadius * sinf(a); // vertex
			float t = (float)j / (numSides - 1); // normalized index
			float brightness = 0.5f + 0.2f * (1.0f - fabsf(2.0f * t - 1.0f)); // brightness
			glColor3f(0.36f * brightness, 0.18f * brightness, 0.05f * brightness); // color
			glVertex3f(xPos, 0.0f, zPos); glVertex3f(xPos, h, zPos); // bottom and top vertex
		}
		glEnd(); // end bark body

		for (int k = 0; k <= 1; ++k) {
			float cy = k ? h + coneHeight : -coneHeight; // cone tip y
			float by = k ? h : 0.0f; // cone base y
			glBegin(GL_TRIANGLES); // draw cone cap
			for (int j = 0; j < numSides; ++j) {
				float a = dAngle * j, aNext = a + dAngle; // angles
				float x0 = barkRadius * cosf(a), z0 = barkRadius * sinf(a); // first vertex
				float x1 = barkRadius * cosf(aNext), z1 = barkRadius * sinf(aNext); // second vertex
				float t = (float)j / (numSides - 1); // normalized index
				float brightness = 0.1f + 0.2f * (1.0f - fabsf(2.0f * t - 1.0f)); // brightness
				glColor3f(0.36f * brightness, 0.18f * brightness, 0.05f * brightness); // color
				glVertex3f(x0, by, z0); glVertex3f(x1, by, z1); glVertex3f(0.0f, cy, 0.0f); // triangle
			}
			glEnd(); // end cone cap
		}

		glPopMatrix(); // restore transform
	}
}

void drawBRANCHES() {
	const float trunkHeight = 10.0f; // trunk height
	const int numBranches = 80, numSides = 13; // branch count and sides
	const float radius = 0.25f, hMin = 0.3f, hMax = 2.5f, cone = 0.15f, offsetR = 7.0f; // branch dimensions
	const float yMin = trunkHeight * 0.5f - 2.0f, yMax = trunkHeight * 0.5f + 2.0f; // vertical bounds
	const float dAngle = 2.0f * M_PI / (numSides - 1); // angle step

	for (int i = 0; i < numBranches; ++i) {
		float angle = Ranf(0.f, 2.0f * M_PI); // random angle
		float r = Ranf(0.f, offsetR); // radial distance
		float x = r * cosf(angle), z = r * sinf(angle); // x and z position
		float y = Ranf(yMin, yMax); // y position

		float dx = x, dy = y - trunkHeight * 0.5f, dz = z; // direction vector
		float len = sqrtf(dx * dx + dy * dy + dz * dz); // vector length
		dx /= len; dy /= len; dz /= len; // normalize direction

		float yaw = atan2f(-dx, -dz) * 180.0f / M_PI; // yaw angle
		float pitch = Ranf(-30.0f, -20.0f); // pitch angle
		float height = Ranf(hMin, hMax); // branch height

		glPushMatrix(); // save transform
		glTranslatef(x, y, z); // move to branch position
		glRotatef(yaw, 0.0f, 1.0f, 0.0f); // rotate around Y
		glRotatef(pitch, 1.0f, 0.0f, 0.0f); // rotate around X

		glBegin(GL_TRIANGLE_STRIP); // draw branch body
		for (int j = 0; j < numSides; ++j) {
			float a = dAngle * j; // current angle
			float xPos = radius * cosf(a), zPos = radius * sinf(a); // vertex position
			float t = (float)j / (numSides - 1); // normalized index
			float brightness = 0.3f + 0.2f * (1.0f - fabsf(2.0f * t - 1.0f)); // brightness
			glColor3f(0.36f * brightness, 0.18f * brightness, 0.05f * brightness); // color
			glVertex3f(xPos, 0.0f, zPos); glVertex3f(xPos, height, zPos); // bottom and top vertex
		}
		glEnd(); // end branch body

		for (int k = 0; k <= 1; ++k) {
			float cy = k ? height + cone : -cone; // cone tip y
			float by = k ? height : 0.0f; // cone base y
			glBegin(GL_TRIANGLES); // draw cone cap
			for (int j = 0; j < numSides; ++j) {
				float a = dAngle * j, aNext = a + dAngle; // angles
				float x0 = radius * cosf(a), z0 = radius * sinf(a); // first vertex
				float x1 = radius * cosf(aNext), z1 = radius * sinf(aNext); // second vertex
				float t = (float)j / (numSides - 1); // normalized index
				float brightness = 0.1f + 0.2f * (1.0f - fabsf(2.0f * t - 1.0f)); // brightness
				glColor3f(0.36f * brightness, 0.18f * brightness, 0.05f * brightness); // color
				glVertex3f(x0, by, z0); glVertex3f(x1, by, z1); glVertex3f(0.0f, cy, 0.0f); // triangle
			}
			glEnd(); // end cone cap
		}

		glPopMatrix(); // restore transform
	}
}

void drawLEAVES() {
	const float trunkHeight = 10.0f; // trunk height
	const int leafCount = 1200, numSides = 9; // leaf count and sides
	const float radiusMin = 0.3f, radiusMax = 1.0f, thickness = 0.05f; // leaf size
	const float offsetXZ = 10.0f; // horizontal spread
	const float offsetYMin = trunkHeight * 0.5f - 3.0f, offsetYMax = trunkHeight * 0.5f + 4.0f; // vertical bounds

	for (int i = 0; i < leafCount; ++i) {
		float radius = Ranf(radiusMin, radiusMax); // random radius
		float theta = Ranf(0.f, 2.0f * M_PI); // azimuth angle
		float phi = Ranf(0.f, M_PI / 2.0f); // elevation angle
		float r = offsetXZ * Ranf(0.5f, 1.0f); // radial distance
		float x = r * cosf(theta) * sinf(phi), z = r * sinf(theta) * sinf(phi); // x and z position
		float y = offsetYMin + r * cosf(phi); // y position

		float rx = Ranf(10.0f, 30.0f); // x rotation
		float ry = Ranf(0.0f, 360.0f); // y rotation
		float rz = Ranf(10.0f, 30.0f); // z rotation

		glPushMatrix(); // save transform
		glTranslatef(x, y, z); // move to leaf position
		glRotatef(rx, 1.0f, 0.0f, 0.0f); // rotate around X
		glRotatef(ry, 0.0f, 1.0f, 0.0f); // rotate around Y
		glRotatef(rz, 0.0f, 0.0f, 1.0f); // rotate around Z

		glBegin(GL_TRIANGLES); // draw leaf top and bottom
		for (int j = 0; j < numSides; ++j) {
			float a = 2.0f * M_PI * j / numSides, aNext = a + 2.0f * M_PI / numSides; // angles
			float x1 = radius * cosf(a), z1 = radius * sinf(a); // first vertex
			float x2 = radius * cosf(aNext), z2 = radius * sinf(aNext); // second vertex
			float t = (float)j / (numSides - 1); // normalized index
			float brightness = 0.4f + 0.3f * (1.0f - fabsf(2.0f * t - 1.0f)); // brightness
			glColor3f(brightness * (0.6f + 0.4f * t), brightness * (0.1f + 0.2f * t), 0.0f); // color
			glVertex3f(0.0f, thickness * 0.5f, 0.0f); glVertex3f(x1, thickness * 0.5f, z1); glVertex3f(x2, thickness * 0.5f, z2); // top
			glVertex3f(0.0f, -thickness * 0.5f, 0.0f); glVertex3f(x2, -thickness * 0.5f, z2); glVertex3f(x1, -thickness * 0.5f, z1); // bottom
		}
		glEnd(); // end leaf top and bottom

		glBegin(GL_TRIANGLE_STRIP); // draw leaf sides
		for (int j = 0; j < numSides; ++j) {
			float a = 2.0f * M_PI * j / numSides, aNext = a + 2.0f * M_PI / numSides; // angles
			float x1 = radius * cosf(a), z1 = radius * sinf(a); // first vertex
			float x2 = radius * cosf(aNext), z2 = radius * sinf(aNext); // second vertex
			float t = (float)j / (numSides - 1); // normalized index
			float brightness = 0.25f + 0.15f * (1.0f - fabsf(2.0f * t - 1.0f)); // brightness
			glColor3f(brightness * (0.6f + 0.4f * t), brightness * (0.1f + 0.2f * t), 0.0f); // color
			glVertex3f(x1, -thickness * 0.5f, z1); glVertex3f(x1, thickness * 0.5f, z1); // side 1
			glVertex3f(x2, -thickness * 0.5f, z2); glVertex3f(x2, thickness * 0.5f, z2); // side 2
		}
		glEnd(); // end leaf sides

		glPopMatrix(); // restore transform
	}
}

void drawFALLENLEAVES() {
	const float trunkHeight = 10.0f; // trunk height
	const int leafCount = 250, numSides = 5; // leaf count and sides
	const float radiusMin = 0.3f, radiusMax = 0.8f, thickness = 0.05f; // leaf size
	const float offsetXZ = 9.0f; // horizontal spread
	const float offsetYMin = -trunkHeight * 0.5f + 0.5f, offsetYMax = -trunkHeight * 0.5f; // vertical bounds

	for (int i = 0; i < leafCount; ++i) {
		float radius = Ranf(radiusMin, radiusMax); // random radius
		float theta = Ranf(0.f, 2.0f * M_PI); // random angle
		float r = Ranf(0.f, offsetXZ); // radial distance
		float x = r * cosf(theta), z = r * sinf(theta); // x and z position
		float y = Ranf(offsetYMin, offsetYMax); // y position
		float ry = Ranf(0.f, 360.0f); // random Y rotation

		glPushMatrix(); // save transform
		glTranslatef(x, y, z); // move to leaf position
		glRotatef(ry, 0.0f, 1.0f, 0.0f); // rotate around Y

		glBegin(GL_TRIANGLES); // draw leaf top and bottom
		for (int j = 0; j < numSides; ++j) {
			float a = 2.0f * M_PI * j / numSides, aNext = a + 2.0f * M_PI / numSides; // angles
			float x1 = radius * cosf(a), z1 = radius * sinf(a); // first vertex
			float x2 = radius * cosf(aNext), z2 = radius * sinf(aNext); // second vertex
			float t = (float)j / (numSides - 1); // normalized index
			float brightness = 0.5f + 0.3f * (1.0f - fabsf(2.0f * t - 1.0f)); // brightness
			glColor3f(brightness * (1.0f - 0.1f * t), brightness * (0.9f - 0.3f * t), 0.0f); // color
			glVertex3f(0.0f, thickness * 0.5f, 0.0f); glVertex3f(x1, thickness * 0.5f, z1); glVertex3f(x2, thickness * 0.5f, z2); // top
			glVertex3f(0.0f, -thickness * 0.5f, 0.0f); glVertex3f(x2, -thickness * 0.5f, z2); glVertex3f(x1, -thickness * 0.5f, z1); // bottom
		}
		glEnd(); // end leaf top and bottom

		glBegin(GL_TRIANGLE_STRIP); // draw leaf sides
		for (int j = 0; j < numSides; ++j) {
			float a = 2.0f * M_PI * j / numSides, aNext = a + 2.0f * M_PI / numSides; // angles
			float x1 = radius * cosf(a), z1 = radius * sinf(a); // first vertex
			float x2 = radius * cosf(aNext), z2 = radius * sinf(aNext); // second vertex
			float t = (float)j / (numSides - 1); // normalized index
			float brightness = 0.4f + 0.2f * (1.0f - fabsf(2.0f * t - 1.0f)); // brightness
			glColor3f(brightness * (0.9f - 0.2f * t), brightness * (0.8f - 0.1f * t), 0.0f); // color
			glVertex3f(x1, -thickness * 0.5f, z1); glVertex3f(x1, thickness * 0.5f, z1); // side 1
			glVertex3f(x2, -thickness * 0.5f, z2); glVertex3f(x2, thickness * 0.5f, z2); // side 2
		}
		glEnd(); // end leaf sides

		glPopMatrix(); // restore transform
	}
}



// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void 
InitLists()
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting InitLists.\n");

	float dx = BOXSIZE / 2.f;
	float dy = BOXSIZE / 2.f;
	float dz = BOXSIZE / 2.f;

	glutSetWindow(MainWindow);

	// Create the object display list
	BoxList = glGenLists(1);
	glNewList(BoxList, GL_COMPILE);



	drawTRUNK();
	drawBARK();
	drawBRANCHES();
	drawLEAVES();
	drawFALLENLEAVES();


	
	glEndList();

	// Create the axes display list
	AxesList = glGenLists(1);
	glNewList(AxesList, GL_COMPILE);
	glLineWidth(AXES_WIDTH);
	Axes(1.5);
	glLineWidth(1.);
	glEndList();
}





























// initialize the glui window:

void
InitMenus( )
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting InitMenus.\n");

	glutSetWindow( MainWindow );

	int numColors = sizeof( Colors ) / ( 3*sizeof(float) );
	int colormenu = glutCreateMenu( DoColorMenu );
	for( int i = 0; i < numColors; i++ )
	{
		glutAddMenuEntry( ColorNames[i], i );
	}

	int axesmenu = glutCreateMenu( DoAxesMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthcuemenu = glutCreateMenu( DoDepthMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthbuffermenu = glutCreateMenu( DoDepthBufferMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthfightingmenu = glutCreateMenu( DoDepthFightingMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int debugmenu = glutCreateMenu( DoDebugMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int projmenu = glutCreateMenu( DoProjectMenu );
	glutAddMenuEntry( "Orthographic",  ORTHO );
	glutAddMenuEntry( "Perspective",   PERSP );

	int mainmenu = glutCreateMenu( DoMainMenu );
	glutAddSubMenu(   "Axes",          axesmenu);
	glutAddSubMenu(   "Axis Colors",   colormenu);

#ifdef DEMO_DEPTH_BUFFER
	glutAddSubMenu(   "Depth Buffer",  depthbuffermenu);
#endif

#ifdef DEMO_Z_FIGHTING
	glutAddSubMenu(   "Depth Fighting",depthfightingmenu);
#endif

	glutAddSubMenu(   "Depth Cue",     depthcuemenu);
	glutAddSubMenu(   "Projection",    projmenu );
	glutAddMenuEntry( "Reset",         RESET );
	glutAddSubMenu(   "Debug",         debugmenu);
	glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:

	glutAttachMenu( GLUT_RIGHT_BUTTON );
}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

	switch( c )
	{
		case 'o':
		case 'O':
			NowProjection = ORTHO;
			break;

		case 'p':
		case 'P':
			NowProjection = PERSP;
			break;

		case 'q':
		case 'Q':
		case ESCAPE:
			DoMainMenu( QUIT );	// will not return here
			break;				// happy compiler

		default:
			fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
	}

	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		case SCROLL_WHEEL_UP:
			Scale += SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		case SCROLL_WHEEL_DOWN:
			Scale -= SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}

	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}

	glutSetWindow(MainWindow);
	glutPostRedisplay();

}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 )
	{
		Xrot += ( ANGFACT*dy );
		Yrot += ( ANGFACT*dx );
	}

	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	DepthBufferOn = 1;
	DepthFightingOn = 0;
	DepthCueOn = 0;
	Scale  = 1.0;
	ShadowsOn = 0;
	NowColor = YELLOW;
	NowProjection = PERSP;
	Xrot = Yrot = 0.;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = { 0.f, 1.f, 0.f, 1.f };

static float xy[ ] = { -.5f, .5f, .5f, -.5f };

static int xorder[ ] = { 1, 2, -3, 4 };

static float yx[ ] = { 0.f, 0.f, -.5f, .5f };

static float yy[ ] = { 0.f, .6f, 1.f, 1.f };

static int yorder[ ] = { 1, 2, 3, -2, 4 };

static float zx[ ] = { 1.f, 0.f, 1.f, 0.f, .25f, .75f };

static float zy[ ] = { .5f, .5f, -.5f, -.5f, 0.f, 0.f };

static int zorder[ ] = { 1, 2, 3, 4, -5, 6 };

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;

	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:
	
	float i = (float)floor( h );
	float f = h - i;
	float p = v * ( 1.f - s );
	float q = v * ( 1.f - s*f );
	float t = v * ( 1.f - ( s * (1.f-f) ) );

	float r=0., g=0., b=0.;			// red, green, blue
	switch( (int) i )
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
Unit( float v[3] )
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
