#include "EllipticParaboloid.h"
#include "shared.h"

EllipticParaboloid::EllipticParaboloid(double a = 1, double b = 1) {
	this->a = a;
	this->b = b;
	this->a2 = a * a;
	this->b2 = b * b;
}

Point EllipticParaboloid::func(double x, double y) {
	/*
	f(x, y) = (x^2)/(a^2) + (y^2)/(b^2) - z
	*/

	double z = (x * x) / a2 + (y * y) / b2;
	Point pt = *new Point(x, y, z);

	Point pt_normal = normal_vec(x, y, z);
	pt.nx = pt_normal.x;
	pt.ny = pt_normal.y;
	pt.nz = pt_normal.z;

	return pt;
}

double EllipticParaboloid::grad_x(const Point& pt) {
	return 2.0 * pt.x / a2;
}

double EllipticParaboloid::grad_y(const Point& pt) {
	return 2.0 * pt.y / b2;
}

double EllipticParaboloid::grad_z(const Point& pt) {
	return -1.0;
}