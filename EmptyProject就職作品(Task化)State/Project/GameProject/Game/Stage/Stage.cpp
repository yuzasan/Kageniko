#include "Stage.h"
#include "StageWatch.h"
#include "../../Navigation/NavNode.h"
#include "../../Navigation/NavManeger.h"

Stage* Stage::ms_instance = nullptr;

//経路探索用ノードのテーブル
std::list<CVector3D> Stage::ms_nodes =
{
	/*
	CVector3D(-13.5f,0.0f,-13.5f),		//行き止まり
	CVector3D(-13.5f,0.0f,-9.0f),
	CVector3D(-13.5f,0.0f,-5.5f),
	CVector3D(-13.5f,0.0f,-2.2f),
	CVector3D(-13.5f,0.0f, 1.5f),		//行き止まり
	CVector3D(-13.5f,0.0f, 5.0f),
	CVector3D(-13.5f,0.0f, 13.0f),

	CVector3D(-10.5f,0.0f,9.0f),		//行き止まり
	CVector3D(-10.5f,0.0f,5.0f),
	CVector3D(-10.5f,0.0f,1.0f),
	CVector3D(-10.5f,0.0f,-2.5f),
	CVector3D(-10.5f,0.0f,-9.5f),
	CVector3D(-10.5f,0.0f,-13.0f),

	CVector3D(-6.0f,0.0f,1.5f),
	CVector3D(-6.0f,0.0f,5.0f),
	CVector3D(-6.0f,0.0f,9.0f),
	CVector3D(-6.0f,0.0f,13.0f),

	CVector3D(-2.0f,0.0f, 5.0f),		//行き止まり
	CVector3D(-2.0f,0.0f, 1.5f),		//行き止まり
	CVector3D(-2.0f,0.0f,-5.5f),		//行き止まり

	CVector3D( 2.5f,0.0f,-13.0f),		//行き止まり
	CVector3D( 2.5f,0.0f,-9.0f),
	CVector3D( 2.5f,0.0f,-5.5f),
	CVector3D( 2.5f,0.0f, 5.0f),		//行き止まり
	CVector3D( 2.5f,0.0f, 13.0f),		//行き止まり

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
	CVector3D(11.0f,0.0f, 2.0f),		//行き止まり
	CVector3D(11.0f,0.0f,-2.5f),
	CVector3D(11.0f,0.0f,-6.5f),
	CVector3D(11.0f,0.0f,-10.0f),
	CVector3D(11.0f,0.0f,-13.5f),		//行き止まり

	CVector3D(13.0f,0.0f,-13.5f),		//行き止まり
	CVector3D(13.0f,0.0f,-10.0f),
	CVector3D(13.0f,0.0f,-6.5f),
	CVector3D(13.0f,0.0f, 5.5f),
	CVector3D(13.0f,0.0f, 9.0f),
	CVector3D(13.0f,0.0f, 13.0f),		//行き止まり
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

//コンストラクタ
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
			//テクスチャ―の貼り方変更
			t->SetWrapST(GL_CLAMP_TO_EDGE);
			t->SetFilter(GL_LINEAR);
		}
	}

	m_Watchmodel = new StageWatch();

	//経路探索用のノードを作成
	CreateNavNodes();
}

//デストラクタ
Stage::~Stage()
{
	if (ms_instance == this) {
		ms_instance = nullptr;
	}
}

//経路探索用のノードを作成
void Stage::CreateNavNodes()
{
	//テーブルの座標に経路探索用のノードを作成
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


//指定したレイとStageのコリジョンとのヒット判定
bool Stage::CollisionRay(const CVector3D& start, const CVector3D& end, CVector3D* outHitPos, CVector3D* outHitNormal)
{
	//フィールドが作成されていなければ、スルー
	Stage* s = ms_instance;
	if (s == nullptr) return false;

	//コリジョンモデルが作成されていなければ、スルー
	//CModel* col = s->GetModel();
	CModel* col = s->GetColNavModel();
	if (col == nullptr) return false;

	//コリジョンが存在すれば、レイとのヒット判定の結果を返す
	return col->CollisionRay(outHitPos, outHitNormal, start, end);
}

void Stage::Update()
{

}

void Stage::Render()
{
	//■スカイボックス
	//背景として描画するので、一番最初に描画する

	//深度テストOFF
	//ここでの描画は深度バッファへ影響しない
	glDisable(GL_DEPTH_TEST);
	//ライティング無効
	CLight::SetLighting(false);
	//現在使用中のカメラを取得
	CCamera* back = CCamera::GetCurrent();
	//スカイボックス用カメラを取得
	CCamera c = *back;
	//ビュー行列を取得
	CMatrix matrix = c.GetViewMatrix();
	//ビュー行列の座標を無効
	matrix.m03 = 0; matrix.m13 = 0; matrix.m23 = 0;
	//スカイボックス用のビュー行列に設定
	c.SetViewMatrix(matrix);
	//使用するカメラをスカイボックス用に設定
	CCamera::SetCurrent(&c);

	//スカイボックス描画
	skybox.Render();
	//設定を元の設定に戻す
	//カメラを元のカメラに戻す
	CCamera::SetCurrent(back);
	//ライティング有効
	CLight::SetLighting(true);
	//深度テストON
	glEnable(GL_DEPTH_TEST);
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	/*m_model.SetScale(1, 1, 1);
	m_model.Render();*/
	m_model->SetScale(1, 1, 1);
	//m_model->Render();
}

void Stage::NoEnemyRender()
{
	//■スカイボックス
	//背景として描画するので、一番最初に描画する

	//深度テストOFF
	//ここでの描画は深度バッファへ影響しない
	glDisable(GL_DEPTH_TEST);
	//ライティング無効
	CLight::SetLighting(false);
	//現在使用中のカメラを取得
	CCamera* back = CCamera::GetCurrent();
	//スカイボックス用カメラを取得
	CCamera c = *back;
	//ビュー行列を取得
	CMatrix matrix = c.GetViewMatrix();
	//ビュー行列の座標を無効
	matrix.m03 = 0; matrix.m13 = 0; matrix.m23 = 0;
	//スカイボックス用のビュー行列に設定
	c.SetViewMatrix(matrix);
	//使用するカメラをスカイボックス用に設定
	CCamera::SetCurrent(&c);

	//スカイボックス描画
	skybox.Render();
	//設定を元の設定に戻す
	//カメラを元のカメラに戻す
	CCamera::SetCurrent(back);
	//ライティング有効
	CLight::SetLighting(true);
	//深度テストON
	glEnable(GL_DEPTH_TEST);
	if (CShadow::GetInstance()->GetState() == CShadow::eShadow)return;
	/*m_model.SetScale(1, 1, 1);
	m_model.Render();*/
	m_model->SetScale(1, 1, 1);
	//m_model->Render();
}