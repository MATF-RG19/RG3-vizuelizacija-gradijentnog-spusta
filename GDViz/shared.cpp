#include <math.h>
#include <stdexcept>
#include <iostream>

#include "shared.h"

Point::Point() {
	x = y = z = 0;
	nx = ny = nz = 0;
}

Point::Point(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
	nx = ny = nz = 0;
}

Point::Point(double x, double y, double z, double nx, double ny, double nz) {
	Point::Point(x, y, z);
	this->nx = nx;
	this->ny = ny;
	this->nz = nz;
}

Point Point::operator*(const double lmbd) const {
	Point ret;
	ret.x = lmbd * this->x;
	ret.y = lmbd * this->y;
	ret.z = lmbd * this->z;

	return ret;
}

Point Point::operator+(const Point& pt) const {
	Point ret;
	ret.x = this->x + pt.x;
	ret.y = this->y + pt.y;
	ret.z = this->z + pt.z;
	ret.nx = this->nx + pt.nx;
	ret.ny = this->ny + pt.ny;
	ret.nz = this->nz + pt.nz;

	return ret;
}

Point Point::operator-(const Point& pt) const {
	return *this + pt * (-1.0);
}

std::ostream& operator<<(std::ostream& out, const Point& pt) {
	/*
	Returns (x, y, z; nx, ny, nz).
	*/

	out << "(" << pt.x << ", " << pt.y << ", " << pt.z << "; " <<
		pt.nx << ", " << pt.ny << ", " << pt.nz << ")";

	return out;
}

std::vector<double> LinearAlgebra::linspace(double a, double b, int n) {
	/*
	Equidistantly samples range [a, b] in n points.

	Throws:
		-domain_error: Negative number of samples.
	*/

	if (n < 0)
		throw std::domain_error("Number of samples has to be non-negative");
	else if (n == 0)
		return std::vector<double>();

	std::vector<double> samples(n, 0);
	samples[0] = a;
	if (n > 0) {		
		samples[n - 1] = b;

		double h = (b - a) / (static_cast<double>(n) - 1);
		for (int i = 1; i <= n - 2; i++) {
			samples[i] = a + static_cast<double>(i) * h;
		}
	}

	return samples;
}

std::vector<double> LinearAlgebra::arange(double a, double b, double h) {
	/*
	Equidistantly samples range [a, b] using step h if it's possible.

	Throws:
		-domain_error: Negative step.
	*/

	if (h <= EPS)
		throw std::domain_error("Step has to be strictly positive");

	if (a >= b - EPS)
		return std::vector<double>();

	std::vector<double> samples;
	samples.emplace_back(a);
	for (int i = 1;; i++) {
		double val = a + static_cast<double>(i)* h;
		if (val >= b - EPS)
			break;

		samples.emplace_back(val);
	}

	return samples;
}

double LinearAlgebra::norm(const Point& pt) {
	return sqrt(pt.x * pt.x + pt.y * pt.y + pt.z * pt.z);
}

Point LinearAlgebra::normalize(const Point& pt) {
	/*
	Normalizing given point to unit norm.

	Throws:
		-domain_error: Zero point.
	*/

	double pt_norm = LinearAlgebra::norm(pt);
	if (abs(pt_norm) <= EPS)
		throw std::domain_error("Zero-vector can't be normalized");

	Point ret(pt.x / pt_norm, pt.y / pt_norm, pt.z / pt_norm);

	return ret;
}

int sign(double x) {
	/*
	Returns sign of a number.
	*/

	if (abs(x) <= EPS)
		return 0;
	else if (x > EPS)
		return 1;
	return -1;
}
