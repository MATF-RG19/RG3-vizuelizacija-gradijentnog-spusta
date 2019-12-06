#include <stdexcept>

#include "ManifoldBase.h"
#include "shared.h"

std::vector<std::vector<Point>> ManifoldBase::sample(
	const std::pair<double, double>& x_range,
	const std::pair<double, double>& y_range,
	int sample_size) {
	/*
	Sample function in the given range with given number of points.
	Sampled points as well as normal vectors in these points are returned.
	*/

	if (x_range.first > x_range.second + EPS ||
		y_range.first > y_range.second + EPS)
		throw std::domain_error("Ranges are not valid");

	std::vector<double> x_linspace = LinearAlgebra::linspace(
		x_range.first, x_range.second, sample_size);
	std::vector<double> y_linspace = LinearAlgebra::linspace(
		y_range.first, y_range.second, sample_size);
	
	std::vector<std::vector<Point>> sampled(
		sample_size, std::vector<Point>(sample_size));
	for (int i = 0; i < sample_size; i++) {
		for (int j = 0; j < sample_size; j++) {
			double x = x_linspace[i], y = y_linspace[j];
			sampled[i][j] = func(x, y);
		}
	}

	return sampled;
}

Point ManifoldBase::sample(double x, double y) {
	/*
	Sampling singleton point.
	*/

	auto sampled = ManifoldBase::sample({ x, x }, { y, y }, 1);

	return sampled[0][0];
}

Point ManifoldBase::normal_vec(const Point& pt) {
	/*
	Calculating normal vector for the given point.
	Normal vector is the vector orthogonal to the tangent plane.

	(!) Normal vector is not normalized.
	*/

	double nx = -grad_x(pt);
	double ny = -grad_y(pt);
	double nz = -grad_z(pt);

	return *new Point(nx, ny, nz);
}

Point ManifoldBase::normal_vec(double x, double y, double z) {
	/*
	Calculating normal vector for the given point.
	Normal vector is the vector orthogonal to the tangent plane.

	(!) Normal vector is not normalized.
	*/

	Point pt = *new Point(x, y, z);

	return normal_vec(pt);
}

Point ManifoldBase::func(double x, double y) {
	return *new Point();
}

double ManifoldBase::grad_x(const Point& pt) {
	return 0.0;
}

double ManifoldBase::grad_y(const Point& pt) {
	return 0.0;
}

double ManifoldBase::grad_z(const Point& pt) {
	return 0.0;
}

void ManifoldBase::set_lazy_flag(bool lazy_flag) {
	is_calculated = lazy_flag;
}

bool ManifoldBase::get_lazy_flag(void) {
	return is_calculated;
}