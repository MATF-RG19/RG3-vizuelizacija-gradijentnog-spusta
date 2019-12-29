#ifndef _SHARED_H
#define _SHARED_H

/*
Contains functions and constants that are shared across whole project.
*/

#include <iostream> // ostream
#include <math.h> // sqrt
#include <vector>

const double EPS = 1e-16;
const double PI = 3.14159265359;

struct Point {
	double x, y, z; // raw point
	double nx, ny, nz; // normal vector

	Point();
	Point(double, double, double);
	Point(double, double, double, double, double, double);

	Point operator*(const double) const;
	Point operator+(const Point&) const;
	Point operator-(const Point&) const;
	friend std::ostream& operator<<(std::ostream& out, const Point& pt);
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
		/*
		Transforming matrix of shape (N, M) into array of shape (N * M).
		Similar to Python's flatten().
		*/

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
		/*
		Transforming array of shape (N * M) into matrix of shape (N, M).
		*/

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
		/*
		Transposing matrix of shape (N, M) into matrix of shape (M, N).
		*/

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

int sign(double);

#endif
