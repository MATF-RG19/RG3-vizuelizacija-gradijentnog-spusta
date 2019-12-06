#pragma once
#include "ManifoldBase.h"
#include "shared.h"

/*
Hyperbolic paraboloid in form z = (x^2)/(a^2) - (y^2)/(b^2).

(!) Function with saddle point. Divergence behavior expected.
*/

class HyperbolicParaboloid :
	public ManifoldBase {

private:
	double a, b;
	double a2, b2; // squared a and b

public:
	HyperbolicParaboloid(double, double);

	Point func(double, double);
	double grad_x(const Point&);
	double grad_y(const Point&);
	double grad_z(const Point&);
	double get_a(void);
	double get_b(void);
};

