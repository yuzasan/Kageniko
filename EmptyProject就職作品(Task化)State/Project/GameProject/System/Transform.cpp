#include "Transform.h"

//コンストラクタ
Transform::Transform()
	: m_parent(nullptr)
{
	//座標、回転値、スケール値を初期化
	SetPos(CVector3D::zero);
	SetRot(CVector3D::zero);
	SetScale(CVector3D::one);
}

//デストラクタ
Transform::~Transform()
{
}

//親を設定
void Transform::SetParent(Transform* parent)
{
	//既に設定されている親であれば処理しない
	if (parent == m_parent) return;

	//ワールド値を記憶しておく
	CVector3D pos = GetPos();
	CVector3D rot = GetRot();
	CVector3D scale = GetScale();

	//親変更
	m_parent = parent;

	//ワールド値を設定しなおして
	//親子関係の計算を行う
	SetPos(pos);
	SetRot(rot);
	SetScale(scale);
}

//親を取得
Transform* Transform::GetParent() const
{
	return m_parent;
}

//ワールド座標設定
void Transform::SetPos(const CVector3D& worldPos)
{
	//親が存在
	if (m_parent != nullptr)
	{
		//親の逆行列を掛けることで
		//親から見た相対的な（ローカル）座標に変換
		CMatrix im = m_parent->GetMatrix().GetInverse();
		m_position = im * worldPos;
	}
	//親が存在しない
	else
	{
		//そのまま設定
		m_position = worldPos;
	}
}

//ワールド座標設定（float版）
void Transform::SetPos(const float x, const float y, const float z)
{
	SetPos(CVector3D(x, y, z));
}

//ローカル座標設定
void Transform::SetLocalPos(const CVector3D& localPos)
{
	//ローカルな座標はそのまま設定
	m_position = localPos;
}

//ローカル座標設定（float版）
void Transform::SetLocalPos(const float x, const float y, const float z)
{
	SetLocalPos(CVector3D(x, y, z));
}

//ワールド座標取得
CVector3D Transform::GetPos() const
{
	CVector3D pos = m_position;
	//親が存在
	if (m_parent != nullptr)
	{
		//親の行列を掛けて、子のワールド座標を算出
		pos = m_parent->GetMatrix() * pos;
	}
	return pos;
}

//ローカル座標取得
CVector3D Transform::GetLocalPos() const
{
	return m_position;
}

//ワールド回転値設定
void Transform::SetRot(const CVector3D& worldRot)
{
	CMatrix m = CMatrix::MRotation(worldRot);
	//親が存在
	if (m_parent != nullptr)
	{
		//親の回転行列の逆行列を掛けることで
		//親から見た相対的な（ローカル）回転行列に変換
		m = m_parent->GetMatrixR().GetInverse() * m;
	}
	m_rotation = m.GetEuler(CMatrix::eZYX);
}

//ワールド回転値設定（float版）
void Transform::SetRot(const float x, const float y, const float z)
{
	SetRot(CVector3D(x, y, z));
}

//ローカル回転値設定
void Transform::SetLocalRot(const CVector3D& localRot)
{
	//ローカルな回転値はそのまま設定
	CMatrix m = CMatrix::MRotation(localRot);
	m_rotation = m.GetEuler(CMatrix::eZYX);
}

//ローカル回転値設定（float版）
void Transform::SetLocalRot(const float x, const float y, const float z)
{
	SetLocalRot(CVector3D(x, y, z));
}

//ワールド回転値取得
CVector3D Transform::GetRot() const
{
	CVector3D rot = m_rotation;
	//親が存在
	if (m_parent != nullptr)
	{
		//親の行列を掛けて、子のワールド回転行列を算出
		CMatrix m = m_parent->GetMatrix() * CMatrix::MRotation(rot);
		rot = m.GetEuler(CMatrix::eZYX);
	}
	return rot;
}

//ローカル回転値取得
CVector3D Transform::GetLocalRot() const
{
	return m_rotation;
}

//ワールドスケール値設定
void Transform::SetScale(const CVector3D& worldScale)
{
	//親が存在
	if (m_parent != nullptr)
	{
		//親のスケール行列の逆行列を掛けることで
		//親から見た相対的な（ローカル）スケール値に変換
		CMatrix m = m_parent->GetMatrixS().GetInverse();
		m_scale = m * worldScale;
	}
	//親が存在しない
	else
	{
		//そのまま設定
		m_scale = worldScale;
	}
}

//ワールドスケール値設定（float版）
void Transform::SetScale(const float x, const float y, const float z)
{
	SetScale(CVector3D(x, y, z));
}

//ローカルスケール値設定
void Transform::SetLocalScale(const CVector3D& localScale)
{
	m_scale = localScale;
}

//ローカルスケール値設定（float版）
void Transform::SetLocalScale(const float x, const float y, const float z)
{
	SetLocalScale(CVector3D(x, y, z));
}

//ワールドスケール値取得
CVector3D Transform::GetScale() const
{
	CVector3D scale = m_scale;
	//親が存在
	if (m_parent != nullptr)
	{
		//親のスケール行列を掛けて、
		//子のワールドでのスケール値を算出
		CMatrix m = m_parent->GetMatrixS();
		scale = m * scale;
	}
	return scale;
}

//ローカルスケール値取得
CVector3D Transform::GetLocalScale() const
{
	return m_scale;
}

//ワールド行列を取得
CMatrix Transform::GetMatrix() const
{
	//自身のローカル行列を算出
	//（平行移動行列×回転行列×スケール行列）
	CMatrix m =
		CMatrix::MTranselate(m_position) *
		CMatrix::MRotation(m_rotation) *
		CMatrix::MScale(m_scale);

	//親が存在する場合は
	if (m_parent != nullptr)
	{
		//親のワールド行列を掛けることで
		//子のローカル行列をワールド行列に変換
		m = m_parent->GetMatrix() * m;
	}

	return m;
}

//ワールド平行移動行列を取得
CMatrix Transform::GetMatrixT() const
{
	//ローカルな行列を取得
	CMatrix m = CMatrix::MTranselate(m_position);
	//親が存在
	if (m_parent != nullptr)
	{
		//親のワールド行列を掛けて、
		//子のワールド行列に変換
		m = m_parent->GetMatrix() * m;
	}
	return m;
}

//ワールド回転行列を取得
CMatrix Transform::GetMatrixR() const
{
	//ローカルな行列を取得
	CMatrix m = CMatrix::MRotation(m_rotation);
	//親が存在
	if (m_parent != nullptr)
	{
		//親のワールド行列を掛けて、
		//子のワールド行列に変換
		m = m_parent->GetMatrixR() * m;
	}
	return m;
}

//ワールドスケール行列を取得
CMatrix Transform::GetMatrixS() const
{
	//ローカルな行列を取得
	CMatrix m = CMatrix::MScale(m_scale);
	//親が存在
	if (m_parent != nullptr)
	{
		//親のワールド行列を掛けて、
		//子のワールド行列に変換
		m = m_parent->GetMatrixS() * m;
	}
	return m;
}