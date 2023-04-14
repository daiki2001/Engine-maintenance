#pragma once
#include "DebugText.h"
#include <d3dx12.h>
#include "./Math/EngineMath.h"
#include <vector>

namespace Engine
{
namespace Library
{
class DrawPolygon final : public DebugText
{
private: //シングルトン化
	DrawPolygon();
	DrawPolygon(const DrawPolygon&) = delete;
	~DrawPolygon();
	const DrawPolygon& operator=(const DrawPolygon&) = delete;
public:
	static DrawPolygon* GetInstance();

private: // エイリアス
	using Vector3 = Engine::Math::Vector3;
	template <class T>
	using vector = std::vector<T>;

public: // メンバ変数
	Vector3 light;    //光源
private:
	int polygonCount; //オブジェクトの数
	size_t loopCount; //ループした回数

	Vector3 lightVec; //光線

	size_t objModelCount; //objファイルから読み込んだモデルの数
	vector<size_t> verticesCount; //
	vector<Material> material;

	// 関数の初期化フラグ
	bool isDrawOBJPolygonInit = false;

public: // メンバ関数
	// 三角形の作成
	int CreateTriangle(const Vector3& vertex1, const DirectX::XMFLOAT2& uv1,
					   const Vector3& vertex2, const DirectX::XMFLOAT2& uv2,
					   const Vector3& vertex3, const DirectX::XMFLOAT2& uv3,
					   bool isFill = true);
	// 矩形の作成
	int CreateRect(float width, float height, bool isFill = true);
	// 正多角形の作成
	int CreateCircle(float r, const size_t& divNum, bool isFill = true);
	// 直方体の作成
	int Create3Dbox(float width, float height, float depth, bool isFill = true);
	int Create3Dbox(Vector3 size, bool isFill = true)
	{
		return Create3Dbox(size.x, size.y, size.z, isFill);
	};
	// 正多角錐の作成
	int CreateCorn(float r, float h, const size_t& divNum, bool isFill = true);
	// 正多角柱の作成
	int CreateCylinder(float r, float h, const size_t& divNum, bool isFill = true);
	// 球体の作成
	int CreateSphere(float r, const size_t& divNum, bool isFill = true);

	// objファイルによるモデルの作成（filePath：objファイルの名前、directoryPath：モデルのファイルがあるフォルダ）
	int CreateOBJModel(const char* filePath, const char* directoryPath);

	// 計算モデルデータ・オブジェクトデータの削除
	int DeleteModelData(int* modelData);

	// テクスチャのタイリング
	void Tiring(int polygonData, float uvScaleX, float uvScaleY);
	// テクスチャの自動タイリング
	void AutoTiring(int polygonData, int graphHandle);
	// テクスチャの移動
	void MoveUV(int polygonData, int graphHandle, DirectX::XMFLOAT2 speed);

	// オブジェクトの描画処理
	int Draw(int polygonData, const Vector3& position, const Matrix4& rotation, const Vector3& scale,
			 const XMFLOAT4& color, int graphHandle = 0, bool isFill = true,
			 bool isOrthoGraphic = false, bool viewMultiFlag = true, Object* parent = nullptr);

	// モデルの描画処理
	int DrawOBJ(int object, const Vector3& position, const Matrix4& rotation, const Vector3& scale,
				const XMFLOAT4& color = Color::AddAlphaValue(Color::WHITE, 1.0f),
				bool isOrthoGraphic = false, bool viewMultiFlag = true, Object* parent = nullptr);

	// カメラの作成
	int CreateCamera(const Vector3& cameraPos, const Vector3& cameraTarget, const Vector3& upVector);

	// スクリーン座標からワールド座標を求める
	DirectX::XMFLOAT3 ScreenToWorld(int x, int y, float z);
	// ビュー行列の逆行列を取得（移動量は0固定）
	DirectX::XMMATRIX InverseView();

	// 内部の初期化用関数
	void PolygonLoopEnd();
private:
	// 描画関数の初期化
	int DrawPolygonInit();

	// 描画関数の初期化
	int DrawOBJInit();

	// マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/*頂点座標とインデックスデータ計算用*/
	void Circle(const Vector3& centerPos, float r, const size_t& divNum,
				bool flag, Vertex* v, unsigned short* index);

	// データの消去
	void DataClear();
};
} //Library
} //Engine
