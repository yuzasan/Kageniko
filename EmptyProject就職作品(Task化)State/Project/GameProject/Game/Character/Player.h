#pragma once
#include "CharaBase.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class PlayerCamera;
class Enemy;

class Player : public CharaBase {
private:
	CModelA3M m_model;			//���f���f�[�^
	CVector3D m_moveDir;		//�ړ�����
	CVector3D m_rot_target;
	//Task* mp_camera;
	PlayerCamera* mp_camera;
	Enemy* mp_enemy;
	Task* mp_isenemy;
	Task* mp_tyukan;
	Task* mp_fellbox;
	float m_viewAngle;			//����p�x
	float m_viewLength;			//���싗��
	int m_gauge;				//�F
	CVector3D m_tyukan;			//���ԍ��W
	float m_elapsedTime;		//�ҋ@�o�ߎ���
	CVector4D color;

	//�A�j���[�V�����ԍ�
	enum class AnimId 
	{
		Idle,	//�ҋ@
		Walk,	//����
	};

	//���
	enum class State
	{
		Move,		//�ړ����
		Invisible,	//�������	
	};
	State m_state;	//���݂̏��

	//�ړ���Ԃ̏���
	void StateMove();
	//������Ԃ̏���
	void StateInvisible();

public:
	bool m_isSearch;	//�T���͈͂ɂ��邩�ǂ���
	//bool m_isFind;
	Player(const CVector3D& pos);
	~Player();
	void Update();
	void Render();
	/// <summary>
	/// enemy�ȊO�̕`�揈��
	/// </summary>
	void NoEnemyRender();
	// �G�����������ǂ���
	bool IsFoundEnemy() const;
	//���ݒn����G�����邩
	bool IsSearchEnemy() const;
	void Shot();
	//�f�o�b�O����
	void Debug();
	void Collision(Task* b);
	void LateUpdate();
};