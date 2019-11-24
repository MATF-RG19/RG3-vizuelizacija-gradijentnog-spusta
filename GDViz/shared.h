#pragma once

#include <iostream>
#include <vector>
#include <math.h>

const double EPS = 1e-16;
const double PI = 3.14159265359;

struct Point {
	double x, y, z; // raw point
	double nx, ny, nz; // normal vector

	Point();
	Point(double, double, double);
	Point(double, double, double, double, double, double);

	friend std::ostream& operator << (std::ostream& out, const Point& pt);
};

namespace LinearAlgebra {
	double norm(const Point&);
	Point normalize(const Point&);
	std::vector<double> linspace(double, double, int);
	std::vector<double> arange(double, double, double);
}

namespace TensorManipulation {
	template<typename T>
	std::vector<T> mat2vec(const std::vector<std::vector<T>>& mat) {
		int n = static_cast<int>(mat.size());
		std::vector<T> vec(n * n);

		for (auto i = 0; i < mat.size(); i++) {
			for (auto j = 0; j < mat[i].size(); j++) {
				vec[i * n + j] = mat[i][j];
			}
		}

		return vec;
	}

	template<typename T>
	std::vector<std::vector<T>> vec2mat(const std::vector<T> vec) {
		int n = static_cast<int>(round(sqrt(vec.size())));
		std::vector<std::vector<T>> mat(n, std::vector<T>(n));

		for (auto i = 0; i < mat.size(); i++) {
			for (auto j = 0; j < mat[i].size(); j++) {
				mat[i][j] = vec[i * n + j];
			}
		}

		return mat;
	}

	template<typename T>
	std::vector<std::vector<T>> transpose(const std::vector<std::vector<T>>& mat) {
		int n = static_cast<int>(mat.size());
		int m = static_cast<int>(mat[0].size());

		std::vector<std::vector<T>> mat_T = std::vector<std::vector<T>>(m, std::vector<T>(n));
		for (auto i = 0; i < mat.size(); i++) {
			for (auto j = 0; j < mat[i].size(); j++) {
				mat_T[j][i] = mat[i][j];
			}
		}

		return mat_T;
	}
}

template<typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& vec) {
	for (auto i = 0; i < vec.size(); i++) {
		if (i)
			out << ", ";

		out << vec[i];
	}

	return out;
}
