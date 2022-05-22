#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <time.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************************************************************************************************************/

// CONSTANTS
#define HOUR_HAND_LENGTH 50.0
#define MINUTE_HAND_LENGTH 70.7106781187
#define SECOND_HAND_LENGTH 78.1024967591
#define CLOCK_RADIUS 80.0
#define PI 3.14159
#define CLOCK_VOL 100.00
#define ANGLE_ONE_MIN PI / 30.0
#define NO_TIME_ZONES 8
int mode = -1;
int loop[NO_TIME_ZONES] = {[0 ... NO_TIME_ZONES - 1] = 1};

/************************************************************************************************************/

// STRUCTURE DEFINING POINT
typedef struct Point
{
	double x, y;
} point;

// ANGLES FOR EACH HAND
double secondAngle = 0, minuteAngle = 0, hourAngle = 0;

/************************************************************************************************************/

// FUNCTION TO INITIALIZE STUFF
void init(void)
{
	gluOrtho2D(-100, 100, -100, 100);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glShadeModel(GL_SMOOTH);
	glLoadIdentity();
}

/************************************************************************************************************/

// FUNCTION TO DRAW A POINT
void drawPoint(point p)
{
	glBegin(GL_POINTS);
	glVertex2f(p.x, p.y);
	glEnd();
}

/************************************************************************************************************/

// FUNCTION TO DRAW A LINE
void drawLine(double x, double y, double angle)
{
	glVertex2f(x, x);
	glVertex2f(y * cos(angle), y * sin(angle));
}

/************************************************************************************************************/

void addDate()
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	char *s = asctime(timeinfo);
	char y[20];
	int count_space = 0, i;
	for (i = 0; i < strlen(s); ++i)
	{
		if (s[i] == ' ')
		{
			count_space++;
			y[i] = ' ';
		}
		else
		{
			y[i] = s[i];
		}
		if (count_space == 3)
			break;
	}
	y[i] = '\0';
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(-10, 90);
	for (i = 0; y[i] != '\0'; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, y[i]);
	}
}

/************************************************************************************************************/

// FUNCTION TO DRAW THE BASIC CLOCK STRUCTURE
void drawMarks(void)
{
	point sp, ep, p;
	int count = 0;
	double i = PI / 6.0;
	sp.x = 0, sp.y = 0;
	ep.x = 0, ep.y = CLOCK_RADIUS;
	glPointSize(10.0);
	drawPoint(sp);
	for (i = 0; i <= 2 * PI; i += PI / 30.0)
	{
		if (i == PI / 2.0)
		{
			p.x = CLOCK_RADIUS;
		}
		else if (i == 3 * PI / 2.0)
		{
			p.x = -CLOCK_RADIUS;
		}
		else
		{
			p.x = ep.y * sin(i);
		}
		p.y = ep.y * cos(i);
		if (count % 5 == 0)
		{ 
			glPointSize(7.0);
			drawPoint(p);
		}
		else
		{
			glPointSize(3.0);
			drawPoint(p);
		}
		count++;
	}
}

/************************************************************************************************************/

// MAIN DRAW FUNCTION
void drawClock(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 1.0);
	glLineWidth(1.0);
	drawMarks();
	addDate();
	glLineWidth(6.0);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	drawLine(0.0, HOUR_HAND_LENGTH, -hourAngle + PI / 2);
	glEnd();

	glLineWidth(2.5);
	glColor3f(0.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	drawLine(0.0, MINUTE_HAND_LENGTH, -minuteAngle + PI / 2);
	glEnd();

	glLineWidth(1.0);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	drawLine(0.0, SECOND_HAND_LENGTH, -secondAngle + PI / 2);
	glEnd();
	glFlush();
	glutSwapBuffers();
}

/************************************************************************************************************/

// FUNCTION TO MAINTAIN ASPECT RATIO WHEN SCREEN IS RESIZED -- VIEWPORT TRANSFORMATION
void reshape(int w, int h)
{
	double aspectRatio;
	// to avoid division by 0
	if (h == 0)
	{
		h = 1;
	}
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	aspectRatio = (double)w / (double)h;
	if (w <= h)
		glOrtho(-CLOCK_VOL, CLOCK_VOL, -CLOCK_VOL / aspectRatio, CLOCK_VOL / aspectRatio, 1.0, -1.0);
	else
		glOrtho(-CLOCK_VOL * aspectRatio, CLOCK_VOL * aspectRatio, -CLOCK_VOL, CLOCK_VOL, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mode = 0;
	}
	glutPostRedisplay();
}
/************************************************************************************************************/
void drawDetails(char ch[], float x, float y)
{
	int i;
	glRasterPos2f(x, y);

	for (i = 0; ch[i] != '\0'; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ch[i]);
	}
}

//introduction page: press left to continue
void intro()
{
	glClearColor(0.0625f, 0.09375f, 0.125f , 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glColor3f(242.0f/256.0f, 170.0f/256.0f, 76.0f/256.0f);
	drawDetails("BEI II/II - COMPUTER GRAPHICS MINI PROJECT", -80, 80);

	drawDetails("TITLE : OPENGL COLOUR CLOCK", -80, 60);

	drawDetails("BY : ", -50, 30);
	drawDetails("PRABIGYA PATHAK  076BEI022", -50, 10);
	drawDetails("PRAYAG MAN MANE  076BEI027", -50, -10);
	drawDetails("RAHUL SHRESTHA   076BEI028", -50, -30);
	drawDetails("TRIBHUWAN BHATTA 076BEI046", -50, -50);

	drawDetails("PRESS LEFT MOUSE BUTTON TO CONTINUE...", -80, -80);
	glutSwapBuffers();
	glFlush();
}

void display(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	if (mode == -1)
	{
		intro();
		glFlush();
	}
	//
	else
	{
		glClearColor(0.0625f, 0.09375f, 0.125f , 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		drawClock();
		glFlush();
	}
}
/************************************************************************************************************/

// TIMER FUNCTION
void handAngleDisplay(struct tm *t)
{
	if (t->tm_min >= 60)
	{
		t->tm_hour++;
		t->tm_min -= 60;
	}
	if (t->tm_min < 0)
	{
		t->tm_hour--;
		t->tm_min += 60;
	}
	secondAngle = (double)(t->tm_sec) / 30.0 * PI;
	minuteAngle = (double)(t->tm_min) / 30.0 * PI + secondAngle / 60.0;
	hourAngle = (double)(t->tm_hour > 12 ? t->tm_hour - 12 : t->tm_hour) / 6.0 * PI + minuteAngle / 12.0;

	glutPostRedisplay();
}
void redraw()
{
	time_t tim = time(0);
	struct tm *t;
	t = localtime(&tim);
	handAngleDisplay(t);
	if (loop[0] == 1)
		glutTimerFunc(15, redraw, 1);
}
// For Sydney, Australia GMT +11:00
void zone1Draw()
{

	time_t tim = time(0);
	struct tm *t;
	t = localtime(&tim);
	t->tm_hour += 5;
	t->tm_min += 15;
	handAngleDisplay(t);
	if (loop[1] == 1)
		glutTimerFunc(15, zone1Draw, 1);
}
// For New York, USA GMT -5:00
void zone2Draw()
{
	time_t tim = time(0);
	struct tm *t;
	t = localtime(&tim);
	t->tm_hour -= 10;
	t->tm_min -= 45;
	handAngleDisplay(t);
	if (loop[2] == 1)
		glutTimerFunc(15, zone2Draw, 1);
}
// For Kabul, Afghanistan GMT+4:30
void zone3Draw()
{
	time_t tim = time(0);
	struct tm *t;
	t = localtime(&tim);
	t->tm_hour -= 1;
	t->tm_min -= 15;
	handAngleDisplay(t);
	if (loop[3] == 1)
		glutTimerFunc(15, zone3Draw, 1);
}
//	For Tokyo, Japan GMT+9:00
void zone4Draw()
{
	time_t tim = time(0);
	struct tm *t;
	t = localtime(&tim);
	t->tm_hour += 3;
	t->tm_min += 15;
	handAngleDisplay(t);
	if (loop[4] == 1)
		glutTimerFunc(15, zone4Draw, 1);
}
// For Barcelona, Spain GMT+1:00
void zone5Draw()
{
	time_t tim = time(0);
	struct tm *t;
	t = localtime(&tim);
	t->tm_hour -= 4;
	t->tm_min -= 45;
	handAngleDisplay(t);
	if (loop[5] == 1)
		glutTimerFunc(15, zone5Draw, 1);
}
// For Buenos Aires, Argentina GMT-3:00
void zone6Draw()
{
	time_t tim = time(0);
	struct tm *t;
	t = localtime(&tim);
	t->tm_hour -= 8;
	t->tm_min -= 45;
	handAngleDisplay(t);
	if (loop[6] == 1)
		glutTimerFunc(15, zone6Draw, 1);
}

//options for time-zones
void menu(int num)
{
	int i;
	int value = num;
	switch (value)
	{
	case 1:
	{
		glutSwapBuffers();
		for (i = 0; i <= NO_TIME_ZONES; i++)
			loop[i] = 0;
		loop[1] = 1;
		zone1Draw();
		break;
	}
	case 2:
	{
		glutSwapBuffers();
		for (i = 0; i <= NO_TIME_ZONES; i++)
			loop[i] = 0;
		loop[2] = 1;
		zone2Draw();
		break;
	}
	case 3:
	{
		glFlush();
		for (i = 0; i <= NO_TIME_ZONES; i++)
			loop[i] = 0;
		loop[3] = 1;
		zone3Draw();
		break;
	}
	case 4:
	{
		for (i = 0; i <= NO_TIME_ZONES; i++)
			loop[i] = 0;
		loop[4] = 1;
		zone4Draw();
		break;
	}
	case 5:
	{
		for (i = 0; i <= NO_TIME_ZONES; i++)
			loop[i] = 0;
		loop[5] = 1;
		zone5Draw();
		break;
	}
	case 6:
	{
		for (i = 0; i <= NO_TIME_ZONES; i++)
			loop[i] = 0;
		loop[6] = 1;
		zone6Draw();
		break;
	}
	case 7:
	{
		for (i = 0; i <= NO_TIME_ZONES; i++)
			loop[i] = 0;
		loop[0] = 1;
		redraw();
		break;
	}
	case 8:
	{
		exit(0);
		break;
	}
	default:
	{
		break;
	}
	}
	glutPostRedisplay();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutPostRedisplay();
	glutMainLoop();
}
void createmenu()
{
	int sub1 = glutCreateMenu(menu);
	glutAddMenuEntry("Sydney, Australia, GMT+11:00", 1);
	glutAddMenuEntry("New York, United States, GMT-5:00", 2);
	glutAddMenuEntry("Kabul, Afghanistan, GMT+4:30", 3);
	glutAddMenuEntry("Tokyo, Japan, GMT+9:00", 4);
	glutAddMenuEntry("Barcelona, Spain, GMT+1:00", 5);
	glutAddMenuEntry("Buenos Aires, Argentina, GMT-3:00", 6);
	glutAddMenuEntry("Kathmandu, Nepal, GMT+5:45 Local Time", 7);
	glutAddMenuEntry("Quit!", 8);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutDisplayFunc(display);
	glutMainLoop();
}
/************************************************************************************************************/

// DRIVER FUNCTION
int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("World Clock");
	init();
	glutMouseFunc(mouse);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(15, redraw, 1);
	createmenu();
	glutMainLoop();
	return 0;
}
