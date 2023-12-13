#include "UI.h"
#include "../Gamedata/GameData.h"

#define FADE_TIME 1.0f
Fade* Fade::ms_instance = nullptr;

Fade::Fade() : Task(TaskType::eFade, true)
	, m_isFadeOut(false)
	, m_isFade(false)
	, m_alpha(0)
{
	m_time = 0;
}

Fade::~Fade(){
	
}

Fade* Fade::Instance(){
	if (ms_instance == nullptr) {
		ms_instance = new Fade();
	}
	return ms_instance;
}

bool Fade::IsFade()
{
	return Instance()->m_isFade;
}

void Fade::FadeIn(){
	Instance()->m_isFade = true;
	Instance()->m_isFadeOut = false;
	Instance()->m_time = 0.0f;
}

void Fade::FadeOut(){
	Instance()->m_isFade = true;
	Instance()->m_isFadeOut = true;
	Instance()->m_time = 0.0f;
}

void Fade::LateUpdate(){
	if (!m_isFade)return;
	if (!m_isFadeOut) {
		//フェードイン
		m_time += CFPS::GetDeltaTime();
		if (m_time < FADE_TIME) {
			m_alpha = 1.0f - m_time / FADE_TIME;
		}
		else {
			m_isFade = false;
			m_alpha = 0.0f;
		}
	}
	else {
		//フェードアウト
		m_time += CFPS::GetDeltaTime();
		if (m_time < FADE_TIME) {
			m_alpha = m_time / FADE_TIME;
		}
		else {
			m_isFade = false;
			m_alpha = 1.0f;
		}
	}
}

void Fade::Draw(){
	if (m_alpha == 0.0f)return;
	CVector4D color = CVector4D(0.0f, 0.0f, 0.0f, m_alpha);
	Utility::DrawQuad(CVector2D(0, 0), CVector2D(SCREEN_WIDTH, SCREEN_HEIGHT), color);
}
