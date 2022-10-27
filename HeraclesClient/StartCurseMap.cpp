#include "pch.h"
#include "StartCurseMap.h"

#include "AltarState.h"
#include "GameObjectTemplate.h"
#include "PlayerCamera.h"

#include "GameState.h"
#include "SceneController.h"
#include "Entrance.h"
#include "IngameUI.h"
#include "SoundManager.h"
#include "BoxCollider.h"

#include "UIManager.h"
#include "DialogUI.h"
#include "SettingUI.h"
#include "TutorialUI.h"
#include "Timer.h"

namespace Heracles
{
	using namespace InGameObject;

	StartCurseMap::StartCurseMap(tstring sceneName) :SceneBase(sceneName)
	{
	}
	void StartCurseMap::BuildScene()
	{
		GameState::GetInstance()->mapCleared = true;
		GameState::GetInstance()->currMonsterNum = 0;
		GameState::GetInstance()->clearedMapNum = 0;

		SetSkyBox(TEXT("SkyBox"));

		SetIBL(TEXT("MSIBL"), TEXT("Resources/Texture/SunSubMixer_specularIBL.dds"), TEXT("Resources/Texture/SunSubMixer_diffuseIBL.dds"));

		std::shared_ptr<GameObject> player = CreatePlayer(this);

		if (GameState::GetInstance()->exitType)
		{
			player->GetComponent<Transform>()->SetPosition(this->SpawnA, Space::WORLD);
		}
		else
		{
			player->GetComponent<Transform>()->SetPosition(this->SpawnB, Space::WORLD);
		}

		auto camera = CreateCamera(this);
		camera->GetComponent<Player::PlayerCamera>()->SetTarget(player->GetComponent<Transform>());

		this->sceneController = CreateSceneController(this);
		this->sceneController->GetComponent<SceneController>()->SelectNextMap();

		this->CreateWallCollider();
		this->SetMonsterWave();
		this->CreateObjects();
		this->CreateUI();
		this->CreatePointLights();
		SoundManager::GetInstance()->PlayBGM("StartCurseSceneMap");

		// 기본 대화 상태
		auto dialog = CreateEmpty();
		dialog->SetName(TEXT("DialogUI"));
		dialog->SetTag(TEXT("UI"));

		// 먼저 만들어야.. preUpdate를 먼저합니다.
		auto tutorial = CreateEmpty();
		tutorial->SetName(TEXT("TutorialUI"));
		tutorial->SetTag(TEXT("UI"));

		auto altar = CreateAltar(this);
		
		// 죽었을 때 맵 번호 넣기.
		dialog->AddComponent<UI::DialogUI>()->StartNowDialog(GameState::GetInstance()->dieStage);

		// altar에 캐싱
		tutorial->AddComponent<UI::TutorialUI>()->SetDialog(dialog->GetComponent<UI::DialogUI>());
		altar->GetComponent<Altar::AltarState>()->SetDialog(dialog->GetComponent<UI::DialogUI>());
		altar->GetComponent<Altar::AltarState>()->SetTutorial(tutorial->GetComponent<UI::TutorialUI>());

		// 게임의 시작이기 때문에 타이머를 리셋 해준다..?
		Timer::GetInstance()->Reset();
	}
	void StartCurseMap::CreateWallCollider()
	{
		CreateWall(this, Vector(0.f, 0.f, 20.f), 80.f, 1.f);
		CreateWall(this, Vector(0.f, 0.f, -20.f), 80.f, 1.f);
		CreateWall(this, Vector(-40.f, 0.f, 0.f), 1.f, 40.f);
		CreateWall(this, Vector(40.f, 0.f, 0.f), 1.f, 40.f);
		CreateWall(this, Vector(0.f, 0.f, 12.f), 30.f, 16.f);
		CreateWall(this, Vector(0.f, 0.f, -12.f), 30.f, 16.f);
	}
	void StartCurseMap::SetMonsterWave()
	{
	}
	void StartCurseMap::CreateObjects()
	{
		// 시작 맵
		// 제단 -20 0 0 에 생성
			// => 현재 맵에서 바로 뽑아와볼 것이라서.. 모델 위치 0, 0, 0 박스 콜라이더 위치 -20으로 조정..
			
		std::shared_ptr<Entrance> ent1 = nullptr;
		if (GameState::GetInstance()->clearedMapNum == 2)
		{
			ent1 = CreateEntranceWithIndex(this, this->ExitA, 3)->GetComponent<Entrance>();
		}
		else
		{
			ent1 = CreateEntranceWithIndex(this, this->ExitA, GameState::GetInstance()->rewardIdx1)->GetComponent<Entrance>();
		}
		
		ent1->GetGameObject()->GetComponent<Transform>()->SetRotation(Vector(0.f, 90.f, 0.f), Space::LOCAL);
		ent1->GetGameObject()->GetComponent<Transform>()->SetPosition(ent1->GetGameObject()->GetComponent<Transform>()->GetWorldPosition() + Vector(0.f, 0.f, 2.f), Space::WORLD);

		ent1->GetGameObject()->GetComponent<BoxCollider>()->SetLocalCenter(Vector(-5.f, 0.f, -8.f));
		ent1->SetNextScene(GameState::GetInstance()->GetNextMap1());
		ent1->SetEntranceType(false);

		auto ent2 = CreateEntranceWithIndex(this, this->ExitB, GameState::GetInstance()->rewardIdx2)->GetComponent<Entrance>();
		ent2->GetGameObject()->GetComponent<Transform>()->SetPosition(ent2->GetGameObject()->GetComponent<Transform>()->GetWorldPosition() + Vector(-14.5f, 0.f, -4.f), Space::WORLD);
		ent2->GetGameObject()->GetComponent<BoxCollider>()->SetLocalCenter(Vector(8.f, 0.f, -5.f));
		ent2->SetNextScene(GameState::GetInstance()->GetNextMap2());
		ent2->SetEntranceType(true);

		std::shared_ptr<GameObject> map = CreateEmpty();

		map->AddComponent<MeshRenderer>()->Init("AltarMapFloor", "Resources/Model/Stage_40_80_Altar_Floor.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("AltarMapBrazier", "Resources/Model/Stage_40_80_Altar_Brazier.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("AltarMapFloorFrame", "Resources/Model/Stage_40_80_Altar_FloorFrame.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("AltarMapPot", "Resources/Model/Stage_40_80_Altar_Pot.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("AltarMapTapestry", "Resources/Model/Stage_40_80_Altar_Tapestry.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("AltarMapWall", "Resources/Model/Stage_40_80_Altar_Wall.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("AltarMapMemoryGem", "Resources/Model/Stage_40_80_Altar_MemoryGem.ase", RendererBase::ModelType::NONE_AO);
		map->AddComponent<MeshRenderer>()->Init("AltarMapAltar", "Resources/Model/Stage_40_80_Altar_Altar.ase", RendererBase::ModelType::NONE_AO);

		
	}
	void StartCurseMap::CreateUI()
	{
		std::shared_ptr<GameObject> ui = CreateEmpty();
		ui->AddComponent<UI::InGameUI>()->SetMaxHP(100.0f);
		ui->GetComponent<UI::InGameUI>()->SetHP(100.0f);

		ui->GetComponent<UI::InGameUI>()->SetCurse("Pain");
		ui->GetComponent<UI::InGameUI>()->SetCurse("Blind");
		ui->GetComponent<UI::InGameUI>()->SetCurse("Madness");

		ui->GetComponent<UI::InGameUI>()->SetEquipment("MasterSword");
		ui->GetComponent<UI::InGameUI>()->SetSwitching("AtenaSword");
		ui->GetComponent<UI::InGameUI>()->Swap();
		ui->GetComponent<UI::InGameUI>()->SetMemories(0);

		ui->AddComponent<UI::SettingUI>()->SetBackButtonEnable(true);
	}

	void StartCurseMap::CreatePointLights()
	{
		float r = 0xff;
		float g = 0x63;
		float b = 0x49;

		float intensity = 1.0f;

		GraphicsEngineSpace::PointLight* pLight1 = CreatePointLight();
		pLight1->position = { 28.0f, 4.693f, 8.0f };
		pLight1->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight1->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight2 = CreatePointLight();
		pLight2->position = { 28.0f, 4.693f, -8.0f };
		pLight2->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight2->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight3 = CreatePointLight();
		pLight3->position = { -22.0f, 4.693f, -17.0f };
		pLight3->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight3->intensity = intensity;

		GraphicsEngineSpace::PointLight* pLight4 = CreatePointLight();
		pLight4->position = { -30.0f, 4.693f, 17.0f };
		pLight4->color = { r / 255.0f, g / 255.0f, b / 255.0f };
		pLight4->intensity = intensity;
	}
}