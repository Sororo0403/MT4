#define _USE_MATH_DEFINES
#include <Novice.h>
#include <cmath>
#include <cstring>

const char kWindowTitle[] = "MT4 Quaternion Confirm";

// ============================
// Quaternion 構造体
// ============================
struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

// ============================
// Quaternion 関数群
// ============================
Quaternion IdentityQuaternion() {
	return {0.0f, 0.0f, 0.0f, 1.0f};
}

Quaternion Conjugate(const Quaternion &q) {
	return {-q.x, -q.y, -q.z, q.w};
}

float Norm(const Quaternion &q) {
	return std::sqrt(
		q.w * q.w +
		q.x * q.x +
		q.y * q.y +
		q.z * q.z
	);
}

Quaternion Normalize(const Quaternion &q) {
	float n = Norm(q);
	if (n == 0.0f) {
		return IdentityQuaternion();
	}
	return {
		q.x / n,
		q.y / n,
		q.z / n,
		q.w / n
	};
}

Quaternion Inverse(const Quaternion &q) {
	float normSq =
		q.w * q.w +
		q.x * q.x +
		q.y * q.y +
		q.z * q.z;

	if (normSq == 0.0f) {
		return IdentityQuaternion();
	}

	Quaternion c = Conjugate(q);
	return {
		c.x / normSq,
		c.y / normSq,
		c.z / normSq,
		c.w / normSq
	};
}

Quaternion Multiply(const Quaternion &q, const Quaternion &r) {
	Quaternion result;
	result.w = q.w * r.w - q.x * r.x - q.y * r.y - q.z * r.z;
	result.x = q.w * r.x + q.x * r.w + q.y * r.z - q.z * r.y;
	result.y = q.w * r.y - q.x * r.z + q.y * r.w + q.z * r.x;
	result.z = q.w * r.z + q.x * r.y - q.y * r.x + q.z * r.w;
	return result;
}

// ============================
// エントリーポイント
// ============================
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	Novice::Initialize(kWindowTitle, 1280, 720);

	char keys[256] = {0};
	char preKeys[256] = {0};

	// ----------------------------
	// テスト用Quaternion
	// ----------------------------
	Quaternion q1 = {2.0f, 3.0f, 4.0f, 1.0f};
	Quaternion q2 = {1.0f, 3.0f, 5.0f, 2.0f};

	while (Novice::ProcessMessage() == 0) {

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		Novice::BeginFrame();

		// ----------------------------
		// 計算
		// ----------------------------
		Quaternion identity = IdentityQuaternion();
		Quaternion conj = Conjugate(q1);
		Quaternion inv = Inverse(q1);
		Quaternion normQ = Normalize(q1);
		Quaternion mul1 = Multiply(q1, q2);
		Quaternion mul2 = Multiply(q2, q1);
		float norm = Norm(q1);

		int y = 20;

		auto PrintQ = [&](const char *label, const Quaternion &q) {
			Novice::ScreenPrintf(
				20, y,
				"%s : %.2f %.2f %.2f %.2f",
				label, q.x, q.y, q.z, q.w
			);
			y += 20;
			};

		PrintQ("Identity", identity);
		PrintQ("Conjugate", conj);
		PrintQ("Inverse", inv);
		PrintQ("Normalize", normQ);
		PrintQ("Multiply(q1, q2)", mul1);
		PrintQ("Multiply(q2, q1)", mul2);

		Novice::ScreenPrintf(20, y, "Norm : %.2f", norm);

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
