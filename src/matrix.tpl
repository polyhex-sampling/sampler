#include <cmath>
#include <cstdlib>

#include "vector.hpp"


template <class T>
Matrix2x2<T>::Matrix2x2()
: m_a(0), m_b(0), m_c(0), m_d(0)
{}

template <class T>
Matrix2x2<T>::Matrix2x2(T a, T b, T c, T d)
: m_a(a), m_b(b), m_c(c), m_d(d)
{}

template <class T>
Matrix2x2<T> Matrix2x2<T>::operator*(const Matrix2x2<T>& m) const
{
	T na = m_a*m.a() + m_b*m.c();
	T nb = m_a*m.b() + m_b*m.d();
	T nc = m_c*m.a() + m_d*m.c();
	T nd = m_c*m.b() + m_d*m.d();

	return Matrix2x2<T>(na, nb, nc, nd);
}

template <class T>
Vector<T> Matrix2x2<T>::operator*(const Vector<T>& v) const
{
	Vector<T> res;
	res.x() = m_a*v.x() + m_b*v.y();
	res.y() = m_c*v.x() + m_d*v.y();

	return res;
}

template <class T> template <class U>
Matrix2x2<T>::Matrix2x2(const Matrix2x2<U>& m)
{
	m_a = (T)m.a();
	m_b = (T)m.b();
	m_c = (T)m.c();
	m_d = (T)m.d();
}

template <class T>
T Matrix2x2<T>::det() const
{
	return m_a*m_d-m_b*m_c;
}

template <class T>
void Matrix2x2<T>::transpose()
{
	T b = m_b;
	m_b = m_c;
	m_c =  b;
}

template <class T>
void Matrix2x2<T>::inverse()
{
	T a=m_a, b=m_b, c=m_c, d=m_d;
	T det = this->det();
	m_a = d/det;
	m_b = -b/det;
	m_c = -c/det;
	m_d = a/det;
}

template <class T>
cf_T Matrix2x2<T>::getAngle() const
{
	Vector<cd_T> vHex(m_a, m_b);
	Vector<cf_T> vCart = vHex.hex2cart();
	return std::atan(vCart.y()/vCart.x());
}

template <class T>
cf_T Matrix2x2<T>::getScale() const
{
	Vector<cd_T> vHex(m_a, m_b);
	Vector<cf_T> vCart = vHex.hex2cart();
	return std::sqrt(vCart.x()*vCart.x() + vCart.y()*vCart.y());
}

//double Matrix2x2::getBijAngle() const
//{
//	int ci = det();
//
//	double m_a;
//	double m_b;
//	double m_c = ci;
//
//	switch(ci)
//	{
//		case 1:
//			m_a = 1;
//			m_b = 0;
//			break;
//		case 7:
//			m_a = 3;
//			m_b = 5;
//			break;
//		case 13:
//			m_a = 8;
//			m_b = 7;
//			break;
//		case 19:
//			m_a = 5;
//			m_b = 16;
//			break;
//		case 21:
//			m_a = 15;
//			m_b = 9;
//			break;
//		case 31:
//			m_a = 24;
//			m_b = 11;
//			break;
//		case 37:
//			m_a = 7;
//			m_b = 33;
//			break;
//		case 43:
//			m_a = 35;
//			m_b = 13;
//			break;
//		case 57:
//			m_a = 48;
//			m_b = 15;
//			break;
//		default:
//			std::cerr << "Unknown BijMatrix (det = " << ci << ")" << std::endl;
//			std::exit(EXIT_FAILURE);
//	}
//
//	double angle = std::acos((m_a*m_a+m_c*m_c-m_b*m_b)/(2*m_a*m_c));
//	return angle;
//}
//
//int Matrix2x2::getHolladayDelta(int& approx) const
//{
//	int ci = det();
//	int result;
//	switch(ci)
//	{
//		case 1:
//			result = 0;
//			break;
//		case 7:
//			{int tmp[3] = {5, 19, 19};
//				result = tmp[approx-1];}
//			break;
//		case 13:
//			{int tmp[3] = {10, 23, 1037};
//				result = tmp[approx-1];}
//			break;
//		case 19:
//			{int tmp[3] = {8, 293, 2820};
//				result = tmp[approx-1];}
//			break;
//		case 31:
//			{int tmp[3] = {26, 522, 6288};
//				result = tmp[approx-1];}
//			break;
//		case 37:
//			{int tmp[3] = {11, 788, 36382};
//				result = tmp[approx-1];}
//			break;
//		case 43:
//			{int tmp[3] = {27, 424, 44800};
//				result = tmp[approx-1];}
//			break;
//		case 61:
//			{int tmp[3] = {14, 1661, 76081};
//				result = tmp[approx-1];}
//			break;
//		case 91:
//			{int tmp[3] = {17, 3020, 69268};
//				result = tmp[approx-1];}
//			break;
//		default:
//			std::cerr << "Unknown Holladay delta (det = " << ci << ")" << std::endl;
//			std::exit(EXIT_FAILURE);
//	}
//	return result;
//}

template <class T>
std::ostream& operator<<(std::ostream &os, const Matrix2x2<T>& m)
{
	os << "(" << m.a() << ", " << m.b() << " | " << m.c() << ", " << m.d() << ")";
	return os;
}

template <class T>
std::istream& operator>>(std::istream &is, Matrix2x2<T>& m)
{
	T a, b, c, d;
	is >> a >> b >> c >> d;
	m = Matrix2x2<T>(a, b, c, d);
	return is;
}
