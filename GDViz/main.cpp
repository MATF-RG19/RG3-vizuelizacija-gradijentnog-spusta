#include <GL/glut.h>

#include "shared.h"
#include "FuncAndGrads.h"

void on_keyboard(unsigned char key, int x, int y);
void on_reshape(int width, int height);
void on_display(void);
void draw_manifold(void);
void draw_sphere(double r, double x, double y);

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

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

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
		(GLdouble)100.0
	);
}

void on_display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		(GLdouble)2, (GLdouble)60, (GLdouble)5,
		(GLdouble)0, (GLdouble)5, (GLdouble)0,
		(GLdouble)0, (GLdouble)1, (GLdouble)0
	);

	GLfloat light_position[] = { 1, 1, 1, 0};
	GLfloat ambient_light[] = { 0.1, 0.1, 0.1, 1 };
	GLfloat diffuse_light[] = { 1, 1, 1, 1 };
	GLfloat specular_light[] = { 0.75, 0.75, 0.75, 1 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

	draw_manifold();

	// showing just 20 iterations as a test
	double x = -4.5, y = -4.5, lr = 0.1;
	for (int i = 0; i < 20; i++) {
		draw_sphere(0.5, x, y);
		x -= lr * grad_x(x, y);
		y -= lr * grad_y(x, y);
	}

	glutSwapBuffers();
}

void draw_sphere(double r, double x, double y) {
	/*
	Draws little sphere in the given point. Using for descent visualization.
	*/

	GLfloat ambient_material[] = { 0.25, 0.25, 0.25, 1 };
	GLfloat diffuse_material[] = { 0.75, 0.5, 0.25, 1 };
	GLfloat specular_material[] = { 1, 0, 0, 1 };
	GLfloat shininess = 10;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_material);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	glPushMatrix();
	glTranslatef(x + r, func(x, y) - r, y + r);
	glutSolidSphere(r, 10, 10);
	glPopMatrix();
}

void draw_manifold() {
	/*
	Draws 2D manifold in the 3D. It's drawn as stripes of triangles.
	*/

	GLfloat ambient_material[] = { 0.25, 0.25, 0.25, 1 };
	GLfloat diffuse_material[] = { 0.25, 0.5, 0.75, 1 };
	GLfloat specular_material[] = { 0, 0, 1, 1 };
	GLfloat shininess = 10;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_material);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	pair<double, double> x_range = { -6, 6 };
	pair<double, double> y_range = { -6, 6 };
	double step = 0.1;
	
	auto set_normal_and_vertex = [&](double x, double y) {
		double z = func(x, y);
		glVertex3f(x, z, y);
		glNormal3f(-grad_x(x, y), -grad_z(x, y), -grad_y(x, y));
	};

	for (double y = y_range.first; y <= y_range.second + EPS; y += step) {
		glBegin(GL_TRIANGLE_STRIP);
		for (double x = x_range.first; x <= x_range.second + EPS; x += step) {
			set_normal_and_vertex(x, y);
			set_normal_and_vertex(x, y - step);
		}
		glEnd();
	}
}