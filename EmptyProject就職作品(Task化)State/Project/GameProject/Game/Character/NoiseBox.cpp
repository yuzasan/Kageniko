#include "NoiseBox.h"
#include "Player.h"
#include "Enemy.h"
#include "../../Navigation/NavNode.h"
#include "../../Navigation/NavManeger.h"

NoiseBox::NoiseBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	: CharaBase(TaskType::eNoiseBox)
	, mp_player(nullptr)
	, mp_enemy(nullptr)
	, m_isNoise(false)
	, m_isNoisemove(false)
{
	m_pos = pos;
	m_rot = rot;
	m_boxSize = size;
	m_obb = COBB(
		m_pos,
		m_rot,
		m_boxSize
	);

	//プレイヤー位置に経路探索用のノードを作成
	m_navNode = new NavNode
	(
		m_pos + CVector3D(0.0f, 1.0f, 0.0f),
		NavNode::NodeType::Destination
	);
	m_navNode->SetNodeColor(CVector3D(0.25f, 1.0f, 0.25f));
}

NoiseBox::~NoiseBox()
{
}

void NoiseBox::Update()
{
	//プレイヤー取得
	if (!mp_player)
	{
		mp_player = dynamic_cast<Player*>(TaskManeger::FindObject(TaskType::ePlayer));
	}
	//敵取得
	if (!mp_enemy)
	{
		mp_enemy = dynamic_cast<Enemy*>(TaskManeger::FindObject(TaskType::eEnemy));
	}
}

void NoiseBox::Render()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	Utility::DrawOBB(m_obb, CVector4D(0.25f, 1.0f, 0.25f, 1));
}

void NoiseBox::NoEnemyRender()
{
	Utility::DrawOBB(m_obb, CVector4D(0.25f, 1.0f, 0.25f, 1));
}

void NoiseBox::Collision(Task* b) {
	switch (b->GetType()) {
	case TaskType::ePlayer:
	{
		//■OBBとカプセル
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb, mp_player->m_lineS, mp_player->m_lineE, mp_player->m_rad, &axis, &dist)) {
			m_isNoise = true;
		}
	}
	break;
	case TaskType::eEnemy:
	{
		//■OBBとカプセル
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb, mp_enemy->m_lineS, mp_enemy->m_lineE, mp_enemy->m_rad, &axis, &dist)) {
			if (m_isNoise) {
				m_isNoise = false;
				m_isNoisemove = false;
				Kill();
			}
		}
	}
	break;
	}
}
