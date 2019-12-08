#include <math.h>

#include "CubicProduct.h"
#include "shared.h"

CubicProduct::CubicProduct(double a = 1) {
	this->a = a;
}

Point CubicProduct::func(double x, double y) {
	/*
	z = a * (x*y)^3
	*/

	double z = a * pow(x * y, 3);
	Point pt(x, y, z);

	Point pt_normal = normal_vec(x, y, z);
	pt.nx = pt_normal.x;
	pt.ny = pt_normal.y;
	pt.nz = pt_normal.z;

	return pt;
}

double CubicProduct::grad_x(const Point& pt) {
	return 3.0 * a * pt.y * pow(pt.x * pt.y, 2);
}

double CubicProduct::grad_y(const Point& pt) {
	return 3.0 * a * pt.x * pow(pt.x * pt.y, 2);
}

double CubicProduct::grad_z(const Point& pt) {
	return -1.0;
}

double CubicProduct::get_a(void) {
	return a;
}