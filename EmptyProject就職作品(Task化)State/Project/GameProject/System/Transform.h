#pragma once
#include "GLLibrary.h"

//�I�u�W�F�N�g�̍��W�A��]�l�A�X�P�[���l�̐���N���X
class Transform
{
private:
	CVector3D	m_position;		//���W
	CVector3D	m_rotation;		//��]�l
	CVector3D	m_scale;		//�X�P�[���l

	CMatrix		m_matrix;		//�s��
	CMatrix		m_matrixT;		//���s�ړ��s��
	CMatrix		m_matrixR;		//��]�s��
	CMatrix		m_matrixS;		//�X�P�[���s��

	Transform* m_parent;		//�e�̃|�C���^

public:
	//�R���X�g���N�^
	Transform();
	//�f�X�g���N�^
	virtual ~Transform();

	//�e��ݒ�
	void SetParent(Transform* parent);
	//�e���擾
	Transform* GetParent() const;

	//���[���h���W�ݒ�
	void SetPos(const CVector3D& worldPos);
	//���[���h���W�ݒ�ifloat�Łj
	void SetPos(const float x, const float y, const float z);
	//���[�J�����W�ݒ�
	void SetLocalPos(const CVector3D& localPos);
	//���[���h���W�ݒ�ifloat�Łj
	void SetLocalPos(const float x, const float y, const float z);
	//���[���h���W�擾
	CVector3D GetPos() const;
	//���[�J�����W�擾
	CVector3D GetLocalPos() const;

	//���[���h��]�l�ݒ�
	void SetRot(const CVector3D& worldRot);
	//���[���h��]�l�ݒ�ifloat�Łj
	void SetRot(const float x, const float y, const float z);
	//���[�J����]�l�ݒ�
	void SetLocalRot(const CVector3D& localRot);
	//���[�J����]�l�ݒ�ifloat�Łj
	void SetLocalRot(const float x, const float y, const float z);
	//���[���h��]�l�擾
	CVector3D GetRot() const;
	//���[�J����]�l�擾
	CVector3D GetLocalRot() const;

	//���[���h�X�P�[���l�ݒ�
	void SetScale(const CVector3D& worldScale);
	//���[���h�X�P�[���l�ݒ�ifloat�Łj
	void SetScale(const float x, const float y, const float z);
	//���[�J���X�P�[���l�ݒ�
	void SetLocalScale(const CVector3D& localScale);
	//���[�J���X�P�[���l�ݒ�ifloat�Łj
	void SetLocalScale(const float x, const float y, const float z);
	//���[���h�X�P�[���l�擾
	CVector3D GetScale() const;
	//���[�J���X�P�[���l�擾
	CVector3D GetLocalScale() const;

	//���[���h�s����擾
	CMatrix GetMatrix() const;
	//���[���h���s�ړ��s����擾
	CMatrix GetMatrixT() const;
	//���[���h��]�s����擾
	CMatrix GetMatrixR() const;
	//���[���h�X�P�[���s����擾
	CMatrix GetMatrixS() const;
};