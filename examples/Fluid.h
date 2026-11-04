#pragma once

#include <tools.h>
#include <cmath>
#include <algorithm>
#include <vector>

namespace fluidSim
{
	using namespace mathT;

	class Fluid2
	{
	private:

		//vector fields
		std::vector<vec2f> vel; //velocity field
		std::vector<vec2f> vel0; // advected velocity field
		std::vector<unsigned int> col; // color field
		std::vector<char> ch; // character field
		std::vector<float> density; // divergence field
		std::vector<float> s; 
		float viscosity, diff;

		std::vector<vec2f> forces;

		vec2i size;
	public:
		Fluid2(int w, int h)
			: size(w, h), density(1)
		{
			for (int i = 0; i < size.x; i++)
				for (int j = 0; j < size.y; j++)
				{
					s.push_back(1);
					density.push_back(1);
					col.push_back(float(i) / float(size.x) * 2);
					ch.push_back(i > float(size.x) / 2.f ? ' ' : 'x');
					vel.push_back(vec2f(1, 0));
					vel0.push_back(vec2f(1, 0));
				}


			//add gravity
			this->addForce(vec2f(0, -9.8));
		}
		~Fluid2()
		{

		}
		int indx(int i, int j)
		{
			return j* size.x + i;
		}

		void setBoundaries(int xyx0y0ds)
		{
			switch(xyx0y0ds)
			{
			case 0:
				for (int i = 0; i < size.x; i++)
				{
					this->vel.at(indx(i, 0)).y= -this->vel.at(indx(i, 1)).y;
					this->vel.at(indx(i, size.y-1)).y = -this->vel.at(indx(i, size.y-2)).y;
				}

				//corners

				this->vel.at(indx(0, 0)).x = (this->vel.at(indx(1, 0)).x + this->vel.at(indx(0, 1)).x) / 2.f;
				this->vel.at(indx(size.x - 1, 0)).x = (this->vel.at(indx(size.x - 2, 0)).x + this->vel.at(indx(size.x - 1, 1)).x) / 2.f;
				this->vel.at(indx(size.x - 1, size.y - 1)).x = (this->vel.at(indx(size.x - 2, size.y - 1)).x + this->vel.at(indx(size.x - 1, size.y - 2)).x) / 2.f;
				this->vel.at(indx(0, size.y - 1)).x = (this->vel.at(indx(1, size.y - 1)).x + this->vel.at(indx(0, size.y - 2)).x) / 2.f;
				break;
			
			case 1:
				for (int j = 0; j < size.y; j++)
				{

					this->vel.at(indx(0, j)).x = -this->vel.at(indx(1, j)).x;
					this->vel.at(indx(size.x - 1, j)).x = -this->vel.at(indx(size.x-2, j)).x;
				}
				this->vel.at(indx(0, 0)).y = (this->vel.at(indx(1, 0)).y + this->vel.at(indx(0, 1)).y) / 2.f;
				this->vel.at(indx(size.x - 1, 0)).y = (this->vel.at(indx(size.x - 2, 0)).y + this->vel.at(indx(size.x - 1, 1)).y) / 2.f;
				this->vel.at(indx(size.x - 1, size.y - 1)).y = (this->vel.at(indx(size.x - 2, size.y - 1)).y + this->vel.at(indx(size.x - 1, size.y - 2)).y) / 2.f;
				this->vel.at(indx(0, size.y - 1)).y = (this->vel.at(indx(1, size.y - 1)).y + this->vel.at(indx(0, size.y - 2)).y) / 2.f;

				break;
			case 2:
				for (int i = 0; i < size.x; i++)
				{
					this->vel0.at(indx(i, 0)).y = -this->vel0.at(indx(i, 1)).y;
					this->vel0.at(indx(i, size.y - 1)).y = -this->vel0.at(indx(i, size.y - 2)).y;
				}
				break;
			case 3:
				for (int j = 0; j < size.y; j++)
				{

					this->vel0.at(indx(0, j)).x = -this->vel0.at(indx(1, j)).x;
					this->vel0.at(indx(size.x - 1, j)).x = -this->vel0.at(indx(size.x - 2, j)).x;
				}
				break;
			case 4:
				for (int i = 0; i < size.x; i++)
				{
					this->vel.at(indx(i, 0)).y = -this->vel.at(indx(i, 1)).y;
					this->vel.at(indx(i, size.y - 1)).y = -this->vel.at(indx(i, size.y - 2)).y;
				}
				break;
			case 5:
				break;

			}

		}

		void addDensity(int i, int j, float amount)
		{
			this->density.at(indx(i,j)) += amount;
		}
		void addForce(vec2f f)
		{
			this->forces.push_back(f);
		}
		void addVelocity(int i, int j, vec2f v)
		{
			this->vel.at(indx(i, j)) += v;
		}
		void updateForces(const float &dt)
		{

			for (int i = 0; i < size.x; i++)
				for (int j = 0; j < size.y; j++)
				{
					int indx = j * size.x + i;
					for (auto e : forces)
						this->vel[indx] += e * dt;
				}
		}
		void update(const float &dt)
		{
			this->updateForces(dt);
			
			float funnyconstant = 1.f/(1.f+6*dt*viscosity);
			//diffusion
			for(int i = 1; i < size.x-1; i++)
				for (int j = 1; j < size.y-1; j++)
				{
					this->vel.at(indx(i, j)) = (this->vel0.at(indx(i,j))
						+ (   this->vel.at(indx(i-1, j))
							+ this->vel.at(indx(i+1, j))
							+ this->vel.at(indx(i, j-1))
							+ this->vel.at(indx(i, j+1))
							)*dt*viscosity
					) * funnyconstant;
				}
			this->setBoundaries(0);
			this->setBoundaries(1);

			//projection

			//advection

			for (int i = 1; i < size.x-1; i++)
				for (int j = 1; j < size.y - 1; j++)
				{
					int k = indx(i, j);
					float x = i - dt * this->vel.at(k).x,
						  y = j - dt * this->vel.at(k).y;

					if (x < 0.5f) x = 0.5f;
					if (x > size.x + 0.5f) x = size.x + 0.5f;

					int i0 = floor(x), j0 = floor(y);
					int i1 = ceil(x), j1 = ceil(y);

					float wx0, wx1 = x-i0, wy0, wy1 = x-j0;
					wx0 = 1 - wx1;
					wy0 = 1 - wy1;
					this->vel.at(k)= (vel0.at(indx(i0,j0))* wy0 + vel0.at(indx(i0,j1)) * wy1) * wx0 + (vel0.at(indx(i1, j0)) * wy0 + vel0.at(indx(i1, j1)) * wy1)* wx1;
				}
			this->setBoundaries(0);

			this->setBoundaries(1);
			//projection
			
			
			//diffusion
			funnyconstant = 1.f / (1.f + 6 * dt * diff);
			for (int i = 1; i < size.x - 1; i++)
				for (int j = 1; j < size.y - 1; j++)
				{
					this->s.at(indx(i, j)) = (this->density.at(indx(i, j))
						+ (this->s.at(indx(i - 1, j))
							+ this->s.at(indx(i + 1, j))
							+ this->s.at(indx(i, j - 1))
							+ this->s.at(indx(i, j + 1))
							) * dt * viscosity
						) * funnyconstant;
				}
			this->setBoundaries(0);
			this->setBoundaries(1);


			//advection

			for (int i = 1; i < size.x - 1; i++)
				for (int j = 1; j < size.y - 1; j++)
				{
					int k = indx(i, j);
					float x = i - dt * this->density.at(k),
						y = j - dt * this->density.at(k);

					if (x < 0.5f) x = 0.5f;
					if (x > size.x + 0.5f) x = size.x + 0.5f;

					int i0 = floor(x), j0 = floor(y);
					int i1 = ceil(x), j1 = ceil(y);

					float wx0, wx1 = x - i0, wy0, wy1 = x - j0;
					wx0 = 1 - wx1;
					wy0 = 1 - wy1;
					this->density.at(k) = (s.at(indx(i0, j0)) * wy0 + s.at(indx(i0, j1)) * wy1) * wx0 + (s.at(indx(i1, j0)) * wy0 + s.at(indx(i1, j1)) * wy1) * wx1;
				}
			this->setBoundaries(0);

			this->setBoundaries(1);
		}
		unsigned int getPixel(int x, int y)
		{
			if (x > size.x || x < 0.f || y > size.y || y < 0.f)
				return -1;
			return this->col.at(indx(x, y)) << 4;
		}
	};

	//3d case
	/*
	class Fluid3 {
	private:
		//flattened array for 3d velocity components field at each point
		std::vector<float> Vx, Vy, Vz;
		// initial velocity
		std::vector<float> Vx0, Vy0, Vz0; 
		
		//for diffirentiating between substances
		std::vector<unsigned short> colorMap;
		//density field
		std::vector<float> density;
		//pressure field
		std::vector<float> pressure;

		double viscosity;
		//Dissipation Rate
		double aS;
		//Diffusion Constant
		double kS;
		int width, height, depth;
		//False for periodic boundaries, True for blocking boundaires for each axis 
		bool boundary[3];

		//forces to add to the fluids
		std::vector<vec3d> forces;

		//function to simplify indexation in one way so to not confuse the order
		//-1 for safety 
		inline int index(int i,int  j, int k) {
			if (i < 0 || i >= width || j < 0 || j >= height || k < 0 || k >= depth)
				return -1;
			return k + j*depth + i*height*depth;
		}

		//allocating memory for the grids given the width, height and depth and set it to a defined value?
		void constructGrid() {
			int size = width * height * depth;
			this->Vx.resize(size, 0.f);
			this->Vy.resize(size, 0.f);
			this->Vz.resize(size, 0.f);
			this->Vx0.resize(size, 0.f);
			this->Vy0.resize(size, 0.f);
			this->Vz0.resize(size, 0.f);

			this->colorMap.resize(size, 0);// 1 unsigned short = 2*1 bytes => width*height*depth*2 bytes
			this->density.resize(size, 0.f); // => width*height*depth*8 bytes
			this->pressure.resize(size, 0.f); //=> width*height*depth*8 bytes
		}

		//boundary function
		void set_bnd(int b, std::vector<float>& x, int N) {
			// 1. Set the 6 Faces of the cube
			for (int j = 1; j <= N; j++) {
				for (int i = 1; i <= N; i++) {
					// Front and Back faces (Z axis)
					x[index(i, j, 0)] = b == 3 ? -x[index(i, j, 1)] : x[index(i, j, 1)];
					x[index(i, j, N + 1)] = b == 3 ? -x[index(i, j, N)] : x[index(i, j, N)];
					// Top and Bottom faces (Y axis)
					x[index(i, 0, j)] = b == 2 ? -x[index(i, 1, j)] : x[index(i, 1, j)];
					x[index(i, N + 1, j)] = b == 2 ? -x[index(i, N, j)] : x[index(i, N, j)];

					// Left and Right faces (X axis)
					x[index(0, i, j)] = b == 1 ? -x[index(1, i, j)] : x[index(1, i, j)];
					x[index(N + 1, i, j)] = b == 1 ? -x[index(N, i, j)] : x[index(N, i, j)];
				}
			}

			// 2. Set the 12 Edges (averaging the two adjacent faces)
			for (int i = 1; i <= N; i++) {
				x[index(0, i, 0)] = 0.5f * (x[index(1, i, 0)] + x[index(0, i, 1)]);
				x[index(N + 1, i, 0)] = 0.5f * (x[index(N, i, 0)] + x[index(N + 1, i, 1)]);
				x[index(0, i, N + 1)] = 0.5f * (x[index(1, i, N + 1)] + x[index(0, i, N)]);
				x[index(N + 1, i, N + 1)] = 0.5f * (x[index(N, i, N + 1)] + x[index(N + 1, i, N)]);

				x[index(0, 0, i)] = 0.5f * (x[index(1, 0, i)] + x[index(0, 1, i)]);
				x[index(N + 1, 0, i)] = 0.5f * (x[index(N, 0, i)] + x[index(N + 1, 1, i)]);
				x[index(0, N + 1, i)] = 0.5f * (x[index(1, N + 1, i)] + x[index(0, N, i)]);
				x[index(N + 1, N + 1, i)] = 0.5f * (x[index(N, N + 1, i)] + x[index(N + 1, N, i)]);

				x[index(i, 0, 0)] = 0.5f * (x[index(i, 1, 0)] + x[index(i, 0, 1)]);
				x[index(i, N + 1, 0)] = 0.5f * (x[index(i, N, 0)] + x[index(i, N + 1, 1)]);
				x[index(i, 0, N + 1)] = 0.5f * (x[index(i, 1, N + 1)] + x[index(i, 0, N)]);
				x[index(i, N + 1, N + 1)] = 0.5f * (x[index(i, N, N + 1)] + x[index(i, N + 1, N)]);
			}

			// 3. Set the 8 Corners (averaging the three adjacent edges/faces)
			x[index(0, 0, 0)] = 0.333f * (x[index(1, 0, 0)] + x[index(0, 1, 0)] + x[index(0, 0, 1)]);
			x[index(N + 1, 0, 0)] = 0.333f * (x[index(N, 0, 0)] + x[index(N + 1, 1, 0)] + x[index(N + 1, 0, 1)]);
			x[index(0, N + 1, 0)] = 0.333f * (x[index(1, N + 1, 0)] + x[index(0, N, 0)] + x[index(0, N + 1, 1)]);
			x[index(N + 1, N + 1, 0)] = 0.333f * (x[index(N, N + 1, 0)] + x[index(N + 1, N, 0)] + x[index(N + 1, N + 1, 1)]);

			x[index(0, 0, N + 1)] = 0.333f * (x[index(1, 0, N + 1)] + x[index(0, 1, N + 1)] + x[index(0, 0, N)]);
			x[index(N + 1, 0, N + 1)] = 0.333f * (x[index(N, 0, N + 1)] + x[index(N + 1, 1, N + 1)] + x[index(N + 1, 0, N)]);
			x[index(0, N + 1, N + 1)] = 0.333f * (x[index(1, N + 1, N + 1)] + x[index(0, N, N + 1)] + x[index(0, N + 1, N)]);
			x[index(N + 1, N + 1, N + 1)] = 0.333f * (x[index(N, N + 1, N + 1)] + x[index(N + 1, N, N + 1)] + x[index(N + 1, N + 1, N)]);
		}
	public:
		Fluid3(int width, int height, int depth, double viscosity)
			: width(width), height(height), depth(depth), viscosity(viscosity)
		{
			this->boundary[0] = 1;
			this->boundary[1] = 1;
			this->boundary[2] = 1;
			
			this->constructGrid();

		}
		virtual ~Fluid3() {

		}

		void lin_solve(int b, std::vector<float>& x, std::vector<float>& x0, float a, float c, int iter, int N) {
			float cRecip = 1.0f / c;
			for (int k = 0; k < iter; k++) {
				for (int m = 1; m <= N; m++) {
					for (int j = 1; j <= N; j++) {
						for (int i = 1; i <= N; i++) {
							x[index(i, j, m)] =
								(x0[index(i, j, m)]
									+ a * (x[index(i + 1, j, m)]
										+ x[index(i - 1, j, m)]
										+ x[index(i, j + 1, m)]
										+ x[index(i, j - 1, m)]
										+ x[index(i, j, m + 1)]
										+ x[index(i, j, m - 1)]
										)) * cRecip;
						}
					}
				}
				set_bnd(b, x, N); // Enforces boundary conditions
			}
		}

		void project(int M, int N, int O, float* u, float* v, float* w, float* p, float* div)
		{
			int i, j, k;

			for (i = 1; i <= M; i++) {
				for (j = 1; j <= N; j++) {
					for (k = 1; k <= O; k++) {
						div[index(i, j, k)] = -1.0 / 3.0 * ((u[index(i + 1, j, k)] - u[index(i - 1, j, k)]) / M + (v[index(i, j + 1, k)] - v[index(i, j - 1, k)]) / M + (w[index(i, j, k + 1)] - w[index(i, j, k - 1)]) / M);
						p[index(i, j, k)] = 0;
					}
				}
			}

			set_bnd(M, N, O, 0, div); set_bnd(M, N, O, 0, p);

			lin_solve(M, N, O, 0, p, div, 1, 6);

			for (i = 1; i <= M; i++) {
				for (j = 1; j <= N; j++) {
					for (k = 1; k <= O; k++) {
						u[IX(i, j, k)] -= 0.5f * M * (p[IX(i + 1, j, k)] - p[IX(i - 1, j, k)]);
						v[IX(i, j, k)] -= 0.5f * M * (p[IX(i, j + 1, k)] - p[IX(i, j - 1, k)]);
						w[IX(i, j, k)] -= 0.5f * M * (p[IX(i, j, k + 1)] - p[IX(i, j, k - 1)]);
					}
				}
			}

			set_bnd(M, N, O, 1, u); set_bnd(M, N, O, 2, v); set_bnd(M, N, O, 3, w);
		}
		void vel_step(int M, int N, int O, float* u, float* v, float* w, float* u0, float* v0, float* w0, float visc, float dt)
		{
			add_source(M, N, O, u, u0, dt); add_source(M, N, O, v, v0, dt); add_source(M, N, O, w, w0, dt);
			SWAP(u0, u); diffuse(M, N, O, 1, u, u0, visc, dt);
			SWAP(v0, v); diffuse(M, N, O, 2, v, v0, visc, dt);
			SWAP(w0, w); diffuse(M, N, O, 3, w, w0, visc, dt);
			project(M, N, O, u, v, w, u0, v0);
			SWAP(u0, u); SWAP(v0, v); SWAP(w0, w);
			advect(M, N, O, 1, u, u0, u0, v0, w0, dt); advect(M, N, O, 2, v, v0, u0, v0, w0, dt); advect(M, N, O, 3, w, w0, u0, v0, w0, dt);
			project(M, N, O, u, v, w, u0, v0);
		}

		void step(const float& dt);

		void addForce(vec3d force);


	};*/

	class Fluid3 {
	private:
		size_t width, height, depth;
		float dt;       // Time step
		float diff;     // Diffusion rate
		float visc;     // Viscosity

		// Scalar fields
		std::vector<float> density;
		std::vector<float> density_prev;

		// Vector velocity fields (x, y, z components)
		std::vector<float> Vx, Vy, Vz;
		std::vector<float> Vx0, Vy0, Vz0;

		inline size_t IX(size_t x, size_t y, size_t z) const {
			return x + y * width + z * width * height;
		}

		// Boundary conditions handler
		void set_bnd(int b, std::vector<float>& x) {
			// Enforce solid wall boundary conditions on outer shell
			for (size_t j = 1; j < height - 1; ++j) {
				for (size_t i = 1; i < width - 1; ++i) {
					x[IX(i, j, 0)] = b == 3 ? -x[IX(i, j, 1)] : x[IX(i, j, 1)];
					x[IX(i, j, depth - 1)] = b == 3 ? -x[IX(i, j, depth - 2)] : x[IX(i, j, depth - 2)];
				}
			}
			for (size_t k = 1; k < depth - 1; ++k) {
				for (size_t i = 1; i < width - 1; ++i) {
					x[IX(i, 0, k)] = b == 2 ? -x[IX(i, 1, k)] : x[IX(i, 1, k)];
					x[IX(i, height - 1, k)] = b == 2 ? -x[IX(i, height - 2, k)] : x[IX(i, height - 2, k)];
				}
			}
			for (size_t k = 1; k < depth - 1; ++k) {
				for (size_t j = 1; j < height - 1; ++j) {
					x[IX(0, j, k)] = b == 1 ? -x[IX(1, j, k)] : x[IX(1, j, k)];
					x[IX(width - 1, j, k)] = b == 1 ? -x[IX(width - 2, j, k)] : x[IX(width - 2, j, k)];
				}
			}
		}

		// Linear solver for diffusion & pressure Poisson equation (Gauss-Seidel relaxation)
		void lin_solve(int b, std::vector<float>& x, const std::vector<float>& x0, float a, float c, int iter = 4) {
			float cRecip = 1.0f / c;
			for (int k_iter = 0; k_iter < iter; ++k_iter) {
				for (size_t k = 1; k < depth - 1; ++k) {
					for (size_t j = 1; j < height - 1; ++j) {
						for (size_t i = 1; i < width - 1; ++i) {
							x[IX(i, j, k)] = (x0[IX(i, j, k)] +
								a * (x[IX(i + 1, j, k)] + x[IX(i - 1, j, k)] +
									x[IX(i, j + 1, k)] + x[IX(i, j - 1, k)] +
									x[IX(i, j, k + 1)] + x[IX(i, j, k - 1)])) * cRecip;
						}
					}
				}
				set_bnd(b, x);
			}
		}

		// Diffuse scalar or velocity fields
		void diffuse(int b, std::vector<float>& x, const std::vector<float>& x0, float diff_rate) {
			float a = dt * diff_rate * (width - 2) * (height - 2) * (depth - 2);
			lin_solve(b, x, x0, a, 1 + 6 * a);
		}

		// Project velocity field to ensure incompressibility (zero divergence)
		void project(std::vector<float>& vx, std::vector<float>& vy, std::vector<float>& vz,
			std::vector<float>& p, std::vector<float>& div) {
			for (size_t k = 1; k < depth - 1; ++k) {
				for (size_t j = 1; j < height - 1; ++j) {
					for (size_t i = 1; i < width - 1; ++i) {
						div[IX(i, j, k)] = -0.5f * (
							vx[IX(i + 1, j, k)] - vx[IX(i - 1, j, k)] +
							vy[IX(i, j + 1, k)] - vy[IX(i, j - 1, k)] +
							vz[IX(i, j, k + 1)] - vz[IX(i, j, k - 1)]
							) / max(width, max(height, depth));
						p[IX(i, j, k)] = 0;
					}
				}
			}
			set_bnd(0, div);
			set_bnd(0, p);
			lin_solve(0, p, div, 1, 6);

			for (size_t k = 1; k < depth - 1; ++k) {
				for (size_t j = 1; j < height - 1; ++j) {
					for (size_t i = 1; i < width - 1; ++i) {
						vx[IX(i, j, k)] -= 0.5f * (p[IX(i + 1, j, k)] - p[IX(i - 1, j, k)]) * width;
						vy[IX(i, j, k)] -= 0.5f * (p[IX(i, j + 1, k)] - p[IX(i, j - 1, k)]) * height;
						vz[IX(i, j, k)] -= 0.5f * (p[IX(i, j, k + 1)] - p[IX(i, j, k - 1)]) * depth;
					}
				}
			}
			set_bnd(1, vx);
			set_bnd(2, vy);
			set_bnd(3, vz);
		}

		// Advection step using back-tracing (semi-Lagrangian method)
		void advect(int b, std::vector<float>& d, const std::vector<float>& d0,
			const std::vector<float>& vx, const std::vector<float>& vy, const std::vector<float>& vz) {
			float dt0 = dt * (width - 2);

			for (size_t k = 1; k < depth - 1; ++k) {
				for (size_t j = 1; j < height - 1; ++j) {
					for (size_t i = 1; i < width - 1; ++i) {
						float x = i - dt0 * vx[IX(i, j, k)];
						float y = j - dt0 * vy[IX(i, j, k)];
						float z = k - dt0 * vz[IX(i, j, k)];

						// Clamp to grid interior
						x = std::clamp(x, 0.5f, static_cast<float>(width) - 1.5f);
						y = std::clamp(y, 0.5f, static_cast<float>(height) - 1.5f);
						z = std::clamp(z, 0.5f, static_cast<float>(depth) - 1.5f);

						size_t i0 = static_cast<size_t>(x), i1 = i0 + 1;
						size_t j0 = static_cast<size_t>(y), j1 = j0 + 1;
						size_t k0 = static_cast<size_t>(z), k1 = k0 + 1;

						float s1 = x - i0, s0 = 1.0f - s1;
						float t1 = y - j0, t0 = 1.0f - t1;
						float u1 = z - k0, u0 = 1.0f - u1;

						// Trilinear interpolation
						d[IX(i, j, k)] =
							u0 * (t0 * (s0 * d0[IX(i0, j0, k0)] + s1 * d0[IX(i1, j0, k0)]) +
								t1 * (s0 * d0[IX(i0, j1, k0)] + s1 * d0[IX(i1, j1, k0)])) +
							u1 * (t0 * (s0 * d0[IX(i0, j0, k1)] + s1 * d0[IX(i1, j0, k1)]) +
								t1 * (s0 * d0[IX(i0, j1, k1)] + s1 * d0[IX(i1, j1, k1)]));
					}
				}
			}
			set_bnd(b, d);
		}

	public:
		Fluid3(size_t w, size_t h, size_t d, float time_step, float diffusion, float viscosity)
			: width(w), height(h), depth(d), dt(time_step), diff(diffusion), visc(viscosity),
			density(w* h* d, 0.0f), density_prev(w* h* d, 0.0f),
			Vx(w* h* d, 0.0f), Vy(w* h* d, 0.0f), Vz(w* h* d, 0.0f),
			Vx0(w* h* d, 0.0f), Vy0(w* h* d, 0.0f), Vz0(w* h* d, 0.0f) {
		}

		// Add source values to a grid cell
		void addDensity(size_t x, size_t y, size_t z, float amount) {
			density[IX(x, y, z)] += amount;
		}

		void addVelocity(size_t x, size_t y, size_t z, float amountX, float amountY, float amountZ) {
			Vx[IX(x, y, z)] += amountX;
			Vy[IX(x, y, z)] += amountY;
			Vz[IX(x, y, z)] += amountZ;
		}

		// Step the physics world forward
		void step() {
			// Velocity step
			diffuse(1, Vx0, Vx, visc);
			diffuse(2, Vy0, Vy, visc);
			diffuse(3, Vz0, Vz, visc);

			project(Vx0, Vy0, Vz0, Vx, Vy);

			advect(1, Vx, Vx0, Vx0, Vy0, Vz0);
			advect(2, Vy, Vy0, Vx0, Vy0, Vz0);
			advect(3, Vz, Vz0, Vx0, Vy0, Vz0);

			project(Vx, Vy, Vz, Vx0, Vy0);

			// Density step
			diffuse(0, density_prev, density, diff);
			advect(0, density, density_prev, Vx, Vy, Vz);
		}

		float getDensity(size_t x, size_t y, size_t z) const {
			return density[IX(x, y, z)];
		}

		size_t getWidth() const { return width; }
		size_t getHeight() const { return height; }
		size_t getDepth() const { return depth; }
	};
};
