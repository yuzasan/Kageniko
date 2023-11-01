#pragma once

//タスクの種類
//上から順番に更新、描画される
enum class TaskType {
	//3D
	eCamera,		//メインカメラ
	eField,			//フィールド
	eFellBox,		//デスゾーン
	eBox,			//すり抜けボックス
	eRotateBox,		//回転ボックス
	eMoveBox,		//移動ボックス
	eHideBox,		//隠れボックス
	eTyukanBox,		//中間ボックス
	eEnemyManager,	//敵管理
	eAppearPoint,	//移動点
	ePlayer,		//プレイヤー
	eEnemy,			//敵
	ePlayer_Attack,	//プレイヤー攻撃
	eEnemy_Attack,	//敵攻撃
	eBullet,		//弾
	eEffect,		//エフェクト
	eItem,			//アイテム
	eFieldCamera,	//ミニマップ

	//2D
	eUI,			//UI
	eScene,			//シーン
	eDebug,			//デバッグ

	eStart2D=eUI,	//=の部分から2D
};

//タスククラス
class Task {
private:
	TaskType m_type;	//タスクの種類
	//元の場所
	//bool m_isKill;		//削除フラグ
public:
	//TaskType	m_type;		//オブジェクトの種類
	CVector3D	m_pos;		//座標
	CVector3D	m_rot;		//回転値
	CVector3D	m_vec;		//移動ベクトル
	CVector3D	m_dir;		//前方向

	//球、カプセル用
	float		m_rad;		//半径
	//カプセル用
	CVector3D	m_lineS;	//線分始点
	CVector3D	m_lineE;	//線分終点
	//OBB用
	COBB m_obb;				//Box
	float m_time;			//時間経過
	bool m_isGround;		//着地フラグ
	bool m_isHide;			//HideBoxに隠れているか
	bool m_isFind;			//PlayerがEnemyに触っているか
	bool m_isFindplayer;	//Enemyの探知範囲以内にPlayerがいるか


	//攻撃の連続ヒット防止措置
	int m_hit_no;			//攻撃ヒット側番号
	int m_attack_no;		//攻撃番号
	bool m_isKill;			//削除フラグ
	//コンストラクタ
	Task(TaskType type);
	//デストラクタ
	virtual ~Task();

	//タスクの種類を取得
	TaskType GetType() const;

	//オブジェクトが持つモデルの取得
	virtual CModel* GetModel();

	//削除フラグを取得
	bool IsKill() const;
	//タスクを削除
	void Kill();
	
	//更新
	virtual void Update();
	//描画
	virtual void Render();
	//enemy以外の描画処理
	virtual void NoEnemyRender();
	//描画後に描画
	virtual void LateRender();
	//描画後に更新
	virtual void LateUpdate();
	//オブジェクト同士の当たり判定検証
	virtual void Collision(Task* b);
	
};