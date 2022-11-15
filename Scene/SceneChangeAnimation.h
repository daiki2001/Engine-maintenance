#pragma once
#include "./Header/DrawPolygon.h"

class SceneChangeAnimation
{
private: //静的メンバ変数
	static DrawPolygon* draw;

public: //静的メンバ関数
	static void StaticInit(DrawPolygon* draw) { SceneChangeAnimation::draw = draw; }

private: //メンバ変数
	bool isAnimation; //シーン遷移のアニメーションが行われているかどうか
	bool isChange;    //シーン遷移するかどうか

	int radius; //半径

	int maskScreen; //マスク用のスクリーン
	int tempScreen; //一時保存用のスクリーン

	int circle; //遷移アニメーション用オブジェクト
	int sound;  //SE

public: //メンバ関数
	SceneChangeAnimation(DrawPolygon* draw);
	~SceneChangeAnimation();

	void Init();
	void Update();
	void Draw();
	void Start();

	void Load();
	void Release();

	bool GetAnimation() { return isAnimation; }
	bool GetChange() { return isChange; }
private:
	void ScreenInit();
};
