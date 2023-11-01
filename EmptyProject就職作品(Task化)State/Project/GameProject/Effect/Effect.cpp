#include "Effect.h"

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
