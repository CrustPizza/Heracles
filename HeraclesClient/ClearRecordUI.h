#pragma once

namespace Heracles
{
	namespace UI
	{
		class Canvas;
		using GraphicsEngineSpace::TextureBase;

		class ClearRecordUI : public ComponentBase
		{
			static GraphicsEngineSpace::Canvas* canvas;

			GraphicsEngineSpace::TextUI* currentClearRecord;
			GraphicsEngineSpace::TextUI* clearRecord[3];

		public:
			ClearRecordUI(std::weak_ptr<GameObject> obj);
			~ClearRecordUI();

			void Awake() override;
			void Start() override;
			void PreUpdate(float tick) override;
			void Update(float tick) override;
			void LateUpdate(float tick) override;

			void SetRecord(float currentClearRecord);

		private:
			void CreateRecordBoard(TextureBase* recordBoard);
			void CreateClearRecordText();
			void CreateQuitButton(TextureBase* hoverTexture);
		};
	}
}