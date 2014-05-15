#ifndef VECTOR_H
#define VECTOR_H


#include <iostream>

#include "type.hpp"


template <class T>
class Vector
{
	template <class U>
	friend class Vector;

	private:
		T m_x, m_y;
		static const double vx, vy, yu, yv;

	public:
		Vector();
		Vector(const T i_x, const T i_y);

		const T& x() const { return m_x; };
		const T& y() const { return m_y; };
		T& x() { return m_x; };
		T& y() { return m_y; };
		Vector<cf_T> hex2cart() const ;
		Vector<cf_T> cart2hex() const ;

		Vector<T> operator+(const Vector<T>& v) const;
		Vector<T> operator/(const T& div) const;
		Vector<T> operator*(const T& div) const;
		template <class U> Vector(const Vector<U>& v);
		T norm() const;
};

template <class T>
std::ostream& operator<<(std::ostream &os, const Vector<T>& v);

#include "vector.tpl"

typedef Vector<cd_T> DPoint;
typedef Vector<cf_T> Point;

#endif
