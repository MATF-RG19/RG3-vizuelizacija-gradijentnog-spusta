#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <algorithm>

#include <GL/glut.h>
#include "EllipticParaboloid.h"
#include "HyperbolicParaboloid.h"
#include "MultivariateSine.h"
#include "main.h"
#include "shared.h"

// global variables
bool ongoing_animation;
Point sphere_center; // center of sphere used for GD
ManifoldBase* manifold = nullptr;
std::vector<std::vector<Point>> manifold_pts; // sampled points of manifold; lazy update
std::tuple<double, double, double> zoom_factor(1.0, 1.0, 1.0);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(INIT_WINDOW_SIZE[0], INIT_WINDOW_SIZE[1]);
	glutInitWindowPosition(INIT_WINDOW_POSITION[0], INIT_WINDOW_POSITION[1]);
	glutCreateWindow(argv[0]);

	glutKeyboardFunc(on_keyboard);
	glutReshapeFunc(on_reshape);
	glutDisplayFunc(on_display);
	glutMouseFunc(on_mouse);

	glClearColor(0, 0, 0, 0);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	glutMainLoop();

	return 0;
}

void on_keyboard(unsigned char key, int x, int y) {
	/*
	glutPostRedisplay() won't be called in case the same manifold is chosen twice.
	*/

	switch (key) {
	case 27:
		exit(0);
	case '1':
		if (manifold_type != ManifoldType::elliptic_paraboloid) {
			manifold_type = ManifoldType::elliptic_paraboloid;
			if (manifold) {
				delete(manifold);
				manifold = nullptr;
			}

			manifold_pts.clear();
			manifold = dynamic_cast<ManifoldBase*>(new EllipticParaboloid(1.5, 1.5));
			glutPostRedisplay();
		}
		break;
	case '2':
		if (manifold_type != ManifoldType::hyperbolic_paraboloid) {
			manifold_type = ManifoldType::hyperbolic_paraboloid;
			if (manifold) {
				delete(manifold);
				manifold = nullptr;
			}

			manifold_pts.clear();
			manifold = dynamic_cast<ManifoldBase*>(new HyperbolicParaboloid(1.5, 1.5));
			glutPostRedisplay();
		}
		break;
	case '3':
		if (manifold_type != ManifoldType::multivariate_sine) {
			manifold_type = ManifoldType::multivariate_sine;
			if (manifold) {
				delete(manifold);
				manifold = nullptr;
			}

			manifold_pts.clear();
			manifold = dynamic_cast<ManifoldBase*>(new MultivariateSine(1.0));
			glutPostRedisplay();
		}
		break;
	case 'G':
	case 'g':
		if (!ongoing_animation) {
			sphere_center = manifold->sample(-2.5, 5.0);
			ongoing_animation = true;

			glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
		}
		break;
	case 'S':
	case 's':
		ongoing_animation = false;
		break;
	}
}

void on_reshape(int width, int height) {
	glViewport(
		static_cast<GLint>(0),
		static_cast<GLint>(0),
		static_cast<GLsizei>(width),
		static_cast<GLsizei>(height)
	);

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
		static_cast<GLdouble>(6), static_cast<GLdouble>(18), static_cast<GLdouble>(12),
		static_cast<GLdouble>(0), static_cast<GLdouble>(5), static_cast<GLdouble>(0),
		static_cast<GLdouble>(0), static_cast<GLdouble>(1), static_cast<GLdouble>(0)
	);
	glScalef(
		static_cast<GLfloat>(std::get<0>(zoom_factor)),
		static_cast<GLfloat>(std::get<1>(zoom_factor)),
		static_cast<GLfloat>(std::get<2>(zoom_factor))
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

	if (manifold) {
		draw_manifold();
		draw_sphere(SPHERE_RADIUS);
	}

	glutSwapBuffers();
}

void on_timer(int id) {
	if (id != TIMER_ID)
		return;

	glutPostRedisplay();

	sphere_center.x -= LEARNING_RATE * sphere_center.x;
	sphere_center.y -= LEARNING_RATE * sphere_center.y;
	sphere_center = manifold->sample(sphere_center.x, sphere_center.y);
	std::cerr << "Sphere center: " << sphere_center << std::endl;

	if (ongoing_animation)
		glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
}

void draw_sphere(double r) {
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

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(sphere_center.x + r, sphere_center.z - r, sphere_center.y + r);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glutSolidSphere(r, 10, 10);
	glPopMatrix();
}

void on_mouse(int button, int state, int x, int y) {
	if (button == 3 || button == 4) { // scroll events
		std::get<0>(zoom_factor) += (button == 3 ? 1 : -1) * ZOOM_STEP;
		std::get<1>(zoom_factor) += (button == 3 ? 1 : -1) * ZOOM_STEP;
		std::get<2>(zoom_factor) += (button == 3 ? 1 : -1) * ZOOM_STEP;

		std::get<0>(zoom_factor) = std::clamp(std::get<0>(zoom_factor),
			1.0 / ZOOM_THRESHOLD, ZOOM_THRESHOLD);
		std::get<1>(zoom_factor) = std::clamp(std::get<1>(zoom_factor),
			1.0 / ZOOM_THRESHOLD, ZOOM_THRESHOLD);
		std::get<2>(zoom_factor) = std::clamp(std::get<2>(zoom_factor),
			1.0 / ZOOM_THRESHOLD, ZOOM_THRESHOLD);

		glutPostRedisplay();
	}
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

	init_manifold();
	auto sampled = TensorManipulation::transpose(manifold_pts);

	glColor4f(1.0, 1.0, 1.0, 0.75);
	for (int i = 1; i < MANIFOLD_SAMPLE_SIZE; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j < MANIFOLD_SAMPLE_SIZE; j++) {
			Point cur_pt = sampled[i][j];
			glVertex3f(cur_pt.x, cur_pt.z, cur_pt.y);
			glNormal3f(cur_pt.nx, cur_pt.nz, cur_pt.ny);

			Point prev_pt = sampled[i - 1][j];
			glVertex3f(prev_pt.x, prev_pt.z, prev_pt.y);
			glNormal3f(prev_pt.nx, prev_pt.nz, prev_pt.ny);
		}
		glEnd();
	}
}

void init_manifold(void) {
	if (!manifold->get_lazy_flag()) {
		std::pair<double, double> x_range = { -10.0, 10.0 }; // TODO: hardcoded values
		std::pair<double, double> y_range = { -10.0, 10.0 }; // TODO: hardcoded values
		manifold_pts = manifold->sample(
			x_range,
			y_range,
			MANIFOLD_SAMPLE_SIZE
		);
	}

	manifold->set_lazy_flag(true);
}