#pragma once

class SceneChangeAnimation
{
private: //メンバ変数
	bool isAnimation; //シーン遷移のアニメーションが行われているかどうか
	bool isChange;    //シーン遷移するかどうか

	int radius; //半径

	int maskScreen; //マスク用のスクリーン
	int tempScreen; //一時保存用のスクリーン

	int circle; //遷移アニメーション用オブジェクト
	int sound;  //SE

public: //メンバ関数
	SceneChangeAnimation();
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
