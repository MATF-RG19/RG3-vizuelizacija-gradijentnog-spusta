// constants
const int INIT_WINDOW_SIZE[] = { 800, 800 };
const int INIT_WINDOW_POSITION[] = { 100, 100 };
const int MANIFOLD_SAMPLE_SIZE = 500; // how many points to sample from manifold
const int TIMER_ID = 0;
const int TIMER_INTERVAL = 10; // how much does each iteration last
const double LR_STEP = 0.25; // percentage of LR change
const double SPHERE_RADIUS = 0.25; // size of GD sphere
const double SCALE_FACTOR = 1.1; // how much does zoom change per keypress
const double TRANSLATE_FACTOR = 0.25;

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

enum class ManifoldType {
	/*
	All possible types of manifolds.
	*/

	elliptic_paraboloid, hyperbolic_paraboloid, multivariate_sine
} manifold_type;