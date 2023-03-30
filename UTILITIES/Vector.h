#pragma once
#include<cassert>
#include<cmath>
#include <corecrt_math_defines.h> // For PI cosnstant

#include <cmath>

#define DECL_ALIGN(x) __declspec(align(x))
#define ALIGN16 DECL_ALIGN(16)
#define ALIGN16_POST DECL_ALIGN(16)
#define CHECK_VALID( _v)	0
struct Matrix3x4
{
	float matrixArray[12]; // Changed to matrix array as using a matrix3x4 in reclass was giving wrong values
};
struct Matrix4x4
{
	float matrix4x4[16]{};
};

class Vector2
{
public:
	float m_x {};
	float m_y {};

	Vector2() = default;
	Vector2( float x, float y )
		:m_x { x }, m_y { y }
	{
	}
	Vector2& operator=( const Vector2& vec )
	{
		m_x = vec.m_x;
		m_y = vec.m_y;
		return *this;
	}

	Vector2& operator+=( const Vector2& vec )
	{
		m_x += vec.m_x;
		m_y += vec.m_y;
		return *this;
	}
	Vector2& operator-=( const Vector2& vec )
	{
		m_x -= vec.m_x;
		m_y -= vec.m_y;
		return *this;
	}
	Vector2& operator*=( const Vector2& vec )
	{
		m_x *= vec.m_x;
		m_y *= vec.m_y;
		return *this;
	}
	Vector2& operator/=( const Vector2& vec )
	{
		m_x /= vec.m_x;
		m_y /= vec.m_y;
		return *this;
	}
	Vector2& operator*=( const float f )
	{
		m_x *= f;
		m_y *= f;
		return *this;
	}
	Vector2& operator/=( const float f )
	{
		m_x /= f;
		m_y /= f;

		return *this;
	}

	friend Vector2 operator+( const Vector2& vec1, const Vector2& vec2 )
	{
		return { vec1.m_x + vec2.m_x , vec1.m_y + vec2.m_y };
	}
	friend Vector2 operator-( const Vector2& vec1, const Vector2& vec2 )
	{
		return{ vec1.m_x - vec2.m_x,vec1.m_y - vec2.m_y };
	}

	float Length() const
	{
		return sqrt( (m_x * m_x) + (m_y * m_y) );
	}

	Vector2& Normalize()
	{
		return { (*this) *= (1 / Length()) };
	}
	float DistanceTo( const Vector2& vec )
	{
		return(*this - vec).Length();
	}
	float Dot( const Vector2& vec )
	{
		return { (m_x * vec.m_x) + (m_y * vec.m_y) };
	}

	friend Vector2 RotatePoint( Vector2 pointToRotate, Vector2 centrePoint, float angle, bool angleInRadians = false )
	{
		if (!angleInRadians)
			angle = (float) (angle * M_PI / 180.0f);
		float cosTheta = (float) cos( angle );
		float sineTheta = (float) sin( angle );
		Vector2 returnVec = {
			cosTheta * (pointToRotate.m_x - centrePoint.m_x) - sineTheta * (pointToRotate.m_y - centrePoint.m_y),
			sineTheta * (pointToRotate.m_x - centrePoint.m_x) + cosTheta * (pointToRotate.m_y - centrePoint.m_y) };
		returnVec += centrePoint;
		return returnVec;
	}

};

class Vector3
{
	
public:
	float m_x {};
	float m_y {};
	float m_z {};
	
	Vector3( float x, float y, float z )
		:m_x { x }, m_y { y }, m_z { z }
	{
	}
	Vector3( void )
	{
		Invalidate();
	}

	Vector3( const float* clr )
	{
		m_x = clr[0];
		m_y = clr[1];
		m_z = clr[2];
	}

	void Init( float ix = 0.0f, float iy = 0.0f, float iz = 0.0f )
	{
		m_x = ix; m_y = iy; m_z = iz;
	}
	bool IsValid() const
	{
		return std::isfinite( m_x ) && std::isfinite( m_y ) && std::isfinite( m_z );
	}
	void Invalidate()
	{
		m_x = m_y = m_z = std::numeric_limits<float>::infinity();
	}


	void Zero()
	{
		m_x = m_y = m_z = 0.0f;
	}

	bool operator==( const Vector3& src ) const
	{
		return (src.m_x == m_x) && (src.m_y == m_y) && (src.m_z == m_z);
	}
	bool operator!=( const Vector3& src ) const
	{
		return (src.m_x != m_x) || (src.m_y != m_y) || (src.m_z !=m_z);
	}

	Vector3& operator=( const Vector3& vec )
	{
		m_x = vec.m_x;
		m_y = vec.m_y;
		m_z = vec.m_z;
		return *this;
	}

	Vector3& operator+=( const Vector3& vec )
	{
		m_x += vec.m_x;
		m_y += vec.m_y;
		m_z +=vec.m_z;
		return *this;
	}
	Vector3& operator-=( const Vector3& vec )
	{
		m_x -= vec.m_x;
		m_y -= vec.m_y;
		m_z -= vec.m_z;
		return *this;
	}
	Vector3& operator*=( const Vector3& vec )
	{
		m_x *= vec.m_x;
		m_y *= vec.m_y;
		m_z *= vec.m_z;
		return *this;
	}
	Vector3& operator/=( const Vector3& vec )
	{
		m_x /= vec.m_x;
		m_y /= vec.m_y;
		m_z /= vec.m_z;
		return *this;
	}
	Vector3& operator*=( const float f )
	{
		m_x *= f;
		m_y *= f;
		m_z *= f;
		return *this;
	}
	Vector3& operator/=( const float f )
	{
		m_x /= f;
		m_y /= f;
		m_z /= f;
		return *this;
	}
	float& operator[]( int i )
	{
		assert( (i >= 0) && (i < 3) );
		return ((float*) this)[i];
	}

	float operator[]( int i ) const
	{
		assert( (i >= 0) && (i < 3) );
		return ((float*) this)[i];
	}
	friend Vector3 operator+( const Vector3& vec1, const Vector3& vec2 )
	{
		return{ vec1.m_x + vec2.m_x,vec1.m_y + vec2.m_y,vec1.m_z + vec2.m_z };
	}
	friend Vector3 operator-( const Vector3& vec1, const Vector3& vec2 )
	{
		return{ vec1.m_x - vec2.m_x,vec1.m_y - vec2.m_y,vec1.m_z - vec2.m_z };
	}
	friend void VectorAdd( const Vector3& a, const Vector3& b, Vector3& c )
	{
		
		c.m_x = a.m_x + b.m_x;
		c.m_y = a.m_y + b.m_y;
		c.m_z = a.m_z + b.m_z;
	}
	friend void VectorSubtract( const Vector3& a, const Vector3& b, Vector3& c )
	{
	
		c.m_x = a.m_x - b.m_x;
		c.m_y = a.m_y - b.m_y;
		c.m_z = a.m_z - b.m_z;
	}
	friend void VectorCopy( const Vector3& src, Vector3& dst )
	{

		dst.m_x = src.m_x;
		dst.m_y = src.m_y;
		dst.m_z = src.m_z;
	}
	friend void VectorClear( Vector3& a )
	{
		a.m_x = a.m_y = a.m_z = 0.0f;
	}

	float Length() const
	{
		return sqrt( (m_x*m_x) + (m_y*m_y) + (m_z*m_z) );
	}
	float LengthSqr() const
	{
		return ( (m_x * m_x) + (m_y * m_y) + (m_z * m_z) );
	}
	Vector3& Normalize()
	{
		return { (*this) *= (1 / Length()) };
	}
	float DistanceTo( const Vector3& vec )
	{
		return(*this - vec).Length();
	}
	float Dot( const Vector3& vec )
	{
		return { (m_x * vec.m_x) + (m_y * vec.m_y) + (m_z * vec.m_z) };
	}
	Vector3 Cross( const Vector3& vec ) const
	{
		return { ((m_y * vec.m_z) - (m_z * vec.m_y)), ((m_z * vec.m_x) - (m_x * vec.m_z)), ((m_x * vec.m_y) - (m_y * vec.m_x)) };
	}
};

class Vector4 :public Vector3
{
public:
	float m_x{};
	float m_y{};
	float m_z{};
	float m_w{};

};

class ALIGN16 VectorAligned : public Vector3
{
public:
	inline VectorAligned( void )
	{
	};

	inline VectorAligned(Vector3 X, Vector3 Y, Vector3 Z )
	{
		//Init(X, Y, Z);
	}

	explicit VectorAligned( const Vector3& vOther )
	{
		Init( vOther.m_x, vOther.m_y, vOther.m_z );
	}

	VectorAligned& operator=( const Vector3& vOther )
	{
		Init( vOther.m_x, vOther.m_y, vOther.m_z );
		return *this;
	}

	float w;	// this space is used anyway
} ALIGN16_POST;