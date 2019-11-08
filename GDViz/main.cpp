#include <GL/glut.h>
#include <iostream>
#include <math.h>

const double PI = acos(-1.0);
const double EPS = 1e-2;

void on_keyboard(unsigned char key, int x, int y);
void on_reshape(int width, int height);
void on_display(void);
void draw_manifold(void);
void set_normal_and_vertex(double x, double y);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);

	glutKeyboardFunc(on_keyboard);
	glutReshapeFunc(on_reshape);
	glutDisplayFunc(on_display);

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glutMainLoop();

	return 0;
}

void on_keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
	}
}

void on_reshape(int width, int height) {
	glViewport((GLint)0, (GLint)0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		(GLdouble)60.0,
		(GLdouble)width / height,
		(GLdouble)1.0,
		(GLdouble)10.0
	);
}

void on_display(void) {
	GLfloat light_position[] = { 1, 2, 3, 0};

	GLfloat ambient_light[] = { 0.1, 0.1, 0.1, 1 };
	GLfloat ambient_material[] = { 0.25, 0.25, 0.25, 1 };

	GLfloat diffuse_light[] = { 0.75, 0, 0, 1 };
	GLfloat diffuse_material[] = { 0.5, 0.5, 0.5, 1 };

	GLfloat specular_light[] = { 0.75, 0.25, 0, 1 };
	GLfloat specular_material[] = { 0.75, 0.75, 0.75, 1 };

	GLfloat shininess = 10;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*glBegin(GL_LINES);
	{
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);
	}
	glEnd();*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		(GLdouble)1, (GLdouble)1, (GLdouble)2,
		(GLdouble)0, (GLdouble)0, (GLdouble)0,
		(GLdouble)0, (GLdouble)1, (GLdouble)0
	);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_material);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	glShadeModel(GL_SMOOTH);

	draw_manifold();
	//glutSolidSphere(2, 50, 50);

	glutSwapBuffers();
}

void draw_manifold() {
	double u_range[] = { 0, 2 * PI };
	//double v_range[] = { 0, PI / 2 };
	double v_range[] = { PI / 2, PI };
	double step = 0.01;

	for (double v = v_range[0]; v <= v_range[1] + EPS; v += step) {
		glBegin(GL_TRIANGLE_STRIP);
		for (double u = u_range[0]; u <= u_range[1] + EPS; u += step) {
			set_normal_and_vertex(u, v);
			set_normal_and_vertex(u, v + step);
		}
		glEnd();
	}
}

void set_normal_and_vertex(double u, double v) {
	double x = cos(u) * sin(v);
	double y = sin(u) * sin(v);
	double z = cos(v);

	glVertex3f((GLfloat)x, (GLfloat)z, (GLfloat)y);
	glNormal3f((GLfloat)-2*x, (GLfloat)-2*z, (GLfloat)-2*y);
}