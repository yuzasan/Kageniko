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

//�������x
#define WALK_SPEED 10.0f;
//��]���x
#define ROTATE_SPEED 15.0f
//�W�����v���x
#define JUMP_SPEED 10.0f
//��W�����v���x
#define	HIGH_JUMP_SPEED 16.0f
//�ҋ@����
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

	//�v���C���[�ʒu�Ɍo�H�T���p�̃m�[�h���쐬
	m_navNode = new NavNode
	(
		m_pos + CVector3D(0.0f, 1.0f, 0.0f),
		NavNode::NodeType::Destination
	);
	m_navNode->SetNodeColor(CVector3D(1.0f, 0.25f, 0.25f));

}

Player::~Player() {

}

//�ړ���Ԃ̏���
void Player::StateMove() {
	//�ړ����x�����Z�b�g
	m_vec.x = 0.0f;
	m_vec.z = 0.0f;

	//���͕����̃x�N�g�����擾
	CVector3D inputDir = CVector3D::zero;
	if (HOLD(CInput::eLeft))	inputDir.x = 1.0f;
	if (HOLD(CInput::eRight))	inputDir.x = -1.0f;
	if (HOLD(CInput::eUp))		inputDir.z = 1.0f;
	if (HOLD(CInput::eDown))	inputDir.z = -1.0f;

	//�L�[�����͂���Ă���΁A
	if (inputDir.LengthSq() > 0.1f) {
		if (mp_camera->GetModel() == 0) {
			//�J�����̌����ɍ��킹���A�ړ��x�N�g�������߂�
			CMatrix camMtx = CMatrix::MRotation(mp_camera->m_rot);
			m_moveDir = camMtx * inputDir;
			m_moveDir.y = 0.0f;
			m_moveDir.Normalize();
		}
		else {
			m_moveDir = CVector3D::Leap(m_moveDir, inputDir.GetNormalize(), 0.25f);
		}
		//�v���C���[�̌��������X�Ɉړ������֌�����
		m_dir = CVector3D::Sleap(m_dir, m_moveDir, ROTATE_SPEED * CFPS::GetDeltaTime());
		//�v���C���[�̌������f
		m_rot.y = atan2f(m_dir.x, m_dir.z);

		//�W�����v
		if (m_isGround) {
			if (HOLD(CInput::eButton5)) {
				m_time += CFPS::GetDeltaTime();
			}
			else {
				//3�b�ȏ�Space�L�[�������ő�W�����v
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

		//�ړ����x���擾
		float moveSpeed = WALK_SPEED;

		//�ړ������ƈړ����x����ړ��x�N�g�������߂�
		CVector3D moveVec = m_moveDir * moveSpeed;
		m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
		//�ړ��A�j���[�V�����Đ�
		m_model.ChangeAnimation((int)AnimId::Walk);
	}
	//�L�[�����͂���Ă��Ȃ�
	else {
		//�ҋ@�A�j���[�V�����Đ�
		m_model.ChangeAnimation((int)AnimId::Idle);
	}

	//�W�����v
	if (m_isGround) {
		if (HOLD(CInput::eButton5)) {
			m_time += CFPS::GetDeltaTime();
		}
		else {
			//3�b�ȏ�Space�L�[�������ő�W�����v
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

//�������
void Player::StateInvisible() {
	m_pos = CVector3D(0.0f, 10000.0f, 0.0f);
	//�ҋ@���ԑ҂�
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
	//�J�����̕����x�N�g��
	CVector3D cam_dir = CCamera::GetCurrent()->GetDir();
	//���͉�]�l
	float key_ang = 0;
	//�J�����̉�]�l
	float cam_ang = atan2(cam_dir.x, cam_dir.z);

	if (!mp_camera) {
		mp_camera = dynamic_cast<PlayerCamera*>(TaskManeger::FindObject(TaskType::eCamera));
	}
	//�L�����N�^�[�̉�]�l���J�����̉�]�l�ɍ��킹��
	m_rot.y = mp_camera->m_rot.y;

	if (HOLD(CInput::eLeft))key_dir.x = 1;
	if (HOLD(CInput::eRight))key_dir.x = -1;
	if (HOLD(CInput::eUp))key_dir.z = 1;
	if (HOLD(CInput::eDown))key_dir.z = -1;

	if (key_dir.LengthSq() > 0.1) {
		//���͕���������͉�]�l���v�Z
		key_ang = atan2(key_dir.x, key_dir.z);
		//�J�����̉�]�l�Ɠ��͉�]�l����L�����N�^�[�̉�]�l������
		//������rot.y�ɐݒ肹����U�ڕW�l��ݒ肷��
		//m_rot_target.y = cam_ang + key_ang;
		m_rot.y = cam_ang + key_ang;
		//�����x�N�g��
		CVector3D dir(sin(m_rot.y), 0, cos(m_rot.y));
		//�ړ�
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
			//3�b�ȏ�Space�L�[�������ő�W�����v
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
	//printf("%.0f�b\n", m_time);
	if (!mp_isenemy) {
		mp_isenemy = TaskManeger::FindObject(TaskType::eEnemy);
	}
	if (PUSH(CInput::eMouseL) && !mp_isenemy->m_isFindplayer) {//if (HOLD(CInput::eMouseL)) {
		Shot();
	}
	*/
	// ����͈͂̃J���[(�����F�͗�)
	color = CVector4D(0.0f, 1.0f, 0.0f, 0.75f);

	if (!mp_enemy) {
		mp_enemy = dynamic_cast<Enemy*>(TaskManeger::FindObject(TaskType::eEnemy));
	}

	if (!mp_isenemy) {
		mp_isenemy = TaskManeger::FindObject(TaskType::eEnemy);
	}

	//���C���J�����擾
	if (!mp_camera) {
		mp_camera = dynamic_cast<PlayerCamera*>(TaskManeger::FindObject(TaskType::eCamera));
	}

	//���݂̏�Ԃɍ��킹�āA������؂�ւ���
	switch (m_state)
	{
	//�ړ����
	case State::Move:
		StateMove();
		break;
	//�������
	case State::Invisible:
		StateInvisible();
		break;
	}

	//�d�͗���
	if (m_vec.y < -10.0f) {//m_vec.y < -0.26f

	}
	else {
		m_vec.y -= GRAVITY * CFPS::GetDeltaTime();
	}
	//�ړ�
	m_pos += m_vec * CFPS::GetDeltaTime();

	//�J�v�Z��
	float height = 0.5f;
	m_lineS = m_pos + CVector3D(0, height - m_rad, 0);
	m_lineE = m_pos + CVector3D(0, m_rad, 0);

	if (PUSH(CInput::eMouseR)){
		printf("x:%.0f,y:%.0f,z:%.0f\n", m_pos.x, m_pos.y, m_pos.z);
	}

	// �x�[�X�N���X�̍X�V
	CharaBase::Update();

	NavNode* node = NavManeger::Instance()->GetNearNavNode(m_navNode);

	//DebugPrint::Print("P:%f, %f, %f", m_pos.x, m_pos.y, m_pos.z);
	if (node != nullptr){
		CVector3D npos = node->GetPos();
		//DebugPrint::Print("N:%f, %f, %f", npos.x, npos.y, npos.z);
	}

	//�f�o�b�O�\��
	//Debug();
}

void Player::Render() {
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(1.0f, 1.0f, 1.0f);
	m_model.Render();
	//�v���C���[�J�v�Z���̕\��
	//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 0, 1));
	//��]�l��������x�N�g�����v�Z
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	//��̌�����
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
	//�v���C���[�J�v�Z���̕\��
	//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 0, 1));
	
	//��]�l��������x�N�g�����v�Z
	CVector3D dir(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)));
	//��̌�����
	CMatrix m;
	m.LookAt(m_pos + CVector3D(0, 0.5f, 0), m_pos + CVector3D(0, 0.1f, 0) + dir * m_viewLength, CVector3D(0, 1, 0));
	Utility::DrawSector(m, -DtoR(m_viewAngle), DtoR(m_viewAngle), m_viewLength, color);
}

bool Player::IsFoundEnemy() const
{
	CVector3D enemyPos = mp_enemy->m_pos;

	// �G����v���C���[�܂ł̃x�N�g�������߂�
	CVector3D vec = enemyPos - m_pos;

	// �G�̐��ʃx�N�g�� �� �G����v���C���[�܂ł̃x�N�g�� ��
	// ���ς�����Ċp�x�����߂�
	float dot = CVector3D::Dot(m_dir, vec.GetNormalize());
	// ���߂��p�x(����)������p�x�������f����
	if (dot < cos(DtoR(m_viewAngle))) return false;

	// �G����v���C���[�܂ł̋��������߂�
	float length = vec.Length();
	// ���߂����������싗���������f����
	if (length > m_viewLength) return false;

	// �Ō�Ɏ��g����v���C���[�܂ł̊Ԃ�
	// �Օ������Ȃ������肷��
	if (!IsSearchEnemy()) return false;
	
	return true;
}

//���ݒn����G�����邩
bool Player::IsSearchEnemy() const
{
	CVector3D enemyPos = mp_enemy->m_pos;
	CVector3D vec = enemyPos - m_pos;
	// ���݈ʒu����v���C���[�܂ł̋��������߂�
	float dist = vec.Length();

	// �v���C���[�̈ʒu�܂ł̃��C�ƕǂƂ̏Փ˔�����s��
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

	// �ǂƏՓ˂��Ă��Ȃ��������́A
	// �Փˈʒu���v���C���[��艜�̈ʒu�ł���Ȃ�΁A
	// �������ʂ��Ă���̂ŁA�v���C���[����������
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
	//�ː��̕���(�v���C���[�O����)
	CVector3D dir = CMatrix::MRotation(r).GetFront();
	//�n�_
	CVector3D lineS = m_pos + CVector3D(0, 0.75f, 0);
	//�I�_
	CVector3D lineE = m_pos + CVector3D(0, 0.75f, 0) + dir * m_viewLength;

	//�ł��߂��I�u�W�F�N�g�ւ̋���
	float dist = FLT_MAX;
	//�ː��Ƃ̏Փ˓_
	CVector3D hit_field_point;
	//�Փ˂����X�e�[�W�I�u�W�F�N�g
	Stage* hit_field = nullptr;
	if (Stage* s = dynamic_cast<Stage*>(TaskManeger::FindObject(TaskType::eField))) {
		//�ڐG�ʂ̖@����(�g��Ȃ�)
		CVector3D n;
		if (s->GetModel()->CollisionRay(&hit_field_point, &n, lineS, lineE)) {
			//���ˈʒu����ڐG�ʂւ̋���
			dist = (hit_field_point - lineS).LengthSq();
			//�ڐG�����X�e�[�W���X�V
			hit_field = s;
		}
	}
	//�ڐG�����G
	Enemy* hit_enemy = nullptr;
	//�S�G��T��
	auto list = TaskManeger::FindObjects(TaskType::eEnemy);
	for (auto b : list) {
		if (Enemy* e = dynamic_cast<Enemy*>(b)) {
			//�ː��Ƃ̏Փ˒n�_
			CVector3D c;
			//�e�̐����œG�Ƃ̔�����s��
			if (e->CollisionLine(lineS, lineE, dir, &c) > 0) {
				//���ˈʒu����ł��߂��G�𒲂ׂ�
				float l = (c - lineS).LengthSq();
				if (dist > l) {
					dist = l;
					hit_enemy = e;
				}
			}
		}
	}
	//�ł��߂��G�ɓ�����
	if (hit_enemy) {
		mp_enemy = hit_enemy;
		//m_isFind = true;
		mp_enemy->m_isFind = true;
		//printf("��������\n");
	}
	//�X�e�[�W�ɓ�����
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
		//�J�v�Z���ƃ��f���̏Փ�
		auto tri = b->GetModel()->CollisionCupsel(m_lineS, m_lineE, m_rad);
		//�ڐG�����ʂ̐��J��Ԃ�
		for (auto& t : tri) {
			if (t.m_normal.y < -0.5f) {//if (t.m_normal.y < 0.5f) {
				//�ʂ����������V��ɓ�������	
				//�㏸���x��0�ɖ߂�
				if (m_vec.y > 0)
					m_vec.y = 0;
			}
			else if (t.m_normal.y > 0.5f) {//else if (t.m_normal.y > -0.5f) {
				//�ʂ���������n�ʂɓ�������
				//�d�͗������x��0�ɖ߂�
				if (m_vec.y < 0)
					m_vec.y = 0;
				m_isGround = true;
			}
			float max_y = max(t.m_vertex[0].y, max(t.m_vertex[1].y, t.m_vertex[2].y));
			//�ڐG�����ʂ̕����ցA�߂荞�񂾕������߂�
			CVector3D nv = t.m_normal * (m_rad - t.m_dist);
			//�ł��傫�Ȉړ��ʂ����߂�
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
		//�G�̔���
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
		//dynamic_cast�͐G��Ă���Ώۂ��擾��Base�ɂ���public�̊֐��E�ϐ����g����
		if (TyukanBox* t = dynamic_cast<TyukanBox*>(b)) {
			//��OBB�ƃJ�v�Z��
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
		//��OBB�ƃJ�v�Z��
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
	DebugPrint::Print("Player���W : X:%f , Y:%f , Z:%f", m_pos.x, m_pos.y, m_pos.z);
}

