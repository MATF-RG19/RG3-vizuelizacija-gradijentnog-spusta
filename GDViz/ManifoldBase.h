#ifndef _MANIFOLDBASE_H
#define _MANIFOLDBASE_H

#include <vector>
#include <utility>

#include "shared.h"

/*
Base class for manifolds.

It's abstract class and its' primary usage is for polymorphism.
Each specific manifold should inherit this class.
*/

class ManifoldBase {
protected:
	bool is_calculated = false; // manifold won't be sampled twice

public:
	virtual Point func(double, double);
	virtual double grad_x(const Point&);
	virtual double grad_y(const Point&);
	virtual double grad_z(const Point&);

	Point normal_vec(const Point&);
	Point normal_vec(double, double, double);

	std::vector<std::vector<Point>> sample(
		const std::pair<double, double>&,
		const std::pair<double, double>&,
		int);
	Point sample(double, double);
	Point grad(const Point&);

	void set_lazy_flag(bool);
	bool get_lazy_flag(void);
};

#endif
