#pragma once
#include "CharaBase.h"
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class Player;
class NavNode;
class SearchNode;

class Enemy : public CharaBase {
	//����̊p�x
	float view_ang = DtoR(35);
	//����̋���
	float view_length = 5;
public:
	/*struct CourseData {
		CVector3D * points;
		int size;
	};*/
private:
	Player* mp_player;				//�v���C���[
	Task* mp_field;					//�X�e�[�W
	CModelA3M m_model;				//���f��
	CVector3D m_moveDir;			//�ړ�����
	CVector3D m_movePos;			//�ړ�����W
	NavNode* m_moveNode;			//�ړ��ړI�n�̃m�[�h
	NavNode* m_nextNode;			//���Ɉړ�����m�[�h�̊i�[�p
	NavNode* m_lostNode;			//�v���C���[�����������ꏊ�ɔz�u�����m�[�h
	SearchNode* m_searchNode;		//�T�����̃m�[�h
	CVector4D color;
	float m_elapsedTime;	//�ҋ@�o�ߎ���
	CVector3D m_pos_old;
	
	CVector3D m_rot_target;
	//const CourseData* mp_cource;	//�ʂ铹
	//int		m_cource_idx;

	//�A�j���[�V�����ԍ�
	enum class AnimId
	{
		Idle,	//�ҋ@
		Walk,	//����
	};

	//���
	enum class State
	{
		Idle,	//�ҋ@
		Move,	//�ړ�
		Chase,	//�ǐ�
		Lost,	//�v���C���[����������
	};
	State m_state;	//���݂̏��

	//�ҋ@��Ԃ̏���
	void StateIdle();
	//�ړ���Ԃ̏���
	void StateMove();
	//�ǐՏ�Ԃ̏���
	void StateChase();
	//�v���C���[������������Ԃ̏���
	void StateLost();

public:
	bool isFoll;
	int m_cntfoll;
	bool m_isTouch;
	bool m_isWall;
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�o���ʒu</param>
	Enemy(const CVector3D& pos);
	//�f�X�g���N�^
	~Enemy();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Render();
	/// <summary>
	/// �����蔻�茟��
	/// </summary>
	/// <param name="b">�Ώە�</param>
	void Collision(Task* b);
	/// <summary>
	/// ���ʂ̃J�v�Z���Ɛ����Ƃ̔���
	/// </summary>
	/// <param name="lineS">�����n�_</param>
	/// <param name="lineE">�����I�_</param>
	/// <param name="dir">��������</param>
	/// <param name="cross">�ڐG�_</param>
	/// <returns>1�Ȃ�ڐG��������:(0)��ڐG</returns>
	int CollisionLine(const CVector3D& lineS, const CVector3D& lineE, const CVector3D& dir, CVector3D* cross);
	//�v���C���[�����������ǂ���
	bool IsFoundPlayer();
	//�ǂ��Ȃ���
	void Shot();
	//�f�o�b�O����
	void Debug();
	//�A�C�e��
	void FollBox();
	void LateRender();

	//���݈ʒu����v���C���[�����������ǂ���
	bool IsLookPlayer() const;

	//�w����W�ւ̈ړ�����
	bool MoveTo(const CVector3D& target);

	//�T���m�[�h��ݒ�
	void SetSearchNode(SearchNode* node);
	//�T�����̃m�[�h���擾
	SearchNode* GetSearchNode() const;

};