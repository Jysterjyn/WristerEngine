#include "TutorialScene.h"
#include "ShareValue.h"
#include "CollisionManager.h"

using namespace WristerEngine::_2D;

void TutorialScene::Initialize()
{
	stage.Initialize();
	uiDrawer->Initialize();
}

void TutorialScene::Update()
{
	// リセット
	if (OperateConfig::GetInstance()->GetTrigger("Pause"))
	{
		sceneManager->ChangeScene(Scene::Tutorial);
		WristerEngine::Constant::GetInstance()->LoadConstants();
		return;
	}

	if (ShareValue::GetInstance()->isGoal)
	{
		ShareValue::GetInstance()->isGoal = false;
		sceneManager->ChangeScene(Scene::Play);
		return;
	}

	if (ShareValue::GetInstance()->isGameOver)
	{
		sceneManager->ChangeScene(Scene::GameOver);
		return;
	}

	stage.Update();

	uiDrawer->Update();
}

void TutorialScene::Draw()
{
	stage.Draw();
	uiDrawer->Draw();
}

void UIDrawerTutorialScene::Initialize()
{
	// 地面のUIの初期化
	sprites["Ground"] = Sprite::Create("brick.png");
	std::unique_ptr<Sprite>& s = sprites["Ground"];
	s->position.y = WristerEngine::WIN_SIZE.y - Const(float, "GroundHeight");
	s->textureSize.x *= (WristerEngine::WIN_SIZE.x / s->size.x);
	s->textureSize.y *= (Const(float, "GroundHeight") / s->size.y);
	s->size.x = WristerEngine::WIN_SIZE.x;
	s->size.y = Const(float, "GroundHeight");
}
