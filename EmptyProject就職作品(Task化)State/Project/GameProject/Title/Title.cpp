#include "Title.h"
#include "../Game/Game.h"
//#include "../Base/Base.h"

Title::Title():Task(TaskType::eScene), m_title_text("C:\\Windows\\Fonts\\msgothic.ttc", 64) {
	m_img = COPY_RESOURCE("Title", CImage);
}

Title::~Title(){
	//�S�ẴI�u�W�F�N�g��j��
	//Base::KillALL();
	//�Q�[���V�[����
	//Base::Add(new Game());
	
	//�S�ẴI�u�W�F�N�g��j��
	Task::Kill();
	//�Q�[���V�[����
	new Game();
}

void Title::Update() 
{	//���[�h�I���`�F�b�N
	if (CLoadThread::GetInstance()->CheckEnd()) {
		//�������Ă���
		//���N���b�N
		if (PUSH(CInput::eMouseL)) {
			Kill();
		}
	}
}

void Title::Render()
{
	m_img.Draw();
	if (CLoadThread::GetInstance()->CheckEnd()) {
		//�������Ă���
		//�����\��
		m_title_text.Draw(800, 800, 0, 0, 0, "���N���b�N");
	}
}
