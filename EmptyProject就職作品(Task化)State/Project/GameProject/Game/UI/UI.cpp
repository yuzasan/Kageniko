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

Timer::Timer() : Task(TaskType::eUI)
	,m_ang(0)
	,m_ang2(0)
	,m_time(0)
{
	m_img = COPY_RESOURCE("Needle", CImage);
	m_img2 = COPY_RESOURCE("Needlethin", CImage);
}

Timer::~Timer()
{

}

void Timer::Update() {
	m_time++;
	if (m_time % 60 == 0) {
		if (abs(m_ang) % 360 == 0) {
			m_ang = 0;
			if (abs(m_ang2) % 360 == 0) {
				m_ang2 = 0;
			}
			if (m_time != 60) {
				m_ang2 -= 1;
			}
		}
		m_ang -= 6;
	}
}

void Timer::Draw() {
	m_img.SetAng(DtoR(m_ang2));
	m_img.SetPos(SCREEN_WIDTH - 100, 100);
	m_img.SetCenter(32, 64);
	m_img2.SetAng(DtoR(m_ang));
	m_img2.SetPos(SCREEN_WIDTH - 100, 100);
	m_img2.SetCenter(32, 64);
	Utility::DrawCircle(CVector2D(SCREEN_WIDTH - 100, 100), 80, CVector4D(0.5, 0.0, 1.0, 0.5f));
	m_img.Draw();
	m_img2.Draw();
}

Font::Font(const char* text, const int& d, const CVector2D& pos,const CVector3D& color):Task(TaskType::eUI),m_text("C:\\Windows\\Fonts\\msgothic.ttc", 64)
	,m_texts(text)
	, m_d(d)
	,m_iti(pos)
	,m_color(color)
{
	//m_text.Draw(pos.x,pos.y, color.x, color.y, color.z, text,d);
}

Font::~Font()
{
}

void Font::Update()
{

}

void Font::Draw()
{
	m_text.Draw(m_iti.x, m_iti.y, m_color.x, m_color.y, m_color.z, m_texts, m_d);
}
