#include <core/matrix.h>
#include <core/float4.h>
#include <core/point.h>
#include <core/vector.h>

using namespace rt;

//ctor
Matrix::Matrix(const Float4& r1, const Float4& r2, const Float4& r3, const Float4& r4)
{
	rows_[0] = r1;
	rows_[1] = r2;
	rows_[2] = r3;
	rows_[3] = r4;
}

//index operator
Float4& Matrix::operator[](int index)
{
	return rows_[index];
}

//index operator
Float4 Matrix::operator[](int index) const
{
	return rows_[index];
}

//operator Matrix+Matrix
Matrix Matrix::operator+(const Matrix& b) const
{
	return Matrix(rows_[0] + b[0], rows_[1] + b[1], rows_[2] + b[2], rows_[3] + b[3]);
}

//operator Matrix-Matrix
Matrix Matrix::operator-(const Matrix& b) const
{
	return Matrix(rows_[0] - b[0], rows_[1] - b[1], rows_[2] - b[2], rows_[3] - b[3]);
}

//transpose
Matrix Matrix::transpose() const
{
	Matrix m = *this;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = i+1; j < 4; ++j)
		{
			float temp = m[i][j];
			m[i][j] = m[j][i];
			m[j][i] = temp;
		}
	}

	return m;
}

//invert
Matrix Matrix::invert() const
{
	Matrix result;
	const Matrix& m = *this;

	result[0][0] = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
	result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
	result[2][0] = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
	result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

	float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];

	if (det == 0)
	{
		return Matrix::zero();
	}

	result[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
	result[1][1] = m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
	result[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
	result[3][1] = m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
	result[0][2] = m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
	result[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
	result[2][2] = m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
	result[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
	result[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
	result[1][3] = m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
	result[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
	result[3][3] = m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

	result = result*(1.0f / det);

	return result;
}

//operator ==
bool Matrix::operator==(const Matrix& b) const
{
	return rows_[0] == b[0] && rows_[1] == b[1] && rows_[2] == b[2] && rows_[3] == b[3];
}

//operator !=
bool Matrix::operator!=(const Matrix& b) const
{
	return !(*this == b);
}

//operator Matrix*Float4
Float4 Matrix::operator*(const Float4& b) const
{
	return Float4(dot(rows_[0], b), dot(rows_[1], b), dot(rows_[2], b), dot(rows_[3], b));
}

//operator Matrix*Vector
Vector Matrix::operator*(const Vector& b) const
{
	return Vector((*this)*Float4(b));
}

//operator Matrix*Point
Point Matrix::operator*(const Point& b) const
{
	return Point((*this)*Float4(b));
}

//det
float Matrix::det() const
{
	Matrix result;
	const Matrix& m = *this;

	result[0][0] = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
	result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
	result[2][0] = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
	result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];
	
	float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];

	return det;
}

//zero
Matrix Matrix::zero()
{
	Matrix m;

	for (int i = 0; i < 4; ++i)
	{
		m[i] = Float4::rep(0.0f);
	}

	return m;
}

//identity
Matrix Matrix::identity()
{
	Matrix m = Matrix::zero();

	m[0][0] = 1.0f;
	m[1][1] = 1.0f;
	m[2][2] = 1.0f;
	m[3][3] = 1.0f;

	return m;
}

//system
Matrix Matrix::system(const Vector& e1, const Vector& e2, const Vector& e3)
{
	return Matrix(Float4(e1), Float4(e2), Float4(e3), Float4(0.0f, 0.0f, 0.0f, 1.0f)).transpose();
}

//product
Matrix rt::product(const Matrix& a, const Matrix& b)
{
	Matrix result;
	Matrix temp = b.transpose();

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result[i][j] = dot(a[i], temp[j]);
		}
	}

	return result;
}

//operator Matrix*scalar
Matrix rt::operator*(const Matrix& a, float scalar)
{
	return Matrix(a[0] * scalar, a[1] * scalar, a[2] * scalar, a[3] * scalar);
}

//operator scalar*Matrix
Matrix rt::operator*(float scalar, const Matrix& a)
{
	return Matrix(scalar * a[0], scalar * a[1], scalar * a[2], scalar * a[3]);
}