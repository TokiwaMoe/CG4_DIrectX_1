#pragma once
#include"Title.h"
#include"GameScene.h"

class SceneManegar
{
public:
	void Initialize();
	void Update();
	void Draw();

private:
	Title* title;
	GameScene* gameScene;
	/*Clear* cleaer;
	Over* over;*/
};
