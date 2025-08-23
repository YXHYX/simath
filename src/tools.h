#pragma once

#include <math.h>
#include <iostream>

#define DPI 6.2831853
#define MPI 3.1415926
#define HPI 1.5707963
#define QPI 0.7853981

//todo make quaternions
namespace mathT
{
	template<typename T> 
	struct vec2 {
		T x, y;

		vec2() { x = 0; y = 0; }
		vec2(T X, T Y)
		{
			this->x = X;
			this->y = Y;
		}
		vec2(T X)
		{
			this->x = X;
			this->y = X;
		}
		
		template<typename U>
		vec2(const vec2<U>& a)
		{
			this->x = T(a.x);
			this->y = T(a.y);
		}
		vec2<T> operator+(vec2<T> other)
		{
			return vec2<T>(this->x + other.x, this->y + other.y);
		}
		vec2<T> operator-(vec2<T> other)
		{
			return vec2<T>(this->x - other.x, this->y - other.y);
		}
		vec2<T> operator*(T other)
		{
			return vec2<T>(this->x * other, this->y * other);
		}
		T operator* (vec2<T> other)
		{
			return vec2<T>(this->x * other, this->y * other);
		}
		vec2<T> operator/(T other)
		{
			if(other != 0)
				return vec2<T>(this->x / other, this->y / other);
			return vec2<T>();
		}
		T & operator[](unsigned int i)
		{
			if (i > 1)
				throw "ERROR::VECTOR::INDEX_OUT_OF_RANGE\n";
			switch (i)
			{
			case 0:
				return x;
			case 1:
				return y;
			}
		}
		bool operator==(vec2<T> other)
		{
			return (this->x == other.x && this->y == other.y);
		}
		template<typename U>
		void operator=(vec2<U> other)
		{
			this->x = T(other.x);
			this->y = T(other.y);
		}
		void operator+=(vec2<T> other)
		{
			this->x += other.x;
			this->y += other.y;
		}
		void operator-=(vec2<T> other)
		{
			this->x -= other.x;
			this->y -= other.y;
		}
		void operator*=(T other)
		{
			this->x *= other;
			this->y *= other;
		}
		void operator/=(T other)
		{
			if(other != 0)
			{
				this->x /= other;
				this->y /= other;
			}
			else
			{
				this->x = 0;
				this->y = 0;
			}
		}

	};

	template<typename T>
	struct vec3 {
		T x, y, z;

		vec3() { x = 0; y = 0; z = 0; }
		vec3(T X, T Y, T Z)
		{
			this->x = X;
			this->y = Y;
			this->z = Z;
		}
		vec3(T X)
		{
			this->x = X;
			this->y = X;
			this->z = X;
		}
		template<typename U>
		vec3(const vec3<U> & a)
		{
			this->x = T(a.x);
			this->y = T(a.y);
			this->z = T(a.z);
		}

		vec3<T> operator+(vec3<T> other)
		{
			return vec3<T>(this->x + other.x, this->y + other.y, this->z + other.z);
		}

		vec3<T> operator-(vec3<T> other)
		{
			return vec3<T>(this->x - other.x, this->y - other.y, this->z - other.z);
		}
		vec3<T> operator*(T other)
		{
			return vec3<T>(this->x * other, this->y * other, this->z * other);
		}
		T operator* (vec3<T> other)
		{
			return vec3<T>(this->x * other, this->y * other, this->z * other);
		}
		vec3<T> operator/(T other)
		{
			if(other != 0)
				return vec3<T>(this->x / other, this->y / other, this->z / other);
			return vec3<T>();
		}

		T& operator[](unsigned int i)
		{
			if (i > 2)
				throw "ERROR::VECTOR::INDEX_OUT_OF_RANGE\n";
			switch (i)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			}
		}

		bool operator==(vec3<T> other)
		{
			return (this->x == other.x && this->y == other.y && this->z == other.z);
		}

		void operator+=(vec3<T> other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
		}

		template<typename U>
		void operator=(vec3<U> other)
		{
			this->x = T(other.x);
			this->y = T(other.y);
			this->z = T(other.z);
		}

		void operator-=(vec3<T> other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
		}

		void operator/=(T f)
		{
			if (f != 0)
			{
				this->x /= f;
				this->y /= f;
				this->z /= f;
			}
			else
			{
				this->x = 0;
				this->y = 0;
				this->z = 0;
			}
		}
		void operator*=(T f)
		{
			this->x *= f;
			this->y *= f;
			this->z *= f;
		}
	};

	template<typename T>
	struct vec4 {
		T x, y, z, w;

		vec4() { x = 0; y = 0; z = 0; w = 0; }
		vec4(T X, T Y, T Z, T W)
		{
			this->x = X;
			this->y = Y;
			this->z = Z;
			this->w = W;
		}
		vec4(T X)
		{
			this->x = X;
			this->y = X;
			this->z = X;
			this->w = X;
		}
		template<typename U>
		vec4(const vec4<U>& a)
		{
			this->x = T(a.x);
			this->y = T(a.y);
			this->z = T(a.z);
			this->w = T(a.w);
		}
		T & operator[](unsigned int i)
		{
			if(i > 3)
				throw "ERROR::VECTOR::INDEX_OUT_OF_RANGE\n";
			switch (i)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			}
		}
		template<typename U>
		void operator=(vec4<U> other)
		{
			this->x = T(other.x);
			this->y = T(other.y);
			this->z = T(other.z);
			this->w = T(other.w);
		}
		bool operator==(vec4<T> other)
		{
			return (this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w);
		}
		void operator+=(vec4<T> other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			this->w += other.w;
		}

		void operator-=(vec4<T> other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			this->w -= other.w;
		}

		void operator/=(T f)
		{
			if (f != 0)
			{
				this->x /= f;
				this->y /= f;
				this->z /= f;
				this->w /= f;
			}
			else
			{
				this->x = 0;
				this->y = 0;
				this->z = 0;
				this->w = 0;
			}
		}
		void operator*=(T f)
		{
			this->x *= f;
			this->y *= f;
			this->z *= f;
			this->w *= f;
		}
		T operator* (vec4<T> other)
		{
			return vec4<T>(this->x * other, this->y * other, this->z * other, this->w * other);
		}
		vec4<T> operator+(vec4<T> other)
		{
			return vec4<T>(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
		}
		vec4<T> operator-(vec4<T> other)
		{
			return vec4<T>(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
		}
		vec4<T> operator*(T other)
		{
			return vec4<T>(this->x * other, this->y * other, this->z * other, this->w * other);
		}
		vec4<T> operator/(T other)
		{
			if (other != 0)
				return vec4<T>(this->x / other, this->y / other, this->z / other, this->w / other);
			return vec4<T>();
		}

	};
	
	



	//           MATRICES            \\

	template<typename T>
	struct mat2x2 {
		vec2<T> A, B;
		mat2x2() {}
		mat2x2(const mat2x2<T>& m)
		{
			A = m.A;
			B = m.B;
		}
		mat2x2(vec2<T> a, vec2<T> b)
		{
			this->A = a;
			this->B = b;
		}
		mat2x2(T a, T b, T c, T d) 
		{
			this->A.x = a;
			this->A.y = b;
			this->B.x = c;
			this->B.y = d;
		}
		mat2x2(T a)
		{
			this->A.x = a;
			this->A.y = a;
			this->B.x = a;
			this->B.y = a;
		}

		mat2x2<T> operator+(mat2x2<T> other)
		{
			return mat2x2<T>(this->A + other.A, this->B + other.B);
		}

		mat2x2<T> operator-(mat2x2<T> other)
		{
			return mat2x2<T>(this->A - other.A, this->B - other.B);
		}
	
		mat2x2<T> operator*(mat2x2<T> other)
		{
			return mat2x2<T>(this->A.x * other.A.x + this->A.y * other.B.x, this->A.x * other.A.y + this->A.y * other.B.y,
				this->B.x * other.A.x + this->B.y * other.B.x, this->B.x * other.A.y + this->B.y * other.B.y);
		}
		vec2<T> operator*(vec2<T> other)
		{
			return vec2<T>(this->A.x * other.x + this->A.y * other.y, this->B.x * other.x * this->B.y * other.y);
		}
		mat2x2<T> operator*(T other)
		{
			return mat2x2<T>(this->A * other, this->B * other);
		}
		mat2x2<double> inverse()
		{
			//if determinant is null ignore
			double det = A.x * B.y - A.y * B.x;
			if(det != 0.0)
				return mat2x2<double>(B.y / det, -A.y / det, -B.x / det , A.x / det);
			return mat2x2<T>();
		}

		vec2<T> & operator[](unsigned int i)
		{
			if (i > 1)
				throw "ERROR::MATRIX::INDEX_OUT_OF_RANGE\n";
			switch (i)
			{
			case 0:
				return this->A;
			case 1:
				return this->B;
			}
		}

		bool operator==(mat2x2<T> other)
		{
			return (this->A == other.A && this->B == other.B);
		}

		template<typename U>
		void operator=(mat2x2<U> other)
		{
			this->A = other.A;
			this->B = other.B;
		}


		void operator+=(mat2x2<T> other)
		{
			this->A += other.A;
			this->B += other.B;
		}

		void operator-=(mat2x2<T> other)
		{
			this->A -= other.A;
			this->B -= other.B;
		}

		void operator/=(T f)
		{
			if (f != 0)
			{
				this->A /= f;
				this->B /= f;
			}
		}
		void operator*=(T f)
		{
			this->A *= f;
			this->B *= f;
		}
		void operator*=(mat2x2 f)
		{
			*this = *this * f;
		}

	};
	template<typename T>
	struct mat3x3 {
		vec3<T> A, B, C;
		mat3x3() {}
		mat3x3(const mat3x3<T>& m)
		{
			A = m.A;
			B = m.B;
			C = m.C;
		}
		mat3x3(vec3<T> a, vec3<T> b, vec3<T> c)
		{
			this->A = a;
			this->B = b;
			this->C = c;
		}
		mat3x3(T a, T b, T c, T d, T e, T f, T g, T h, T i)
		{
			this->A.x = a;
			this->A.y = b;
			this->A.z = c;

			this->B.x = d;
			this->B.y = e;
			this->B.z = f;

			this->C.x = g;
			this->C.y = h;
			this->C.z = i;
		}
		mat3x3(T a)
		{
			this->A.x = a;
			this->A.y = a;
			this->A.z = a;

			this->B.x = a;
			this->B.y = a;
			this->B.z = a;

			this->C.x = a;
			this->C.y = a;
			this->C.z = a;
		}

		mat3x3<T> operator+(mat3x3<T> other)
		{
			return mat3x3<T>(this->A + other.A, this->B + other.B, this->C + other.C);
		}

		mat3x3<T> operator-(mat3x3<T> other)
		{
			return mat3x3<T>(this->A - other.A, this->B - other.B, this->C - other.C);
		}

		mat3x3<T> operator*(mat3x3<T> other)
		{
			T a, b, c, d, e, f, g, h, i;
			a = this->A.x * other.A.x + this->A.y * other.B.x + this->A.z * other.C.x;
			b = this->A.x * other.A.y + this->A.y * other.B.y + this->A.z * other.C.y;
			c = this->A.x * other.A.z + this->A.y * other.B.z + this->A.z * other.C.z;
		
			d = this->B.x * other.A.x + this->B.y * other.B.x + this->B.z * other.C.x;
			e = this->B.x * other.A.y + this->B.y * other.B.y + this->B.z * other.C.y;
			f = this->B.x * other.A.z + this->B.y * other.B.z + this->B.z * other.C.z;
		
			g = this->C.x * other.A.x + this->C.y * other.B.x + this->C.z * other.C.x;
			h = this->C.x * other.A.y + this->C.y * other.B.y + this->C.z * other.C.y;
			i = this->C.x * other.A.z + this->C.y * other.B.z + this->C.z * other.C.z;
		
			return mat3x3<T>(a,b,c,d,e,f,g,h,i);
		}
		vec3<T> operator*(vec3<T> other)
		{
			T a, b, c;
			a = this->A.x * other.x + this->A.y * other.y + this->A.z * other.z;
			b = this->B.x * other.x + this->B.y * other.y + this->B.z * other.z;
			c = this->C.x * other.x + this->C.y * other.y + this->C.z * other.z;
			return vec3<T>(a, b, c);
		}

		mat3x3<T> operator*(T other)
		{
			return mat3x3<T>(this->A * other, this->B * other, this->C * other);
		}
		vec3<T> & operator[](unsigned int i)
		{
			if (i > 2)
				throw "ERROR::MATRIX::INDEX_OUT_OF_RANGE\n";
			switch (i)
			{
			case 0:
				return this->A;
			case 1:
				return this->B;
			case 2:
				return this->C;
			}
		}
		mat3x3<double> inverse()
		{
			//if determinant is null ignore
			double det = A.x * B.y * C.z + 
				A.y * B.z * C.x + 
				A.z * B.x * C.y + 
				- A.y * B.x * C.z
				- A.z * B.y * C.x
				- A.x * B.z * C.y;
			if (det != 0.0)
			{
				double a, b, c, d, e, f, g, h, i;
				a = B.y * C.z - B.z * C.y;
				b = B.x * C.z - B.z * C.x;
				c = B.x * C.y - B.x * C.y;

				d = A.y * C.z - A.z * C.y;
				e = A.x * C.z - A.z * C.x;
				f = A.x * C.y - A.y * C.x;
				
				g = A.y * B.z - A.z * B.y;
				h = A.x * B.z - A.z * B.x;
				i = A.x * B.y - B.x * A.y;

				return mat3x3<double>(a,b,c,d,e,f,g,h,i)/det;
			}
			return mat3x3<double>();
		}

		bool operator==(mat3x3<T> other)
		{
			return (this->A == other.A && this->B == other.B && this->C == other.C);
		}

		template<typename U>
		void operator=(mat3x3<U> other)
		{
			this->A = other.A;
			this->B = other.B;
			this->C = other.C;
		}

		void operator+=(mat3x3<T> other)
		{
			this->A += other.A;
			this->B += other.B;
			this->C += other.C;
		}

		void operator-=(mat3x3<T> other)
		{
			this->A -= other.A;
			this->B -= other.B;
			this->C -= other.C;
		}

		void operator/=(T f)
		{
			if (f != 0)
			{
				this->A /= f;
				this->B /= f;
				this->C /= f;
			}
		}
		void operator*=(T f)
		{
			this->A *= f;
			this->B *= f;
			this->C *= f;
		}
		void operator*=(mat3x3 f)
		{
			*this = *this * f;
		}

	};
	/*
	* A x y z w
	* B x y z w
	* C x y z w
	* D x y z w
	*/
	template<typename T>
	struct mat4x4 {
		vec4<T> A, B, C, D;
		mat4x4() {}
		mat4x4(const mat4x4<T>& m) 
		{
			A = m.A;
			B = m.B;
			C = m.C;
			D = m.D;
		}
		mat4x4(vec4<T> a, vec4<T> b, vec4<T> c, vec4<T> d)
		{
			this->A = a;
			this->B = b;
			this->C = c;
			this->D = d;
		}
		mat4x4(T a, T b, T c, T d, T e, T f, T g, T h, T i, T j, T k, T l, T m, T n, T o, T p)
		{
			this->A.x = a;
			this->A.y = b;
			this->A.z = c;
			this->A.w = d;

			this->B.x = e;
			this->B.y = f;
			this->B.z = g;
			this->B.w = h;

			this->C.x = i;
			this->C.y = j;
			this->C.z = k;
			this->C.w = l;
			
			this->D.x = m;
			this->D.y = n;
			this->D.z = o;
			this->D.w = p;
		}
		mat4x4(T a)
		{
			this->A.x = a;
			this->A.y = a;
			this->A.z = a;
			this->A.w = a;

			this->B.x = a;
			this->B.y = a;
			this->B.z = a;
			this->B.w = a;

			this->C.x = a;
			this->C.y = a;
			this->C.z = a;
			this->C.w = a;

			this->D.x = a;
			this->D.y = a;
			this->D.z = a;
			this->D.w = a;
		}
		vec4<T>& operator[](unsigned int i)
		{
			if (i > 3)
				throw "ERROR::MATRIX::INDEX_OUT_OF_RANGE\n";
			switch (i)
			{
			case 0:
				return this->A;
			case 1:
				return this->B;
			case 2:
				return this->C;
			case 3:
				return this->D;
			}
		}
		mat4x4<T> operator+(mat4x4<T> other)
		{
			return mat4x4<T>(this->A + other.A, this->B + other.B, this->C + other.C, this->D + other.D);
		}

		mat4x4<T> operator-(mat4x4<T> other)
		{
			return mat4x4<T>(this->A - other.A, this->B - other.B, this->C - other.C, this->D - other.D);
		}

		mat4x4<T> operator*(mat4x4<T> other)
		{
			T a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p;
			a = this->A.x * other.A.x + this->A.y * other.B.x + this->A.z * other.C.x + this->A.w * other.D.x;
			b = this->A.x * other.A.y + this->A.y * other.B.y + this->A.z * other.C.y + this->A.w * other.D.y;
			c = this->A.x * other.A.z + this->A.y * other.B.z + this->A.z * other.C.z + this->A.w * other.D.z;
			d = this->A.x * other.A.w + this->A.y * other.B.w + this->A.z * other.C.w + this->A.w * other.D.w;

			e = this->B.x * other.A.x + this->B.y * other.B.x + this->B.z * other.C.x + this->B.w * other.D.x;
			f = this->B.x * other.A.y + this->B.y * other.B.y + this->B.z * other.C.y + this->B.w * other.D.y;
			g = this->B.x * other.A.z + this->B.y * other.B.z + this->B.z * other.C.z + this->B.w * other.D.z;
			h = this->B.x * other.A.w + this->B.y * other.B.w + this->B.z * other.C.w + this->B.w * other.D.w;

			i = this->C.x * other.A.x + this->C.y * other.B.x + this->C.z * other.C.x + this->C.w * other.D.x;
			j = this->C.x * other.A.y + this->C.y * other.B.y + this->C.z * other.C.y + this->C.w * other.D.y;
			k = this->C.x * other.A.z + this->C.y * other.B.z + this->C.z * other.C.z + this->C.w * other.D.z;
			l = this->C.x * other.A.w + this->C.y * other.B.w + this->C.z * other.C.w + this->C.w * other.D.w;

			m = this->D.x * other.A.x + this->D.y * other.B.x + this->D.z * other.C.x + this->D.w * other.D.x;
			n = this->D.x * other.A.y + this->D.y * other.B.y + this->D.z * other.C.y + this->D.w * other.D.y;
			o = this->D.x * other.A.z + this->D.y * other.B.z + this->D.z * other.C.z + this->D.w * other.D.z;
			p = this->D.x * other.A.w + this->D.y * other.B.w + this->D.z * other.C.w + this->D.w * other.D.w;

			return mat4x4<T>(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p);

		}

		vec4<T> operator*(vec4<T> other)
		{
			T a, b, c, d;
			a = this->A.x * other.x + this->A.y * other.y + this->A.z * other.z + this->A.w * other.w;
			b = this->B.x * other.x + this->B.y * other.y + this->B.z * other.z + this->B.w * other.w;
			c = this->C.x * other.x + this->C.y * other.y + this->C.z * other.z + this->C.w * other.w;
			d = this->D.x * other.x + this->D.y * other.y + this->D.z * other.z + this->D.w * other.w;

			return vec4<T>(a, b, c, d);
		}

		mat4x4<T> operator*(T other)
		{
			return mat4x4<T>(this->A * other, this->B * other, this->C * other, this->D * other);
		}
		
		mat4x4<double> invert() 
		{
			mat4x4<double> M = *this;
			mat4x4<double> I(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

			for (int i = 0; i < 4; i++)
			{
				// make sure the pivot is not null
				if (M[i][i] == 0)
				{
					int c = -1;
					double max = 0;
					for (int k = 0; k < 4; k++)
					{
						if (max < abs(M[k][i]))
						{
							max = abs(M[k][i]);
							c = k;
						}
					}
					if (c == -1) //not invertible
						return mat4x4<double>();
					vec4<double> temp = M[i], tempp = I[i];
					M[i] = M[c];
					I[i] = I[c];

					M[c] = temp;
					I[c] = tempp;
				}
				// reduce the rows below pivots
				if (i == 3)
					continue;
				for (int j = i; j < 3; j++)
				{
					for (int k = j + 1; k < 4; k++)
					{
						double  coef = M[j][k] / M[j][j];
						for( int l = 0; l < 4; l++)
						{
							M[l][k] = M[l][k] - coef * M[l][j];
							I[l][k] = I[l][k] - coef * I[l][j];
						}

						M[j][k] = 0;
					}
				}
			}

			//rescale pivots back to 1
			for (int i = 0; i < 4; i++)
			{
				double div = M[i][i];
				for(int j = 0; j < 4; j++)
				{
					M[j][i] /= div;
					I[j][i] /= div;
				}
			}

			// reduce the rows above pivots
			for(int i = 0; i < 4; i++)
			{
				for (int j = i+1; j < 4; j++)
				{
					double coef = M[j][i];
					for(int k = 0; k < 4; k++)
					{
						M[k][i] -= M[k][j] * coef;
						I[k][i] -= I[k][j] * coef;
					}

					M[j][i] = 0;
				}
			}

			return I;
		}

		bool operator==(mat3x3<T> other)
		{
			return (this->A == other.A && this->B == other.B && this->C == other.C && this->D == other.D);
		}

		template<typename U>
		void operator=(mat4x4<U> other)
		{
			this->A = other.A;
			this->B = other.B;
			this->C = other.C;
			this->D = other.D;
		}

		void operator+=(mat4x4<T> other)
		{
			this->A += other.A;
			this->B += other.B;
			this->C += other.C;
			this->D += other.D;
		}

		void operator-=(mat4x4<T> other)
		{
			this->A -= other.A;
			this->B -= other.B;
			this->C -= other.C;
			this->D -= other.D;
		}

		void operator/=(T f)
		{
			if (f != 0)
			{
				this->A /= f;
				this->B /= f;
				this->C /= f;
				this->D /= f;
			}
		}
		void operator*=(T f)
		{
			this->A *= f;
			this->B *= f;
			this->C *= f;
			this->D *= f;
		}
		void operator*=(mat4x4 f)
		{
			*this = *this * f;
		}

	};

	template<typename T>
	inline mat2x2<T> identity2x2()
	{
		return mat2x2<T>(1, 0, 0, 1);
	}

	template<typename T>
	mat3x3<T> identity3x3()
	{
		return mat3x3<T>(1, 0, 0, 0, 1, 0, 0, 0, 1);
	}

	template<typename T>
	mat4x4<T> identity4x4()
	{
		return mat4x4<T>(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	}

	//			 QUATERNIONS		 \\
	
	template<typename T>
	struct quat
	{
		T a, b, c, d;

		quat(T x, T y, T z, T w)
			: a(x), b(y), c(z), d(w) {
		}
		quat() : a(0), b(0), c(0), d(0) {}
		quat<double> inverse()
		{
			if (this == quat<T>())
				return quat<T>();
			return quat<T>(a, -b, -c, -d) / (a * a + b * b + c * c + d * d);
		}
		quat<T> operator+(quat<T>& other)
		{
			return quat(a + other.a, b + other.b, c + other.c, d + other.d);
		}
		quat<T> operator-(quat<T>& other)
		{
			return quat(a - other.a, b - other.b, c - other.c, d - other.d);
		}
		quat<T> operator*(quat<T> other)
		{
			return quat(a * other.a - b * other.b - c * other.c - d * other.d,
				a * other.b + b * other.a + c * other.d - d * other.c,
				a * other.c - b * other.d + c * other.a + d * other.b,
				a * other.d + b * other.c - c * other.b + d * other.a);
		}
		void operator=(quat<T> other)
		{
			a = other.a;
			b = other.b;
			c = other.c;
			d = other.d;
		}

		template <typename U>
		quat<T> operator*(U scale)
		{
			return quat(a * scale, b * scale, c * scale, d * scale);
		}
		template <typename U>
		quat<double> operator/(U scale)
		{
			if (scale == 0)
				throw"QUAT : division by zero";
			return quat(a / scale, b / scale, c / scale, d / scale);

		}
		bool operator==(quat<T> other)
		{
			if (a == other.a && b == other.b && c == other.c && d == other.d)
				return true;
			return false;
		}
		bool operator!=(quat<T>& other)
		{
			return *this == other;
		}
		void operator+=(quat<T>& other)
		{
			a += other.a;
			b += other.b;
			c += other.c;
			d += other.d;
		}
		void operator-=(quat<T>& other)
		{
			a -= other.a;
			b -= other.b;
			c -= other.c;
			d -= other.d;
		}
		void operator*=(quat<T>& other)
		{
			*this = *this * other;
		};
		template<typename U>
		void operator*=(U& scale)
		{
			*this = *this * scale;
		};
		mat4x4<T> rotation()
		{
			mat4x4<T> mat = identity4x4<T>();
			T a2 = a * a, b2 = b * b, c2 = c * c, d2 = d * d, bc = b * c, ad = a * d, ac = a * c, bd = b * d, ab = a * b, cd = c * d;
			mat[0][0] = a2 + b2 - c2 - d2;
			mat[0][1] = 2 * (bc - ad);
			mat[0][2] = 2 * (ac + bd);

			mat[1][0] = 2 * (bc + ad);
			mat[1][1] = a2 - b2 + c2 - d2;
			mat[1][2] = 2 * (cd - ab);

			mat[2][0] = 2 * (bd - ac);
			mat[2][1] = 2 * (ab + cd);
			mat[2][2] = a2 - b2 - c2 + d2;

			return mat;
		}
	};

	///  TYPE DEFINITIONS

	typedef vec2<float> vec2f;
	typedef vec2<unsigned int> vec2u;
	typedef vec2<int> vec2i;
	typedef vec2<double> vec2d;
	typedef vec2<unsigned char> vec2uc;

	typedef vec3<float> vec3f;
	typedef vec3<unsigned int> vec3u;
	typedef vec3<int> vec3i;
	typedef vec3<double> vec3d;
	typedef vec3<unsigned char> vec3uc;
	
	typedef vec4<float> vec4f;
	typedef vec4<unsigned int> vec4u;
	typedef vec4<int> vec4i;
	typedef vec4<double> vec4d;
	typedef vec4<unsigned char> vec4uc;

	typedef mat2x2<float> mat2x2f;
	typedef mat2x2<unsigned int> mat2x2u;
	typedef mat2x2<int> mat2x2i;
	typedef mat2x2<double> mat2x2d;

	typedef mat3x3<float> mat3x3f;
	typedef mat3x3<unsigned int> mat3x3u;
	typedef mat3x3<int> mat3x3i;
	typedef mat3x3<double> mat3x3d;

	typedef mat4x4<float> mat4x4f;
	typedef mat4x4<unsigned int> mat4x4u;
	typedef mat4x4<int> mat4x4i;
	typedef mat4x4<double> mat4x4d;

	typedef quat<float> quatf;
	typedef quat<unsigned int> quatu;
	typedef quat<int> quati;
	typedef quat<double> quatd;

	template<typename T>
	inline T dot(vec2<T> A, vec2<T> B)
	{
		return T(A.x * B.x + A.y * B.y);
	}

	template<typename T>
	inline T dot(vec3<T> A, vec3<T> B)
	{
		return T(A.x * B.x + A.y * B.y + A.z * B.z);
	}


	template<typename T>
	inline T dot(vec4<T> A, vec4<T> B)
	{
		return T(A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w);
	}


	template<typename T>
	T norm(vec2<T> A)
	{
		return sqrt(A.x * A.x + A.y * A.y);
	}
	template<typename T>
	T norm(vec3<T> A)
	{
		return sqrt(A.x * A.x + A.y * A.y + A.z * A.z);
	}
	template<typename T>
	T norm(vec4<T> A)
	{
		return sqrt(A.x * A.x + A.y * A.y + A.z * A.z + A.w * A.w);
	}
	template<typename T>
	T norm(quat<T> A)
	{
		return sqrt(A.a * A.a + A.b * A.b + A.c * A.c + A.d * A.d);
	}
	template<typename T>
	vec2<double> normalize(vec2<T> A)
	{
		if(A != vec2<T>(0,0))
		{
			return vec2<double>(A/norm(A));
		}
		return vec2<double>(0,0);
	}
	template<typename T>
	vec3<double> normalize(vec3<T> A)
	{
		if (A == vec3<T>(0, 0, 0))
			return vec3<double>(0, 0, 0);

		return vec3<double>(A / norm(A));
	}
	template<typename T>
	vec4<double> normalize(vec4<T> A)
	{
		if (A != vec4<T>(0, 0, 0, 0))
		{
			return vec4<double>(A / norm(A));
		}
		return vec4<double>(0, 0, 0, 0);
	}

	template<typename T>
	quat<double> normalize(quat<T> q)
	{
		if (!(q == quat<T>(0, 0, 0, 0)))
		{
			return quat<double>(q/norm(q));
		}
		return quat<double>(0, 0, 0, 0);
	}

	template<typename T>
	vec3<T> cross(vec3<T> A, vec3<T> B)
	{
		return vec3<T>(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
	}

	template<typename T>
	void print(mat2x2<T> M)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				double d = M[j][i];
				std::cout << ((d < 0) ? " " : "  ") << d;
			}
			std::cout << std::endl;
		}
	}

	template<typename T>
	void print(mat3x3<T> M)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				double d = M[j][i];
				std::cout << ((d < 0) ? " " : "  ") << d;
			}
			std::cout << std::endl;
		}
	}
	
	template<typename T>
	void print(mat4x4<T> M) 
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				double d = M[j][i];
				std::cout << ((d < 0) ? " " : "  ") << d;
			}
			std::cout << std::endl;
		}
	}
	namespace transformation
	{
		//void rotate(mat4x4d mat, vec3d rot);
		inline void translate(mat4x4d& mat, vec3d pos)
		{
			mat4x4d translation = identity4x4<double>();
			translation[0][3] = -pos.x;
			translation[1][3] = -pos.y;
			translation[2][3] = -pos.z;
			mat = mat*translation;
		}
		inline void scale(mat4x4d& mat, vec3d scale)
		{
			mat[0][0] *= scale.x;
			mat[1][1] *= scale.y;
			mat[2][2] *= scale.z;
		}
		
		inline void rotate(mat4x4d& mat, vec3d axis, double theta)
		{
			//theta =theta- 2*MPI*int(theta / (2 * MPI));
			mat = mat * normalize(quat<double>(cos(theta/2), axis.x, axis.y, axis.z)).rotation();
		}
		//rotate along each axis (x,y,z) with their given angle (theta beta gamma) in the xyz order (NOT COMMUTATIVE!!!!)
		inline void rotate(mat4x4d& mat, vec3d axis)
		{
			//theta =theta- 2*MPI*int(theta / (2 * MPI));
			mat = mat * normalize(quat<double>(cos(axis.x / 2), 1, 0, 0)).rotation();
			mat = mat * normalize(quat<double>(cos(axis.y / 2), 0, 1, 0)).rotation();
			mat = mat * normalize(quat<double>(cos(axis.z / 2), 0, 0, 1)).rotation();
		}
	};
}