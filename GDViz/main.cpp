#include <iostream>
#include <utility>
#include <vector>

#include <GL/glut.h>
#include "EllipticParaboloid.h"
#include "HyperbolicParaboloid.h"
#include "shared.h"

const int INIT_WINDOW_SIZE[] = { 800, 800 };
const int INIT_WINDOW_POSITION[] = { 100, 100 };

void on_keyboard(unsigned char key, int x, int y);
void on_reshape(int width, int height);
void on_display(void);
void draw_manifold(void);
void draw_sphere(double r, double x, double y);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(INIT_WINDOW_SIZE[0], INIT_WINDOW_SIZE[1]);
	glutInitWindowPosition(INIT_WINDOW_POSITION[0], INIT_WINDOW_POSITION[1]);
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
		static_cast<GLdouble>(60.0),
		static_cast<GLdouble>(width / height),
		static_cast<GLdouble>(1.0),
		static_cast<GLdouble>(100.0)
	);
}

void on_display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		static_cast<GLdouble>(5), static_cast<GLdouble>(12), static_cast<GLdouble>(5),
		static_cast<GLdouble>(0), static_cast<GLdouble>(5), static_cast<GLdouble>(0),
		static_cast<GLdouble>(0), static_cast<GLdouble>(1), static_cast<GLdouble>(0)
	);

	GLfloat light_position[] = { 0, 1, 0, 0};
	GLfloat ambient_light[] = { 0.1, 0.1, 0.1, 1 };
	GLfloat diffuse_light[] = { 0.7, 0.7, 0.7, 1 };
	GLfloat specular_light[] = { 0.9, 0.9, 0.9, 1 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

	draw_manifold();

	// showing just 20 iterations as a test
	/*double x = -2.5, y = -2, lr = 0.1;
	for (int i = 0; i < 20; i++) {
		draw_sphere(0.5, x, y);
		x -= lr * grad_x(x, y);
		y -= lr * grad_y(x, y);
	}*/

	glutSwapBuffers();
}

void draw_sphere(double r, double x, double y) {
	/*
	Draws little sphere in the given point. Using for descent visualization.
	*/

	/*GLfloat ambient_material[] = { 0.25, 0.25, 0.25, 1 };
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
	glPopMatrix();*/
}

void draw_manifold() {
	/*
	Draws 2D manifold in the 3D. It's drawn as stripes of triangles.
	*/

	GLfloat ambient_material[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat diffuse_material[] = { 0.25, 0.25, 0.75, 1 };
	GLfloat specular_material[] = { 0.5, 0, 0, 1 };
	GLfloat shininess = 10;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_material);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	int sample_size = 100;
	std::pair<double, double> x_range = { -3.0, 3.0 };
	std::pair<double, double> y_range = { -2.0, 2.0 };
	// EllipticParaboloid* EP = new EllipticParaboloid(1.5, 1.5);
	HyperbolicParaboloid* HP = new HyperbolicParaboloid(1.5, 1.5);
	std::vector<std::vector<Point>> sampled = HP->sample(x_range, y_range, sample_size);
	auto sampled_T = TensorManipulation::transpose(sampled);

	for (int i = 1; i < sample_size; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j < sample_size; j++) {
			Point cur_pt = sampled_T[i][j];
			glVertex3f(cur_pt.x, cur_pt.z, cur_pt.y);
			glNormal3f(cur_pt.nx, cur_pt.nz, cur_pt.ny);

			Point prev_pt = sampled_T[i - 1][j];
			glVertex3f(prev_pt.x, prev_pt.z, prev_pt.y);
			glNormal3f(prev_pt.nx, prev_pt.nz, prev_pt.ny);
		}
		glEnd();
	}
}