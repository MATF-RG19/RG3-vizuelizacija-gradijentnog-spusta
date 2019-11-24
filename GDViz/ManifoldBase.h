#pragma once
#include <vector>
#include <utility>

#include "shared.h"

class ManifoldBase {
public:
	virtual Point func(double, double) = 0;
	virtual double grad_x(const Point&) = 0;
	virtual double grad_y(const Point&) = 0;
	virtual double grad_z(const Point&) = 0;

	Point normal_vec(const Point&);
	Point normal_vec(double, double, double);

	std::vector<std::vector<Point>> sample(
		const std::pair<double, double>&,
		const std::pair<double, double>&,
		int);
};
