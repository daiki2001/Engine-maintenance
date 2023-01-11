#pragma once
#include "./Header/EngineGeneral.h"
#include "./Math/Collision/CollisionInfo.h"
#include "./ShaderMgr/ShaderManager.h"
#include <vector>
#include <wrl.h>
#include <d3dx12.h>
#include <memory>

/*DirectDrawingのバージョン指定*/
#define DIRECTDRAWING_VERSION 0x0001

class BaseCollider;

// 頂点データ構造体
struct Vertex
{
	Math::Vector3 pos;    //xyz座標
	Math::Vector3 normal; //法線ベクトル
	DirectX::XMFLOAT2 uv; //uv座標
};

// 1オブジェクトの頂点情報をまとめた構造体
struct VertexData
{
	/*配列にして使うことが前提*/

private: // エイリアス
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ変数
	std::vector<Vertex> vertices;    //頂点データ
	ComPtr<ID3D12Resource> vertBuff; //頂点バッファの生成
	D3D12_VERTEX_BUFFER_VIEW vbView = {}; //頂点バッファビューの作成

	std::vector<unsigned short> indices; //インデックスデータ
	ComPtr<ID3D12Resource> indexBuff;    //インデックスバッファの設定
	D3D12_INDEX_BUFFER_VIEW ibView = {};      //インデックスバッファビューの作成

public: // メンバ関数
	~VertexData()
	{
		ContainerClear(vertices);
		ContainerClear(indices);
	}
};

// 頂点データ構造体(スプライト用)
struct SpriteVertex
{
	Math::Vector3 pos;    //xyz座標
	DirectX::XMFLOAT2 uv; //uv座標
};

// 定数バッファ用データ構造体
struct ConstBufferData
{
	DirectX::XMFLOAT4 color; //色 (RGBA)
	Math::Matrix4 mat;       //3D変換行列
	Math::Vector3 lightVec;  //光源へのベクトル
};
// 定数バッファ用データ構造体(マテリアル)
struct MaterialConstBufferData
{
	Math::Vector3 ambient;  //アンビエント係数
	float pad1;             //パディング
	Math::Vector3 diffuse;  //ディフューズ係数
	float pad2;             //パディング
	Math::Vector3 specular; //スペキュラー係数
	float alpha;            //アルファ
};
// 定数バッファ用データ構造体(スプライト用)
struct SpriteConstBufferData
{
	DirectX::XMFLOAT4 color; //色 (RGBA)
	Math::Matrix4 mat;       //3D変換行列
};

// ディスクリプタハンドル
struct DescHandle
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandle; //CPU側のハンドル
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle; //GPU側のハンドル
};

// 各オブジェクト(スプライト)で共通のデータをまとめた構造体
struct CommonData
{
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate[5]; //パイプラインの状態
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;    //ルートシグネチャ

	// コンストラクタ
	CommonData();
};

// オブジェクトのワールド行列をまとめた構造体
class Object
{
	/*配列にして使うことが前提*/
public: // メンバ変数
	int polygonData; //頂点情報の要素番号
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff; //定数バッファ

	Math::Vector3 position;  //座標
	Math::Matrix4 rotation;  //回転行列
	Math::Vector3 scale;     //スケール
	DirectX::XMFLOAT4 color; //色

	Math::Matrix4 matWorld; //ワールド行列

	Object* parent; //親の要素番号

	const char* name = nullptr; //クラス名(デバッグ用)
	BaseCollider* collider = nullptr; //コライダー

public: // メンバ関数
	Object();
	virtual ~Object();

	// 初期化
	virtual void Init();
	// 更新
	virtual void Update();

	// コライダーの設定
	void SetCollider(BaseCollider* collider);
	// 衝突時コールバック関数
	virtual void OnCollision(const CollisionInfo& info) {}
};

// マテリアル
struct Material
{
	std::string name;            //マテリアル名
	Math::Vector3 ambient;       //アンビエント影響度
	Math::Vector3 diffuse;       //ディフューズ影響度
	Math::Vector3 specular;      //スペキュラー影響度
	float alpha;                 //アルファ
	std::string textureFilename; //テクスチャファイル名

	size_t textrueIndex; //テクスチャのインデックス

	// コンストラクタ
	Material()
	{
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0f;

		textrueIndex = 0;
	}

	// デストラクタ
	~Material()
	{
		ContainerClear(name);
		ContainerClear(textureFilename);
	}
};

// objファイルのデータ
class OBJData : public Object
{
	/*配列にして使うことが前提*/
	/*先にモデル不要のオブジェクトを生成する*/
public:
	std::string name;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialConstBuff; //定数バッファ
	Material material;

	// コンストラクタ
	OBJData() : Object()
	{
		name = {};
		material = {};
	}
	// デストラクタ
	virtual ~OBJData() override
	{
		Object::~Object();
	}
};

struct OBJSubset
{
	size_t objStart;
	size_t verticesCount;
};

// スプライト一枚分のデータをまとめた構造体
struct Sprite
{
	/*配列にして使うことが前提*/

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;  //頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView;                  //頂点バッファビュー
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff; //定数バッファ

	Math::Matrix4 matWorld = DirectX::XMMatrixIdentity(); //ワールド行列
	DirectX::XMFLOAT4 color = { 1, 1, 1, 1 }; //色 (RGBA)

	Math::Vector3 pos = { 0, 0, 0 }; //座標
	float rotation = 0.0f; //回転角
	DirectX::XMFLOAT2 size = { 0.000000001f, 0.000000001f }; //大きさ

	DirectX::XMFLOAT2 anchorpoint = { 0.5f, 0.5f }; //アンカーポイント

	// 描画範囲
	DirectX::XMFLOAT2 texLeftTop = { 0, 0 };
	DirectX::XMFLOAT2 texSize = { 100, 100 };

	bool isTriangle = false;
};

class DirectDrawing
{
public: // エイリアス
	using BlendMode = ShaderManager::BlendMode;
protected:
	// DirectX::を省略
	using Vector3 = Math::Vector3;
	using Matrix4 = Math::Matrix4;
	// std::を省略
	template<class T> using vector = std::vector<T>;
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス
	// オブジェクトデータ（スプライトのデータ）のインデックス
	struct IndexData
	{
		int constant; //定数バッファ
		int textrue;  //テクスチャ
	};

public: // 静的メンバ変数
	static bool isDepthWriteBan; //trueだったら、デプスに対しての上書き禁止
private:
	static int objShader;          //*.objデータのシェーダー
	static int objGraphicPipeline; //*.objデータのグラフィックパイプライン
	static int objRootSignature;   //*.objデータのルートシグネチャ
	static int objPipelineState;   //*.objデータのパイプライン

	static int materialShader;          //マテリアルデータのシェーダー
	static int materialGraphicPipeline; //マテリアルデータのグラフィックパイプライン
	static int materialRootSignature;   //マテリアルデータのルートシグネチャ
	static int materialPipelineState;   //マテリアルデータのパイプライン

	static int spriteShader;          //スプライトデータのシェーダー
	static int spriteGraphicPipeline; //スプライトデータのグラフィックパイプライン
	static int spriteRootSignature;   //スプライトデータのルートシグネチャ
	static int spritePipelineState;   //スプライトデータのパイプライン

	static int inputLayout3d; //3dデータのインプットレイアウト
	static int inputLayout2d; //2dデータのインプットレイアウト

public: // 静的メンバ関数
	static void ChangeOBJShader();
	static void ChangeMaterialShader();
	static void ChangeSpriteShader();

	static int GetObjShader() { return objShader; }
	static int GetObjGraphicPipeline() { return objGraphicPipeline; }
	static int GetObjRootSignature() { return objRootSignature; }
	static int GetObjPipelineState() { return objPipelineState; }

	static int GetMaterialShader() { return materialShader; }
	static int GetMaterialGraphicPipeline() { return materialGraphicPipeline; }
	static int GetMaterialRootSignature() { return materialRootSignature; }
	static int GetMaterialPipelineState() { return materialPipelineState; }

	static int GetSpriteShader() { return spriteShader; }
	static int GetSpriteGraphicPipeline() { return spriteGraphicPipeline; }
	static int GetSpriteRootSignature() { return spriteRootSignature; }
	static int GetSpritePipelineState() { return spritePipelineState; }

	static int Get2dInputLayout() { return inputLayout2d; }
	static int Get3dInputLayout() { return inputLayout3d; }

protected: // メンバ変数
	vector<VertexData> vertices; //頂点データ
	Vertex* vertMap;

	vector<OBJData> obj; //オブジェクトデータ
	vector<IndexData> objIndex; //オブジェクトデータのインデックス
	ComPtr<ID3D12DescriptorHeap> basicDescHeap; //定数バッファ用のデスクリプタヒープ
	vector<DescHandle> srvHandle;
	size_t nullBufferCount; //ワールド行列だけのオブジェクトデータの数

	vector<Sprite> sprite;         //スプライトのデータ
	vector<IndexData> spriteIndex; //スプライトのデータのインデックス

	vector<OBJSubset> objSubset;
private:
	// 関数の初期化フラグ
	bool isDrawingInit = false;
	bool isMaterialInit = false;
	bool isSpriteDrawingInit = false;
	bool isBasicDescHeapInit = false;

public: // メンバ関数
	// コンストラクタ
	DirectDrawing();
	// デストラクタ
	~DirectDrawing();

	/// <summary>
	/// 空の定数バッファの生成
	/// </summary>
	/// <param name="pos"> 座標 </param>
	/// <param name="rota"> 回転行列 </param>
	/// <param name="scale"> スケール </param>
	/// <returns> オブジェクトデータの要素番号 </returns>
	int CreateNullConstant(const Vector3& pos, const Matrix4& rota, const Vector3& scale);
	/// <summary>
	/// 定数バッファの更新
	/// </summary>
	/// <param name="pos"> 座標 </param>
	/// <param name="rota"> 回転行列 </param>
	/// <param name="scale"> スケール </param>
	/// <param name="objectIndex"> オブジェクトデータの要素番号 </param>
	/// <param name="polygonData"> 頂点情報の要素番号 </param>
	/// <param name="parent"> 親の要素番号 </param>
	void UpdataConstant(const Vector3& pos, const Matrix4& rota, const Vector3& scale,
						const DirectX::XMFLOAT4& color, int objectIndex, int polygonData = -1,
						Object* parent = nullptr);
	// 描画の際のブレンドモードをセットする
	int SetDrawBlendMode(int blendMode);

	OBJData* GetObjData(int object)
	{
		if (object < 0 || object >= (int)obj.size()) { return nullptr; }
		return &obj[object];
	}
protected:
	// 初期化
	int Init();
	// オブジェクトの描画関係の初期化
	void DrawingInit();
	// オブジェクトの描画関係の初期化
	void MaterialInit();
	// スプライトの描画関係の初期化
	void SpriteDrawingInit();
	// 頂点バッファとインデックスバッファの生成
	int CreateVertexAndIndexBuffer();
	// 定数バッファの生成
	HRESULT CreateConstBuffer(int* objIndex);
	// スプライトの生成
	int CreateSprite();
	// ディスクリプタヒープの初期化
	HRESULT BasicDescHeapInit();

	// 描画処理の共通部分
	void BaseDrawGraphics();
	// 描画処理の共通部分(スプライト用)
	void BaseDrawSpriteGraphics();
private:
	// データの消去
	void DataClear();
};
