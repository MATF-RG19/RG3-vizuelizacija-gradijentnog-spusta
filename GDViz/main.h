#include <utility>
#include <vector>
#include <random>

#include "shared.h"

// constants
const int MANIFOLD_SAMPLE_SIZE = 500; // how many points to sample from manifold
const int TIMER_ID = 0;
const int TIMER_INTERVAL = 10; // how much does each iteration last
const double LR_STEP = 0.25; // percentage of LR change
const double SPHERE_RADIUS = 0.25; // size of GD sphere
const double SCALE_FACTOR = 1.1; // how much does zoom change per keypress
const double TRANSLATE_FACTOR = 0.25;
const double X_ABSMAX = 10;
const double Y_ABSMAX = 10;

const std::pair<int, int> WINDOW_POS = { 0, 0 };
std::default_random_engine rnd_gen;
std::uniform_real_distribution<double> distribution(-1.0, 1.0);

void on_keyboard(unsigned char key, int x, int y);
void on_special(int key, int x, int y);
void on_reshape(int width, int height);
void on_display(void);
void on_timer(int);
void on_mouse(int, int, int, int);
void on_motion(int, int);

void init_matrix(GLfloat*);
void init_manifold(void);
void draw_manifold(void);
void draw_sphere(double);
Point calc_barycenter(const std::vector<Point>&);

enum class ManifoldType {
	/*
	All possible types of manifolds.
	*/

	elliptic_paraboloid, hyperbolic_paraboloid,
	multivariate_sine, cubic_product
} manifold_type;