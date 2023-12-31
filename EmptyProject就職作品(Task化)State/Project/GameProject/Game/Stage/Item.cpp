#include "Item.h"
#include "../GameData/GameData.h"

Item::Item(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	: Task(TaskType::eItem)
{
	m_pos = pos;
	m_rot = rot;
	m_itemSize = size;
	m_obb = COBB(
		m_pos,
		m_rot,
		m_itemSize
	);
	mp_player = TaskManeger::FindObject(TaskType::ePlayer);
}

Item::~Item() {

}

//更新処理
void Item::Update()
{
}

//描画処理
void Item::Render()
{
	Utility::DrawOBB(m_obb, CVector4D(0.5, 0, 1, 1));
}

void Item::NoEnemyRender()
{
	Utility::DrawOBB(m_obb, CVector4D(0.5, 0, 1, 1));
}

void Item::Collision(Task* b) {
	switch (b->GetType()) {
	case TaskType::ePlayer:
	{
		//■OBBとカプセル
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb, mp_player->m_lineS, mp_player->m_lineE, mp_player->m_rad, &axis, &dist)) {
			GameData::m_item += 1;
			printf("item %d個\n", GameData::m_item);
			Kill();
		}
	}
	break;
	}
}