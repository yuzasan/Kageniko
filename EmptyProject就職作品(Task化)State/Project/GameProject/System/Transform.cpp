#include "Transform.h"

//�R���X�g���N�^
Transform::Transform()
	: m_parent(nullptr)
{
	//���W�A��]�l�A�X�P�[���l��������
	SetPos(CVector3D::zero);
	SetRot(CVector3D::zero);
	SetScale(CVector3D::one);
}

//�f�X�g���N�^
Transform::~Transform()
{
}

//�e��ݒ�
void Transform::SetParent(Transform* parent)
{
	//���ɐݒ肳��Ă���e�ł���Ώ������Ȃ�
	if (parent == m_parent) return;

	//���[���h�l���L�����Ă���
	CVector3D pos = GetPos();
	CVector3D rot = GetRot();
	CVector3D scale = GetScale();

	//�e�ύX
	m_parent = parent;

	//���[���h�l��ݒ肵�Ȃ�����
	//�e�q�֌W�̌v�Z���s��
	SetPos(pos);
	SetRot(rot);
	SetScale(scale);
}

//�e���擾
Transform* Transform::GetParent() const
{
	return m_parent;
}

//���[���h���W�ݒ�
void Transform::SetPos(const CVector3D& worldPos)
{
	//�e������
	if (m_parent != nullptr)
	{
		//�e�̋t�s����|���邱�Ƃ�
		//�e���猩�����ΓI�ȁi���[�J���j���W�ɕϊ�
		CMatrix im = m_parent->GetMatrix().GetInverse();
		m_position = im * worldPos;
	}
	//�e�����݂��Ȃ�
	else
	{
		//���̂܂ܐݒ�
		m_position = worldPos;
	}
}

//���[���h���W�ݒ�ifloat�Łj
void Transform::SetPos(const float x, const float y, const float z)
{
	SetPos(CVector3D(x, y, z));
}

//���[�J�����W�ݒ�
void Transform::SetLocalPos(const CVector3D& localPos)
{
	//���[�J���ȍ��W�͂��̂܂ܐݒ�
	m_position = localPos;
}

//���[�J�����W�ݒ�ifloat�Łj
void Transform::SetLocalPos(const float x, const float y, const float z)
{
	SetLocalPos(CVector3D(x, y, z));
}

//���[���h���W�擾
CVector3D Transform::GetPos() const
{
	CVector3D pos = m_position;
	//�e������
	if (m_parent != nullptr)
	{
		//�e�̍s����|���āA�q�̃��[���h���W���Z�o
		pos = m_parent->GetMatrix() * pos;
	}
	return pos;
}

//���[�J�����W�擾
CVector3D Transform::GetLocalPos() const
{
	return m_position;
}

//���[���h��]�l�ݒ�
void Transform::SetRot(const CVector3D& worldRot)
{
	CMatrix m = CMatrix::MRotation(worldRot);
	//�e������
	if (m_parent != nullptr)
	{
		//�e�̉�]�s��̋t�s����|���邱�Ƃ�
		//�e���猩�����ΓI�ȁi���[�J���j��]�s��ɕϊ�
		m = m_parent->GetMatrixR().GetInverse() * m;
	}
	m_rotation = m.GetEuler(CMatrix::eZYX);
}

//���[���h��]�l�ݒ�ifloat�Łj
void Transform::SetRot(const float x, const float y, const float z)
{
	SetRot(CVector3D(x, y, z));
}

//���[�J����]�l�ݒ�
void Transform::SetLocalRot(const CVector3D& localRot)
{
	//���[�J���ȉ�]�l�͂��̂܂ܐݒ�
	CMatrix m = CMatrix::MRotation(localRot);
	m_rotation = m.GetEuler(CMatrix::eZYX);
}

//���[�J����]�l�ݒ�ifloat�Łj
void Transform::SetLocalRot(const float x, const float y, const float z)
{
	SetLocalRot(CVector3D(x, y, z));
}

//���[���h��]�l�擾
CVector3D Transform::GetRot() const
{
	CVector3D rot = m_rotation;
	//�e������
	if (m_parent != nullptr)
	{
		//�e�̍s����|���āA�q�̃��[���h��]�s����Z�o
		CMatrix m = m_parent->GetMatrix() * CMatrix::MRotation(rot);
		rot = m.GetEuler(CMatrix::eZYX);
	}
	return rot;
}

//���[�J����]�l�擾
CVector3D Transform::GetLocalRot() const
{
	return m_rotation;
}

//���[���h�X�P�[���l�ݒ�
void Transform::SetScale(const CVector3D& worldScale)
{
	//�e������
	if (m_parent != nullptr)
	{
		//�e�̃X�P�[���s��̋t�s����|���邱�Ƃ�
		//�e���猩�����ΓI�ȁi���[�J���j�X�P�[���l�ɕϊ�
		CMatrix m = m_parent->GetMatrixS().GetInverse();
		m_scale = m * worldScale;
	}
	//�e�����݂��Ȃ�
	else
	{
		//���̂܂ܐݒ�
		m_scale = worldScale;
	}
}

//���[���h�X�P�[���l�ݒ�ifloat�Łj
void Transform::SetScale(const float x, const float y, const float z)
{
	SetScale(CVector3D(x, y, z));
}

//���[�J���X�P�[���l�ݒ�
void Transform::SetLocalScale(const CVector3D& localScale)
{
	m_scale = localScale;
}

//���[�J���X�P�[���l�ݒ�ifloat�Łj
void Transform::SetLocalScale(const float x, const float y, const float z)
{
	SetLocalScale(CVector3D(x, y, z));
}

//���[���h�X�P�[���l�擾
CVector3D Transform::GetScale() const
{
	CVector3D scale = m_scale;
	//�e������
	if (m_parent != nullptr)
	{
		//�e�̃X�P�[���s����|���āA
		//�q�̃��[���h�ł̃X�P�[���l���Z�o
		CMatrix m = m_parent->GetMatrixS();
		scale = m * scale;
	}
	return scale;
}

//���[�J���X�P�[���l�擾
CVector3D Transform::GetLocalScale() const
{
	return m_scale;
}

//���[���h�s����擾
CMatrix Transform::GetMatrix() const
{
	//���g�̃��[�J���s����Z�o
	//�i���s�ړ��s��~��]�s��~�X�P�[���s��j
	CMatrix m =
		CMatrix::MTranselate(m_position) *
		CMatrix::MRotation(m_rotation) *
		CMatrix::MScale(m_scale);

	//�e�����݂���ꍇ��
	if (m_parent != nullptr)
	{
		//�e�̃��[���h�s����|���邱�Ƃ�
		//�q�̃��[�J���s������[���h�s��ɕϊ�
		m = m_parent->GetMatrix() * m;
	}

	return m;
}

//���[���h���s�ړ��s����擾
CMatrix Transform::GetMatrixT() const
{
	//���[�J���ȍs����擾
	CMatrix m = CMatrix::MTranselate(m_position);
	//�e������
	if (m_parent != nullptr)
	{
		//�e�̃��[���h�s����|���āA
		//�q�̃��[���h�s��ɕϊ�
		m = m_parent->GetMatrix() * m;
	}
	return m;
}

//���[���h��]�s����擾
CMatrix Transform::GetMatrixR() const
{
	//���[�J���ȍs����擾
	CMatrix m = CMatrix::MRotation(m_rotation);
	//�e������
	if (m_parent != nullptr)
	{
		//�e�̃��[���h�s����|���āA
		//�q�̃��[���h�s��ɕϊ�
		m = m_parent->GetMatrixR() * m;
	}
	return m;
}

//���[���h�X�P�[���s����擾
CMatrix Transform::GetMatrixS() const
{
	//���[�J���ȍs����擾
	CMatrix m = CMatrix::MScale(m_scale);
	//�e������
	if (m_parent != nullptr)
	{
		//�e�̃��[���h�s����|���āA
		//�q�̃��[���h�s��ɕϊ�
		m = m_parent->GetMatrixS() * m;
	}
	return m;
}