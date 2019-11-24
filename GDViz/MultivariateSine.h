#pragma once

#include "ManifoldBase.h"
#include "shared.h"

class MultivariateSine :
	public ManifoldBase {

private:
	double a;

public:
	MultivariateSine(double);

	Point func(double, double);
	double grad_x(const Point&);
	double grad_y(const Point&);
	double grad_z(const Point&);
};

