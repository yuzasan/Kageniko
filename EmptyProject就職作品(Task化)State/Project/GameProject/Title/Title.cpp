#include "Title.h"
#include "../Game/Game.h"
//#include "../Base/Base.h"

Title::Title():Task(TaskType::eScene), m_title_text("C:\\Windows\\Fonts\\msgothic.ttc", 64) {
	m_img = COPY_RESOURCE("Title", CImage);
}

Title::~Title(){
	//全てのオブジェクトを破棄
	//Base::KillALL();
	//ゲームシーンへ
	//Base::Add(new Game());
	
	//全てのオブジェクトを破棄
	Task::Kill();
	//ゲームシーンへ
	new Game();
}

void Title::Update() 
{	//ロード終了チェック
	if (CLoadThread::GetInstance()->CheckEnd()) {
		//完了している
		//左クリック
		if (PUSH(CInput::eMouseL)) {
			Kill();
		}
	}
}

void Title::Render()
{
	m_img.Draw();
	if (CLoadThread::GetInstance()->CheckEnd()) {
		//完了している
		//文字表示
		m_title_text.Draw(800, 800, 0, 0, 0, "左クリック");
	}
}
