#pragma once
#include "./Header/EngineGeneral.h"
#include <vector>

class Camera final
{
public: // シングルトン化
	static Camera* Get();
private:
	Camera();
	Camera(const Camera&) = delete;
	~Camera() {}
	Camera operator=(const Camera&) = delete;

private: // エイリアス
	using XMVECTOR = DirectX::XMVECTOR;
	using Vector3 = Math::Vector3;
	using Matrix4 = Math::Matrix4;

public: // サブクラス
	enum Projection
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

public: // 定数
	static const size_t MAIN_CAMERA;

public: // 静的メンバ変数
	static float targetRadius; //注視点からの距離
	static float longitude;    //経度
	static float latitude;     //緯度

	static Vector3 pos;    //カメラの位置
	static Vector3 target; //カメラの注視点
	static Vector3 upVec;  //上ベクトル

	static Matrix4 matProjection[2];     //プロジェクション行列
	static std::vector<Matrix4> matView; //ビュー変換行列(カメラ)
	static size_t cameraNo;              //カメラの番号（最初はMAIN_CAMERAを指している）
private:
	static float nearClip; //ニアクリップ距離
	static float farClip;  //ファークリップ距離

public: // 静的メンバ関数
	// 初期化処理
	static void Init();
	// 更新処理
	static void Update();

	// キャラクタ姿勢行列の生成
	static Matrix4 CreateCamera(const XMVECTOR& pos, const XMVECTOR& target, const XMVECTOR& upVector);
	// キャラクタ姿勢束縛行列の生成
	static Matrix4 CreateCameraFix(const XMVECTOR& pos, const XMVECTOR& target, const XMVECTOR& upVector);

	// カメラの切り替え
	static int ChangeCamera(const size_t& cameraNo);
	// カメラの設定
	static void SetCamera(const Vector3& cameraPos, const Vector3& cameraTarget, const Vector3& upVector);
	// カメラの取得
	static Matrix4 GetMatView() { return matView[cameraNo]; };

	// ニアクリップ距離の設定
	static int SetNear(float nearClip);
	// ファークリップ距離の設定
	static int SetFar(float farClip);
};
