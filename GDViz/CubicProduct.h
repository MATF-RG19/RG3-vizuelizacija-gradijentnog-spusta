#ifndef _CUBICPRODUCT_H
#define _CUBICPRODUCT_H

#include "ManifoldBase.h"
#include "shared.h"

/*
Cubic product function in form z = a * (xy) ^ 3.

(!) Non-convex function with vanishing gradient at (0, 0).
*/

class CubicProduct :
	public ManifoldBase {

private:
	double a;

public:
	CubicProduct(double);

	Point func(double, double);
	double grad_x(const Point&);
	double grad_y(const Point&);
	double grad_z(const Point&);
	double get_a(void);
};

#endif
