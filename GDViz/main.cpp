#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <algorithm>

#include <GL/glut.h>

#include "CubicProduct.h"
#include "EllipticParaboloid.h"
#include "HyperbolicParaboloid.h"
#include "main.h"
#include "MultivariateSine.h"
#include "shared.h"
#include "Texture.h"

/*
-Upon starting the program, background will be black without any other object.
By selecting proper manifold (see on_keyboard()), manifold be drawn.

-Upon initializing animation (see on_keyboard()) little sphere will be shown.

-Upon starting the animation, GD will start.
*/

bool initialized_animation, ongoing_animation;
double LEARNING_RATE = 0.01; // can be changed on keypress
GLfloat rotate_matrix[16], scale_matrix[16], translate_matrix[16];

Point sphere_center; // sphere used for GD
Point barycenter; // barycenter of sampled points

ManifoldBase* manifold = nullptr; // initially, manifold is not chosen
std::pair<int, int> mouse_pos; // mouse coordinates
std::pair<int, int> window_size = {
	static_cast<int>(800.0 / SPLIT_SCREEN_RATIO),
	800
}; // window size; updates on reshape
std::vector<std::vector<Point>> manifold_pts; // sampled points of manifold


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

	// random sample point
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
	case Interface::init_animation:
		// initializing animation

		sphere_center = manifold->sample(X_ABSMAX * dir_x, Y_ABSMAX * dir_y); // random point on manifold
		initialized_animation = true;
		ongoing_animation = false;
		glutPostRedisplay();
		break;
	case Interface::start_animation:
		// starting animation

		if (!ongoing_animation && initialized_animation) {
			ongoing_animation = true;
			glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
		}
		break;
	case Interface::pause_animation:
		// stopping animation

		ongoing_animation = false;
		break;
	case Interface::increase_lr:
		// increasing LR

		LEARNING_RATE *= (1.0 + LR_STEP);
		glutPostRedisplay();
		break;
	case Interface::decrease_lr:
		// decreasing LR

		LEARNING_RATE *= (1.0 - LR_STEP);
		glutPostRedisplay();
		break;
	case Interface::reset_view:
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
}

void on_display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(
		static_cast<GLint>(0),
		static_cast<GLint>(0),
		static_cast<GLsizei>(window_size.first * SPLIT_SCREEN_RATIO),
		static_cast<GLsizei>(window_size.second)
	);
	show_visualization();

	glViewport(
		static_cast<GLint>(window_size.first * SPLIT_SCREEN_RATIO),
		static_cast<GLint>(0),
		static_cast<GLsizei>(window_size.first * (1 - SPLIT_SCREEN_RATIO)),
		static_cast<GLsizei>(window_size.second)
	);
	show_log();

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

	sphere_center = sphere_center - manifold->grad(sphere_center) * LEARNING_RATE; // GD step
	sphere_center.x = std::clamp(sphere_center.x, -X_ABSMAX, X_ABSMAX); // ensure to fit the window
	sphere_center.y = std::clamp(sphere_center.y, -Y_ABSMAX, Y_ABSMAX); // ensure to fit the window
	sphere_center = manifold->sample(sphere_center.x, sphere_center.y); // sampling new point

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
	/*
	Rotate scene around X or Y axis based on mouse motion.
	*/

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
	glGetFloatv(GL_MODELVIEW_MATRIX, rotate_matrix); // saving new model matrix
	glPopMatrix();

	glutPostRedisplay();
}

void show_visualization() {
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		static_cast<GLdouble>(60.0),
		static_cast<GLdouble>(window_size.first / window_size.second),
		static_cast<GLdouble>(1.0),
		static_cast<GLdouble>(100.0)
	);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (manifold) {
		GLdouble center_x = static_cast<GLdouble>(barycenter.x);
		GLdouble center_y = static_cast<GLdouble>(barycenter.z);
		GLdouble center_z = static_cast<GLdouble>(barycenter.y);
		GLdouble eye_x = center_x + 20.0;
		GLdouble eye_y = center_y + 30.0;
		GLdouble eye_z = center_z + 20.0;

		gluLookAt(
			eye_x, eye_y, eye_z,
			center_x, center_y, center_z,
			0, 1, 0
		);
	}
	glMultMatrixf(rotate_matrix); // rotation from mouse motion
	glMultMatrixf(scale_matrix); // scale from mouse scroll
	glMultMatrixf(translate_matrix); // translation from arrow keys

	GLfloat light_position[] = { 0, 1, 0, 0 };
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

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_NORMALIZE);
	glDisable(GL_SMOOTH);
}

void show_log() {
	/*
	Showing log on the screen: learning rate, position and gradient's magnitude.
	*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(
		static_cast<GLdouble>(0),
		static_cast<GLdouble>(window_size.first * (1 - SPLIT_SCREEN_RATIO)),
		static_cast<GLdouble>(0),
		static_cast<GLdouble>(window_size.second)
	);
	glTranslatef(
		static_cast<GLfloat>(0),
		static_cast<GLfloat>(window_size.second - TEXT_SPACING),
		static_cast<GLfloat>(0)
	);
	glScalef(
		static_cast<GLfloat>(FONT_SCALE),
		static_cast<GLfloat>(FONT_SCALE),
		static_cast<GLfloat>(1)
	);

	std::vector<std::string> msgs;
	if (manifold) {
		std::string msg = "Selected manifold: ";
		switch (manifold_type){
		case ManifoldType::elliptic_paraboloid:
			msg += "Elliptic paraboloid\n";
			break;
		case ManifoldType::hyperbolic_paraboloid:
			msg += "Hyperbolic paraboloid\n";
			break;
		case ManifoldType::multivariate_sine:
			msg += "Multivariate sine\n";
			break;
		case ManifoldType::cubic_product:
			msg += "Cubic product\n";
			break;
		default:
			break;
		}
		msgs.emplace_back(msg);
		msgs.emplace_back("\n");

		msgs.emplace_back(std::string(1, Interface::init_animation) + ": initialize animation\n");
		msgs.emplace_back(std::string(1, Interface::start_animation) + ": start animation\n");
		msgs.emplace_back(std::string(1, Interface::pause_animation) + ": pause animation\n");
		msgs.emplace_back(std::string(1, Interface::increase_lr) +  ": increase learning rate\n");
		msgs.emplace_back(std::string(1, Interface::decrease_lr) + ": decrease learning rate\n");
		msgs.emplace_back(std::string(1, Interface::reset_view) + ": reset view\n");
		msgs.emplace_back("\n");

		msgs.emplace_back("Learning rate: " + std::to_string(LEARNING_RATE) + "\n");
		
		if (initialized_animation) {
			msgs.emplace_back(
				"Current point: (" +
				std::to_string(sphere_center.x) +
				", " +
				std::to_string(sphere_center.y) +
				", " +
				std::to_string(sphere_center.z) +
				")\n"
			);

			if (ongoing_animation)
				msgs.emplace_back(
					"Gradient magnitude: " +
					std::to_string(LinearAlgebra::norm(manifold->grad(sphere_center)))
				);
		}
	}
	else {
		msgs.emplace_back("(1) Elliptic paraboloid");
		msgs.emplace_back("(2) Hyperbolic paraboloid");
		msgs.emplace_back("(3) Multivariate sine");
		msgs.emplace_back("(4) Cubic product");
	}

	render_text(msgs);
}

void render_text(const std::vector<std::string>& msgs) {
	/*
	Rendering text on the scene.
	Text is scaled on reasonable size.
	*/

	glLineWidth(static_cast<GLfloat>(1));
	glColor3ubv(TEXT_COLOR);

	for (const std::string msg : msgs) {
		glPushMatrix();
		for (const char c : msg) {
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, c);
		}
		glPopMatrix();
		glScalef(1.0 / FONT_SCALE, 1.0 / FONT_SCALE, 1);
		glTranslatef(0, -TEXT_SPACING, 0);
		glScalef(FONT_SCALE, FONT_SCALE, 1);
	}
}

void init_matrix(GLfloat* matrix) {
	/*
	Makes given matrix identity.
	*/

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();
}

void init_manifold(void) {
	/*
	Sampling selected manifold in case it's not.
	*/

	if (!manifold->get_lazy_flag()) {
		std::pair<double, double> x_range = { -X_ABSMAX, X_ABSMAX };
		std::pair<double, double> y_range = { -Y_ABSMAX, Y_ABSMAX };
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

	// violet
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

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(sphere_center.x, sphere_center.z, sphere_center.y);

	Texture texture("Resources/sphere_texture.bmp"); // used for mapping texture to sphere
	texture.bind();

	std::vector<double> theta_vec = LinearAlgebra::linspace(0.0, 2.0 * PI,
		MANIFOLD_SAMPLE_SIZE);
	std::vector<double> phi_vec = LinearAlgebra::linspace(0.0, PI,
		MANIFOLD_SAMPLE_SIZE);

	glBegin(GL_POINTS); // sampling sphere
	{
		for (const double& phi : phi_vec) {
			for (const double& theta : theta_vec) {
				GLfloat x = static_cast<GLfloat>(r * cos(theta) * sin(phi));
				GLfloat y = static_cast<GLfloat>(r * sin(theta) * sin(phi));
				GLfloat z = static_cast<GLfloat>(r * cos(phi));

				glNormal3f(2.0 * x, 2.0 * z, 2.0 * y);
				glTexCoord2f(0.5 * (1.0 + x / r), 0.5 * (1.0 + y / r)); // fitting in (s, t) system
				glVertex3f(x, z, y);
			}
		}
	}
	glEnd();
	
	glPopMatrix();
	texture.unbind();
	glDisable(GL_TEXTURE_2D);
}

Point calc_barycenter(const std::vector<Point>& pts) {
	/*
	Calculating barycenter of sampled points.
	Camera will be looking at the barycenter.
	*/
	
	Point barycenter;
	for (const Point& pt : pts)
		barycenter = barycenter + pt;
	barycenter = barycenter * (1.0 / static_cast<double>(pts.size()));

	return barycenter;
}