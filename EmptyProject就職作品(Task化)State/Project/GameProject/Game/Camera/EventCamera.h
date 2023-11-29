#pragma once
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"

class EnentCamera : public Task {
private:
	//���f���I�u�W�F�N�g
	CVector3D eventcam_at;
	CVector3D eventcam_pos;
	//��]���x�W��
	const float cam_speed = 0.002f;
public:
	EnentCamera();
	~EnentCamera();
	void Update();
	void Render();
};
