#pragma once


#include "rtweekend.h"

class perlin {

public:

	perlin() {
		randvec = new vec3[point_count];

		for (int i = 0; i < point_count; i++) {
			randvec[i] = normalize(vec3::random(-1, 1));
		}

		perm_x = perlin_generate_perm();
		perm_y = perlin_generate_perm();
		perm_z = perlin_generate_perm();
	}

	~perlin() {
		delete[] randvec;
		delete[] perm_x;
		delete[] perm_y;
		delete[] perm_z;
	}

	double noise(const point3& p) const {
		auto u = p.x() - floor(p.x());
		auto v = p.y() - floor(p.y());
		auto w = p.z() - floor(p.z());


		auto i = static_cast<int>(floor(p.x()));
		auto j = static_cast<int>(floor(p.y()));
		auto k = static_cast<int>(floor(p.z()));

		vec3 vector[2][2][2];

		for (int di = 0; di < 2; di++) {
			for (int dj = 0; dj < 2; dj++) {
				for (int dk = 0; dk < 2; dk++) {
					vector[di][dj][dk] = randvec[
							perm_x[(i + di) & 255] ^
							perm_y[(j + dj) & 255] ^
							perm_z[(k + dk) & 255]
					];
				}
			}
		}

		return perlin_trilinear_interp(vector, u, v, w);
	}

	double turb(const point3& p, int depth = 7) const {
		auto accum = 0.0;
		auto temp_p = p;
		auto weight = 1.0;

		for (int i = 0; i < depth; i++) {
			accum += weight * noise(temp_p);
			weight *= 0.5;
			temp_p *= 2;
		}

		return fabs(accum);
	}

private:
	const int point_count = 256;
	vec3* randvec;
	int* perm_x;
	int* perm_y;
	int* perm_z;

	int* perlin_generate_perm() {
		auto p = new int[point_count];
		for (int i = 0; i < point_count; i++) {
			p[i] = i;
		}
		permuate(p, point_count);
		return p;
	}

	void permuate(int* p, int count) {
		for (int i = count - 1; i > 0; i--) {
			int target = random_int(0, i);
			int temp = p[target];
			p[target] = p[i];
			p[i] = temp;
		}
	}

	static double perlin_trilinear_interp(vec3 vec[2][2][2], double u, double v, double w) {
		// hermitian interpolation: to get rid of Mach-Band.
		auto uu = u * u * (3 - 2 * u);
		auto vv = v * v * (3 - 2 * v);
		auto ww = w * w * (3 - 2 * w);

		double accum = 0.0;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					vec3 weight(u - i, v - j, w - k);
					accum += dot(vec[i][j][k], weight) *
						(i * uu + (1 - i) * (1 - uu)) *
						(j * vv + (1 - j) * (1 - vv)) *
						(k * ww + (1 - k) * (1 - ww));
				}
			}
		}
		return accum;
	}

};

