#pragma once
#include "../../TaskSystem/Task.h"
#include "../../System/Transform.h"

class ObjectBase :public Task,public Transform{
public:
	//コンストラクタ
	ObjectBase(TaskType type);
	//デストラクタ
	virtual ~ObjectBase();
};