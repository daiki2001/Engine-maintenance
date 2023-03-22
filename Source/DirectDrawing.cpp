#include "./Header/DirectDrawing.h"
#include "./Header/DirectXInit.h"
#include "./Header/Camera.h"
#include "./Math/Collision/BaseCollider.h"
#include "./Math/Collision/CollisionManager.h"
#include "./Header/SafeDelete.h"

namespace
{
ShaderManager* shaderMgr = ShaderManager::Get();
}

CommonData::CommonData() :
	pipelinestate{},
	rootsignature(nullptr)
{
}

Object::Object() :
	polygonData(FUNCTION_ERROR),
	constBuff{},
	position(0.0f, 0.0f, 0.0f),
	rotation(DirectX::XMMatrixIdentity()),
	scale(1.0f, 1.0f, 1.0f),
	color(Color::AddAlphaValue(Color::WHITE, 1.0f)),
	matWorld(DirectX::XMMatrixIdentity()),
	parent(nullptr),
	name(nullptr),
	collider(nullptr)
{
}

Object::~Object()
{
	if (collider != nullptr)
	{
		// コライダーの登録を解除する
		CollisionManager::Get()->RemoveCollider(collider);
	}

	safe_delete(parent);
	safe_delete(collider);
}

void Object::Init()
{
	name = typeid(*this).name();
}

void Object::Update()
{
	// 当たり判定更新
	if (collider != nullptr)
	{
		collider->Update();
	}
}

void Object::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);
	this->collider = collider;
	// コライダーを登録する
	CollisionManager::Get()->AddCollider(collider);
	// コライダーを更新しておく
	collider->Update();
}

/*static変数の初期化*/
bool DirectDrawing::isDepthWriteBan = false;

int DirectDrawing::objShader = Engine::FUNCTION_ERROR;
int DirectDrawing::objGraphicPipeline = Engine::FUNCTION_ERROR;
int DirectDrawing::objRootSignature = Engine::FUNCTION_ERROR;
int DirectDrawing::objPipelineState = Engine::FUNCTION_ERROR;

int DirectDrawing::materialShader = Engine::FUNCTION_ERROR;
int DirectDrawing::materialGraphicPipeline = Engine::FUNCTION_ERROR;
int DirectDrawing::materialRootSignature = Engine::FUNCTION_ERROR;
int DirectDrawing::materialPipelineState = Engine::FUNCTION_ERROR;

int DirectDrawing::spriteShader = Engine::FUNCTION_ERROR;
int DirectDrawing::spriteGraphicPipeline = Engine::FUNCTION_ERROR;
int DirectDrawing::spriteRootSignature = Engine::FUNCTION_ERROR;
int DirectDrawing::spritePipelineState = Engine::FUNCTION_ERROR;

int DirectDrawing::inputLayout3d = Engine::FUNCTION_ERROR;
int DirectDrawing::inputLayout2d = Engine::FUNCTION_ERROR;

void DirectDrawing::DataClear()
{
	ContainerClear(sprite);
	ContainerClear(spriteIndex);
	ContainerClear(vertices);
	ContainerClear(obj);
	ContainerClear(objIndex);
	ContainerClear(objSubset);

	/*'vertices'で開放漏れが発生している?*/
	vector<VertexData>().swap(vertices);

#ifdef _DEBUG
	OutputDebugStringA("'DirectDrawing'のデータを削除しました。\n");
#endif // _DEBUG
}

void DirectDrawing::ChangeOBJShader()
{
	shaderMgr->ChangePipelineState(
		DirectXInit::GetCommandList(),
		objRootSignature,
		objPipelineState);
}

void DirectDrawing::ChangeMaterialShader()
{
	shaderMgr->ChangePipelineState(
		DirectXInit::GetCommandList(),
		materialRootSignature,
		materialPipelineState);
}

void DirectDrawing::ChangeSpriteShader()
{
	shaderMgr->ChangePipelineState(
		DirectXInit::GetCommandList(),
		spriteRootSignature,
		spritePipelineState);
}

DirectDrawing::DirectDrawing() :
	vertices{},
	vertMap(nullptr),
	obj{},
	objIndex{},
	basicDescHeap{},
	srvHandle{},
	nullBufferCount(0),
	sprite{},
	spriteIndex{}
{
	Init();
}

DirectDrawing::~DirectDrawing()
{
	DataClear();
}

int DirectDrawing::Init()
{
	DrawingInit();
	MaterialInit();
	SpriteDrawingInit();

	return CreateNullConstant(Vector3(), Matrix4(), Vector3(1, 1, 1));
}

void DirectDrawing::DrawingInit()
{
	if (isDrawingInit == true)
	{
		return;
	}

	static auto* dev = DirectXInit::GetDevice();

	ComPtr<ID3DBlob> errorBlob; //エラーオブジェクト
	ComPtr<ID3DBlob> rootSigBlob;

	isDrawingInit = true;

	// 各種シェーダーのコンパイルと読み込み
	objShader = shaderMgr->CreateShader(StringToWString(shadersDirectory + "ObjectVS.hlsl").c_str(),
										StringToWString(shadersDirectory + "ObjectPS.hlsl").c_str());

	// 頂点レイアウト
	if (inputLayout3d ==Engine::FUNCTION_ERROR)
	{
		inputLayout3d = shaderMgr->CreateInputLayout();
		shaderMgr->GetInputLayout(inputLayout3d).PushInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
		shaderMgr->GetInputLayout(inputLayout3d).PushInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
		shaderMgr->GetInputLayout(inputLayout3d).PushInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	}

	objGraphicPipeline = shaderMgr->CreateGPipeline(objShader, inputLayout3d);

	// デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV = {}; //デスクリプタテーブルの設定(シェーダリソース)
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[2]{}; //ルートパラメータの設定
	rootparams[0].InitAsConstantBufferView(0);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV);

	objRootSignature = shaderMgr->CreateRootSignature(objGraphicPipeline, _countof(rootparams), rootparams);
	objPipelineState = shaderMgr->CreatePipelineState(objGraphicPipeline);

	return;
}

void DirectDrawing::MaterialInit()
{
	if (isMaterialInit == true)
	{
		return;
	}

	static auto dev = DirectXInit::GetDevice();

	isMaterialInit = true;

	// 各種シェーダーのコンパイルと読み込み
	materialShader = shaderMgr->CreateShader(StringToWString(shadersDirectory + "MaterialVS.hlsl").c_str(),
											 StringToWString(shadersDirectory + "MaterialPS.hlsl").c_str());

	// 頂点レイアウト
	if (inputLayout3d == Engine::FUNCTION_ERROR)
	{
		inputLayout3d = shaderMgr->CreateInputLayout();
		shaderMgr->GetInputLayout(inputLayout3d).PushInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
		shaderMgr->GetInputLayout(inputLayout3d).PushInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
		shaderMgr->GetInputLayout(inputLayout3d).PushInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	}

	materialGraphicPipeline = shaderMgr->CreateGPipeline(materialShader, inputLayout3d);

	// デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV = {}; //デスクリプタテーブルの設定(シェーダリソース)
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[3]{}; //ルートパラメータの設定
	rootparams[0].InitAsConstantBufferView(0);
	rootparams[1].InitAsConstantBufferView(1);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV);

	materialRootSignature = shaderMgr->CreateRootSignature(materialGraphicPipeline, _countof(rootparams), rootparams);
	materialPipelineState = shaderMgr->CreatePipelineState(materialGraphicPipeline);

	return;
}

void DirectDrawing::SpriteDrawingInit()
{
	if (isSpriteDrawingInit == true)
	{
		return;
	}

	static auto* dev = DirectXInit::GetDevice();

	ComPtr<ID3DBlob> errorBlob; //エラーオブジェクト
	ComPtr<ID3DBlob> rootSigBlob;

	isSpriteDrawingInit = true;

	// 各種シェーダーのコンパイルと読み込み
	spriteShader = shaderMgr->CreateShader(StringToWString(shadersDirectory + "SpriteVS.hlsl").c_str(),
										   StringToWString(shadersDirectory + "SpritePS.hlsl").c_str());

	// 頂点レイアウト
	inputLayout2d = shaderMgr->CreateInputLayout();
	shaderMgr->GetInputLayout(inputLayout2d).PushInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	shaderMgr->GetInputLayout(inputLayout2d).PushInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	spriteGraphicPipeline = shaderMgr->CreateGPipeline(spriteShader, inputLayout2d);

	for (size_t i = 0; i < 5; i++)
	{
		auto& gPipeline = shaderMgr->GetGraphicsPipeline(spriteGraphicPipeline, static_cast<ShaderManager::BlendMode>(i));

		// ラスタライザステート
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; //背面カリングしない

		// デプスステンシルステートの設定
		gPipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; //常に上書き
		gPipeline.DepthStencilState.DepthEnable = false;                      //深度テストをしない
	}

	// デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV = {}; //デスクリプタテーブルの設定(シェーダリソース)
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[2]{}; //ルートパラメータの設定
	rootparams[0].InitAsConstantBufferView(0);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV);

	spriteRootSignature = shaderMgr->CreateRootSignature(spriteGraphicPipeline, _countof(rootparams), rootparams);
	spritePipelineState = shaderMgr->CreatePipelineState(spriteGraphicPipeline);

	return;
}

int DirectDrawing::CreateVertexAndIndexBuffer()
{
	HRESULT hr = S_FALSE;

	// 'vertexNum'が0以下なら生成せずにリターンする
	if (vertices.size() <= 0 || vertices.back().vertices.size() <= 0)
	{
		return Engine::FUNCTION_ERROR;
	}

	static auto* dev = DirectXInit::GetDevice();

#pragma region VertexBuffer

	// 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB;
	sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.back().vertices.size());

	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertices.back().vertBuff));
	if (FAILED(hr))
	{
		return Engine::FUNCTION_ERROR;
	}

	vertices.back().vbView.BufferLocation = vertices.back().vertBuff->GetGPUVirtualAddress();
	vertices.back().vbView.SizeInBytes = sizeVB;
	vertices.back().vbView.StrideInBytes = sizeof(Vertex);

#pragma endregion //VertexBuffer

	if (vertices.back().indices.size() <= 0)
	{
		for (unsigned short i = 0; i < vertices.back().vertices.size(); i++)
		{
			vertices.back().indices.push_back(i);
		}
	}

#pragma region IndexBuffer

	//インデックスデータ全体のサイズ
	UINT sizeIB;
	sizeIB = static_cast<UINT>(sizeof(unsigned short) * vertices.back().indices.size());

	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertices.back().indexBuff));
	if (FAILED(hr))
	{
		return Engine::FUNCTION_ERROR;
	}

	// GPU上のバッファに対応した仮想メモリを取得
	static unsigned short* indexMap = nullptr;
	hr = vertices.back().indexBuff->Map(0, nullptr, (void**)&indexMap);

	// 全インデックスに対して
	for (size_t i = 0; i < vertices.back().indices.size(); i++)
	{
		indexMap[i] = vertices.back().indices[i]; //インデックスをコピー
	}

	// 繋がりを解除
	vertices.back().indexBuff->Unmap(0, nullptr);

	vertices.back().ibView.BufferLocation = vertices.back().indexBuff->GetGPUVirtualAddress();
	vertices.back().ibView.Format = DXGI_FORMAT_R16_UINT;
	vertices.back().ibView.SizeInBytes = sizeIB;

#pragma endregion //IndexBuffer

#pragma region NormalVector

	for (size_t i = 0; i < vertices.back().indices.size() / 3; i++)
	{
		using namespace DirectX;

		// 三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short temp0 = vertices.back().indices[i * 3 + 0];
		unsigned short temp1 = vertices.back().indices[i * 3 + 1];
		unsigned short temp2 = vertices.back().indices[i * 3 + 2];
		// 三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices.back().vertices[temp0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices.back().vertices[temp1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices.back().vertices[temp2].pos);
		// p0→p1ベクトル、p0→p2ベクトルを計算(ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		// 外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		// 正規化
		normal = XMVector3Normalize(normal);
		// 求めた法線を頂点データに代入
		XMStoreFloat3(&vertices.back().vertices[temp0].normal, normal);
		XMStoreFloat3(&vertices.back().vertices[temp1].normal, normal);
		XMStoreFloat3(&vertices.back().vertices[temp2].normal, normal);
	}

	hr = vertices.back().vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (UINT i = 0; i < vertices.back().vertices.size(); i++)
	{
		vertMap[i] = vertices.back().vertices[i]; //座標をコピー
	}

	// マップを解除
	vertices.back().vertBuff->Unmap(0, nullptr);

#pragma endregion //NormalVector

	return static_cast<int>(vertices.size() - 1);
}

HRESULT DirectDrawing::CreateConstBuffer(int* objIndex)
{
	HRESULT hr = S_FALSE;

	static auto* dev = DirectXInit::GetDevice();

	obj.push_back({});

	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xFF) & ~0xFF), //リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&obj[obj.size() - 1].constBuff));
	if (FAILED(hr))
	{
		return hr;
	}
	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MaterialConstBufferData) + 0xFF) & ~0xFF), //リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&obj[obj.size() - 1].materialConstBuff));
	if (FAILED(hr))
	{
		return hr;
	}

	hr = BasicDescHeapInit();
	if (FAILED(hr))
	{
		return hr;
	}

	// 定数バッファビュー生成
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = obj[obj.size() - 1].constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = static_cast<UINT>(obj[obj.size() - 1].constBuff->GetDesc().Width);
	dev->CreateConstantBufferView(
		&cbvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			basicDescHeap->GetCPUDescriptorHandleForHeapStart(),
			*objIndex,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);

	*objIndex = static_cast<int>(obj.size() - 1);
	return hr;
}

int DirectDrawing::CreateNullConstant(const Vector3& pos, const Matrix4& rota, const Vector3& scale)
{
	using namespace DirectX;

	static auto* dev = DirectXInit::GetDevice();
	XMMATRIX rot = XMMatrixIdentity();

	obj.push_back({});

	obj[obj.size() - 1].position = pos;
	obj[obj.size() - 1].rotation = rota;
	obj[obj.size() - 1].scale = scale;
	obj[obj.size() - 1].matWorld = XMMatrixIdentity();

	obj[obj.size() - 1].matWorld *= XMMatrixScaling(
		obj[obj.size() - 1].scale.x,
		obj[obj.size() - 1].scale.y,
		obj[obj.size() - 1].scale.z);
	obj[obj.size() - 1].matWorld *= obj[obj.size() - 1].rotation;
	obj[obj.size() - 1].matWorld *= XMMatrixTranslation(
		obj[obj.size() - 1].position.x,
		obj[obj.size() - 1].position.y,
		obj[obj.size() - 1].position.z);

	nullBufferCount++;

	return static_cast<int>(obj.size() - 1);
}

void DirectDrawing::UpdataConstant(
	const Vector3& pos, const DirectX::XMMATRIX& rota, const Vector3& scale,
	const DirectX::XMFLOAT4& color, int objectIndex, int polygonData,
	Object* parent)
{
	if ((objectIndex < 0 || static_cast<size_t>(objectIndex) >= obj.size()) ||
		(polygonData < -1 || (polygonData != -1 && static_cast<size_t>(polygonData) >= vertices.size())))
	{
		return;
	}

	using namespace DirectX;
	using namespace Engine::Math;

	bool dirtyFlag = false; //ダーティフラグ

	// 頂点情報が違ったら更新
	if (obj[objectIndex].polygonData != polygonData)
	{
		obj[objectIndex].polygonData = polygonData;
	}

	// 座標が違ったら更新し、ダーティフラグをtrueにする
	if (obj[objectIndex].position.x != pos.x ||
		obj[objectIndex].position.y != pos.y ||
		obj[objectIndex].position.z != pos.z)
	{
		obj[objectIndex].position = pos;
		dirtyFlag = true;
	}

	// 回転行列が違ったら更新し、ダーティフラグをtrueにする
	if ((obj[objectIndex].rotation.r[0].m128_f32[0] != rota.r[0].m128_f32[0]) ||
		(obj[objectIndex].rotation.r[0].m128_f32[1] != rota.r[0].m128_f32[1]) ||
		(obj[objectIndex].rotation.r[0].m128_f32[2] != rota.r[0].m128_f32[2]) ||
		(obj[objectIndex].rotation.r[0].m128_f32[3] != rota.r[0].m128_f32[3]) ||
		(obj[objectIndex].rotation.r[1].m128_f32[0] != rota.r[0].m128_f32[0]) ||
		(obj[objectIndex].rotation.r[1].m128_f32[1] != rota.r[0].m128_f32[1]) ||
		(obj[objectIndex].rotation.r[1].m128_f32[2] != rota.r[0].m128_f32[2]) ||
		(obj[objectIndex].rotation.r[1].m128_f32[3] != rota.r[0].m128_f32[3]) ||
		(obj[objectIndex].rotation.r[2].m128_f32[0] != rota.r[0].m128_f32[0]) ||
		(obj[objectIndex].rotation.r[2].m128_f32[1] != rota.r[0].m128_f32[1]) ||
		(obj[objectIndex].rotation.r[2].m128_f32[2] != rota.r[0].m128_f32[2]) ||
		(obj[objectIndex].rotation.r[2].m128_f32[3] != rota.r[0].m128_f32[3]) ||
		(obj[objectIndex].rotation.r[3].m128_f32[0] != rota.r[0].m128_f32[0]) ||
		(obj[objectIndex].rotation.r[3].m128_f32[1] != rota.r[0].m128_f32[1]) ||
		(obj[objectIndex].rotation.r[3].m128_f32[2] != rota.r[0].m128_f32[2]) ||
		(obj[objectIndex].rotation.r[3].m128_f32[3] != rota.r[0].m128_f32[3]))
	{
		obj[objectIndex].rotation = rota;
		dirtyFlag = true;
	}

	// スケールが違ったら更新し、ダーティフラグをtrueにする
	if (obj[objectIndex].scale.x != scale.x ||
		obj[objectIndex].scale.y != scale.y ||
		obj[objectIndex].scale.z != scale.z)
	{
		obj[objectIndex].scale = scale;
		dirtyFlag = true;
	}

	// 親の要素番号が違ったら更新し、ダーティフラグをtrueにする
	if (obj[objectIndex].parent != nullptr)
	{
		obj[objectIndex].parent = parent;
		dirtyFlag = true;
	}

	// ダーティフラグがtrueだったら、ワールド行列を更新
	if (dirtyFlag == true)
	{
		obj[objectIndex].matWorld = XMMatrixIdentity();
		obj[objectIndex].matWorld *= XMMatrixScaling(
			obj[objectIndex].scale.x,
			obj[objectIndex].scale.y,
			obj[objectIndex].scale.z);
		obj[objectIndex].matWorld *= obj[objectIndex].rotation;
		obj[objectIndex].matWorld *= XMMatrixTranslation(
			obj[objectIndex].position.x,
			obj[objectIndex].position.y,
			obj[objectIndex].position.z);

		if (obj[objectIndex].parent != nullptr)
		{
			obj[objectIndex].matWorld *= parent->matWorld;
		}
	}

	// 色が違ったら更新
	if (obj[objectIndex].color != color)
	{
		obj[objectIndex].color = color;
	}
}

int DirectDrawing::CreateSprite()
{
	using namespace DirectX;

	HRESULT hr = S_FALSE;
	static auto* dev = DirectXInit::GetDevice();

	sprite.push_back({});

	static SpriteVertex vert[] = {
		{{}, { 0.0f, 1.0f }},
		{{}, { 0.0f, 0.0f }},
		{{}, { 1.0f, 1.0f }},
		{{}, { 1.0f, 0.0f }},
	};

	// 頂点バッファの生成
	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vert)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&sprite[sprite.size() - 1].vertBuff));
	if (FAILED(hr))
	{
		return Engine::FUNCTION_ERROR;
	}

	// 頂点バッファへのデータ転送
	SpriteVertex* vertexMap = nullptr;
	hr = sprite[sprite.size() - 1].vertBuff->Map(0, nullptr, (void**)&vertexMap);
	memcpy(vertexMap, vert, sizeof(vert));
	sprite[sprite.size() - 1].vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの生成
	sprite[sprite.size() - 1].vbView.BufferLocation = sprite[sprite.size() - 1].vertBuff->GetGPUVirtualAddress();
	sprite[sprite.size() - 1].vbView.SizeInBytes = sizeof(vert);
	sprite[sprite.size() - 1].vbView.StrideInBytes = sizeof(SpriteVertex);

	// 定数バッファの生成
	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xFF) & ~0xFF), //リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&sprite[sprite.size() - 1].constBuff));
	if (FAILED(hr))
	{
		return Engine::FUNCTION_ERROR;
	}

	// 定数バッファにデータ転送
	SpriteConstBufferData* constMap = nullptr;
	hr = sprite[sprite.size() - 1].constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = Color::AddAlphaValue(Color::WHITE, 1.0f); //色指定(RGBA)
	constMap->mat = Camera::matProjection[Camera::Projection::ORTHOGRAPHIC]; //平行投影行列
	sprite[sprite.size() - 1].constBuff->Unmap(0, nullptr);

	return static_cast<int>(sprite.size() - 1);
}

HRESULT DirectDrawing::BasicDescHeapInit()
{
	HRESULT hr;
	static auto* dev = DirectXInit::GetDevice();

	if (isBasicDescHeapInit == true)
	{
		return S_OK;
	}
	else
	{
		isBasicDescHeapInit = true;

		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{}; //設定構造体

		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //シェーダから見える
		descHeapDesc.NumDescriptors = static_cast<UINT>(50000); //定数バッファの数
		// デスクリプタヒープの生成
		hr = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));

		return hr;
	}
}

void DirectDrawing::BaseDrawGraphics()
{
	static auto* cmdList = DirectXInit::GetCommandList();

	// ビューポート領域の設定
	cmdList->RSSetViewports(
		1,
		&CD3DX12_VIEWPORT(
			0.0f,
			0.0f,
			static_cast<float>(DirectXInit::GetInstance()->windowWidth),
			static_cast<float>(DirectXInit::GetInstance()->windowHeight)
		)
	);

	// シザー矩形の設定
	cmdList->RSSetScissorRects(
		1,
		&CD3DX12_RECT(
			0,
			0,
			DirectXInit::GetInstance()->windowWidth,
			DirectXInit::GetInstance()->windowHeight
		)
	);
}

void DirectDrawing::BaseDrawSpriteGraphics()
{
	static auto* cmdList = DirectXInit::GetCommandList();

	// ビューポート領域の設定
	cmdList->RSSetViewports(
		1,
		&CD3DX12_VIEWPORT(
			0.0f,
			0.0f,
			static_cast<float>(DirectXInit::GetInstance()->windowWidth),
			static_cast<float>(DirectXInit::GetInstance()->windowHeight)
		)
	);

	// シザー矩形の設定
	cmdList->RSSetScissorRects(
		1,
		&CD3DX12_RECT(
			0,
			0,
			DirectXInit::GetInstance()->windowWidth,
			DirectXInit::GetInstance()->windowHeight
		)
	);

	// プリミティブ形状の設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

int DirectDrawing::SetDrawBlendMode(int blendMode)
{
	if (blendMode < ShaderManager::BlendMode::NOBLEND ||
		blendMode > ShaderManager::BlendMode::INV)
	{
		return Engine::FUNCTION_ERROR;
	}

	ShaderManager::blendMode = static_cast<ShaderManager::BlendMode>(blendMode);

	return static_cast<int>(ShaderManager::blendMode);
}
