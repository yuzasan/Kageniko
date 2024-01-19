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

	/*CVector3D(-48.0f,0.0f, 48.0f),
	CVector3D( 0.0f,0.0f, 48.0f),
	CVector3D(-48.0f,0.0f,-48.0f),
	CVector3D( 0.0f,0.0f,-48.0f),
	CVector3D( 48.0f,0.0f,-48.0f),
	CVector3D( 48.0f,0.0f, 0.0f),
	CVector3D( 48.0f,0.0f, 48.0f),
	CVector3D(-48.0f,0.0f, 0.0f),
	CVector3D(-13.0f,0.0f, 13.0f),
	CVector3D(-13.0f,0.0f,-13.0f),
	CVector3D( 13.0f,0.0f,-13.0f),*/
	
	CVector3D(-8,6,11),
	CVector3D(-8,6,25),
	CVector3D(-8,6,50),
	CVector3D(-34,6,50),
	CVector3D(-34,6,-2),
	CVector3D(-13,6,-2),
	CVector3D(-58,6,48),
	CVector3D(-58,6,24),
	CVector3D(-58,6,-2),
	CVector3D(-45,6,23),
	CVector3D(-14,6,23),
	CVector3D(-20,6,23),
	CVector3D(-20,6,11),
	CVector3D(-14,6,5),
	CVector3D(-14,6,43),
	CVector3D(-34,6,43),
	CVector3D(-34,6,36),
	CVector3D(-20,6,36),
	CVector3D(-45,6,36),
	CVector3D(-45,6,11),
	CVector3D(-34,6,11),
	CVector3D(-52,6,5),
	CVector3D(-52,6,23),
	CVector3D(-52,6,36),
	CVector3D(-34,6,5),
	CVector3D(-8,13,5),
	CVector3D(-16,17,-4),
	CVector3D(-8,14,0),
	CVector3D(-10,18,-5),
	
};

//�R���X�g���N�^
Stage::Stage()
	:Task(TaskType::eField)
{
	ms_instance = this;

	//m_model = COPY_RESOURCE("Stage", CModelObj);
	m_model = GET_RESOURCE("Stagecol", CModel);
	m_Navmodel = GET_RESOURCE("Stagecol", CModel);
	
	skybox = COPY_RESOURCE("Sky", CModelObj);
	for (int i = 0; i < skybox.GetMaterialSize(); i++) {
		if (CTexture* t = skybox.GetMaterial(i)->mp_texture) {
			//�e�N�X�`���\�̓\����ύX
			t->SetWrapST(GL_CLAMP_TO_EDGE);
			t->SetFilter(GL_LINEAR);
		}
	}

	m_Watchmodel = new StageWatch();

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
	//CModel* col = s->GetModel();
	CModel* col = s->GetColNavModel();
	if (col == nullptr) return false;

	//�R���W���������݂���΁A���C�Ƃ̃q�b�g����̌��ʂ�Ԃ�
	return col->CollisionRay(outHitPos, outHitNormal, start, end);
}

void Stage::Update()
{

}

void Stage::Render()
{
	//���X�J�C�{�b�N�X
	//�w�i�Ƃ��ĕ`�悷��̂ŁA��ԍŏ��ɕ`�悷��

	//�[�x�e�X�gOFF
	//�����ł̕`��͐[�x�o�b�t�@�։e�����Ȃ�
	glDisable(GL_DEPTH_TEST);
	//���C�e�B���O����
	CLight::SetLighting(false);
	//���ݎg�p���̃J�������擾
	CCamera* back = CCamera::GetCurrent();
	//�X�J�C�{�b�N�X�p�J�������擾
	CCamera c = *back;
	//�r���[�s����擾
	CMatrix matrix = c.GetViewMatrix();
	//�r���[�s��̍��W�𖳌�
	matrix.m03 = 0; matrix.m13 = 0; matrix.m23 = 0;
	//�X�J�C�{�b�N�X�p�̃r���[�s��ɐݒ�
	c.SetViewMatrix(matrix);
	//�g�p����J�������X�J�C�{�b�N�X�p�ɐݒ�
	CCamera::SetCurrent(&c);

	//�X�J�C�{�b�N�X�`��
	skybox.Render();
	//�ݒ�����̐ݒ�ɖ߂�
	//�J���������̃J�����ɖ߂�
	CCamera::SetCurrent(back);
	//���C�e�B���O�L��
	CLight::SetLighting(true);
	//�[�x�e�X�gON
	glEnable(GL_DEPTH_TEST);
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	/*m_model.SetScale(1, 1, 1);
	m_model.Render();*/
	m_model->SetScale(1, 1, 1);
	//m_model->Render();
}

void Stage::NoEnemyRender()
{
	//���X�J�C�{�b�N�X
	//�w�i�Ƃ��ĕ`�悷��̂ŁA��ԍŏ��ɕ`�悷��

	//�[�x�e�X�gOFF
	//�����ł̕`��͐[�x�o�b�t�@�։e�����Ȃ�
	glDisable(GL_DEPTH_TEST);
	//���C�e�B���O����
	CLight::SetLighting(false);
	//���ݎg�p���̃J�������擾
	CCamera* back = CCamera::GetCurrent();
	//�X�J�C�{�b�N�X�p�J�������擾
	CCamera c = *back;
	//�r���[�s����擾
	CMatrix matrix = c.GetViewMatrix();
	//�r���[�s��̍��W�𖳌�
	matrix.m03 = 0; matrix.m13 = 0; matrix.m23 = 0;
	//�X�J�C�{�b�N�X�p�̃r���[�s��ɐݒ�
	c.SetViewMatrix(matrix);
	//�g�p����J�������X�J�C�{�b�N�X�p�ɐݒ�
	CCamera::SetCurrent(&c);

	//�X�J�C�{�b�N�X�`��
	skybox.Render();
	//�ݒ�����̐ݒ�ɖ߂�
	//�J���������̃J�����ɖ߂�
	CCamera::SetCurrent(back);
	//���C�e�B���O�L��
	CLight::SetLighting(true);
	//�[�x�e�X�gON
	glEnable(GL_DEPTH_TEST);
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	/*m_model.SetScale(1, 1, 1);
	m_model.Render();*/
	m_model->SetScale(1, 1, 1);
	//m_model->Render();
}