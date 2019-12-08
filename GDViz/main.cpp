#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <algorithm>

#include <GL/glut.h>
#include "CubicProduct.h"
#include "EllipticParaboloid.h"
#include "HyperbolicParaboloid.h"
#include "MultivariateSine.h"
#include "main.h"
#include "shared.h"

/*
-Upon starting the program, background will be black without any other object.
By selecting proper manifold (see on_keyboard()), manifold be drawn.

-Upon initializing animation (see on_keyboard()) little sphere will be shown.

-Upon starting the animation, GD will start.
*/

bool initialized_animation, ongoing_animation;
double LEARNING_RATE = 0.01;
GLfloat rotate_matrix[16], scale_matrix[16], translate_matrix[16];

Point sphere_center; // sphere used for GD
Point barycenter; // barycenter of sampled points

ManifoldBase* manifold = nullptr; // initially, manifold is not chosen
std::pair<int, int> mouse_pos; // mouse coordinates
std::pair<int, int> window_size = { 1000, 1000 }; // window size; updates on reshape
std::vector<std::vector<Point>> manifold_pts; // sampled points of manifold; will be sampled only once


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(window_size.first, window_size.second);
	glutInitWindowPosition(WINDOW_POS.first, WINDOW_POS.second);
	glutCreateWindow(argv[0]);

	glutKeyboardFunc(on_keyboard);
	glutSpecialFunc(on_special);
	glutReshapeFunc(on_reshape);
	glutDisplayFunc(on_display);
	glutMouseFunc(on_mouse);
	glutMotionFunc(on_motion);

	mouse_pos = std::make_pair(0, 0);
	init_matrix(rotate_matrix);
	init_matrix(scale_matrix);
	init_matrix(translate_matrix);
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
	Callback for basic interface events.
	Different numerical values correspond to different manifolds (1, 2, ...).
	Others alpha-numerical values correspond to starting/stopping animation etc.

	(!) glutPostRedisplay() won't be called in case the same manifold is chosen twice.
	*/

	double dir_x = distribution(rnd_gen);
	double dir_y = distribution(rnd_gen);

	switch (key) {
	case 27:
		exit(0);
	case '1':
		if (!manifold || manifold_type != ManifoldType::elliptic_paraboloid) {
			manifold_type = ManifoldType::elliptic_paraboloid;
			if (manifold) {
				delete(manifold);
				manifold = nullptr;
			}
			ongoing_animation = false;
			initialized_animation = false;

			manifold_pts.clear();
			manifold = dynamic_cast<ManifoldBase*>(new EllipticParaboloid(3.5, 3.5));
			init_manifold();
			glutPostRedisplay();
		}
		break;
	case '2':
		if (!manifold || manifold_type != ManifoldType::hyperbolic_paraboloid) {
			manifold_type = ManifoldType::hyperbolic_paraboloid;
			if (manifold) {
				delete(manifold);
				manifold = nullptr;
			}
			ongoing_animation = false;
			initialized_animation = false;

			manifold_pts.clear();
			manifold = dynamic_cast<ManifoldBase*>(new HyperbolicParaboloid(3.5, 3.5));
			init_manifold();
			glutPostRedisplay();
		}
		break;
	case '3':
		if (!manifold || manifold_type != ManifoldType::multivariate_sine) {
			manifold_type = ManifoldType::multivariate_sine;
			if (manifold) {
				delete(manifold);
				manifold = nullptr;
			}
			ongoing_animation = false;
			initialized_animation = false;

			manifold_pts.clear();
			manifold = dynamic_cast<ManifoldBase*>(new MultivariateSine(1.0));
			init_manifold();
			glutPostRedisplay();
		}
		break;
	case '4':
		if (!manifold || manifold_type != ManifoldType::cubic_product) {
			manifold_type = ManifoldType::cubic_product;
			if (manifold) {
				delete(manifold);
				manifold = nullptr;
			}
			ongoing_animation = false;
			initialized_animation = false;

			manifold_pts.clear();
			manifold = dynamic_cast<ManifoldBase*>(new CubicProduct(1e-4));
			init_manifold();
			glutPostRedisplay();
		}
		break;
	case 'I':
	case 'i':
		// initializing animation

		sphere_center = manifold->sample(X_ABSMAX * dir_x, Y_ABSMAX * dir_y); // random point on manifold
		initialized_animation = true;
		ongoing_animation = false;
		glutPostRedisplay();
		break;
	case 'G':
	case 'g':
		// starting animation

		if (!ongoing_animation && initialized_animation) {
			ongoing_animation = true;
			glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
		}
		break;
	case 'S':
	case 's':
		// stopping animation

		ongoing_animation = false;
		break;
	case 'E':
	case 'e':
		// increasing LR

		LEARNING_RATE *= (1.0 + LR_STEP);
		std::cerr << "LEARNING RATE: " << LEARNING_RATE << std::endl;
		glutPostRedisplay();
		break;
	case 'Q':
	case 'q':
		// decreasing LR

		LEARNING_RATE *= (1.0 - LR_STEP);
		std::cerr << "LEARNING RATE: " << LEARNING_RATE << std::endl;
		glutPostRedisplay();
		break;
	case 'R':
	case 'r':
		// nullifying scale, translation and rotation matrices

		mouse_pos = std::make_pair(0, 0);

		init_matrix(rotate_matrix);
		init_matrix(scale_matrix);
		init_matrix(translate_matrix);
		glutPostRedisplay();

		break;
	}
}

void on_special(int key, int x, int y) {
	/*
	Callback for handling arrow keys events.
	*/

	if (manifold) {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		switch (key) {
		case GLUT_KEY_LEFT:
			glTranslatef(TRANSLATE_FACTOR, 0, 0);
			break;
		case GLUT_KEY_RIGHT:
			glTranslatef(-TRANSLATE_FACTOR, 0, 0);
			break;
		case GLUT_KEY_UP:
			glTranslatef(0, 0, TRANSLATE_FACTOR);
			break;
		case GLUT_KEY_DOWN:
			glTranslatef(0, 0, -TRANSLATE_FACTOR);
			break;
		case GLUT_KEY_PAGE_UP:
			glTranslatef(0, -TRANSLATE_FACTOR, 0);
			break;
		case GLUT_KEY_PAGE_DOWN:
			glTranslatef(0, TRANSLATE_FACTOR, 0);
			break;
		}

		glMultMatrixf(translate_matrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, translate_matrix);
		glPopMatrix();
		glutPostRedisplay();
	}
}

void on_reshape(int width, int height) {
	window_size = std::make_pair(width, height);

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
	if (manifold) {
		GLdouble center_x = static_cast<GLdouble>(barycenter.x);
		GLdouble center_y = static_cast<GLdouble>(barycenter.z);
		GLdouble center_z = static_cast<GLdouble>(barycenter.y);
		GLdouble eye_x = center_x + 20;
		GLdouble eye_y = center_y + 30;
		GLdouble eye_z = center_z + 20;

		gluLookAt(
			eye_x, eye_y, eye_z,
			center_x, center_y, center_z,
			0, 1, 0
		);
	}
	glMultMatrixf(rotate_matrix); // rotation from mouse motion
	glMultMatrixf(scale_matrix); // scale from mouse scroll
	glMultMatrixf(translate_matrix); // translation from arrow keys

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
		if (initialized_animation)
			draw_sphere(SPHERE_RADIUS);
	}

	glutSwapBuffers();
}

void on_timer(int id) {
	/*
	Callback for timer events.
	Each timer event draws manifold after GD iteration.
	*/

	if (id != TIMER_ID)
		return;

	glutPostRedisplay();

	std::cerr << "Sphere center: " << sphere_center << std::endl;
	std::cerr << "Gradient: " << manifold->grad(sphere_center) << std::endl;
	sphere_center = sphere_center - manifold->grad(sphere_center) * LEARNING_RATE;
	sphere_center.x = std::clamp(sphere_center.x, -X_ABSMAX, X_ABSMAX); // ensure to fit the window
<<<<<<< HEAD
	sphere_center.y = std::clamp(sphere_center.y, -Y_ABSMAX, Y_ABSMAX); // ensure to fit the window
=======
	sphere_center.y = std::clamp(sphere_center.x, -Y_ABSMAX, Y_ABSMAX); // ensure to fit the window
>>>>>>> 251894278892f576fbe2c4a5436908177ec27ba7
	sphere_center = manifold->sample(sphere_center.x, sphere_center.y);

	if (ongoing_animation)
		glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
}

void on_mouse(int button, int state, int x, int y) {
	/*
	Callback for mouse events.
	Handling scroll events and mouse rotations.
	*/

	mouse_pos = std::make_pair(x, y);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if (button == 3)
		glScalef(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR);
	else if (button == 4)
		glScalef(1 / SCALE_FACTOR, 1 / SCALE_FACTOR, 1 / SCALE_FACTOR);

	glMultMatrixf(scale_matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, scale_matrix);
	glPopMatrix();
	glutPostRedisplay();
}

void on_motion(int x, int y) {
	int delta_x = x - mouse_pos.first;
	int delta_y = y - mouse_pos.second;
	mouse_pos = std::make_pair(x, y);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	GLfloat angle_factor = 180.0;
	GLfloat angle_x = static_cast<GLfloat>(delta_x) / window_size.first;
	GLfloat angle_y = static_cast<GLfloat>(delta_y) / window_size.second;
	glRotatef(angle_factor * angle_x, 0, 1, 0);
	glRotatef(angle_factor * angle_y, 1, 0, 0);
	glMultMatrixf(rotate_matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, rotate_matrix);
	glPopMatrix();

	glutPostRedisplay();
}

void init_matrix(GLfloat* matrix) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();
}

void init_manifold(void) {
	if (!manifold->get_lazy_flag()) {
		std::pair<double, double> x_range = { -X_ABSMAX, X_ABSMAX }; // TODO: hardcoded values
		std::pair<double, double> y_range = { -Y_ABSMAX, Y_ABSMAX }; // TODO: hardcoded values
		manifold_pts = manifold->sample(
			x_range,
			y_range,
			MANIFOLD_SAMPLE_SIZE
		);
		barycenter = calc_barycenter(TensorManipulation::mat2vec(manifold_pts));
	}

	manifold->set_lazy_flag(true);
}

void draw_manifold() {
	/*
	Draws 2D manifold in the 3D.
	It's drawn as stripes of triangles.
	*/

	GLfloat ambient_material[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat diffuse_material[] = { 0.25, 0.25, 0.75, 1 };
	GLfloat specular_material[] = { 0.5, 0, 0, 1 };
	GLfloat shininess = 10;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_material);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	auto sampled = TensorManipulation::transpose(manifold_pts);

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

void draw_sphere(double r) {
	/*
	Draws little sphere in the given point.
	Used for GD visualization.
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
	glTranslatef(sphere_center.x, sphere_center.z, sphere_center.y);
	glutSolidSphere(r, 50, 50);
	glPopMatrix();
}

Point calc_barycenter(const std::vector<Point>& pts) {
	Point barycenter;
	for (const Point& pt : pts)
		barycenter = barycenter + pt;
	barycenter = barycenter * (1.0 / static_cast<double>(pts.size()));

	return barycenter;
}