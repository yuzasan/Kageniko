#include "Player.h"
#include "PlayerCamera.h"
#include "Enemy.h"
#include "../Stage/TyukanBox.h"
#include "../Stage/Stage.h"
#include "../Gamedata/GameData.h"
#include "Game/Game.h"
#include "Debug/DebugPrint.h"
#include "Effect/Effect.h"
#include "../../Navigation/NavNode.h"
#include "../../Navigation/NavManeger.h"

//歩き速度
#define WALK_SPEED 10.0f;
//回転速度
#define ROTATE_SPEED 15.0f
//ジャンプ速度
#define JUMP_SPEED 10.0f
//大ジャンプ速度
#define	HIGH_JUMP_SPEED 16.0f
//待機時間
#define IDLE_TIME 3.0f

Player::Player(const CVector3D& pos) :CharaBase(TaskType::ePlayer)
	,mp_camera(nullptr)
	,m_moveDir(0.0f,0.0f,0.0f)
	,m_elapsedTime(0.0f)
	,m_state(State::Move)
	,m_viewAngle(35.0f)
	,m_viewLength(3.0f)
	,m_isSearch(true)
{
	m_model = COPY_RESOURCE("Ninja", CModelA3M);
	m_pos = m_tyukan = pos;
	//m_rad = 0.4f;
	//m_rad = 0.2f;
	m_rad = 0.4f;
	m_time = 0;
	m_isGround = true;
	m_isHide = false;
	//m_isFind = false;
	mp_enemy = nullptr;
	mp_tyukan = nullptr;
	mp_fellbox = nullptr;

	//プレイヤー位置に経路探索用のノードを作成
	m_navNode = new NavNode
	(
		m_pos + CVector3D(0.0f, 1.0f, 0.0f),
		NavNode::NodeType::Destination
	);
	m_navNode->SetNodeColor(CVector3D(1.0f, 0.25f, 0.25f));

}

Player::~Player() {

}

//移動状態の処理
void Player::StateMove() {
	//移動速度をリセット
	m_vec.x = 0.0f;
	m_vec.z = 0.0f;

	//入力方向のベクトルを取得
	CVector3D inputDir = CVector3D::zero;
	if (HOLD(CInput::eLeft))	inputDir.x = 1.0f;
	if (HOLD(CInput::eRight))	inputDir.x = -1.0f;
	if (HOLD(CInput::eUp))		inputDir.z = 1.0f;
	if (HOLD(CInput::eDown))	inputDir.z = -1.0f;

	//キーが入力されていれば、
	if (inputDir.LengthSq() > 0.1f) {
		if (mp_camera->GetModel() == 0) {
			//カメラの向きに合わせた、移動ベクトルを求める
			CMatrix camMtx = CMatrix::MRotation(mp_camera->m_rot);
			m_moveDir = camMtx * inputDir;
			m_moveDir.y = 0.0f;
			m_moveDir.Normalize();
		}
		else {
			m_moveDir = CVector3D::Leap(m_moveDir, inputDir.GetNormalize(), 0.25f);
		}
		//プレイヤーの向きを徐々に移動方向へ向ける
		m_dir = CVector3D::Sleap(m_dir, m_moveDir, ROTATE_SPEED * CFPS::GetDeltaTime());
		//プレイヤーの向き反映
		m_rot.y = atan2f(m_dir.x, m_dir.z);

		//ジャンプ
		if (m_isGround) {
			if (HOLD(CInput::eButton5)) {
				m_time += CFPS::GetDeltaTime();
			}
			else {
				//3秒以上Spaceキー長押しで大ジャンプ
				if (m_time >= 1) {//if (m_time >= 3) {
					m_vec.y = HIGH_JUMP_SPEED;
					m_isGround = false;
				}
				else if (m_time != 0) {
					m_vec.y = JUMP_SPEED;
					m_isGround = false;
				}
				m_time = 0;
			}
		}

		if (IsFoundEnemy()) {
			if (m_isSearch) {
				new Effect2D("Nekonote");
			}
			if (PUSH(CInput::eMouseL) && !mp_isenemy->m_isFindplayer) {//if (HOLD(CInput::eMouseL)) {
				Shot();
				m_isSearch = false;
			}
		}

		//移動速度を取得
		float moveSpeed = WALK_SPEED;

		//移動方向と移動速度から移動ベクトルを求める
		CVector3D moveVec = m_moveDir * moveSpeed;
		m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
		//移動アニメーション再生
		m_model.ChangeAnimation((int)AnimId::Walk);
	}
	//キーが入力されていない
	else {
		//待機アニメーション再生
		m_model.ChangeAnimation((int)AnimId::Idle);
	}

	//ジャンプ
	if (m_isGround) {
		if (HOLD(CInput::eButton5)) {
			m_time += CFPS::GetDeltaTime();
		}
		else {
			//3秒以上Spaceキー長押しで大ジャンプ
			if (m_time >= 1) {//if (m_time >= 3) {
				m_vec.y += HIGH_JUMP_SPEED;
				m_isGround = false;
			}
			else if (m_time != 0) {
				m_vec.y += JUMP_SPEED;
				m_isGround = false;
			}
			m_time = 0;
		}
	}

	/*if (!mp_isenemy) {
		mp_isenemy = TaskManeger::FindObject(TaskType::eEnemy);
	}*/
	if (IsFoundEnemy()) {
		if (m_isSearch) {
			new Effect2D("Nekonote");
		}
		if (PUSH(CInput::eMouseL) && !mp_isenemy->m_isFindplayer) {//if (HOLD(CInput::eMouseL)) {
			Shot();
			m_isSearch = false;
		}
	}

}

//透明状態
void Player::StateInvisible() {
	m_pos = CVector3D(0.0f, 10000.0f, 0.0f);
	//待機時間待ち
	if (m_elapsedTime < IDLE_TIME) {
		m_elapsedTime += CFPS::GetDeltaTime();
	}
	else {
		m_elapsedTime = 0.0f;
		m_pos = m_tyukan;
		m_state = State::Move;
	}
}

void Player::Update() {
	/*
	CVector3D key_dir(0, 0, 0);
	//カメラの方向ベクトル
	CVector3D cam_dir = CCamera::GetCurrent()->GetDir();
	//入力回転値
	float key_ang = 0;
	//カメラの回転値
	float cam_ang = atan2(cam_dir.x, cam_dir.z);

	if (!mp_camera) {
		mp_camera = dynamic_cast<PlayerCamera*>(TaskManeger::FindObject(TaskType::eCamera));
	}
	//キャラクターの回転値をカメラの回転値に合わせる
	m_rot.y = mp_camera->m_rot.y;

	if (HOLD(CInput::eLeft))key_dir.x = 1;
	if (HOLD(CInput::eRight))key_dir.x = -1;
	if (HOLD(CInput::eUp))key_dir.z = 1;
	if (HOLD(CInput::eDown))key_dir.z = -1;

	if (key_dir.LengthSq() > 0.1) {
		//入力方向から入力回転値を計算
		key_ang = atan2(key_dir.x, key_dir.z);
		//カメラの回転値と入力回転値からキャラクターの回転値を決定
		//即座にrot.yに設定せず一旦目標値を設定する
		//m_rot_target.y = cam_ang + key_ang;
		m_rot.y = cam_ang + key_ang;
		//方向ベクトル
		CVector3D dir(sin(m_rot.y), 0, cos(m_rot.y));
		//移動
		m_pos += dir * move_speed;
		m_model.ChangeAnimation(1);
	}
	else {
		m_model.ChangeAnimation(0);
	}
	m_model.UpdateAnimation();

	if (m_isGround) {
		if (HOLD(CInput::eButton5)) {
			m_time += CFPS::GetDeltaTime();
		}
		else {
			//3秒以上Spaceキー長押しで大ジャンプ
			if (m_time >= 1) {//if (m_time >= 3) {
				m_vec.y = 0.5f;
				m_isGround = false;
			}
			else if (m_time != 0) {
				m_vec.y = 0.3f;
				m_isGround = false;
			}
			m_time = 0;
		}
	}
	//printf("%.0f秒\n", m_time);
	if (!mp_isenemy) {
		mp_isenemy = TaskManeger::FindObject(TaskType::eEnemy);
	}
	if (PUSH(CInput::eMouseL) && !mp_isenemy->m_isFindplayer) {//if (HOLD(CInput::eMouseL)) {
		Shot();
	}
	*/
	// 視野範囲のカラー(初期色は緑)
	color = CVector4D(0.0f, 1.0f, 0.0f, 0.75f);

	if (!mp_enemy) {
		mp_enemy = dynamic_cast<Enemy*>(TaskManeger::FindObject(TaskType::eEnemy));
	}

	if (!mp_isenemy) {
		mp_isenemy = TaskManeger::FindObject(TaskType::eEnemy);
	}

	//メインカメラ取得
	if (!mp_camera) {
		mp_camera = dynamic_cast<PlayerCamera*>(TaskManeger::FindObject(TaskType::eCamera));
	}

	//現在の状態に合わせて、処理を切り替える
	switch (m_state)
	{
	//移動状態
	case State::Move:
		StateMove();
		break;
	//透明状態
	case State::Invisible:
		StateInvisible();
		break;
	}

	//重力落下
	if (m_vec.y < -10.0f) {//m_vec.y < -0.26f

	}
	else {
		m_vec.y -= GRAVITY * CFPS::GetDeltaTime();
	}
	//移動
	m_pos += m_vec * CFPS::GetDeltaTime();

	//カプセル
	float height = 0.5f;
	m_lineS = m_pos + CVector3D(0, height - m_rad, 0);
	m_lineE = m_pos + CVector3D(0, m_rad, 0);

	if (PUSH(CInput::eMouseR)){
		printf("x:%.0f,y:%.0f,z:%.0f\n", m_pos.x, m_pos.y, m_pos.z);
	}

	// ベースクラスの更新
	CharaBase::Update();

	NavNode* node = NavManeger::Instance()->GetNearNavNode(m_navNode);

	//DebugPrint::Print("P:%f, %f, %f", m_pos.x, m_pos.y, m_pos.z);
	if (node != nullptr){
		CVector3D npos = node->GetPos();
		//DebugPrint::Print("N:%f, %f, %f", npos.x, npos.y, npos.z);
	}

	//デバッグ表示
	//Debug();
}

void Player::Render() {
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(1.0f, 1.0f, 1.0f);
	m_model.Render();
	//プレイヤーカプセルの表示
	//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 0, 1));
	//回転値から方向ベクトルを計算
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	//扇の見た目
	CMatrix m;
	m.LookAt(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * m_viewLength, CVector3D(0, 1, 0));
	Utility::DrawSector(m, -DtoR(m_viewAngle), DtoR(m_viewAngle), m_viewLength, color);
}

void Player::NoEnemyRender()
{
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(1.0f, 1.0f, 1.0f);
	m_model.Render();
	//プレイヤーカプセルの表示
	//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 0, 1));
	
	//回転値から方向ベクトルを計算
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	//扇の見た目
	CMatrix m;
	m.LookAt(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * m_viewLength, CVector3D(0, 1, 0));
	Utility::DrawSector(m, -DtoR(m_viewAngle), DtoR(m_viewAngle), m_viewLength, color);
}

bool Player::IsFoundEnemy() const
{
	CVector3D enemyPos = mp_enemy->m_pos;

	// 敵からプレイヤーまでのベクトルを求める
	CVector3D vec = enemyPos - m_pos;

	// 敵の正面ベクトル と 敵からプレイヤーまでのベクトル で
	// 内積を取って角度を求める
	float dot = CVector3D::Dot(m_dir, vec.GetNormalize());
	// 求めた角度(内積)が視野角度内か判断する
	if (dot < cos(DtoR(m_viewAngle))) return false;

	// 敵からプレイヤーまでの距離を求める
	float length = vec.Length();
	// 求めた距離が視野距離内か判断する
	if (length > m_viewLength) return false;

	// 最後に自身からプレイヤーまでの間に
	// 遮蔽物がないか判定する
	if (!IsSearchEnemy()) return false;
	
	return true;
}

//現在地から敵がいるか
bool Player::IsSearchEnemy() const
{
	CVector3D enemyPos = mp_enemy->m_pos;
	CVector3D vec = enemyPos - m_pos;
	// 現在位置からプレイヤーまでの距離を求める
	float dist = vec.Length();

	// プレイヤーの位置までのレイと壁との衝突判定を行う
	CVector3D start = m_pos;
	CVector3D end = enemyPos;
	start.y = 1.0f;
	end.y = 1.0f;
	CVector3D hitPos, hitNormal;
	if (Stage::CollisionRay(start, end, &hitPos, &hitNormal))
	{
		float hitDist = (hitPos - start).Length();
		if (dist > hitDist) return false;
	}

	// 壁と衝突していないもしくは、
	// 衝突位置がプレイヤーより奥の位置であるならば、
	// 視線が通っているので、プレイヤーが見える状態
	return true;
}

void Player::Shot(){
	if (IsFoundEnemy()) {
		mp_enemy->m_isFind = true;
	}
	else {
		if (mp_enemy != nullptr) {
			mp_enemy->m_isFind = false;
		}
	}
	/*
	//CVector3D r = mp_camera->m_rot;
	CVector3D r = CVector3D(mp_camera->m_rot.x, m_rot.y, m_rot.z);//m_rot
	//射線の方向(プレイヤー前方向)
	CVector3D dir = CMatrix::MRotation(r).GetFront();
	//始点
	CVector3D lineS = m_pos + CVector3D(0, 0.75f, 0);
	//終点
	CVector3D lineE = m_pos + CVector3D(0, 0.75f, 0) + dir * m_viewLength;

	//最も近いオブジェクトへの距離
	float dist = FLT_MAX;
	//射線との衝突点
	CVector3D hit_field_point;
	//衝突したステージオブジェクト
	Stage* hit_field = nullptr;
	if (Stage* s = dynamic_cast<Stage*>(TaskManeger::FindObject(TaskType::eField))) {
		//接触面の法線は(使わない)
		CVector3D n;
		if (s->GetModel()->CollisionRay(&hit_field_point, &n, lineS, lineE)) {
			//発射位置から接触面への距離
			dist = (hit_field_point - lineS).LengthSq();
			//接触したステージを更新
			hit_field = s;
		}
	}
	//接触した敵
	Enemy* hit_enemy = nullptr;
	//全敵を探索
	auto list = TaskManeger::FindObjects(TaskType::eEnemy);
	for (auto b : list) {
		if (Enemy* e = dynamic_cast<Enemy*>(b)) {
			//射線との衝突地点
			CVector3D c;
			//弾の線分で敵との判定を行う
			if (e->CollisionLine(lineS, lineE, dir, &c) > 0) {
				//発射位置から最も近い敵を調べる
				float l = (c - lineS).LengthSq();
				if (dist > l) {
					dist = l;
					hit_enemy = e;
				}
			}
		}
	}
	//最も近い敵に当たる
	if (hit_enemy) {
		mp_enemy = hit_enemy;
		//m_isFind = true;
		mp_enemy->m_isFind = true;
		//printf("当たった\n");
	}
	//ステージに当たる
	else if (hit_field) {
		//m_isFind = false;
		if (mp_enemy != nullptr) {
			mp_enemy->m_isFind = false;
		}
	}
	else {
		if (mp_enemy != nullptr) {
			mp_enemy->m_isFind = false;
		}
	}
	*/
}

void Player::Collision(Task* b) {
	switch (b->GetType()) {
	case TaskType::eField:
	{
		CVector3D v(0, 0, 0);
		//カプセルとモデルの衝突
		auto tri = b->GetModel()->CollisionCupsel(m_lineS, m_lineE, m_rad);
		//接触した面の数繰り返す
		for (auto& t : tri) {
			if (t.m_normal.y < -0.5f) {//if (t.m_normal.y < 0.5f) {
				//面が下向き→天井に当たった	
				//上昇速度を0に戻す
				if (m_vec.y > 0)
					m_vec.y = 0;
			}
			else if (t.m_normal.y > 0.5f) {//else if (t.m_normal.y > -0.5f) {
				//面が上向き→地面に当たった
				//重力落下速度を0に戻す
				if (m_vec.y < 0)
					m_vec.y = 0;
				m_isGround = true;
			}
			float max_y = max(t.m_vertex[0].y, max(t.m_vertex[1].y, t.m_vertex[2].y));
			//接触した面の方向へ、めり込んだ分押し戻す
			CVector3D nv = t.m_normal * (m_rad - t.m_dist);
			//最も大きな移動量を求める
			v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
			v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
			v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
		}
		m_pos += v;
	}
	break;
	case TaskType::eEnemy:
	{
		/*
		//敵の判定
		CVector3D c1, dir1, c2, dir2;
		float dist;
		if (CCollision::CollisionCapsule(m_lineS, m_lineE, m_rad,
			b->m_lineS, b->m_lineE, b->m_rad,
			&dist, &c1, &dir1, &c2, &dir2)) {
			new BlackOut();
			m_state = State::Invisible;
		}
		*/
	}
	break;
	case TaskType::eTyukanBox:
	{
		//dynamic_castは触れている対象を取得※Baseにあるpublicの関数・変数が使える
		if (TyukanBox* t = dynamic_cast<TyukanBox*>(b)) {
			//■OBBとカプセル
			float dist;
			CVector3D axis;
			if (CCollision::CollisionOBBCapsule(t->m_obb, m_lineS, m_lineE, m_rad, &axis, &dist)) {
				m_tyukan = t->m_pos;
			}
		}
	}
	break;
	case TaskType::eFellBox:
	{
		mp_fellbox = TaskManeger::FindObject(TaskType::eFellBox);
		//■OBBとカプセル
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(mp_fellbox->m_obb, m_lineS, m_lineE, m_rad, &axis, &dist)) {
			new BlackOut();
			m_pos = m_tyukan;
		}
	}
	break;
	}
}

void Player::LateUpdate(){
	CVector4D color = CVector4D(0.0f, 0.0f, 1.0f, 1.0f);
	m_gauge = (257.0f / 1.0f * (float)m_time);//m_gauge = (257.0f / 3.0f * (float)m_time);
	if (m_gauge >= 257) {
		m_gauge = 257;
		color = CVector4D(cosf(m_time * 2.0f), sinf(m_time * 7.0f), tanf(m_time * 11.0f), 1);
		//color = CVector4D(cosf(m_time * 5), sinf(m_time * 5), tanf(m_time * 5), 1);
	}
	Utility::DrawQuad(CVector2D(0, 1080), CVector2D(259, -259), CVector4D(0.0f, 0.0f, 0.0f, 1.0f));
	Utility::DrawQuad(CVector2D(1, 1079), CVector2D(257, -m_gauge), color);
	
}

void Player::Debug(){
	DebugPrint::Print("Player座標 : X:%f , Y:%f , Z:%f", m_pos.x, m_pos.y, m_pos.z);
}

