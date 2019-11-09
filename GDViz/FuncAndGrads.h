#pragma once
/*
Contains function declaration and analytic gradients.
*/

#include "shared.h"

static function<double(double, double)> func = [](double x, double y) {return x * x + y * y; };

static function<double(double, double)> grad_x = [](double x, double y) {return 2 * x; };
static function<double(double, double)> grad_y = [](double x, double y) {return 2 * y; };
static function<double(double, double)> grad_z = [](double x, double y) {return -1; };