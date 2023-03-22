#include "TestObject.h"
#include "./Header/Camera.h"
#include "./Header/SafeDelete.h"
#include "./Header/Color.h"
#include "./Header/Error.h"

const float TestObject::sphereRadius = 3.0f;

TestObject* TestObject::Get()
{
	static TestObject instance;
	return &instance;
}

TestObject::TestObject() :
	OBJData(),
	draw(nullptr),
	object(-1),
	isObj(false)
{
}

TestObject::~TestObject()
{
	safe_delete(draw);

	OBJData::~OBJData();
}

void TestObject::Init()
{
	position = Camera::target;
}

void TestObject::Update()
{

}

void TestObject::Draw()
{
	static OBJData* objData = nullptr;
	objData = draw->GetObjData(object);

	if (isObj)
	{
		draw->DrawOBJ(object, objData->position, objData->rotation, objData->scale);
	}
	else
	{
		draw->Draw(object, objData->position, objData->rotation, objData->scale, objData->color);
	}
}

int TestObject::CreateModel(const char* filePath, const char* directoryPath)
{
	if (object != -1)
	{
		return 0;
	}

	if (draw == nullptr)
	{
		Engine::ErrorLog("描画用オブジェクトがありません\n", true);
		return Engine::FUNCTION_ERROR;
	}

	if (filePath == nullptr || directoryPath == nullptr)
	{
		object = draw->CreateSphere(sphereRadius, 16);
		isObj = false;
	}
	else
	{
		object = draw->CreateOBJModel(filePath, directoryPath);
		isObj = true;
	}

	return 0;
}

void TestObject::OnCollision(const CollisionInfo& info)
{
	draw->DrawString(0.0f, 0.0f, 1.0f, Color::AddAlphaValue(Color::WHITE, 1.0f), "Hit");
}
