#include "EventCamera.h"
//static は最初の#includeのところに書く
bool EventCamera::eventrendring = false;

EventCamera::EventCamera() :Task(TaskType::eEventCamera)
	,eventcam_pos(CVector3D::zero)
	,eventcam_at(CVector3D::zero)
{
	
}

EventCamera::~EventCamera(){
	eventrendring = false;
}

void EventCamera::Update(){
	eventcam_at.y += 0.03f;
	eventcam_at.x += 0.02f;
	//Kキー
	if (PUSH(CInput::eButton12)) {
		Kill();
	}
}

void EventCamera::Render(){
	{
		eventrendring = true;
		
		eventcam_pos = eventcam_at + CMatrix::MRotation(m_rot).GetFront();
		//透視投影行列で設定
		CCamera::GetCurrent()->LookAt(eventcam_pos, eventcam_at, CVector3D(0, 1, 0));
	}
}
