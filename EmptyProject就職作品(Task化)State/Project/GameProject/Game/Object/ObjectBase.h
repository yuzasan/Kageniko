#pragma once
#include "../../TaskSystem/Task.h"
#include "../../System/Transform.h"

class ObjectBase :public Task,public Transform{
public:
	//�R���X�g���N�^
	ObjectBase(TaskType type);
	//�f�X�g���N�^
	virtual ~ObjectBase();
};