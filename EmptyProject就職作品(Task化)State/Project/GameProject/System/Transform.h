#pragma once
#include "GLLibrary.h"

//オブジェクトの座標、回転値、スケール値の制御クラス
class Transform
{
private:
	CVector3D	m_position;		//座標
	CVector3D	m_rotation;		//回転値
	CVector3D	m_scale;		//スケール値

	CMatrix		m_matrix;		//行列
	CMatrix		m_matrixT;		//平行移動行列
	CMatrix		m_matrixR;		//回転行列
	CMatrix		m_matrixS;		//スケール行列

	Transform* m_parent;		//親のポインタ

public:
	//コンストラクタ
	Transform();
	//デストラクタ
	virtual ~Transform();

	//親を設定
	void SetParent(Transform* parent);
	//親を取得
	Transform* GetParent() const;

	//ワールド座標設定
	void SetPos(const CVector3D& worldPos);
	//ワールド座標設定（float版）
	void SetPos(const float x, const float y, const float z);
	//ローカル座標設定
	void SetLocalPos(const CVector3D& localPos);
	//ワールド座標設定（float版）
	void SetLocalPos(const float x, const float y, const float z);
	//ワールド座標取得
	CVector3D GetPos() const;
	//ローカル座標取得
	CVector3D GetLocalPos() const;

	//ワールド回転値設定
	void SetRot(const CVector3D& worldRot);
	//ワールド回転値設定（float版）
	void SetRot(const float x, const float y, const float z);
	//ローカル回転値設定
	void SetLocalRot(const CVector3D& localRot);
	//ローカル回転値設定（float版）
	void SetLocalRot(const float x, const float y, const float z);
	//ワールド回転値取得
	CVector3D GetRot() const;
	//ローカル回転値取得
	CVector3D GetLocalRot() const;

	//ワールドスケール値設定
	void SetScale(const CVector3D& worldScale);
	//ワールドスケール値設定（float版）
	void SetScale(const float x, const float y, const float z);
	//ローカルスケール値設定
	void SetLocalScale(const CVector3D& localScale);
	//ローカルスケール値設定（float版）
	void SetLocalScale(const float x, const float y, const float z);
	//ワールドスケール値取得
	CVector3D GetScale() const;
	//ローカルスケール値取得
	CVector3D GetLocalScale() const;

	//ワールド行列を取得
	CMatrix GetMatrix() const;
	//ワールド平行移動行列を取得
	CMatrix GetMatrixT() const;
	//ワールド回転行列を取得
	CMatrix GetMatrixR() const;
	//ワールドスケール行列を取得
	CMatrix GetMatrixS() const;
};