#ifndef _ELLIPTIC_PARABOLOID_H
#define _ELLIPTIC_PARABOLOID_H

#include "ManifoldBase.h"
#include "shared.h"

/*
Elliptic paraboloid in form z = (x^2)/(a^2) + (y^2)/(b^2).

(!) Convex function with global minima. Complete convergence behavior expected.
*/

class EllipticParaboloid :
	public ManifoldBase {

private:
	double a, b;
	double a2, b2; // squared a and b

public:
	EllipticParaboloid(double, double);

	Point func(double, double);
	double grad_x(const Point&);
	double grad_y(const Point&);
	double grad_z(const Point&);
	double get_a(void);
	double get_b(void);
};

#endif
