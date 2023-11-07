#include "Effect.h"
#include "../Game/Character/Player.h"
Effect::Effect(const char* model, const CVector3D& pos, float scale, bool builbord, int time) :Task(TaskType::eEffect)
	,m_scale(scale)
	,m_builbord(builbord)
	,m_time(time)
{
	m_pos = pos;
	m_model = COPY_RESOURCE(model, CModelObj);
}

Effect::~Effect()
{
}

void Effect::Update()
{
	if (m_time > 0) m_time -= CFPS::GetDeltaTime();
	if (m_time == 0) Kill();
}

void Effect::Render()
{
	CLight::SetLighting(false);
	glDepthMask(GL_FALSE);
	m_model.SetPos(m_pos);
	m_model.SetScale(m_scale, m_scale, m_scale);
	if (m_builbord) {
		m_model.RenderBuilbord(CCamera::GetCurrent()->GetBuilbordMatrix());
	}
	else
	{
		m_model.Render();
	}
	glDepthMask(GL_TRUE);
	CLight::SetLighting(true);
}

//Effect2D—p
Effect2D::Effect2D(const char* img, const CVector2D& pos):Task(TaskType::eUI)
{
	m_pos2D = pos;
	m_img = COPY_RESOURCE(img, CImage);
}

Effect2D::~Effect2D()
{
}

void Effect2D::Update()
{

}

void Effect2D::LateRender2D()
{
	m_img.SetPos(m_pos2D);
	m_img.Draw();
}
