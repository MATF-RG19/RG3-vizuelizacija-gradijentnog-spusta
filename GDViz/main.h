// constants
const int INIT_WINDOW_SIZE[] = { 800, 800 };
const int INIT_WINDOW_POSITION[] = { 100, 100 };
const int MANIFOLD_SAMPLE_SIZE = 500;
const int TIMER_ID = 0;
const int TIMER_INTERVAL = 10;
const double LEARNING_RATE = 0.01;
const double SPHERE_RADIUS = 0.25;
const double ZOOM_STEP = 0.1;
const double ZOOM_THRESHOLD = 5.0;

void on_keyboard(unsigned char key, int x, int y);
void on_reshape(int width, int height);
void on_display(void);
void on_timer(int);
void on_mouse(int, int, int, int);

void init_manifold(void);
void draw_manifold(void);
void draw_sphere(double);

enum class ManifoldType {
	elliptic_paraboloid, hyperbolic_paraboloid, multivariate_sine
} manifold_type;