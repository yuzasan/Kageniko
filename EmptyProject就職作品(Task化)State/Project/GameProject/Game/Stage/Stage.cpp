#include "Stage.h"
#include "StageWatch.h"
#include "../../Navigation/NavNode.h"
#include "../../Navigation/NavManeger.h"

Stage* Stage::ms_instance = nullptr;

//�o�H�T���p�m�[�h�̃e�[�u��
std::list<CVector3D> Stage::ms_nodes =
{
	/*
	CVector3D(-13.5f,0.0f,-13.5f),		//�s���~�܂�
	CVector3D(-13.5f,0.0f,-9.0f),
	CVector3D(-13.5f,0.0f,-5.5f),
	CVector3D(-13.5f,0.0f,-2.2f),
	CVector3D(-13.5f,0.0f, 1.5f),		//�s���~�܂�
	CVector3D(-13.5f,0.0f, 5.0f),
	CVector3D(-13.5f,0.0f, 13.0f),

	CVector3D(-10.5f,0.0f,9.0f),		//�s���~�܂�
	CVector3D(-10.5f,0.0f,5.0f),
	CVector3D(-10.5f,0.0f,1.0f),
	CVector3D(-10.5f,0.0f,-2.5f),
	CVector3D(-10.5f,0.0f,-9.5f),
	CVector3D(-10.5f,0.0f,-13.0f),

	CVector3D(-6.0f,0.0f,1.5f),
	CVector3D(-6.0f,0.0f,5.0f),
	CVector3D(-6.0f,0.0f,9.0f),
	CVector3D(-6.0f,0.0f,13.0f),

	CVector3D(-2.0f,0.0f, 5.0f),		//�s���~�܂�
	CVector3D(-2.0f,0.0f, 1.5f),		//�s���~�܂�
	CVector3D(-2.0f,0.0f,-5.5f),		//�s���~�܂�

	CVector3D( 2.5f,0.0f,-13.0f),		//�s���~�܂�
	CVector3D( 2.5f,0.0f,-9.0f),
	CVector3D( 2.5f,0.0f,-5.5f),
	CVector3D( 2.5f,0.0f, 5.0f),		//�s���~�܂�
	CVector3D( 2.5f,0.0f, 13.0f),		//�s���~�܂�

	CVector3D( 5.5f,0.0f, 13.0f),
	CVector3D( 5.5f,0.0f, 9.0f),
	CVector3D( 5.5f,0.0f,-2.5f),
	CVector3D( 5.5f,0.0f,-5.5f),
	CVector3D( 5.5f,0.0f,-10.5f),
	CVector3D( 5.5f,0.0f,-13.5f),

	CVector3D( 8.5f,0.0f,-10.5f),
	CVector3D( 8.5f,0.0f,-2.5f),
	CVector3D( 8.5f,0.0f, 1.5f),
	CVector3D( 8.5f,0.0f, 5.0f),
	CVector3D( 8.5f,0.0f, 13.0f),

	CVector3D(11.0f,0.0f, 13.0f),
	CVector3D(11.0f,0.0f, 9.0f),
	CVector3D(11.0f,0.0f, 5.5f),
	CVector3D(11.0f,0.0f, 2.0f),		//�s���~�܂�
	CVector3D(11.0f,0.0f,-2.5f),
	CVector3D(11.0f,0.0f,-6.5f),
	CVector3D(11.0f,0.0f,-10.0f),
	CVector3D(11.0f,0.0f,-13.5f),		//�s���~�܂�

	CVector3D(13.0f,0.0f,-13.5f),		//�s���~�܂�
	CVector3D(13.0f,0.0f,-10.0f),
	CVector3D(13.0f,0.0f,-6.5f),
	CVector3D(13.0f,0.0f, 5.5f),
	CVector3D(13.0f,0.0f, 9.0f),
	CVector3D(13.0f,0.0f, 13.0f),		//�s���~�܂�
	*/

	CVector3D(-48.0f,0.0f, 48.0f),
	CVector3D( 0.0f,0.0f, 48.0f),
	CVector3D(-48.0f,0.0f,-48.0f),
	CVector3D( 0.0f,0.0f,-48.0f),
	CVector3D( 48.0f,0.0f,-48.0f),
	CVector3D( 48.0f,0.0f, 0.0f),
	CVector3D( 48.0f,0.0f, 48.0f),
	CVector3D(-48.0f,0.0f, 0.0f),
	CVector3D(-13.0f,0.0f, 13.0f),
	CVector3D(-13.0f,0.0f,-13.0f),
	CVector3D( 13.0f,0.0f,-13.0f),
	CVector3D( 13.0f,0.0f, 13.0f),
	
};

//�R���X�g���N�^
Stage::Stage()
	:Task(TaskType::eField)
{
	ms_instance = this;

	//m_model = COPY_RESOURCE("Stage", CModelObj);
	m_model = GET_RESOURCE("Stagecol", CModel);
	m_Navmodel = GET_RESOURCE("Stagecol", CModel);
	
	m_Watchmodel = new StageWatch();
	TaskManeger::Instance()->Add(m_Watchmodel);

	//�o�H�T���p�̃m�[�h���쐬
	CreateNavNodes();
}

//�f�X�g���N�^
Stage::~Stage()
{
	if (ms_instance == this) {
		ms_instance = nullptr;
	}
}

//�o�H�T���p�̃m�[�h���쐬
void Stage::CreateNavNodes()
{
	//�e�[�u���̍��W�Ɍo�H�T���p�̃m�[�h���쐬
	for (CVector3D nodePos : ms_nodes) {
		new NavNode(nodePos);
	}

	/*
	float xMin = -14.0f;
	float xMax = 14.0f;
	float zMin = -14.0f;
	float zMax = 14.0f;

	int w = 10;
	int h = 10;
	for (int i = 0; i < w; i++) {
		float wper = (float)i / (w - 1);
		for (int j = 0; j < h; j++) {
			float hper = (float)j / (h - 1);

			CVector3D pos = CVector3D::zero;
			pos.x = Utility::Lerp(xMin, xMax, wper);
			pos.z = Utility::Lerp(zMin, zMax, hper);
			new NavNode(pos);
		}
	}
	*/

}

CModel* Stage::GetColNavModel() const
{
	return m_Navmodel;
}

CModel* Stage::GetModel()
{
	return m_model;
}


//�w�肵�����C��Stage�̃R���W�����Ƃ̃q�b�g����
bool Stage::CollisionRay(const CVector3D& start, const CVector3D& end, CVector3D* outHitPos, CVector3D* outHitNormal)
{
	//�t�B�[���h���쐬����Ă��Ȃ���΁A�X���[
	Stage* s = ms_instance;
	if (s == nullptr) return false;

	//�R���W�������f�����쐬����Ă��Ȃ���΁A�X���[
	CModel* col = s->GetModel();
	if (col == nullptr) return false;

	//�R���W���������݂���΁A���C�Ƃ̃q�b�g����̌��ʂ�Ԃ�
	return col->CollisionRay(outHitPos, outHitNormal, start, end);
}

void Stage::Update()
{

}

void Stage::Render()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	/*m_model.SetScale(1, 1, 1);
	m_model.Render();*/
	m_model->SetScale(1, 1, 1);
	m_model->Render();
}

void Stage::NoEnemyRender()
{
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	/*m_model.SetScale(1, 1, 1);
	m_model.Render();*/
	m_model->SetScale(1, 1, 1);
	m_model->Render();
}