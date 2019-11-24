#include <math.h>

#include "MultivariateSine.h"
#include "shared.h"

MultivariateSine::MultivariateSine(double a = 1) {
	this->a = a;
}

Point MultivariateSine::func(double x, double y) {
	/*
	z = a * sin(x * y)
	*/

	double z = a * sin(x * y);
	Point pt = *new Point(x, y, z);

	Point pt_normal = normal_vec(x, y, z);
	pt.nx = pt_normal.x;
	pt.ny = pt_normal.y;
	pt.nz = pt_normal.z;

	return pt;
}

double MultivariateSine::grad_x(const Point& pt) {
	return static_cast<double>(sign(a)) * a * pt.y * cos(pt.x * pt.y);
}

double MultivariateSine::grad_y(const Point& pt) {
	return static_cast<double>(sign(a)) * a * pt.x * cos(pt.x * pt.y);
}

double MultivariateSine::grad_z(const Point& pt) {
	return static_cast<double>(sign(a)) * (-1.0);
}
