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

	if (x_range.first >= x_range.second - EPS ||
		y_range.first >= y_range.second - EPS)
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

Point ManifoldBase::normal_vec(const Point& pt) {
	double nx = -grad_x(pt);
	double ny = -grad_y(pt);
	double nz = -grad_z(pt);

	return *new Point(nx, ny, nz);
}

Point ManifoldBase::normal_vec(double x, double y, double z) {
	Point pt = *new Point(x, y, z);

	return normal_vec(pt);
}