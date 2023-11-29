#pragma once
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"

class EnentCamera : public Task {
private:
	//モデルオブジェクト
	CVector3D eventcam_at;
	CVector3D eventcam_pos;
	//回転速度係数
	const float cam_speed = 0.002f;
public:
	EnentCamera();
	~EnentCamera();
	void Update();
	void Render();
};
