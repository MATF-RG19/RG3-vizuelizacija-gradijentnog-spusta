#pragma once
#include "ManifoldBase.h"
#include "shared.h"

class EllipticParaboloid :
	public ManifoldBase {

private:
	double a, b;
	double a2, b2;

public:
	EllipticParaboloid(double, double);

	Point func(double, double);
	double grad_x(const Point&);
	double grad_y(const Point&);
	double grad_z(const Point&);
};