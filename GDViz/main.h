#ifndef _MAIN_H
#define _MAIN_H

#include <random>
#include <string>
#include <utility>
#include <vector>

#include "shared.h"

// text & font
const unsigned char TEXT_COLOR[] = { 255, 255, 255 };
const double TEXT_SPACING = 30; // spacing between two rows of text
const double FONT_SCALE = 1e-1;
const double TEXT_WIDTH = 1.0;

// window
const double SPLIT_SCREEN_RATIO = 0.6; // ratio of viewports' widths
const double TRANSLATE_FACTOR = 0.25; // how much movement in each direction
const double SCALE_FACTOR = 1.1; // how much does zoom change per keypress

// gradient descent & manifold
const int MANIFOLD_SAMPLE_SIZE = 500; // how many points to sample from manifold
const double LR_STEP = 0.25; // percentage of LR change
const double SPHERE_RADIUS = 0.25; // size of GD sphere
const double X_ABSMAX = 10; // min/max X coordinate of sample region
const double Y_ABSMAX = 10; // min/max Y coordinate of sample region

// timer
const int TIMER_ID = 0;
const int TIMER_INTERVAL = 10; // how much does each iteration last

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

void show_visualization(void);
void show_log(void);
void render_text(const std::vector<std::string>&);

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

namespace Interface {
	const unsigned char init_animation = 'i';
	const unsigned char start_animation = 'g';
	const unsigned char pause_animation = 's';
	const unsigned char increase_lr = 'e';
	const unsigned char decrease_lr = 'q';
	const unsigned char reset_view = 'r';
}

#endif
