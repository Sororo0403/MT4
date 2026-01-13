#include <Novice.h>
#include <cmath>
#include <cstring>

// =======================
// 定数
// =======================
constexpr float EPSILON = 1e-6f;

// =======================
// Vector3
// =======================
struct Vector3 {
	float x, y, z;
};

float Dot(const Vector3 &a, const Vector3 &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Cross(const Vector3 &a, const Vector3 &b) {
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

float Length(const Vector3 &v) {
	return std::sqrt(Dot(v, v));
}

Vector3 Normalize(const Vector3 &v) {
	float len = Length(v);
	if (len < EPSILON) return {0, 0, 0};
	return {v.x / len, v.y / len, v.z / len};
}

// =======================
// Matrix4x4
// =======================
struct Matrix4x4 {
	float m[4][4];
};

Matrix4x4 IdentityMatrix() {
	Matrix4x4 mat{};
	for (int i = 0; i < 4; i++) {
		mat.m[i][i] = 1.0f;
	}
	return mat;
}

// =======================
// DirectionToDirection
// =======================
Matrix4x4 DirectionToDirection(const Vector3 &from, const Vector3 &to) {
	Vector3 u = Normalize(from);
	Vector3 v = Normalize(to);

	float cosTheta = Dot(u, v);

	// 同方向
	if (cosTheta > 1.0f - EPSILON) {
		return IdentityMatrix();
	}

	Vector3 axis = Cross(u, v);
	float sinTheta = Length(axis);
	axis = Normalize(axis);

	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	Matrix4x4 mat = IdentityMatrix();

	mat.m[0][0] = x * x * (1 - cosTheta) + cosTheta;
	mat.m[0][1] = x * y * (1 - cosTheta) - z * sinTheta;
	mat.m[0][2] = x * z * (1 - cosTheta) + y * sinTheta;

	mat.m[1][0] = y * x * (1 - cosTheta) + z * sinTheta;
	mat.m[1][1] = y * y * (1 - cosTheta) + cosTheta;
	mat.m[1][2] = y * z * (1 - cosTheta) - x * sinTheta;

	mat.m[2][0] = z * x * (1 - cosTheta) - y * sinTheta;
	mat.m[2][1] = z * y * (1 - cosTheta) + x * sinTheta;
	mat.m[2][2] = z * z * (1 - cosTheta) + cosTheta;

	return mat;
}

// =======================
// 行列表示（ScreenPrintf）
// =======================
void PrintMatrix(int x, int y, const char *name, const Matrix4x4 &mat) {
	Novice::ScreenPrintf(x, y, "%s", name);
	for (int r = 0; r < 4; r++) {
		Novice::ScreenPrintf(
			x, y + 20 + r * 20,
			"%6.3f %6.3f %6.3f %6.3f",
			mat.m[r][0], mat.m[r][1], mat.m[r][2], mat.m[r][3]
		);
	}
}

// =======================
// WinMain
// =======================
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	Novice::Initialize("MT4 ScreenPrintf Matrix", 1280, 720);

	char keys[256] = {};
	char preKeys[256] = {};

	// ===== テストデータ =====
	Vector3 from0 = Normalize({1.0f, 0.0f, 0.0f});
	Vector3 to0 = Normalize({-1.0f, 0.0f, 0.0f});

	Vector3 from1 = Normalize({1.0f, 0.7f, 0.5f});
	Vector3 to1 = Normalize({-0.4f, 0.7f, -0.5f});

	Vector3 from2 = Normalize({0.6f, 0.9f, 0.2f});
	Vector3 to2 = Normalize({0.4f, 0.7f, -0.5f});

	Matrix4x4 mat0 = DirectionToDirection(from0, to0);
	Matrix4x4 mat1 = DirectionToDirection(from1, to1);
	Matrix4x4 mat2 = DirectionToDirection(from2, to2);

	while (Novice::ProcessMessage() == 0) {
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		Novice::BeginFrame();

		// ===== スライドと同じ表示 =====
		PrintMatrix(40, 40, "rotateMatrix0", mat0);
		PrintMatrix(40, 160, "rotateMatrix1", mat1);
		PrintMatrix(40, 280, "rotateMatrix2", mat2);

		Novice::EndFrame();

		if (keys[DIK_ESCAPE]) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
