#include <Novice.h>
#include <math.h>

const char kWindowTitle[] = "LE2B_18_ハタナカ_タロウ";

struct Vector3 {
	float x, y, z;
};

struct Matrix4x4 {
	float m[4][4];
};

/// <summary>
/// ベクトルを正規化する
/// </summary>
Vector3 Normalize(const Vector3 &v) {
	float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return {v.x / len, v.y / len, v.z / len};
}

/// <summary>
/// 任意軸まわりの回転行列を作成する
/// </summary>
Matrix4x4 MakeRotateAxisAngle(const Vector3 &axis, float angle) {
	Vector3 n = Normalize(axis);

	float x = n.x;
	float y = n.y;
	float z = n.z;
	float c = cosf(angle);
	float s = sinf(angle);
	float oneMinusC = 1.0f - c;

	Matrix4x4 result{};

	result.m[0][0] = x * x * oneMinusC + c;
	result.m[0][1] = x * y * oneMinusC + z * s;
	result.m[0][2] = x * z * oneMinusC - y * s;
	result.m[0][3] = 0.0f;

	result.m[1][0] = y * x * oneMinusC - z * s;
	result.m[1][1] = y * y * oneMinusC + c;
	result.m[1][2] = y * z * oneMinusC + x * s;
	result.m[1][3] = 0.0f;

	result.m[2][0] = z * x * oneMinusC + y * s;
	result.m[2][1] = z * y * oneMinusC - x * s;
	result.m[2][2] = z * z * oneMinusC + c;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

/// <summary>
/// 行列の内容を画面に表示する
/// </summary>
void PrintMatrix(int x, int y, const Matrix4x4 &m, const char *name) {
	Novice::ScreenPrintf(x, y, "%s", name);
	Novice::ScreenPrintf(x, y + 20, "%6.3f %6.3f %6.3f %6.3f",
		m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3]);
	Novice::ScreenPrintf(x, y + 40, "%6.3f %6.3f %6.3f %6.3f",
		m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3]);
	Novice::ScreenPrintf(x, y + 60, "%6.3f %6.3f %6.3f %6.3f",
		m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3]);
	Novice::ScreenPrintf(x, y + 80, "%6.3f %6.3f %6.3f %6.3f",
		m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);
}

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	Novice::Initialize(kWindowTitle, 1280, 720);

	char keys[256] = {0};
	char preKeys[256] = {0};

	// 軸と角度の設定
	Vector3 axis = {1.0f, 1.0f, 1.0f};
	float angle = 0.44f;

	// 回転行列の生成
	Matrix4x4 rotateMatrix = MakeRotateAxisAngle(axis, angle);

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// 描画処理
		PrintMatrix(50, 50, rotateMatrix, "rotateMatrix");

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
