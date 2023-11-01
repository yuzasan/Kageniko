#pragma once
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class NavNode;

//�L�����N�^�[�̃x�[�X
class CharaBase :public Task {
protected:
	bool m_isGround;
	NavNode* m_navNode;
public:
	//�R���X�g���N�^
	CharaBase(TaskType type);
	//�f�X�g���N�^
	virtual ~CharaBase();

	NavNode* GetNavNode() const;

	//�X�V
	void Uptate();

	// �R���W�����`��
	void RenderCol();
};