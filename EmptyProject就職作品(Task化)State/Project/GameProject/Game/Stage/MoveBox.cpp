#include "MoveBox.h"

MoveBox::MoveBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size,
	const CVector3D& startPos, const CVector3D& endPos,
	const float moveTime, const float intervalTime,const int yxz, const int cnt, bool isStart)//const float moveTime, const float intervalTime,const int number, bool isStart)
	: Task(TaskType::eMoveBox)
	, m_startPos(startPos)
	, m_endPos(endPos)
	, m_moveTime(moveTime)
	, m_intervalTime(intervalTime)
	, m_yxz(yxz)//, m_number(number)
	, m_cnt(cnt)
	, m_state(State::eMove)
	, m_isEnable(isStart)
	, m_isReverse(false)
	, m_elapsedTime(0.0f)
{
	m_pos = pos;
	m_rot = rot;
	m_boxSize = size;
	m_obb = COBB(
		m_pos,
		m_rot,
		m_boxSize
	);
	mp_player = TaskManeger::FindObject(TaskType::ePlayer);
}

MoveBox::~MoveBox() {

}

void MoveBox::Start()
{
	m_isEnable = true;
}

void MoveBox::Stop()
{
	m_isEnable = false;
}

//�ړ���Ԃ̍X�V����
void MoveBox::UpdateMove()
{
	////�J�n�ʒu�ƏI���ʒu���擾
	//CVector3D start = m_isReverse ? m_endPos : m_startPos;
	//CVector3D end = m_isReverse ? m_startPos : m_endPos;
	//
	////�o�ߎ��Ԃ��ړ����Ԃ𒴂��Ă��Ȃ��ꍇ
	//if (m_elapsedTime < m_moveTime)
	//{
	//	if (m_pos.y <= m_endPos.y) {
	//		m_pos.y += 0.1f;
	//		m_obb = COBB(
	//			m_pos,
	//			m_rot,
	//			m_boxSize
	//		);
	//	}
	//	//�o�ߎ��Ԃ����Z
	//	m_elapsedTime += CFPS::GetDeltaTime();
	//}
	////�ړ��I��
	//else
	//{
	//	m_obb = COBB(
	//		m_pos,
	//		m_rot,
	//		m_boxSize
	//	);
	//	//�ړ������𔽓]
	//	m_isReverse = !m_isReverse;
	//	m_elapsedTime -= m_moveTime;
	//	//�ҋ@��Ԃֈڍs
	//	m_state = State::eInterval;
	//}
	switch (m_yxz)//switch (m_number)
	{
	case 0://y���i�㉺�j
		if (m_cnt % 2 == 0) {
			if (m_pos.y <= m_endPos.y) {
				m_pos.y += 0.1f;
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
			}
			else {
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				//�ҋ@��Ԃֈڍs
				m_state = State::eInterval;
			}
		}
		else {
			if (m_pos.y >= m_startPos.y) {
				m_pos.y -= 0.1f;
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
			}
			else {
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				//�ҋ@��Ԃֈڍs
				m_state = State::eInterval;
			}
		}
		break;
	case 1://x���i���E�j
		if (m_cnt % 2 == 0) {
			if (m_pos.x <= m_endPos.x) {
				m_pos.x += 0.1f;
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
			}
			else {
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				//�ҋ@��Ԃֈڍs
				m_state = State::eInterval;
			}
		}
		else {
			if (m_pos.x >= m_startPos.x) {
				m_pos.x -= 0.1f;
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
			}
			else {
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				//�ҋ@��Ԃֈڍs
				m_state = State::eInterval;
			}
		}
		break;
	case 2://z���i�O��j
		if (m_cnt % 2 == 0) {
			if (m_pos.z <= m_endPos.z) {
				m_pos.z += 0.1f;
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
			}
			else {
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				//�ҋ@��Ԃֈڍs
				m_state = State::eInterval;
			}
		}
		else {
			if (m_pos.z >= m_startPos.z) {
				m_pos.z -= 0.1f;
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
			}
			else {
				m_obb = COBB(
					m_pos,
					m_rot,
					m_boxSize
				);
				//�ҋ@��Ԃֈڍs
				m_state = State::eInterval;
			}
		}
		break;
	}
}

//�ҋ@��Ԃ̍X�V����
void MoveBox::UpdateInterval()
{
	//�ҋ@���Ԍo�ߑ҂�
	if (m_elapsedTime < m_intervalTime)
	{
		m_elapsedTime += CFPS::GetDeltaTime();
	}
	//�ҋ@�I��
	else
	{
		m_elapsedTime -= m_intervalTime;
		m_cnt++;
		//�ړ���Ԃֈڍs
		m_state = State::eMove;
	}
}

//�X�V����
void MoveBox::Update()
{
	/*
	m_pos.y += 0.1f;
	m_obb = COBB(
		m_pos,
		m_rot,
		m_boxSize
	);
	*/
	//�ړ����L���łȂ���Ώ������Ȃ�
	if (!m_isEnable) return;

	//��Ԃɍ��킹�čX�V������؂�ւ�
	switch (m_state)
	{
		//�ړ����
	case State::eMove:
		UpdateMove();
		break;
		//�ҋ@���
	case State::eInterval:
		UpdateInterval();
		break;
	}
}

//�`�揈��
void MoveBox::Render()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	switch (m_yxz)//switch (m_number)
	{
	case 0://y���i�㉺�j
		Utility::DrawOBB(m_obb, CVector4D(0, 1, 0, 1));
		break;
	case 1://x���i���E�j
		Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 1));
		break;
	case 2://z���i�O��j
		Utility::DrawOBB(m_obb, CVector4D(0, 0, 1, 1));
		break;
	}
}

void MoveBox::NoEnemyRender()
{
	switch (m_yxz)//switch (m_number)
	{
	case 0://y���i�㉺�j
		Utility::DrawOBB(m_obb, CVector4D(0, 1, 0, 1));
		break;
	case 1://x���i���E�j
		Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 1));
		break;
	case 2://z���i�O��j
		Utility::DrawOBB(m_obb, CVector4D(0, 0, 1, 1));
		break;
	}
}

void MoveBox::Collision(Task* b) {
	switch (b->GetType()) {
	case TaskType::ePlayer:
	{
		//��OBB�ƃJ�v�Z��
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb, mp_player->m_lineS, mp_player->m_lineE, mp_player->m_rad, &axis, &dist)) {
			if (axis.y > 0.5f) {
				//�ʂ���������n�ʂ���������
				//�d�͗������x��0�ɖ߂�
				if (mp_player->m_vec.y < 0) {
					mp_player->m_vec.y = 0;
					mp_player->m_isGround = true;
				}
			}
			//�����߂�
			float s = mp_player->m_rad - dist;
			mp_player->m_pos += axis * s;
		}
	}
	break;
	}
}