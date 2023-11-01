#include "Task.h"
#include "TaskManeger.h"

//�R���X�g���N�^
Task::Task(TaskType type)
	: m_type(type)
	, m_isKill(false)
	, m_attack_no(0)
	, m_hit_no(0)
	, m_isFind(false)
	, m_isFindplayer(false)
	, m_isGround(true)
	, m_isHide(false)
	, m_rad(0.0f)
	, m_time(0.0f)
{
	TaskManeger::Instance()->Add(this);
}

//�f�X�g���N�^
Task::~Task(){
	TaskManeger::Instance()->Remove(this);
}

//�^�X�N�̎�ނ��擾
TaskType Task::GetType() const{
	return m_type;
}

//�I�u�W�F�N�g�������f���̎擾
CModel* Task::GetModel()
{
	return nullptr;
}

//�폜�t���O�擾
bool Task::IsKill() const{
	return m_isKill;
}

//�^�X�N���폜
void Task::Kill(){
	m_isKill = true;
}

//�X�V
void Task::Update() {

}

//�`��
void Task::Render() {

}

//�����蔻��
void Task::Collision(Task* b) {

}

//Enemy�ȊO�`��
void Task::NoEnemyRender(){

}

//�X�V��ɕ`��
void Task::LateRender(){

}

//�X�V��ɍX�V
void Task::LateUpdate(){

}
