#include "pch.h"
#include "ClearRecordUI.h"

#include "UIManager.h"
#include "GraphicsManager.h"
#include "SoundManager.h"
#include "FileManager.h"

namespace Heracles
{
	namespace UI
	{
		float record[3] = { FLT_MAX, FLT_MAX, FLT_MAX };
		GraphicsEngineSpace::Canvas* ClearRecordUI::canvas = nullptr;

		ClearRecordUI::ClearRecordUI(std::weak_ptr<GameObject> obj)
			: ComponentBase(obj)
			, currentClearRecord(nullptr)
			, clearRecord{}
		{
			if (canvas != nullptr)
			{
				currentClearRecord = canvas->GetTextUI("CurrentClearRecord");

				for (int i = 0; i < 3; i++)
					clearRecord[i] = canvas->GetTextUI("ClearRecord_" + std::to_string(i + 1));

				canvas->SetEnable(true);

				return;
			}

			canvas = UIManager::GetInstance()->CreateCanvas("ClearRecord_UI");

			// 팩토리 & 리소스 매니저 포인터 참조
			GraphicsEngineSpace::Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();
			GraphicsEngineSpace::ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

			// Board
			GraphicsEngineSpace::TextureBase* board = resourceManager->GetTexture("RecordBG");

			if (board == nullptr)
				board = factory->CreateTexture("RecordBG", "Resources/UI/win_BG.png");

			CreateRecordBoard(board);

			// Clear Record Text
			CreateClearRecordText();

			// Quit Button
			GraphicsEngineSpace::TextureBase* quitButtonHover = resourceManager->GetTexture("CloseButtonHover");

			if (quitButtonHover == nullptr)
				quitButtonHover = factory->CreateTexture("CloseButtonHover", "Resources/UI/OnMouse_Title2.png");

			CreateQuitButton(quitButtonHover);
		}

		ClearRecordUI::~ClearRecordUI()
		{

		}

		void ClearRecordUI::Awake()
		{

		}

		void ClearRecordUI::Start()
		{

		}

		void ClearRecordUI::PreUpdate(float tick)
		{

		}

		void ClearRecordUI::Update(float tick)
		{

		}

		void ClearRecordUI::LateUpdate(float tick)
		{

		}

		void ClearRecordUI::SetRecord(float currentClearRecord)
		{
			if (currentClearRecord < record[2])
			{
				record[2] = currentClearRecord;

				for (int i = 2; i > 0; i--)
				{
					if (currentClearRecord < record[i - 1])
					{
						record[i] = record[i - 1];
						record[i - 1] = currentClearRecord;
					}
				}
			}

			int totalMin = static_cast<int>(currentClearRecord) / 60;
			int totalSec = static_cast<int>(currentClearRecord) % 60;

			std::string timeStr;
			// 자릿수가 하나면 앞에 0 추가
			if (totalMin / 10 == 0)
				timeStr += "0" + std::to_string(totalMin);
			else
				timeStr += std::to_string(totalMin);

			timeStr += " : ";

			if (totalSec / 10 == 0)
				timeStr += "0" + std::to_string(totalSec);
			else
				timeStr += std::to_string(totalSec);

			this->currentClearRecord->SetText(timeStr);

			// Best Record
			for (int i = 0; i < 3; i++)
			{
				if (record[i] == FLT_MAX)
					break;

				int totalMin = static_cast<int>(record[i]) / 60;
				int totalSec = static_cast<int>(record[i]) % 60;

				std::string timeStr = "";
				// 자릿수가 하나면 앞에 0 추가
				if (totalMin / 10 == 0)
					timeStr += "0" + std::to_string(totalMin);
				else
					timeStr += std::to_string(totalMin);

				timeStr += " : ";

				if (totalSec / 10 == 0)
					timeStr += "0" + std::to_string(totalSec);
				else
					timeStr += std::to_string(totalSec);

				clearRecord[i]->SetText(timeStr);
			}
		}

		void ClearRecordUI::CreateRecordBoard(TextureBase* recordBoard)
		{
			auto* board = canvas->CreateTextureUI("ClearRecoardBoard");

			board->SetTexture(recordBoard);
			board->SetPosition({ 50.0f, 0.0f, 0.1f });
			board->SetWidth(2153.0f);
			board->SetHeight(1199.0f);
		}

		void ClearRecordUI::CreateClearRecordText()
		{
			currentClearRecord = canvas->CreateTextUI("CurrentClearRecord");
			currentClearRecord->SetPosition({ 30.0f, 20.0f, 0.08f });
			currentClearRecord->SetFont("MuseumMedium");
			currentClearRecord->SetFontSize(30.0f);
			currentClearRecord->SetText(L" 00 : 00");

			FileManager::GetInstance()->OpenFile("Save/ClearRecord.bin", GameEngineSpace::FileMode::READ);

			for (int i = 0; i < 3; i++)
			{
				clearRecord[i] = canvas->CreateTextUI("ClearRecord_" + std::to_string(i + 1));
				clearRecord[i]->SetPosition({ 60.0f, 218.0f + i * 95.0f, 0.08f });
				clearRecord[i]->SetFont("MuseumMedium");
				clearRecord[i]->SetFontSize(30.0f);
				clearRecord[i]->SetText(L"기록 없음  ");

				float savedRecord = 0.0f;

				if (FileManager::GetInstance()->GetFloat(savedRecord) == true)
					record[i] = savedRecord;
			}

			FileManager::GetInstance()->CloseFile();
		}

		void ClearRecordUI::CreateQuitButton(TextureBase* hoverTexture)
		{
			auto* quitText = canvas->CreateTextUI("GoToTitleText");
			quitText->SetPosition({ 0.f, 480.f, 0.08f });
			quitText->SetFont("MuseumMedium");
			quitText->SetFontSize(20.f);
			quitText->SetText(L"타이틀로");

			auto* quitButton = canvas->CreateButtonUI("GoToTitleButton");
			quitButton->SetParent(quitText);
			quitButton->SetPosition({ 20.0f, 5.0f, -0.01f });
			quitButton->SetHoverTexture(hoverTexture);
			quitButton->SetWidth(150.0f);
			quitButton->SetHeight(58.0f);
			quitButton->SetEnterEvent([]()
				{
					SoundManager::GetInstance()->PlayEffect("UIHover");
				});
			quitButton->SetClickEvent([]()
				{
					SoundManager::GetInstance()->PlayEffect("AltarClose");

					// 여기서 메인 메뉴 로드 씬..
					SceneManager::GetInstance()->LoadScene(TEXT("MainMenu"));

					FileManager::GetInstance()->OpenFile("Save/ClearRecord.bin", GameEngineSpace::FileMode::WRITE);

					for (int i = 0; i < 3; i++)
					{
						if (record[i] != FLT_MAX)
							FileManager::GetInstance()->SetFloat(record[i]);
					}

					FileManager::GetInstance()->CloseFile();

					canvas->SetEnable(false);
				});
		}
	}
}