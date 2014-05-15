#include "cmath"


template <class T>
const double Vector<T>::vx = 0.5;
template <class T>
const double Vector<T>::vy = 0.866025404;
template <class T>
const double Vector<T>::yu = -0.577350269;
template <class T>
const double Vector<T>::yv = 1.154700538;

template <class T>
Vector<T>::Vector()
: m_x(0), m_y(0)
{}

template <class T>
Vector<T>::Vector(const T i_x, const T i_y)
: m_x(i_x), m_y(i_y)
{}

template <class T>
Vector<cf_T> Vector<T>::hex2cart() const
{
	return Vector<cf_T>((cf_T)m_x + (cf_T)m_y*vx, (cf_T)m_y*vy);
}

template <class T>
Vector<cf_T> Vector<T>::cart2hex() const
{
	return Vector<cf_T>((cf_T)m_x + (cf_T)m_y*yu, (cf_T)m_y*yv);
}

template <class T>
Vector<T> Vector<T>::operator+(const Vector<T>& v) const
{
	return Vector<T>(m_x+v.x(), m_y+v.y());
}

template <class T>
Vector<T> Vector<T>::operator/(const T& div) const
{
	return Vector<T>(m_x/div, m_y/div);
}

template <class T>
Vector<T> Vector<T>::operator*(const T& div) const
{
	return Vector<T>(m_x*div, m_y*div);
}

template <class T> template <class U>
Vector<T>::Vector(const Vector<U>& v)
{
	m_x = (T)v.x();
	m_y = (T)v.y();
}

template <class T>
T Vector<T>::norm() const
{
	return std::sqrt(m_x*m_x + m_y*m_y);
}


template <class T>
std::ostream& operator<<(std::ostream &os, const Vector<T>& v)
{
	os << "(" << v.x() << ", " << v.y() << ")";
	return os;
}
