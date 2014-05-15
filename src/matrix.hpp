#ifndef MATRIX_H
#define MATRIX_H


#include <iostream>

#include "type.hpp"

template <class T>
class Vector;

template <class T>
class Matrix2x2
{
	private:
		T m_a, m_b, m_c, m_d; // top row (m_a, m_b) and bottom row (m_c, m_d)

	public:
		Matrix2x2();
		Matrix2x2(T A, T B, T C, T D);

		const T& a() const {return m_a;}
		const T& b() const {return m_b;}
		const T& c() const {return m_c;}
		const T& d() const {return m_d;}

		Matrix2x2 operator*(const Matrix2x2& m) const; // matrix product with matrix
		Vector<T> operator*(const Vector<T>& m) const; // matrix product with matrix
		template <class U> Matrix2x2(const Matrix2x2<U>& m);

		T det() const; // matrix determinant
		void transpose();
		void inverse();
		cf_T getAngle() const;
		cf_T getScale() const;
//		int getHolladayDelta(int& approx) const;
//		double getBijAngle() const;
};

template <class T>
std::ostream& operator<<(std::ostream &os, const Matrix2x2<T>& m);
template <class T>
std::istream& operator>>(std::istream &is, Matrix2x2<T>& m);


#include "matrix.tpl"

#endif
