#pragma once 
#include <windows.h>
#include <windows.h>
#include <d3d9.h>         
#include <d3d11.h>        
#include <dxva2api.h>      
#include <initguid.h> 
#include <corecrt_math_defines.h>
#include <xmmintrin.h>
#include <immintrin.h>
#include <mutex>
#include <shared_mutex>
#include <algorithm>
#include <numbers>
#include "../../../thirdparty/kernel/comms.hpp"
#include "../../../thirdparty/imgui/imgui.h"

namespace uemath {

	struct flinearcolor {
		float r, g, b, a;

		flinearcolor() : r(), g(), b(), a() {}
		flinearcolor(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
		flinearcolor(const ImVec4& vec) : r(vec.x), g(vec.y), b(vec.z), a(vec.w) {}
	};

	template < class type >
	class tarray
	{
	public:
		tarray() : data(), count(), max_count() {}
		tarray(type* data, std::uint32_t count, std::uint32_t max_count) :
			data(data), count(count), max_count(max_count) {
		}

		__forceinline type get(std::uintptr_t idx)
		{

			return exc->read < type >(
				std::bit_cast <std::uintptr_t> (this->data) + (idx * sizeof(type))
			);
		}

		__forceinline std::vector < type > get_itter()
		{


			if (this->count > this->max_count)
				return { };

			try {
				std::vector < type > buffer(this->count);

				exc->read_array(reinterpret_cast <std::uintptr_t> (this->data), buffer.data(), sizeof(type) * this->count);

				return buffer;
			}
			catch (std::bad_alloc& e) {
				return { };
			}
		}

		__forceinline type operator[](std::uintptr_t idx) const
		{

			return exc->read<type>(std::bit_cast<std::uintptr_t>(this->data) + (idx * sizeof(type)));
		}

		__forceinline std::uintptr_t get_addr()
		{
			return reinterpret_cast <std::uintptr_t> (this->data);
		}

		__forceinline std::uint32_t size() const
		{
			return this->count;
		};

		__forceinline std::uint32_t max_size() const
		{

			return this->max_count;
		};

		__forceinline bool is_valid_index(std::int32_t index) const
		{
			return this->index >= 0 && index < count;
		};

		__forceinline bool is_valid() const
		{
			return this->data != nullptr;
		};

		type* data;
		std::uint32_t count;
		std::uint32_t max_count;
	};


	inline auto to_multiplication(D3DMATRIX matrix_one, D3DMATRIX matrix_two) -> D3DMATRIX {

		D3DMATRIX result{ };

		for (int row = 0; row < 4; ++row) {
			for (int col = 0; col < 4; ++col) {
				result.m[row][col] = 0.0;
				for (int i = 0; i < 4; ++i) {
					result.m[row][col] += matrix_one.m[row][i] * matrix_two.m[i][col];
				}
			}
		}

		return result;
	}



	class f_vector {
	public:

		f_vector() : x(0.f), y(0.f), z(0.f) {}

		f_vector(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

		~f_vector() {}

		double x;
		double y;
		double z;

		inline double dot(f_vector v) {
			return x * v.x + y * v.y + z * v.z;
		}

		double distance_to(f_vector v) const { return sqrt(pow(v.x - x, 2.0) + pow(v.y - y, 2.0) + pow(v.z - z, 2.0)) / 100.0; }

		inline double distance(f_vector v) {
			return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
		}

		[[nodiscard]] inline double vector_scalar(const f_vector& v) {
			return x * v.x + y * v.y + z * v.z;
		}

		inline double length() const {
			return sqrt(x * x + y * y + z * z);
		}

		f_vector operator + (f_vector v) {
			return f_vector(x + v.x, y + v.y, z + v.z);
		}

		f_vector operator - (f_vector v) {
			return f_vector(x - v.x, y - v.y, z - v.z);
		}

		f_vector operator-(const f_vector& other) const {
			return { x - other.x, y - other.y, z - other.z };
		}


		f_vector operator * (double number) const {
			return f_vector(x * number, y * number, z * number);
		}

		f_vector operator / (double number) const {
			return f_vector(x / number, y / number, z / number);
		}

		f_vector& operator *= (double number) {
			x *= number;
			y *= number;
			z *= number;
			return *this;
		}

		void add_scale(const f_vector& v, float scale) {
			x += v.x * scale;
			y += v.y * scale;
			z += v.z * scale;
		}

		bool is_zero() {
			return x == 0 && y == 0 && z == 0;
		}

		f_vector normalize() const {
			double len = length();
			if (len == 0) return f_vector(0, 0, 0);
			return f_vector(x / len, y / len, z / len);
		}
	};

	class f_vector2d {
	public:
		double x, y;

		bool is_valid() const { return this->x && this->y; }

		bool in_screen() const {
			if (this->x < 1920 && this->y < 1080)
				return true;
			return false;
		}

		f_vector2d(double x = 0.0, double y = 0.0) : x(x), y(y) {}

		f_vector2d operator + (const f_vector2d& v) const {
			return f_vector2d(x + v.x, y + v.y);
		}

		f_vector2d operator - (const f_vector2d& v) const {
			return f_vector2d(x - v.x, y - v.y);
		}

		f_vector2d operator * (double scalar) const {
			return f_vector2d(x * scalar, y * scalar);
		}

		bool is_zero() const {
			return x == 0.0 && y == 0.0;
		}
	};

	class f_rotator {
	public:
		f_rotator() : pitch(0), yaw(0), roll(0) {}
		f_rotator(double Pitch, double Yaw, double Roll) : pitch(Pitch), yaw(Yaw), roll(Roll) {}

		f_rotator operator + (const f_rotator& other) const { return { this->pitch + other.pitch, this->yaw + other.yaw, this->roll + other.roll }; }
		f_rotator operator - (const f_rotator& other) const { return { this->pitch - other.pitch, this->yaw - other.yaw, this->roll - other.roll }; }
		f_rotator operator * (double offset) const { return { this->pitch * offset, this->yaw * offset, this->roll * offset }; }
		f_rotator operator / (double offset) const { return { this->pitch / offset, this->yaw / offset, this->roll / offset }; }

		f_rotator& operator = (const double other) { this->pitch = other; this->yaw = other; this->roll = other; return *this; }
		f_rotator& operator *= (const double other) { this->pitch *= other; this->yaw *= other; this->roll *= other; return *this; }
		f_rotator& operator /= (const double other) { this->pitch /= other; this->yaw /= other; this->roll /= other; return *this; }

		f_rotator& operator = (const f_rotator& other) { this->pitch = other.pitch; this->yaw = other.yaw; this->roll = other.roll; return *this; }
		f_rotator& operator += (const f_rotator& other) { this->pitch += other.pitch; this->yaw += other.yaw; this->roll += other.roll; return *this; }
		f_rotator& operator -= (const f_rotator& other) { this->pitch -= other.pitch; this->yaw -= other.yaw; this->roll -= other.roll; return *this; }
		f_rotator& operator /= (const f_rotator& other) { this->pitch /= other.pitch; this->yaw /= other.yaw; this->roll /= other.roll; return *this; }

		operator bool() const { return this->pitch != 0 && this->yaw != 0 && this->roll != 0; }

		friend bool operator == (const f_rotator& a, const f_rotator& b) { return a.pitch == b.pitch && a.yaw == b.yaw && a.roll == b.roll; }
		friend bool operator != (const f_rotator& a, const f_rotator& b) { return !(a == b); }

		f_rotator get() const {
			return f_rotator(pitch, yaw, roll);
		}

		void set(double _Pitch, double _Yaw, double _Roll) {
			pitch = _Pitch;
			yaw = _Yaw;
			roll = _Roll;
		}

		f_rotator normalize() const {
			f_rotator result = get();

			if (std::isfinite(result.pitch) && std::isfinite(result.yaw) && std::isfinite(result.roll)) {
				result.pitch = std::clamp(result.pitch, -89.0, 89.0);
				result.yaw = std::clamp(result.yaw, -180.0, 180.0);
				result.roll = 0.0;
			}

			return result;
		}

		double length() const {

			return std::sqrt(pitch * pitch + yaw * yaw + roll * roll);
		}

		double dot(const f_rotator& V) const {

			return pitch * V.pitch + yaw * V.yaw + roll * V.roll;
		}

		double distance(const f_rotator& V) const {

			return std::sqrt(std::pow(V.pitch - this->pitch, 2.0) + std::pow(V.yaw - this->yaw, 2.0) + std::pow(V.roll - this->roll, 2.0));
		}

		double pitch;
		double yaw;
		double roll;
	};


	struct fplane : f_vector {
		double w = 0;

		fplane() : f_vector(), w(0.0) {}
		fplane(double X, double Y, double Z, double W) : f_vector(X, Y, Z), w(W) {}
	};

	struct ftransform {
		fplane rotation;
		f_vector translation;
		std::uint8_t pad[8];
		f_vector scale;
		std::uint8_t pad2[8];

		auto to_matrix() const -> D3DMATRIX {


			f_vector scale_3d
			(
				(scale.x == 0.0) ? 1.0 : scale.x,
				(scale.y == 0.0) ? 1.0 : scale.y,
				(scale.z == 0.0) ? 1.0 : scale.z
			);
			double x2 = rotation.x + rotation.x;
			double y2 = rotation.y + rotation.y;
			double z2 = rotation.z + rotation.z;
			double xx2 = rotation.x * x2;
			double yy2 = rotation.y * y2;
			double zz2 = rotation.z * z2;
			double yz2 = rotation.y * z2;
			double wx2 = rotation.w * x2;
			double xy2 = rotation.x * y2;
			double wz2 = rotation.w * z2;
			double xz2 = rotation.x * z2;
			double wy2 = rotation.w * y2;

			D3DMATRIX matrix;

			matrix._41 = translation.x;
			matrix._42 = translation.y;
			matrix._43 = translation.z;

			matrix._11 = (1.0 - (yy2 + zz2)) * scale_3d.x;
			matrix._22 = (1.0 - (xx2 + zz2)) * scale_3d.y;
			matrix._33 = (1.0 - (xx2 + yy2)) * scale_3d.z;

			matrix._32 = (yz2 - wx2) * scale_3d.z;
			matrix._23 = (yz2 + wx2) * scale_3d.y;

			matrix._21 = (xy2 - wz2) * scale_3d.y;
			matrix._12 = (xy2 + wz2) * scale_3d.x;

			matrix._31 = (xz2 + wy2) * scale_3d.z;
			matrix._13 = (xz2 - wy2) * scale_3d.x;

			matrix._14 = 0.0f;
			matrix._24 = 0.0f;
			matrix._34 = 0.0f;
			matrix._44 = 1.0f;

			return matrix;
		}
	};


	class cam_data {
	public:
		f_rotator rotation{ };
		f_vector location{ };

		float field_of_view;
	}; inline cam_data camera;

#define m_pi 3.14159265358979323846f

	struct alignas (16) matrix_elements {
		double m11, m12, m13, m14;
		double m21, m22, m23, m24;
		double m31, m32, m33, m34;
		double m41, m42, m43, m44;

		matrix_elements() : m11(0), m12(0), m13(0), m14(0),
			m21(0), m22(0), m23(0), m24(0),
			m31(0), m32(0), m33(0), m34(0),
			m41(0), m42(0), m43(0), m44(0) {
		}
	};

	struct alignas (16) matrix {
		union {
			matrix_elements elements;
			double m[4][4];
		};

		matrix() : elements() {}

		double& operator () (std::size_t row, std::size_t col) { return m[row][col]; }
		const double& operator () (std::size_t row, std::size_t col) const { return m[row][col]; }
	};

	struct alignas (16) fmatrix : public matrix {
		fplane x_plane;
		fplane y_plane;
		fplane z_plane;
		fplane w_plane;

		fmatrix() : matrix(), x_plane(), y_plane(), z_plane(), w_plane() {}
	};


	inline static auto rotation_matrix(uemath::f_rotator rotation) -> D3DMATRIX {
		auto rad_pitch = (rotation.pitch * float(std::numbers::pi) / 180.f);
		auto rad_yaw = (rotation.yaw * float(std::numbers::pi) / 180.f);
		auto rad_roll = (rotation.roll * float(std::numbers::pi) / 180.f);

		auto sp = sinf(rad_pitch);
		auto cp = cosf(rad_pitch);
		auto sy = sinf(rad_yaw);
		auto cy = cosf(rad_yaw);
		auto sr = sinf(rad_roll);
		auto cr = cosf(rad_roll);

		D3DMATRIX matrix;
		matrix.m[0][0] = cp * cy;
		matrix.m[0][1] = cp * sy;
		matrix.m[0][2] = sp;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = sr * sp * cy - cr * sy;
		matrix.m[1][1] = sr * sp * sy + cr * cy;
		matrix.m[1][2] = -sr * cp;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(cr * sp * cy + sr * sy);
		matrix.m[2][1] = cy * sr - cr * sp * sy;
		matrix.m[2][2] = cr * cp;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = 0.0f;
		matrix.m[3][1] = 0.0f;
		matrix.m[3][2] = 0.0f;
		matrix.m[3][3] = 1.f;

		return matrix;
	}
}

namespace d3dmatrix
{
	struct FTransform
	{
		uemath::fplane rot;
		uemath::f_vector translation;
		char    pad[8];
		uemath::f_vector scale;

		inline D3DMATRIX ToMatrixWithScale()
		{
			uemath::f_vector safe_scale(
				(this->scale.x == 0.0f) ? 1.0f : this->scale.x,
				(this->scale.y == 0.0f) ? 1.0f : this->scale.y,
				(this->scale.z == 0.0f) ? 1.0f : this->scale.z
			);

			D3DMATRIX m;
			m._41 = translation.x;
			m._42 = translation.y;
			m._43 = translation.z;

			double x2 = rot.x + rot.x;
			double y2 = rot.y + rot.y;
			double z2 = rot.z + rot.z;

			double xx2 = rot.x * x2;
			double yy2 = rot.y * y2;
			double zz2 = rot.z * z2;
			m._11 = (1.0 - (yy2 + zz2)) * safe_scale.x;
			m._22 = (1.0 - (xx2 + zz2)) * safe_scale.y;
			m._33 = (1.0 - (xx2 + yy2)) * safe_scale.z;

			double yz2 = rot.y * z2;
			double wx2 = rot.w * x2;
			m._32 = (yz2 - wx2) * safe_scale.z;
			m._23 = (yz2 + wx2) * safe_scale.y;

			double xy2 = rot.x * y2;
			double wz2 = rot.w * z2;
			m._21 = (xy2 - wz2) * safe_scale.y;
			m._12 = (xy2 + wz2) * safe_scale.x;

			double xz2 = rot.x * z2;
			double wy2 = rot.w * y2;
			m._31 = (xz2 + wy2) * safe_scale.z;
			m._13 = (xz2 - wy2) * safe_scale.x;

			m._14 = 0.0f;
			m._24 = 0.0f;
			m._34 = 0.0f;
			m._44 = 1.0f;

			return m;
		}

	};

	inline D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
	{


		D3DMATRIX pOut;
		pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
		pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
		pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
		pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
		pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
		pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
		pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
		pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
		pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
		pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
		pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
		pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
		pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
		pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
		pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
		pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

		return pOut;
	}

	inline D3DMATRIX Matrix(uemath::f_vector rot, uemath::f_vector origin = uemath::f_vector(0, 0, 0))
	{


		float radPitch = (rot.x * float(M_PI) / 180.f);
		float radYaw = (rot.y * float(M_PI) / 180.f);
		float radRoll = (rot.z * float(M_PI) / 180.f);

		float SP = sinf(radPitch);
		float CP = cosf(radPitch);
		float SY = sinf(radYaw);
		float CY = cosf(radYaw);
		float SR = sinf(radRoll);
		float CR = cosf(radRoll);

		D3DMATRIX matrix;
		matrix.m[0][0] = CP * CY;
		matrix.m[0][1] = CP * SY;
		matrix.m[0][2] = SP;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = SR * SP * CY - CR * SY;
		matrix.m[1][1] = SR * SP * SY + CR * CY;
		matrix.m[1][2] = -SR * CP;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(CR * SP * CY + SR * SY);
		matrix.m[2][1] = CY * SR - CR * SP * SY;
		matrix.m[2][2] = CR * CP;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = origin.x;
		matrix.m[3][1] = origin.y;
		matrix.m[3][2] = origin.z;
		matrix.m[3][3] = 1.f;

		return matrix;
	}
}


inline D3DMATRIX MatrixMultiplication1(D3DMATRIX pM1, D3DMATRIX pM2)
{


	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}