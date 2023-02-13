#pragma once
#include <cmath>
#include <stdio.h>                  // Required for: sprintf()
#include <time.h>                   // Required for: time() [Used in InitTimer()]
#include <math.h>

#define MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)
#define MATH_RANDOM_MINUS1_1()      ((2.0f*((float)rand()/RAND_MAX))-1.0f)      // Returns a random float between -1 and 1.
#define MATH_RANDOM_0_1()           ((float)rand()/RAND_MAX)                    // Returns a random float between 0 and 1.
#define MATH_FLOAT_SMALL            1.0e-37f
#define MATH_TOLERANCE              2e-37f
#define MATH_E                      2.71828182845904523536f
#define MATH_LOG10E                 0.4342944819032518f
#define MATH_LOG2E                  1.442695040888963387f
#define MATH_PI                     3.14159265358979323846f
#define MATH_PIOVER2                1.57079632679489661923f
#define MATH_PIOVER4                0.785398163397448309616f
#define MATH_PIX2                   6.28318530717958647693f
#define MATH_EPSILON                0.000001f
#define MATH_CLAMP(x, lo, hi)       ((x < lo) ? lo : ((x > hi) ? hi : x))
#define M_1_PI                      0.31830988618379067154
#define SMALL_FLOAT 0.000001f
#define PI 3.14159265358979323846f
#define EPSILON 0.000001f
#define DEG2RAD (PI/180.0f)
#define RAD2DEG (180.0f/PI)
#define DEG_CIRCLE 360
#define DEG_TO_RAD (M_PI / (DEG_CIRCLE / 2))
#define RAD_TO_DEG ((DEG_CIRCLE / 2) / M_PI)

const unsigned int MaxUInt32 = 0xFFFFFFFF;
const int MinInt32 = 0x80000000;
const int MaxInt32 = 0x7FFFFFFF;
const float MaxFloat = 3.402823466e+38F;
const float MinPosFloat = 1.175494351e-38F;

const float Pi = 3.141592654f;
const float TwoPi = 6.283185307f;
const float PiHalf = 1.570796327f;

const float Epsilon = 0.000001f;
const float ZeroEpsilon = 32.0f * MinPosFloat;  // Very small epsilon for checking against 0.0f

enum NoInitHint
{
    NO_INIT
};

inline double	module(double a)
{
	if (a < 0)
		return (-1 * a);
	return (a);
}


inline double	radians(double degrees)
{
	return (degrees / 180 * PI);
}


inline bool	equal(double a, double b)
{
	return (module(a - b) < EPSILON);
}
inline float degToRad( float f )
{
	return f * 0.017453293f;
}

inline float radToDeg( float f )
{
	return f * 57.29577951f;
}

inline float clamp( float f, float min, float max )
{
	if( f < min ) f = min;
	else if( f > max ) f = max;

	return f;
}

inline float minf( float a, float b )
{
	return a < b ? a : b;
}

inline float maxf( float a, float b )
{
	return a > b ? a : b;
}

inline float fsel( float test, float a, float b )
{
	// Branchless selection
	return test >= 0 ? a : b;
}

// Computes a/b, rounded up
// To be used for positive a and b and small numbers (beware of overflows)
inline int idivceil( int a, int b )
{
	return (a + b - 1) / b;
}


// -------------------------------------------------------------------------------------------------
// Conversion
// -------------------------------------------------------------------------------------------------
inline float reciprocal ( const float f )
{
    return 1.0 / f;
}

inline int ftoi_t( double val )
{
	// Float to int conversion using truncation

	return (int)val;
}

inline int ftoi_r( double val )
{
	// Fast round (banker's round) using Sree Kotay's method
	// This function is much faster than a naive cast from float to int

	union
	{
		double dval;
		int ival[2];
	} u;

	u.dval = val + 6755399441055744.0;  // Magic number: 2^52 * 1.5;
	return u.ival[0];         // Needs to be [1] for big-endian
}



// -------------------------------------------------------------------------------------------------
// Vector
// -------------------------------------------------------------------------------------------------

class Vec2
{
public:
	float x, y;
	
	
	// ------------
	// Constructors
	// ------------
	Vec2() : x( 0.0f ), y( 0.0f )
	{
	}
	
	explicit Vec2( NoInitHint )
	{
		// Constructor without default initialization
	}
	
	Vec2( const float x, const float y ) : x( x ), y( y )
	{
	}
	 void set(  float x,  float y )
	{
        this->x=x;
        this->y=y;
      
	}
	// ------
	// Access
	// ------
	float operator[]( unsigned int index ) const
	{
		return *(&x + index);
	}

	float &operator[]( unsigned int index )
	{
		return *(&x + index);
	}
	
	// -----------
	// Comparisons
	// -----------
	bool operator==( const Vec2 &v ) const
	{
		return (x > v.x - Epsilon && x < v.x + Epsilon &&
				y > v.y - Epsilon && y < v.y + Epsilon);
	}
	
	bool operator!=( const Vec2 &v ) const
	{
		return (x < v.x - Epsilon || x > v.x + Epsilon ||
				y < v.y - Epsilon || y > v.y + Epsilon);
	}
	
	// ---------------------
	// Arithmetic operations
	// ---------------------
	Vec2 operator-() const
	{
		return Vec2( -x, -y );
	}
	
	Vec2 operator+( const Vec2 &v ) const
	{
		return Vec2( x + v.x, y + v.y );
	}
	
	Vec2 &operator+=( const Vec2 &v )
	{
		return *this = *this + v;
	}
	
	Vec2 operator-( const Vec2 &v ) const
	{
		return Vec2( x - v.x, y - v.y );
	}
	
	Vec2 &operator-=( const Vec2 &v )
	{
		return *this = *this - v;
	}
	
	Vec2 operator*( const float f ) const
	{
		return Vec2( x * f, y * f );
	}
	
	Vec2 &operator*=( const float f )
	{
		return *this = *this * f;
	}
	
	Vec2 operator/( const float f ) const
	{
		return Vec2( x / f, y / f );
	}
	
	Vec2 &operator/=( const float f )
	{
		return *this = *this / f;
	}
	
	// ----------------
	// Special products
	// ----------------
	float dot( const Vec2 &v ) const
	{
		return x * v.x + y * v.y;
	}
	
	// ----------------
	// Other operations
	// ----------------
	float length() const
	{
		return sqrtf( x * x + y * y );
	}
	
	Vec2 normalized() const
	{
		float invLen = 1.0f / length();
		return Vec2( x * invLen, y * invLen );
	}
	
	void normalize()
	{
		float invLen = 1.0f / length();
		x *= invLen;
		y *= invLen;
	}
	
	Vec2 lerp( const Vec2 &v, float f ) const
	{
		return Vec2( x + (v.x - x) * f, y + (v.y - y) * f );
	}
};


class Vec3
{
public:
	float x, y, z;
	
	
	// ------------
	// Constructors
	// ------------
	Vec3() : x( 0.0f ), y( 0.0f ), z( 0.0f )
	{ 
	}
		
	Vec3(  float x,  float y,  float z ) : x( x ), y( y ), z( z ) 
	{
	}

    void set(  float x,  float y,  float z )
	{
        this->x=x;
        this->y=y;
        this->z=z;
	}

    static Vec3 Normalize(const Vec3 &v) 
    {
    float magnitude = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return Vec3(v.x / magnitude, v.y / magnitude, v.z / magnitude);
    }
    static Vec3 Cross(const Vec3 &a, const Vec3 &b) 
    {
        return Vec3(a.y * b.z - a.z * b.y,
                        a.z * b.x - a.x * b.z,
                        a.x * b.y - a.y * b.x);
    }

    static float Dot(const Vec3 &a, const Vec3 &b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
        
    // ------
	// Access
	// ------
	float operator[]( unsigned int index ) const
	{
		return *(&x + index);
	}

	float &operator[]( unsigned int index )
	{
		return *(&x + index);
	}
	
	// -----------
	// Comparisons
	// -----------
	bool operator==( const Vec3 &v ) const
	{
		return (x > v.x - Epsilon && x < v.x + Epsilon && 
		        y > v.y - Epsilon && y < v.y + Epsilon &&
		        z > v.z - Epsilon && z < v.z + Epsilon);
	}

	bool operator!=( const Vec3 &v ) const
	{
		return (x < v.x - Epsilon || x > v.x + Epsilon || 
		        y < v.y - Epsilon || y > v.y + Epsilon ||
		        z < v.z - Epsilon || z > v.z + Epsilon);
	}
	
	// ---------------------
	// Arithmetic operations
	// ---------------------
	Vec3 operator-() const
	{
		return Vec3( -x, -y, -z );
	}

	Vec3 operator+( const Vec3 &v ) const
	{
		return Vec3( x + v.x, y + v.y, z + v.z );
	}

	Vec3 &operator+=( const Vec3 &v )
	{
		return *this = *this + v;
	}

	Vec3 operator-( const Vec3 &v ) const 
	{
		return Vec3( x - v.x, y - v.y, z - v.z );
	}

	Vec3 &operator-=( const Vec3 &v )
	{
		return *this = *this - v;
	}

	Vec3 operator*( const float f ) const
	{
		return Vec3( x * f, y * f, z * f );
	}

	Vec3 &operator*=( const float f )
	{
		return *this = *this * f;
	}

	Vec3 operator/( const float f ) const
	{
		return Vec3( x / f, y / f, z / f );
	}

	Vec3 &operator/=( const float f )
	{
		return *this = *this / f;
	}

	// ----------------
	// Special products
	// ----------------
	float dot( const Vec3 &v ) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3 cross( const Vec3 &v ) const
	{
		return Vec3( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
	}

	// ----------------
	// Other operations
	// ----------------
	float length() const 
	{
		return sqrtf( x * x + y * y + z * z );
	}

	float length_squared() const
	{
		return x * x + y * y + z * z;
	}

	Vec3 normalized() const
	{
		float invLen = 1.0f / length();
		return Vec3( x * invLen, y * invLen, z * invLen );
	}

	void normalize()
	{
		float invLen = 1.0f / length();
		x *= invLen;
		y *= invLen;
		z *= invLen;
	}

	/*void fromRotation( float angleX, float angleY )
	{
		x = cosf( angleX ) * sinf( angleY ); 
		y = -sinf( angleX );
		z = cosf( angleX ) * cosf( angleY );
	}*/

	Vec3 toRotation() const
	{
		// Assumes that the unrotated view vector is (0, 0, -1)
		Vec3 v;
		
		if( y != 0 ) v.x = atan2f( y, sqrtf( x*x + z*z ) );
		if( x != 0 || z != 0 ) v.y = atan2f( -x, -z );

		return v;
	}

	Vec3 lerp( const Vec3 &v, float f ) const
	{
		return Vec3( x + (v.x - x) * f, y + (v.y - y) * f, z + (v.z - z) * f ); 
	}
};


class Vec4
{
public:
	
	float x, y, z, w;


	Vec4() : x( 0 ), y( 0 ), z( 0 ), w( 0 )
	{
	}
	
	explicit Vec4( const float x, const float y, const float z, const float w ) :
		x( x ), y( y ), z( z ), w( w )
	{
	}

	explicit Vec4( Vec3 v ) : x( v.x ), y( v.y ), z( v.z ), w( 1.0f )
	{
	}

	// ------
	// Access
	// ------
	float operator[]( unsigned int index ) const
	{
		return *(&x + index);
	}

	float &operator[]( unsigned int index )
	{
		return *(&x + index);
	}

	// ---------------------
	// Arithmetic operations
	// ---------------------
	Vec4 operator+( const Vec4 &v ) const
	{
		return Vec4( x + v.x, y + v.y, z + v.z, w + v.w );
	}

	Vec4 operator-() const
	{
		return Vec4( -x, -y, -z, -w );
	}
	
	Vec4 operator*( const float f ) const
	{
		return Vec4( x * f, y * f, z * f, w * f );
	}
};


// -------------------------------------------------------------------------------------------------
// Quat
// -------------------------------------------------------------------------------------------------

class Quat
{
public:	
	
	float x, y, z, w;

	// ------------
	// Constructors
	// ------------
	Quat() : x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f ) 
	{ 
	}
	
	explicit Quat( const float x, const float y, const float z, const float w ) :
		x( x ), y( y ), z( z ), w( w )
	{
	}
	
	Quat( const float eulerX, const float eulerY, const float eulerZ )
	{
		Quat roll( sinf( eulerX * 0.5f ), 0, 0, cosf( eulerX * 0.5f ) );
		Quat pitch( 0, sinf( eulerY * 0.5f ), 0, cosf( eulerY * 0.5f ) );
		Quat yaw( 0, 0, sinf( eulerZ * 0.5f ), cosf( eulerZ * 0.5f ) );
	
		// Order: y * x * z
		*this = pitch * roll * yaw;
	}

	// ---------------------
	// Arithmetic operations
	// ---------------------
	Quat operator*( const Quat &q ) const
	{
		return Quat(
			y * q.z - z * q.y + q.x * w + x * q.w,
			z * q.x - x * q.z + q.y * w + y * q.w,
			x * q.y - y * q.x + q.z * w + z * q.w,
			w * q.w - (x * q.x + y * q.y + z * q.z) );
	}

	Quat &operator*=( const Quat &q )
	{
		return *this = *this * q;
	}

	// ----------------
	// Other operations
	// ----------------

	Quat slerp( const Quat &q, const float t ) const
	{
		// Spherical linear interpolation between two Quats
		// Note: SLERP is not commutative
		
		Quat q1( q );

		// Calculate cosine
		float cosTheta = x * q.x + y * q.y + z * q.z + w * q.w;

		// Use the shortest path
		if( cosTheta < 0 )
		{
			cosTheta = -cosTheta; 
			q1.x = -q.x; q1.y = -q.y;
			q1.z = -q.z; q1.w = -q.w;
		}

		// Initialize with linear interpolation
		float scale0 = 1 - t, scale1 = t;
		
		// Use spherical interpolation only if the Quats are not very close
		if( (1 - cosTheta) > 0.001f )
		{
			// SLERP
			float theta = acosf( cosTheta );
			float sinTheta = sinf( theta );
			scale0 = sinf( (1 - t) * theta ) / sinTheta;
			scale1 = sinf( t * theta ) / sinTheta;
		} 
		
		// Calculate final Quat
		return Quat( x * scale0 + q1.x * scale1, y * scale0 + q1.y * scale1,
		                   z * scale0 + q1.z * scale1, w * scale0 + q1.w * scale1 );
	}

	Quat nlerp( const Quat &q, const float t ) const
	{
		// Normalized linear Quat interpolation
		// Note: NLERP is faster than SLERP and commutative but does not yield constant velocity

		Quat qt;
		float cosTheta = x * q.x + y * q.y + z * q.z + w * q.w;
		
		// Use the shortest path and interpolate linearly
		if( cosTheta < 0 )
			qt = Quat( x + (-q.x - x) * t, y + (-q.y - y) * t,
							 z + (-q.z - z) * t, w + (-q.w - w) * t );
		else
			qt = Quat( x + (q.x - x) * t, y + (q.y - y) * t,
							 z + (q.z - z) * t, w + (q.w - w) * t );

		// Return normalized Quat
		float invLen = 1.0f / sqrtf( qt.x * qt.x + qt.y * qt.y + qt.z * qt.z + qt.w * qt.w );
		return Quat( qt.x * invLen, qt.y * invLen, qt.z * invLen, qt.w * invLen );
	}

	Quat inverted() const
	{
		float len = x * x + y * y + z * z + w * w;
		if( len > 0 )
		{
			float invLen = 1.0f / len;
			return Quat( -x * invLen, -y * invLen, -z * invLen, w * invLen );
		}
		else return Quat();
	}
};


// -------------------------------------------------------------------------------------------------
// Matrix
// -------------------------------------------------------------------------------------------------

class Mat4
{
public:
	
	union
	{
		float c[4][4];	// Column major order for OpenGL: c[column][row]
		float x[16];
	};
	
   void identity() 
  {
 
    for (int i = 0; i < 4; i++) 
    {
      for (int j = 0; j < 4; j++) 
      {
        c[i][j] = (i == j) ? 1.0f : 0.0f;
      }
    }
    
  }
	// --------------
	// Static methods
	// --------------
    static Mat4 Identity() 
  {
    Mat4 matrix;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        matrix.c[i][j] = (i == j) ? 1.0f : 0.0f;
      }
    }
    return matrix;
  }

	static Mat4 Translate( float x, float y, float z )
	{
		Mat4 m;

		m.c[3][0] = x;
		m.c[3][1] = y;
		m.c[3][2] = z;

		return m;
	}

	static Mat4 Scale( float x, float y, float z )
	{
		Mat4 m;
		
		m.c[0][0] = x;
		m.c[1][1] = y;
		m.c[2][2] = z;

		return m;
	}

	static Mat4 Rotate( float x, float y, float z )
	{
		// Rotation order: YXZ [* Vector]
		return Mat4( Quat( x, y, z ) );
	}

	static Mat4 Rotate( Vec3 axis, float angle )
	{
		axis = axis * sinf( angle * 0.5f );
		return Mat4( Quat( axis.x, axis.y, axis.z, cosf( angle * 0.5f ) ) );
	}


	static Mat4 Perspective( float l, float r, float b, float t, float n, float f )
	{
		Mat4 m;

		m.x[0] = 2 * n / (r - l);
		m.x[5] = 2 * n / (t - b);
		m.x[8] = (r + l) / (r - l);
		m.x[9] = (t + b) / (t - b);
		m.x[10] = -(f + n) / (f - n);
		m.x[11] = -1;
		m.x[14] = -2 * f * n / (f - n);
		m.x[15] = 0;

		return m;
	}

    static Mat4 Perspective( float fov, float aspect, float nearPlane, float farPlane)
	{
		Mat4 m;

		float ymax = nearPlane * tanf( degToRad( fov / 2 ) );
	    float xmax = ymax * aspect;

        float frustLeft = -xmax;
        float frustRight = xmax;
        float frustBottom = -ymax;
        float frustTop = ymax;
        
		return Mat4::Perspective(frustLeft, frustRight, frustBottom, frustTop, nearPlane, farPlane);
	}


    // Builds a right-handed perspective projection matrix based on a field of view
	static Mat4 ProjectionMatrix(float fieldOfViewRadians, float aspectRatio, float zNear, float zFar)
	{
		const float h = reciprocal(tan(fieldOfViewRadians*0.5));
		const float w = (h / aspectRatio);

	    Mat4 M;
		M.x[0] = w;
		M.x[1] = 0;
		M.x[2] = 0;
		M.x[3] = 0;

		M.x[4] = 0;
		M.x[5] = h;
		M.x[6] = 0;
		M.x[7] = 0;

		M.x[8] = 0;
		M.x[9] = 0;
		//M[10]
		M.x[11] = -1;

		M.x[12] = 0;
		M.x[13] = 0;
		//M[14]
		M.x[15] = 0;

        M.x[10] = (float)((zFar+zNear)/(zNear-zFar));
        M.x[14] = (float)(2.0f*zNear*zFar/(zNear-zFar));
        

		return M;
	}


	// Builds a left-handed perspective projection matrix based on a field of view
	static Mat4 ProjectionMatrixLH(float fieldOfViewRadians, float aspectRatio, float zNear, float zFar)
	{
    	const float h = reciprocal(tan(fieldOfViewRadians*0.5));
		const float w = (h / aspectRatio);
        Mat4 M;

		M.x[0] = w;
		M.x[1] = 0;
		M.x[2] = 0;
		M.x[3] = 0;

		M.x[4] = 0;
		M.x[5] = (float)h;
		M.x[6] = 0;
		M.x[7] = 0;

		M.x[8] = 0;
		M.x[9] = 0;
		//M[10]
		M.x[11] = 1;

		M.x[12] = 0;
		M.x[13] = 0;
		//M[14]
		M.x[15] = 0;


        M.x[10] = (float)((zFar+zNear)/(zFar-zNear));
        M.x[14] = (float)(2.0f*zNear*zFar/(zNear-zFar));
    


		return M;
	}


        // Builds a left-handed look-at matrix.
        static Mat4 LookAtLH( const Vec3 &position, const Vec3 &target, const Vec3 &upVector)
        {
                Vec3 zaxis = target - position;
                zaxis.normalize();

                Vec3 xaxis = upVector.cross(zaxis);
                xaxis.normalize();

                Vec3 yaxis = zaxis.cross(xaxis);

                Mat4 M;
                M.x[0] = xaxis.x;
                M.x[1] = yaxis.x;
                M.x[2] = zaxis.x;
                M.x[3] = 0;

                M.x[4] = xaxis.y;
                M.x[5] = yaxis.y;
                M.x[6] = zaxis.y;
                M.x[7] = 0;

                M.x[8] = xaxis.z;
                M.x[9] = yaxis.z;
                M.x[10] =zaxis.z;
                M.x[11] = 0;

                M.x[12] = -xaxis.dot(position);
                M.x[13] = -yaxis.dot(position);
                M.x[14] = -zaxis.dot(position);
                M.x[15] = 1;
                return M;
    }

    //Builds a right-handed look-at matrix.
    static Mat4 LookAt( const Vec3 &position, const Vec3 &target, const Vec3 &upVector)
    {
            Vec3 zaxis = position - target;
            zaxis.normalize();

            Vec3 xaxis = upVector.cross(zaxis);
            xaxis.normalize();

            Vec3 yaxis = zaxis.cross(xaxis);

            Mat4 M = Mat4::Identity();
            M.x[0] = xaxis.x;
            M.x[1] = yaxis.x;
            M.x[2] = zaxis.x;
            M.x[3] = 0;

            M.x[4] = xaxis.y;
            M.x[5] = yaxis.y;
            M.x[6] = zaxis.y;
            M.x[7] = 0;

            M.x[8] = xaxis.z;
            M.x[9] = yaxis.z;
            M.x[10] =zaxis.z;
            M.x[11] = 0;

            M.x[12] = -xaxis.dot(position);
            M.x[13] = -yaxis.dot(position);
            M.x[14] = -zaxis.dot(position);
            M.x[15] = 1;
            return M;
}


	static Mat4 OrthoMat( float l, float r, float b, float t, float n, float f )
	{
		Mat4 m;

		m.x[0] = 2 / (r - l);
		m.x[5] = 2 / (t - b);
		m.x[10] = -2 / (f - n);
		m.x[12] = -(r + l) / (r - l);
		m.x[13] = -(t + b) / (t - b);
		m.x[14] = -(f + n) / (f - n);

		return m;
	}

	static void fastMult43( Mat4 &dst, const Mat4 &m1, const Mat4 &m2 )
	{
		// Note: dst may not be the same as m1 or m2

		float *dstx = dst.x;
		const float *m1x = m1.x;
		const float *m2x = m2.x;
		
		dstx[0] = m1x[0] * m2x[0] + m1x[4] * m2x[1] + m1x[8] * m2x[2];
		dstx[1] = m1x[1] * m2x[0] + m1x[5] * m2x[1] + m1x[9] * m2x[2];
		dstx[2] = m1x[2] * m2x[0] + m1x[6] * m2x[1] + m1x[10] * m2x[2];
		dstx[3] = 0.0f;

		dstx[4] = m1x[0] * m2x[4] + m1x[4] * m2x[5] + m1x[8] * m2x[6];
		dstx[5] = m1x[1] * m2x[4] + m1x[5] * m2x[5] + m1x[9] * m2x[6];
		dstx[6] = m1x[2] * m2x[4] + m1x[6] * m2x[5] + m1x[10] * m2x[6];
		dstx[7] = 0.0f;

		dstx[8] = m1x[0] * m2x[8] + m1x[4] * m2x[9] + m1x[8] * m2x[10];
		dstx[9] = m1x[1] * m2x[8] + m1x[5] * m2x[9] + m1x[9] * m2x[10];
		dstx[10] = m1x[2] * m2x[8] + m1x[6] * m2x[9] + m1x[10] * m2x[10];
		dstx[11] = 0.0f;

		dstx[12] = m1x[0] * m2x[12] + m1x[4] * m2x[13] + m1x[8] * m2x[14] + m1x[12] * m2x[15];
		dstx[13] = m1x[1] * m2x[12] + m1x[5] * m2x[13] + m1x[9] * m2x[14] + m1x[13] * m2x[15];
		dstx[14] = m1x[2] * m2x[12] + m1x[6] * m2x[13] + m1x[10] * m2x[14] + m1x[14] * m2x[15];
		dstx[15] = 1.0f;
	}

	// ------------
	// Constructors
	// ------------
	Mat4()
	{
		c[0][0] = 1; c[1][0] = 0; c[2][0] = 0; c[3][0] = 0;
		c[0][1] = 0; c[1][1] = 1; c[2][1] = 0; c[3][1] = 0;
		c[0][2] = 0; c[1][2] = 0; c[2][2] = 1; c[3][2] = 0;
		c[0][3] = 0; c[1][3] = 0; c[2][3] = 0; c[3][3] = 1;
	}

	explicit Mat4( NoInitHint )
	{
		// Constructor without default initialization
	}

	Mat4( const float *floatArray16 )
	{
		for( unsigned int i = 0; i < 4; ++i )
		{
			for( unsigned int j = 0; j < 4; ++j )
			{
				c[i][j] = floatArray16[i * 4 + j];
			}
		}
	}

	Mat4( const Quat &q )
	{
		// Calculate coefficients
		float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
		float xx = q.x * x2,  xy = q.x * y2,  xz = q.x * z2;
		float yy = q.y * y2,  yz = q.y * z2,  zz = q.z * z2;
		float wx = q.w * x2,  wy = q.w * y2,  wz = q.w * z2;

		c[0][0] = 1 - (yy + zz);  c[1][0] = xy - wz;	
		c[2][0] = xz + wy;        c[3][0] = 0;
		c[0][1] = xy + wz;        c[1][1] = 1 - (xx + zz);
		c[2][1] = yz - wx;        c[3][1] = 0;
		c[0][2] = xz - wy;        c[1][2] = yz + wx;
		c[2][2] = 1 - (xx + yy);  c[3][2] = 0;
		c[0][3] = 0;              c[1][3] = 0;
		c[2][3] = 0;              c[3][3] = 1;
	}

	// ----------
	// Matrix sum
	// ----------
	Mat4 operator+( const Mat4 &m ) const 
	{
		Mat4 mf( NO_INIT );
		
		mf.x[0] = x[0] + m.x[0];
		mf.x[1] = x[1] + m.x[1];
		mf.x[2] = x[2] + m.x[2];
		mf.x[3] = x[3] + m.x[3];
		mf.x[4] = x[4] + m.x[4];
		mf.x[5] = x[5] + m.x[5];
		mf.x[6] = x[6] + m.x[6];
		mf.x[7] = x[7] + m.x[7];
		mf.x[8] = x[8] + m.x[8];
		mf.x[9] = x[9] + m.x[9];
		mf.x[10] = x[10] + m.x[10];
		mf.x[11] = x[11] + m.x[11];
		mf.x[12] = x[12] + m.x[12];
		mf.x[13] = x[13] + m.x[13];
		mf.x[14] = x[14] + m.x[14];
		mf.x[15] = x[15] + m.x[15];

		return mf;
	}

	Mat4 &operator+=( const Mat4 &m )
	{
		return *this = *this + m;
	}

	// ---------------------
	// Matrix multiplication
	// ---------------------
	Mat4 operator*( const Mat4 &m ) const 
	{
		Mat4 mf( NO_INIT );
		
		mf.x[0] = x[0] * m.x[0] + x[4] * m.x[1] + x[8] * m.x[2] + x[12] * m.x[3];
		mf.x[1] = x[1] * m.x[0] + x[5] * m.x[1] + x[9] * m.x[2] + x[13] * m.x[3];
		mf.x[2] = x[2] * m.x[0] + x[6] * m.x[1] + x[10] * m.x[2] + x[14] * m.x[3];
		mf.x[3] = x[3] * m.x[0] + x[7] * m.x[1] + x[11] * m.x[2] + x[15] * m.x[3];

		mf.x[4] = x[0] * m.x[4] + x[4] * m.x[5] + x[8] * m.x[6] + x[12] * m.x[7];
		mf.x[5] = x[1] * m.x[4] + x[5] * m.x[5] + x[9] * m.x[6] + x[13] * m.x[7];
		mf.x[6] = x[2] * m.x[4] + x[6] * m.x[5] + x[10] * m.x[6] + x[14] * m.x[7];
		mf.x[7] = x[3] * m.x[4] + x[7] * m.x[5] + x[11] * m.x[6] + x[15] * m.x[7];

		mf.x[8] = x[0] * m.x[8] + x[4] * m.x[9] + x[8] * m.x[10] + x[12] * m.x[11];
		mf.x[9] = x[1] * m.x[8] + x[5] * m.x[9] + x[9] * m.x[10] + x[13] * m.x[11];
		mf.x[10] = x[2] * m.x[8] + x[6] * m.x[9] + x[10] * m.x[10] + x[14] * m.x[11];
		mf.x[11] = x[3] * m.x[8] + x[7] * m.x[9] + x[11] * m.x[10] + x[15] * m.x[11];

		mf.x[12] = x[0] * m.x[12] + x[4] * m.x[13] + x[8] * m.x[14] + x[12] * m.x[15];
		mf.x[13] = x[1] * m.x[12] + x[5] * m.x[13] + x[9] * m.x[14] + x[13] * m.x[15];
		mf.x[14] = x[2] * m.x[12] + x[6] * m.x[13] + x[10] * m.x[14] + x[14] * m.x[15];
		mf.x[15] = x[3] * m.x[12] + x[7] * m.x[13] + x[11] * m.x[14] + x[15] * m.x[15];

		return mf;
	}

	Mat4 operator*( const float f ) const
	{
		Mat4 m( *this );
		
		m.x[0]  *= f; m.x[1]  *= f; m.x[2]  *= f; m.x[3]  *= f;
		m.x[4]  *= f; m.x[5]  *= f; m.x[6]  *= f; m.x[7]  *= f;
		m.x[8]  *= f; m.x[9]  *= f; m.x[10] *= f; m.x[11] *= f;
		m.x[12] *= f; m.x[13] *= f; m.x[14] *= f; m.x[15] *= f;

		return m;
	}

	// ----------------------------
	// Vector-Matrix multiplication
	// ----------------------------
	Vec3 operator*( const Vec3 &v ) const
	{
		return Vec3( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + c[3][0],
		              v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + c[3][1],
		              v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + c[3][2] );
	}

	Vec4 operator*( const Vec4 &v ) const
	{
		return Vec4( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + v.w * c[3][0],
		              v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + v.w * c[3][1],
		              v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + v.w * c[3][2],
		              v.x * c[0][3] + v.y * c[1][3] + v.z * c[2][3] + v.w * c[3][3] );
	}

	Vec3 mult33Vec( const Vec3 &v ) const
	{
		return Vec3( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0],
		              v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1],
		              v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] );
	}
	
	// ---------------
	// Transformations
	// ---------------
	void translate( const float tx, const float ty, const float tz )
	{
		*this = Translate( tx, ty, tz ) * *this;
	}

	void scale( const float sx, const float sy, const float sz )
	{
		*this = Scale( sx, sy, sz ) * *this;
	}

	void rotate( const float rx, const float ry, const float rz )
	{
		*this = Rotate( rx, ry, rz ) * *this;
	}

	// ---------------
	// Other
	// ---------------

	Mat4 transposed() const
	{
		Mat4 m( *this );
		
		for( unsigned int maty = 0; maty < 4; ++maty )
		{
			for( unsigned int matx = maty + 1; matx < 4; ++matx ) 
			{
				float tmp = m.c[matx][maty];
				m.c[matx][maty] = m.c[maty][matx];
				m.c[maty][matx] = tmp;
			}
		}

		return m;
	}

	float determinant() const
	{
		return 
			c[0][3]*c[1][2]*c[2][1]*c[3][0] - c[0][2]*c[1][3]*c[2][1]*c[3][0] - c[0][3]*c[1][1]*c[2][2]*c[3][0] + c[0][1]*c[1][3]*c[2][2]*c[3][0] +
			c[0][2]*c[1][1]*c[2][3]*c[3][0] - c[0][1]*c[1][2]*c[2][3]*c[3][0] - c[0][3]*c[1][2]*c[2][0]*c[3][1] + c[0][2]*c[1][3]*c[2][0]*c[3][1] +
			c[0][3]*c[1][0]*c[2][2]*c[3][1] - c[0][0]*c[1][3]*c[2][2]*c[3][1] - c[0][2]*c[1][0]*c[2][3]*c[3][1] + c[0][0]*c[1][2]*c[2][3]*c[3][1] +
			c[0][3]*c[1][1]*c[2][0]*c[3][2] - c[0][1]*c[1][3]*c[2][0]*c[3][2] - c[0][3]*c[1][0]*c[2][1]*c[3][2] + c[0][0]*c[1][3]*c[2][1]*c[3][2] +
			c[0][1]*c[1][0]*c[2][3]*c[3][2] - c[0][0]*c[1][1]*c[2][3]*c[3][2] - c[0][2]*c[1][1]*c[2][0]*c[3][3] + c[0][1]*c[1][2]*c[2][0]*c[3][3] +
			c[0][2]*c[1][0]*c[2][1]*c[3][3] - c[0][0]*c[1][2]*c[2][1]*c[3][3] - c[0][1]*c[1][0]*c[2][2]*c[3][3] + c[0][0]*c[1][1]*c[2][2]*c[3][3];
	}

	Mat4 inverted() const
	{
		Mat4 m( NO_INIT );

		float d = determinant();
		if( d == 0 ) return m;
		d = 1.0f / d;
		
		m.c[0][0] = d * (c[1][2]*c[2][3]*c[3][1] - c[1][3]*c[2][2]*c[3][1] + c[1][3]*c[2][1]*c[3][2] - c[1][1]*c[2][3]*c[3][2] - c[1][2]*c[2][1]*c[3][3] + c[1][1]*c[2][2]*c[3][3]);
		m.c[0][1] = d * (c[0][3]*c[2][2]*c[3][1] - c[0][2]*c[2][3]*c[3][1] - c[0][3]*c[2][1]*c[3][2] + c[0][1]*c[2][3]*c[3][2] + c[0][2]*c[2][1]*c[3][3] - c[0][1]*c[2][2]*c[3][3]);
		m.c[0][2] = d * (c[0][2]*c[1][3]*c[3][1] - c[0][3]*c[1][2]*c[3][1] + c[0][3]*c[1][1]*c[3][2] - c[0][1]*c[1][3]*c[3][2] - c[0][2]*c[1][1]*c[3][3] + c[0][1]*c[1][2]*c[3][3]);
		m.c[0][3] = d * (c[0][3]*c[1][2]*c[2][1] - c[0][2]*c[1][3]*c[2][1] - c[0][3]*c[1][1]*c[2][2] + c[0][1]*c[1][3]*c[2][2] + c[0][2]*c[1][1]*c[2][3] - c[0][1]*c[1][2]*c[2][3]);
		m.c[1][0] = d * (c[1][3]*c[2][2]*c[3][0] - c[1][2]*c[2][3]*c[3][0] - c[1][3]*c[2][0]*c[3][2] + c[1][0]*c[2][3]*c[3][2] + c[1][2]*c[2][0]*c[3][3] - c[1][0]*c[2][2]*c[3][3]);
		m.c[1][1] = d * (c[0][2]*c[2][3]*c[3][0] - c[0][3]*c[2][2]*c[3][0] + c[0][3]*c[2][0]*c[3][2] - c[0][0]*c[2][3]*c[3][2] - c[0][2]*c[2][0]*c[3][3] + c[0][0]*c[2][2]*c[3][3]);
		m.c[1][2] = d * (c[0][3]*c[1][2]*c[3][0] - c[0][2]*c[1][3]*c[3][0] - c[0][3]*c[1][0]*c[3][2] + c[0][0]*c[1][3]*c[3][2] + c[0][2]*c[1][0]*c[3][3] - c[0][0]*c[1][2]*c[3][3]);
		m.c[1][3] = d * (c[0][2]*c[1][3]*c[2][0] - c[0][3]*c[1][2]*c[2][0] + c[0][3]*c[1][0]*c[2][2] - c[0][0]*c[1][3]*c[2][2] - c[0][2]*c[1][0]*c[2][3] + c[0][0]*c[1][2]*c[2][3]);
		m.c[2][0] = d * (c[1][1]*c[2][3]*c[3][0] - c[1][3]*c[2][1]*c[3][0] + c[1][3]*c[2][0]*c[3][1] - c[1][0]*c[2][3]*c[3][1] - c[1][1]*c[2][0]*c[3][3] + c[1][0]*c[2][1]*c[3][3]);
		m.c[2][1] = d * (c[0][3]*c[2][1]*c[3][0] - c[0][1]*c[2][3]*c[3][0] - c[0][3]*c[2][0]*c[3][1] + c[0][0]*c[2][3]*c[3][1] + c[0][1]*c[2][0]*c[3][3] - c[0][0]*c[2][1]*c[3][3]);
		m.c[2][2] = d * (c[0][1]*c[1][3]*c[3][0] - c[0][3]*c[1][1]*c[3][0] + c[0][3]*c[1][0]*c[3][1] - c[0][0]*c[1][3]*c[3][1] - c[0][1]*c[1][0]*c[3][3] + c[0][0]*c[1][1]*c[3][3]);
		m.c[2][3] = d * (c[0][3]*c[1][1]*c[2][0] - c[0][1]*c[1][3]*c[2][0] - c[0][3]*c[1][0]*c[2][1] + c[0][0]*c[1][3]*c[2][1] + c[0][1]*c[1][0]*c[2][3] - c[0][0]*c[1][1]*c[2][3]);
		m.c[3][0] = d * (c[1][2]*c[2][1]*c[3][0] - c[1][1]*c[2][2]*c[3][0] - c[1][2]*c[2][0]*c[3][1] + c[1][0]*c[2][2]*c[3][1] + c[1][1]*c[2][0]*c[3][2] - c[1][0]*c[2][1]*c[3][2]);
		m.c[3][1] = d * (c[0][1]*c[2][2]*c[3][0] - c[0][2]*c[2][1]*c[3][0] + c[0][2]*c[2][0]*c[3][1] - c[0][0]*c[2][2]*c[3][1] - c[0][1]*c[2][0]*c[3][2] + c[0][0]*c[2][1]*c[3][2]);
		m.c[3][2] = d * (c[0][2]*c[1][1]*c[3][0] - c[0][1]*c[1][2]*c[3][0] - c[0][2]*c[1][0]*c[3][1] + c[0][0]*c[1][2]*c[3][1] + c[0][1]*c[1][0]*c[3][2] - c[0][0]*c[1][1]*c[3][2]);
		m.c[3][3] = d * (c[0][1]*c[1][2]*c[2][0] - c[0][2]*c[1][1]*c[2][0] + c[0][2]*c[1][0]*c[2][1] - c[0][0]*c[1][2]*c[2][1] - c[0][1]*c[1][0]*c[2][2] + c[0][0]*c[1][1]*c[2][2]);
		
		return m;
	}

	void decompose( Vec3 &trans, Vec3 &rot, Vec3 &scale ) const
	{
		// Getting translation is trivial
		trans = Vec3( c[3][0], c[3][1], c[3][2] );

		// Scale is length of columns
		scale.x = sqrtf( c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2] );
		scale.y = sqrtf( c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2] );
		scale.z = sqrtf( c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2] );

		if( scale.x == 0 || scale.y == 0 || scale.z == 0 ) return;

		// Detect negative scale with determinant and flip one arbitrary axis
		if( determinant() < 0 ) scale.x = -scale.x;

		// Combined rotation matrix YXZ
		//
		// Cos[y]*Cos[z]+Sin[x]*Sin[y]*Sin[z]   Cos[z]*Sin[x]*Sin[y]-Cos[y]*Sin[z]  Cos[x]*Sin[y]	
		// Cos[x]*Sin[z]                        Cos[x]*Cos[z]                       -Sin[x]
		// -Cos[z]*Sin[y]+Cos[y]*Sin[x]*Sin[z]  Cos[y]*Cos[z]*Sin[x]+Sin[y]*Sin[z]  Cos[x]*Cos[y]

		rot.x = asinf( -c[2][1] / scale.z );
		
		// Special case: Cos[x] == 0 (when Sin[x] is +/-1)
		float f = fabsf( c[2][1] / scale.z );
		if( f > 0.999f && f < 1.001f )
		{
			// Pin arbitrarily one of y or z to zero
			// Mathematical equivalent of gimbal lock
			rot.y = 0;
			
			// Now: Cos[x] = 0, Sin[x] = +/-1, Cos[y] = 1, Sin[y] = 0
			// => m[0][0] = Cos[z] and m[1][0] = Sin[z]
			rot.z = atan2f( -c[1][0] / scale.y, c[0][0] / scale.x );
		}
		// Standard case
		else
		{
			rot.y = atan2f( c[2][0] / scale.z, c[2][2] / scale.z );
			rot.z = atan2f( c[0][1] / scale.x, c[1][1] / scale.y );
		}
	}

	
	void setCol( unsigned int col, const Vec4& v ) 
	{
		x[col * 4 + 0] = v.x;
		x[col * 4 + 1] = v.y;
		x[col * 4 + 2] = v.z;
		x[col * 4 + 3] = v.w;
	}

	Vec4 getCol( unsigned int col ) const
	{
		return Vec4( x[col * 4 + 0], x[col * 4 + 1], x[col * 4 + 2], x[col * 4 + 3] );
	}

	Vec4 getRow( unsigned int row ) const
	{
		return Vec4( x[row + 0], x[row + 4], x[row + 8], x[row + 12] );
	}

	Vec3 getTrans() const
	{
		return Vec3( c[3][0], c[3][1], c[3][2] );
	}
	
	Vec3 getScale() const
	{
		Vec3 scale;
		// Scale is length of columns
		scale.x = sqrtf( c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2] );
		scale.y = sqrtf( c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2] );
		scale.z = sqrtf( c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2] );
		return scale;
	}
};


// -------------------------------------------------------------------------------------------------
// Plane
// -------------------------------------------------------------------------------------------------

class Plane
{
public:
	Vec3 normal; 
	float dist;

	// ------------
	// Constructors
	// ------------
	Plane() 
	{ 
		normal.x = 0; normal.y = 0; normal.z = 0; dist = 0; 
	};

	explicit Plane( const float a, const float b, const float c, const float d )
	{
		normal = Vec3( a, b, c );
		float invLen = 1.0f / normal.length();
		normal *= invLen;	// Normalize
		dist = d * invLen;
	}

	Plane( const Vec3 &v0, const Vec3 &v1, const Vec3 &v2 )
	{
		normal = v1 - v0;
		normal = normal.cross( v2 - v0 );
		normal.normalize();
		dist = -normal.dot( v0 );
	}

	// ----------------
	// Other operations
	// ----------------
	float distToPoint( const Vec3 &v ) const
	{
		return normal.dot( v ) + dist;
	}
};


// -------------------------------------------------------------------------------------------------
// Intersection
// -------------------------------------------------------------------------------------------------

inline bool rayTriangleIntersection( const Vec3 &rayOrig, const Vec3 &rayDir, 
                                     const Vec3 &vert0, const Vec3 &vert1, const Vec3 &vert2,
                                     Vec3 &intsPoint )
{
	// Idea: Tomas Moeller and Ben Trumbore
	// in Fast, Minimum Storage Ray/Triangle Intersection 
	
	// Find vectors for two edges sharing vert0
	Vec3 edge1 = vert1 - vert0;
	Vec3 edge2 = vert2 - vert0;

	// Begin calculating determinant - also used to calculate U parameter
	Vec3 pvec = rayDir.cross( edge2 );

	// If determinant is near zero, ray lies in plane of triangle
	float det = edge1.dot( pvec );


	// *** Culling branch ***
	/*if( det < Epsilon )return false;

	// Calculate distance from vert0 to ray origin
	Vec3 tvec = rayOrig - vert0;

	// Calculate U parameter and test bounds
	float u = tvec.dot( pvec );
	if (u < 0 || u > det ) return false;

	// Prepare to test V parameter
	Vec3 qvec = tvec.cross( edge1 );

	// Calculate V parameter and test bounds
	float v = rayDir.dot( qvec );
	if (v < 0 || u + v > det ) return false;

	// Calculate t, scale parameters, ray intersects triangle
	float t = edge2.dot( qvec ) / det;*/


	// *** Non-culling branch ***
	if( det > -Epsilon && det < Epsilon ) return 0;
	float inv_det = 1.0f / det;

	// Calculate distance from vert0 to ray origin
	Vec3 tvec = rayOrig - vert0;

	// Calculate U parameter and test bounds
	float u = tvec.dot( pvec ) * inv_det;
	if( u < 0.0f || u > 1.0f ) return 0;

	// Prepare to test V parameter
	Vec3 qvec = tvec.cross( edge1 );

	// Calculate V parameter and test bounds
	float v = rayDir.dot( qvec ) * inv_det;
	if( v < 0.0f || u + v > 1.0f ) return 0;

	// Calculate t, ray intersects triangle
	float t = edge2.dot( qvec ) * inv_det;


	// Calculate intersection point and test ray length and direction
	intsPoint = rayOrig + rayDir * t;
	Vec3 vec = intsPoint - rayOrig;
	if( vec.dot( rayDir ) < 0 || vec.length() > rayDir.length() ) return false;

	return true;
}


inline bool rayAABBIntersection( const Vec3 &rayOrig, const Vec3 &rayDir, 
                                 const Vec3 &mins, const Vec3 &maxs )
{
	// SLAB based optimized ray/AABB intersection routine
	// Idea taken from http://ompf.org/ray/
	
	float l1 = (mins.x - rayOrig.x) / rayDir.x;
	float l2 = (maxs.x - rayOrig.x) / rayDir.x;
	float lmin = minf( l1, l2 );
	float lmax = maxf( l1, l2 );

	l1 = (mins.y - rayOrig.y) / rayDir.y;
	l2 = (maxs.y - rayOrig.y) / rayDir.y;
	lmin = maxf( minf( l1, l2 ), lmin );
	lmax = minf( maxf( l1, l2 ), lmax );
		
	l1 = (mins.z - rayOrig.z) / rayDir.z;
	l2 = (maxs.z - rayOrig.z) / rayDir.z;
	lmin = maxf( minf( l1, l2 ), lmin );
	lmax = minf( maxf( l1, l2 ), lmax );

	if( (lmax >= 0.0f) & (lmax >= lmin) )
	{
		// Consider length
		const Vec3 rayDest = rayOrig + rayDir;
		Vec3 rayMins( minf( rayDest.x, rayOrig.x), minf( rayDest.y, rayOrig.y ), minf( rayDest.z, rayOrig.z ) );
		Vec3 rayMaxs( maxf( rayDest.x, rayOrig.x), maxf( rayDest.y, rayOrig.y ), maxf( rayDest.z, rayOrig.z ) );
		return 
			(rayMins.x < maxs.x) && (rayMaxs.x > mins.x) &&
			(rayMins.y < maxs.y) && (rayMaxs.y > mins.y) &&
			(rayMins.z < maxs.z) && (rayMaxs.z > mins.z);
	}
	else
		return false;
}


inline float nearestDistToAABB( const Vec3 &pos, const Vec3 &mins, const Vec3 &maxs )
{
	const Vec3 center = (mins + maxs) * 0.5f;
	const Vec3 extent = (maxs - mins) * 0.5f;
	
	Vec3 nearestVec;
	nearestVec.x = maxf( 0, fabsf( pos.x - center.x ) - extent.x );
	nearestVec.y = maxf( 0, fabsf( pos.y - center.y ) - extent.y );
	nearestVec.z = maxf( 0, fabsf( pos.z - center.z ) - extent.z );
	
	return nearestVec.length();
}
