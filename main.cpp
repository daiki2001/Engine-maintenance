#include "./Header/DirectXInit.h"
#include "./Header/Camera.h"
#include "./Header/Input.h"
#include "./Header/SceneManager.h"

/*�E�B���h�E�T�C�Y*/
constexpr int window_width = 1280; //����
constexpr int window_height = 768; //�c��

constexpr WCHAR title[] = L"DirectXGame"; //�^�C�g��

constexpr float clearColor[] = { 0.1f, 0.875f, 0.875f, 1.0f }; //�w�i�F

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	DirectXInit* w = DirectXInit::GetInstance();
	DirectXInit::EngineDebug = true;

	w->title = title;
	for (UINT i = 0; i < 4; i++)
	{
		w->clearColor[i] = clearColor[i];
	}
	w->SetWindowSize(window_width, window_height);
	w->Init();

	Camera::Init();
	Input::Init();

	SceneManager sceneManager;

	while (true)
	{
		if (w->WindowMessage() == -1) { break; }

		/* �X�V */
		Input::Update();

		sceneManager.Loop();

		if (Input::IsKey(DIK_ESCAPE)) { break; }
	}

	w->WindowEnd();

	return 0;
}