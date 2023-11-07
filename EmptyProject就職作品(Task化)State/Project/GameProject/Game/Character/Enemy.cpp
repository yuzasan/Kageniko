#include "Enemy.h"
#include "Player.h"
#include "../Stage/Item.h"
#include "../Stage/Stage.h"
#include "../GameData/GameData.h"
#include "Effect/Effect.h"
#include "../../Debug/DebugPrint.h"
#include "../../Navigation/NavNode.h"
#include "../../Navigation/NavManeger.h"
#include "EnemyManager.h"


//歩き速度
#define WALK_SPEED 5.0f//4.8f//2.4f
//追跡速度
//#define CHASE_SPEED 0.05f
//回転速度
#define ROTATE_SPEED 15.0f
//待機時間
#define IDLE_TIME 2.0f
//目的地との間距離
#define SPACE 1.0f
//重力
//#define G 0.8f
//最大重力
//#define MAX_G 0.001f
//移動先
#define MOVE_RANGE_MIN -6.0f//-13.0f
#define MOVE_RANGE_MAX 6.0f//13.0f

Enemy::Enemy(const CVector3D& pos):CharaBase(TaskType::eEnemy)
	,mp_player(nullptr)
	,mp_field(nullptr)
	,m_moveDir(0.0f,0.0f,0.0f)
	,m_movePos(0.0f,0.0f,0.0f)
	,m_moveNode(nullptr)
	,m_elapsedTime(0.0f)
	,m_state(State::Idle)
	,isFoll(false)
	,m_cntfoll(0)
	,m_isTouch(false)
	,m_isWall(false)
	,m_nextNode(nullptr)
	,m_lostNode(nullptr)
{
	//敵の管理クラスのリストに自身を追加
	EnemyManager::Instance()->Add(this);

	mp_player = dynamic_cast<Player*>(TaskManeger::FindObject(TaskType::ePlayer));
	//mp_hidebox = Base::FindObject(BaseType::eHideBox);
	m_model = COPY_RESOURCE("Golem", CModelA3M);
	
	m_pos = pos;
	m_dir = CVector3D(0.0f, 0.0f, 1.0f);
	m_moveDir = m_dir;

	//m_rad = 0.8f / 2.0f;
	//m_rad = 0.8f;
	m_rad = 0.3f;
	m_isFind = false;
	m_isFindplayer = false;

	//敵の経路探索用のノードを作成
	m_navNode = new NavNode
	(
		m_pos + CVector3D(0.0f, 1.0f, 0.0f),
		NavNode::NodeType::Destination
	);
	m_navNode->SetNodeColor(CVector3D(0.125f, 0.25f, 1.0f));
}

//デストラクタ
Enemy::~Enemy() {
	//敵の管理クラスのリストから自信を取り除く
	EnemyManager::Instance()->Remove(this);
}

//待機状態の処理
void Enemy::StateIdle()
{
	//待機アニメーション
	m_model.ChangeAnimation((int)AnimId::Idle);

	m_vec.x = 0.0f;
	m_vec.z = 0.0f;

	//待機時間待ち
	if (m_elapsedTime < IDLE_TIME) {
		m_elapsedTime += CFPS::GetDeltaTime();
	}
	//待機時間が終われば、
	else {
		m_elapsedTime = 0.0f;

		//次に探索するノードを取得
		SearchNode* node = EnemyManager::Instance()->GetNextSearchNode();
		if (node != nullptr) {
			//探索ノードに一番近いノードを目的地とする
			m_searchNode = node;
			m_searchNode->enemy = this;
			m_moveNode = NavManeger::Instance()->GetNearNavNode(m_searchNode->pos);
		}
		//探索するノードが存在しなかった場合
		else {
			//移動位置をランダムで求める
			m_movePos.x = Utility::Rand(MOVE_RANGE_MIN, MOVE_RANGE_MAX);
			m_movePos.z = Utility::Rand(MOVE_RANGE_MIN, MOVE_RANGE_MAX);
			m_moveNode = NavManeger::Instance()->GetNearNavNode(m_movePos);
		}
		//移動状態へ移行
		m_state = State::Move;
	}

	//プレイヤーを見つけたら、強制的に追跡状態へ
	if (IsFoundPlayer()) {
		m_state = State::Chase;
	}
}

//移動状態の処理
void Enemy::StateMove()
{
	//前進アニメーション
	m_model.ChangeAnimation((int)AnimId::Walk);
	if (m_isFind && !IsFoundPlayer()) {
		isFoll = true;
		m_isFind = false;
		m_isTouch = true;
	}
	else {
		FollBox();
		if (m_moveNode != nullptr) {
			//現在位置から目的地のモードまでの経路探索を行う
			NavManeger* navMgr = NavManeger::Instance();
			m_nextNode = navMgr->Navigate(m_navNode, m_moveNode);

			//次に移動すべきノードが存在すれば、
			if (m_nextNode != nullptr) {
				//次に移動すべきノードへ向けて移動
				if (MoveTo(m_nextNode->GetPos())) {
					//移動が終われば、目的地のノードと
					//次に移動すべきノードをnullにする
					m_moveNode = nullptr;
					m_nextNode = nullptr;
					//探索ノードが存在しない場合は、そのまま待機状態へ
					if (m_searchNode == nullptr) {
						m_state = State::Idle;
					}
				}
			}
		}
		else {
			//探索ノードまで移動
			if (MoveTo(m_searchNode->pos)) {
				//移動が終われば、
				m_searchNode->prob = 0.0f;
				m_searchNode->enemy = nullptr;
				m_searchNode = nullptr;
				m_state = State::Idle;
			}
		}
	}
	
	//プレイヤーを見つけたら、強制的に追跡状態へ
	if (IsFoundPlayer()) {
		m_state = State::Chase;
	}
}

void Enemy::StateChase()
{
	//前進アニメーション
	m_model.ChangeAnimation((int)AnimId::Walk);

	//各ノードの座標を取得
	NavNode* playerNode = mp_player->GetNavNode();
	NavNode* enemyNode = m_navNode;
	CVector3D playerNodePos = playerNode->GetPos();
	CVector3D enemyNodePos = enemyNode->GetPos();
	//現在位置からプレイヤーの位置までの距離を求める
	CVector3D vec = playerNodePos - enemyNodePos;
	float dist = vec.Length();

	NavManeger* navMgr = NavManeger::Instance();

	//プレイヤーの位置まで視線が通っているか判定
	CVector3D hitPos, hitNormal;
	enemyNodePos.y = 1.0f;
	playerNodePos.y = 1.0f;
	bool isHit = Stage::CollisionRay(enemyNodePos, playerNodePos, &hitPos, &hitNormal);
	//プレイヤーの位置までレイを飛ばして、何かに衝突したら、
	if (isHit) {
		//プレイヤーまでの距離 > 衝突地点までの距離ならば、
		//自身とプレイヤーまで間に遮蔽物が存在する
		float hitDist = (hitPos - enemyNodePos).Length();
		isHit = dist > hitDist;
	}

	//プレイヤーの位置まで遮蔽物が存在しなければ、
	if (!isHit) {
		//現在位置からプレイヤーの位置までの経路探索を行う
		m_nextNode = navMgr->Navigate(enemyNode, playerNode);

		//見失った時の最終位置に配置するノードを準備
		if (m_lostNode == nullptr) {
			m_lostNode = new NavNode
			(
				playerNodePos,
				NavNode::NodeType::Destination
			);
			m_lostNode->SetNodeColor(CVector3D(1.0f, 0.5f, 0.25f));
		}
		//プレイヤーの最終位置用のノードが既に存在するならば、
		//プレイヤーの最終位置を更新
		else {
			m_lostNode->SetPos(playerNodePos);
		}
		m_isFind = false;
	}
	//プレイヤーとの間に遮蔽物が存在する
	else{
		//自身とプレイヤーのノードの接続を切る
		enemyNode->RemoveConnect(playerNode);

		//現在位置からプレイヤーの最終位置までの経路探索を行う
		m_nextNode = navMgr->Navigate(enemyNode, m_lostNode);

		//見失い状態へ移行
		m_state = State::Lost;
		return;
	}

	//次に移動すべきノードが存在する場合は、
	//そのノードの位置まで移動
	if (m_nextNode != nullptr) {
		MoveTo(m_nextNode->GetPos());
	}
	//次に移動すべきノードが存在しない場合は、
	//待機状態へ移行
	else {
		m_state = State::Idle;
	}
}

//プレイヤーを見失った状態の処理
void Enemy::StateLost() {
	//目的地が存在する
	if (m_nextNode != nullptr) {
		//見失った場合は、視野範囲を無視して、
		//プレイヤーまでの視線が通るかどうかで判定する
		if (IsLookPlayer()) {
			//追跡状態へ移行
			m_state = State::Chase;
			m_isFind = false;
		}
		//プレイヤーが視線の通らないところにいる
		else {
			//目的地まで移動
			if (MoveTo(m_nextNode->GetPos())) {
				if (m_nextNode != m_lostNode) {
					NavManeger* navMgr = NavManeger::Instance();
					m_nextNode = navMgr->Navigate(m_nextNode, m_lostNode);
				}
				else {
					delete m_lostNode;
					m_lostNode = nullptr;
					m_nextNode = nullptr;
				}
			}
		}
	}
	//目的地まで移動が終われば、
	else {
		new Effect("Fukidasi", m_pos + CVector3D(0, 2.5, -1), 1.0f, true, 60);
		//待機状態へ移行
		m_state = State::Idle;
	}
}

int Enemy::CollisionLine(const CVector3D& lineS, const CVector3D& lineE, const CVector3D& dir, CVector3D* cross){
	CVector3D c1, c2, d1, d2;
	float dist;
	if (CCollision::CollisionCapsule(m_lineS, m_lineE, m_rad,
		lineS, lineE, 0.01f,
		&dist, &c1, &d1, &c2, &c2)) {
		//カプセルと弾の線分との接触点
		if (cross) {
			*cross = c2 + -dir * (m_rad - dist);
		}
		return 1;	//当たっていたら1(1)
	}
	return 0;		//当たっていない（0）
}

bool Enemy::IsFoundPlayer(){
	//回転値から方向ベクトルを計算
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	// 角度(内積)と距離で視野範囲内か判定
	CVector3D v1 = (mp_player->m_pos - m_pos);
	float dot = CVector3D::Dot(dir, v1.GetNormalize());//vを正規化して内積
	
	if (v1.Length() <= view_length) {
		if (dot > cos(view_ang)) {
			Shot();
			if (!mp_player->m_isHide && !m_isWall || mp_player->m_isHide && m_isFindplayer == true && !m_isWall) {
				m_isFindplayer = true;
				return true;
			}
		}
	}
	m_isFindplayer = false;
	return false;
}

//現在位置からプレイヤーが見えるかどうか
bool Enemy::IsLookPlayer() const {
	CVector3D playerPos = mp_player->m_pos;
	CVector3D vec = playerPos - m_pos;
	//現在位置からプレイヤーまでの距離を求める
	float dist = vec.Length();

	//プレイヤーの位置までのレイと壁との衝突判定を行う
	CVector3D start = m_pos;
	CVector3D end = playerPos;
	start.y = 1.0f;
	end.y = 1.0f;
	CVector3D hitPos, hitNormal;
	if (Stage::CollisionRay(start, end, &hitPos, &hitNormal)) {
		float hitDist = (hitPos - start).Length();
		if (dist > hitDist)return false;
	}

	//壁と衝突していないもしくは、
	//衝突位置がプレイヤーより奥の位置であるならば、
	//視野が通っているので、プレイヤーが見える状態
	return true;
}

//指定座標への移動処理
bool Enemy::MoveTo(const CVector3D& target) {

	float moveSpeed = WALK_SPEED;

	CVector3D vec = target - m_pos;
	vec.y = 0.0f;
	m_moveDir = vec.GetNormalize();

	//目的地までの距離が移動距離より大きいならば
	float dist = vec.Length();
	float speed = moveSpeed * CFPS::GetDeltaTime();
	if (dist > moveSpeed * CFPS::GetDeltaTime()) {
		CVector3D moveVec = m_moveDir * moveSpeed;
		m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
	}
	//距離が移動速度より短いのであれば、
	else {
		CVector3D moveVec = m_moveDir * dist;
		m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
		//移動終了
		return true;
	}

	//移動継続
	return false;
}

void Enemy::Shot(){
	CVector3D lineS = m_pos + CVector3D(0, 0.75f, 0);
	CVector3D lineE = mp_player->m_pos + CVector3D(0, 0.75f, 0);
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
	//壁に当たる
	if (hit_field) {
		m_isWall = true;
	}
	else {
		m_isWall = false;
	}
}

//探索ノードを設定
void Enemy::SetSearchNode(SearchNode* node)
{
	m_searchNode = node;
}

//探索中のノードを取得
SearchNode* Enemy::GetSearchNode() const
{
	return m_searchNode;
}


void Enemy::Update()
{
	// 視野範囲のカラー(初期色は緑)
	color = CVector4D(0.0f, 1.0f, 0.0f, 0.75f);

	// 現在の状態に合わせて、処理を切り替える
	switch (m_state)
	{
		// 待機状態
	case State::Idle:
		StateIdle();
		break;
		// 移動状態
	case State::Move:
		StateMove();
		break;
		// 追跡状態
	case State::Chase:
		StateChase();
		color = CVector4D(1.0f, 0.0f, 0.0f, 1.0f);
		break;
		//プレイヤーを見失った
	case State::Lost:
		StateLost();
		color = CVector4D(1.0f, 1.0f, 0.0f, 0.75f);
		break;
	}

	//アニメーション更新
	m_model.UpdateAnimation();

	// プレイヤーの向きを徐々に移動方向へ向ける
	m_dir = CVector3D::Sleap(m_dir, m_moveDir, ROTATE_SPEED * CFPS::GetDeltaTime());
	// プレイヤーの向き反映
	m_rot.y = atan2f(m_dir.x, m_dir.z);

	/*m_vec.x *= G;
	if (abs(m_vec.x) < MAX_G)m_vec.x = 0;
	m_vec.z *= G;
	if (abs(m_vec.z) < MAX_G)m_vec.z = 0;*/

	if (m_vec.y < -10.0f) {//m_vec.y < -0.26f

	}
	else {
		m_vec.y -= GRAVITY * CFPS::GetDeltaTime();
	}

	//m_pos.y += m_vec.y;
	m_pos += m_vec * CFPS::GetDeltaTime();

	//カプセル更新
	float height = 2.0f;
	m_lineS = m_pos + CVector3D(0, height - m_rad, 0);
	m_lineE = m_pos + CVector3D(0, m_rad, 0);

	// ベースクラスの更新
	CharaBase::Update();

	//デバッグ表示
	Debug();
}

void Enemy::Render()
{
	//if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	//m_model.SetScale(0.005f, 0.005f, 0.005f);
	m_model.SetScale(0.01f, 0.01f, 0.01f);
	m_model.Render();
	//回転値から方向ベクトルを計算
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	// 視野の範囲を表示
	auto lineWidth = 5.0f;
	Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector4D(1, 0, 1, 1), lineWidth);
	Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
	Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(-view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
	//扇の見た目
	CMatrix m;
	m.LookAt(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector3D(0, 1, 0));
	Utility::DrawSector(m, -view_ang, view_ang, view_length, color);

	//if (mp_player->m_isFind && !IsFoundPlayer()) {
	if (m_isTouch) {//m_isFind && !IsFoundPlayer()
		//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 1, 1));
		Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(1, 0, 1, 1));
	}
	else {
		//敵カプセルの表示
		//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(0, 0, 1, 1));
		Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(0, 0, 1, 1));
	}
	
}

void Enemy::Collision(Task* b)
{
	switch (b->GetType()) {
	case TaskType::eField:
	{
		CVector3D v(0, 0, 0);
		//カプセルとモデルの衝突
		auto tri = b->GetModel()->CollisionCupsel(m_lineS, m_lineE, m_rad);
		//接触した面の数繰り返す
		for (auto& t : tri) {
			if (t.m_normal.y < -0.5f) {
				//面が下向き→天井に当たった	
				//上昇速度を0に戻す
				if (m_vec.y > 0)
					m_vec.y = 0;
			}
			else if (t.m_normal.y > 0.5f) {
				//面が上向き→地面に当たった
				//重力落下速度を0に戻す
				if (m_vec.y < 0)
					m_vec.y = 0;
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
		m_lineS += v;
		m_lineE += v;
	}
	break;
	case TaskType::eEnemy:
	{
		//敵同士の判定
		CVector3D c1, dir1, c2, dir2;
		float dist;
		if (CCollision::CollisionCapsule(m_lineS, m_lineE, m_rad,
			b->m_lineS, b->m_lineE, b->m_rad,
			&dist, &c1, &dir1, &c2, &dir2)) {
			float s = (m_rad + b->m_rad) - dist;
			b->m_pos += dir1 * s * 0.5f;
			b->m_lineS += dir1 * s * 0.5f;
			b->m_lineE += dir1 * s * 0.5f;
			m_pos += dir2 * s * 0.5f;
			m_lineS += dir2 * s * 0.5f;
			m_lineE += dir2 * s * 0.5f;

		}
	}
	break;
	}
}

void Enemy::FollBox() {
	if (isFoll && m_cntfoll < 1) {
		/*
		Base::Add(new Item(
			m_pos + CVector3D(0, 0.5f, 0),
			CVector3D(0.0f, 0.0f, 0.0f),
			CVector3D(0.1f, 0.1f, 0.1f)
		));
		*/
		new Item(
			m_pos + CVector3D(0, 0.5f, 0),
			CVector3D(0.0f, 0.0f, 0.0f),
			CVector3D(0.1f, 0.1f, 0.1f)
		);
		m_cntfoll++;
	}
	isFoll = false;
}

void Enemy::LateRender() {
	if (m_isTouch) {
		m_model.SetPos(m_pos);
		m_model.SetRot(m_rot);
		//m_model.SetScale(0.005f, 0.005f, 0.005f);
		m_model.SetScale(0.01f, 0.01f, 0.01f);
		m_model.Render();
		//回転値から方向ベクトルを計算
		CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
		// 視野の範囲を表示
		auto lineWidth = 5.0f;
		Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector4D(1, 0, 1, 1), lineWidth);
		Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
		Utility::DrawLine(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + CMatrix::MRotationY(-view_ang) * dir * view_length, CVector4D(0, 1, 1, 1), lineWidth);
		//扇の見た目
		CMatrix m;
		m.LookAt(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * view_length, CVector3D(0, 1, 0));
		Utility::DrawSector(m, -view_ang, view_ang, view_length, color);

		//if (mp_player->m_isFind && !IsFoundPlayer()) {
		if (m_isTouch) {//m_isFind && !IsFoundPlayer()
			//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 1, 1));
			Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(1, 0, 1, 1));
		}
		else {
			//敵カプセルの表示
			//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(0, 0, 1, 1));
			Utility::DrawCube(m_pos + CVector3D(0, 2, 0), CVector3D(1, 0, 1), CVector4D(0, 0, 1, 1));
		}
	}
}

void Enemy::Debug() {
	DebugPrint::Print("Enemy座標 : X:%f , Y:%f , Z:%f", m_pos.x, m_pos.y, m_pos.z);
	std::string state = "";
	switch (m_state)
	{
	case Enemy::State::Idle: state = "待機"; break;
	case Enemy::State::Move: state = "移動"; break;
	case Enemy::State::Chase:state = "追跡"; break;
	case Enemy::State::Lost:state = "見失った"; break;
	}
	DebugPrint::Print("State:%s", state.c_str());
}
