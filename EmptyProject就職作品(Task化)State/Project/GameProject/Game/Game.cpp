#include "Game.h"
#include "../Base/Base.h"
#include "Character/Player.h"
#include "Character/Enemy.h"
#include "Character/EnemyManager.h"
#include "Character/NoiseBox.h"
#include "Character/PlayerCamera.h"
#include "Camera/FieldCamera.h"
#include "GameData/GameData.h"
#include "Stage/MoveBox.h"
#include "Stage/RotateBox.h"
#include "Stage/Box.h"
#include "Stage/HideBox.h"
#include "Stage/TyukanBox.h"
#include "Stage/FellBox.h"
#include "Stage/Item.h"
#include "Stage/Stage.h"
#include "Debug/DebugPrint.h"
#include "../Title/Title.h"
#include "TaskSystem/TaskManeger.h"
#include "../Effect/Effect.h"

Game::Game():Task(TaskType::eScene){
	/*Base::Add(new Player(CVector3D(0, 0, 8)));
	Base::Add(new EnemyManager());
	Base::Add(new Stage());
	Base::Add(new PlayerCamera());
	Base::Add(new FieldCamera());*/

	new Player(CVector3D(0, 1, 8));
	//new EnemyManager();
	new Enemy(CVector3D(0, 1, 0));
	new Enemy(CVector3D(0, 1, 0));
	new Enemy(CVector3D(0, 1, 0));
	new Stage();
	new PlayerCamera();
	new FieldCamera();

	//フィールドに配置する床を生成
	float ratio = 1.0f;
	float moveTime = 5.0f * ratio;//float moveTime = 5.0f * ratio;
	float intervalTime = 1.0f * ratio;//float intervalTime = 1.0f * ratio;

	new MoveBox
	(
		CVector3D(-4.0f, 1.0f, 19.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(5.0f, 1.0f, 4.0f),
		CVector3D(0.0f, 1.0f, 0.0f),
		CVector3D(0.0f, 10.0f, 0.0f),
		moveTime, intervalTime, 0, 0, true
	);

	new MoveBox
	(
		CVector3D(-30.0f, 1.0f, 20.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(5.0f, 1.0f, 4.0f),
		CVector3D(0.0f, 1.0f, 0.0f),
		CVector3D(0.0f, 10.0f, 0.0f),
		moveTime, intervalTime, 0, 0, true
	);

	new MoveBox
	(
		CVector3D(-11.0f, 13.0f, 10.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(2.0f, 2.0f, 1.0f),
		CVector3D(-11.0f, 0.0f, 0.0f),
		CVector3D(-9.0f, 0.0f, 0.0f),
		moveTime, intervalTime, 1, 1, true
	);

	new MoveBox
	(
		CVector3D(-11.0f, 13.0f, 6.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(2.0f, 2.0f, 1.0f),
		CVector3D(-11.0f, 0.0f, 0.0f),
		CVector3D(-9.0f, 0.0f, 0.0f),
		moveTime, intervalTime, 1, 0, true
	);

	new MoveBox
	(
		CVector3D(-11.0f, 13.0f, 2.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(2.0f, 2.0f, 1.0f),
		CVector3D(-11.0f, 0.0f, 0.0f),
		CVector3D(-9.0f, 0.0f, 0.0f),
		moveTime, intervalTime, 1, 1, true
	);

	new MoveBox
	(
		CVector3D(-11.0f, 13.0f, -2.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(2.0f, 2.0f, 1.0f),
		CVector3D(-11.0f, 0.0f, 0.0f),
		CVector3D(-9.0f, 0.0f, 0.0f),
		moveTime, intervalTime, 1, 0, true
	);

	new MoveBox
	(
		CVector3D(-11.0f, 13.0f, -6.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(2.0f, 2.0f, 1.0f),
		CVector3D(-11.0f, 0.0f, 0.0f),
		CVector3D(-9.0f, 0.0f, 0.0f),
		moveTime, intervalTime, 1, 1, true
	);

	new RotateBox
	(
		CVector3D(5.0f, 10.0f, -10.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(3.0f, 1.0f, 3.0f),
		DtoR(-45.0f), 2, intervalTime, 0, true
	);

	new RotateBox
	(
		CVector3D(15.0f, 15.0f, -10.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(3.0f, 1.0f, 3.0f),
		DtoR(60.0f), 2, intervalTime, 0, true
	);

	new Box
	(
		CVector3D(26.0f, 20.0f, -10.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(3.0f, 1.0f, 3.0f)
	);

	new MoveBox
	(
		CVector3D(26.0f, 20.0f, -2.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(3.0f, 1.0f, 3.0f),
		CVector3D(26.0f, 20.0f, 0.0f),
		CVector3D(33.0f, 20.0f, 0.0f),
		moveTime, intervalTime, 1, 0, true
	);

	new HideBox
	(
		CVector3D(0.0f, 1.0f, 0.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(1.0f, 2.0f, 1.0f)
	);

	new TyukanBox
	(
		CVector3D(5.0f, 1.0f, 0.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(1.0f, 2.0f, 1.0f)
	);

	new TyukanBox
	(
		CVector3D(10.0f, 1.0f, 0.0f),
		CVector3D(0.0f, 0.0f, 0.0f),
		CVector3D(1.0f, 2.0f, 1.0f)
	);

	//new FellBox
	//(
	//	CVector3D(0.0f, 1.0f, 5.0f),	//pos
	//	CVector3D(0.0f, 0.0f, 0.0f),	//rot
	//	CVector3D(1.0f, 2.0f, 1.0f)		//size
	//);

	new FellBox
	(
		CVector3D(0.0f, -30.0f, 0.0f),	//pos
		CVector3D(0.0f, 0.0f, 0.0f),	//rot
		CVector3D(130.0f, 2.0f, 130.0f)		//size
	);

	new NoiseBox
	(
		CVector3D(5.0f, 0.0f, 10.0f),	//pos
		CVector3D(0.0f, 0.0f, 0.0f),	//rot
		CVector3D(1.0f, 2.0f, 1.0f)		//size
	);

	//new Effect2D("Nekonote");
	CFPS::Wait();
}

Game::~Game()
{
	//全てのオブジェクトを破棄
	//Base::KillALL();
	TaskManeger::KillALL();
	//タイトルシーンへ
	//Base::Add(new Title());
	GameData::m_item = 0;
	//タイトルシーンへ
	new Title();
}

void Game::Update()
{
	//世界の軸を表示
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(100, 0, 0), CVector4D(1, 0, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 100, 0), CVector4D(0, 1, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 0, 100), CVector4D(0, 0, 1, 1));

	for (int i = -5; i <= 5; i++) {
		Utility::DrawLine(CVector3D(-5, 0, i * 1.0f), CVector3D(5, 0, i * 1.0f), CVector4D(0.2, 0.2, 0.2, 1));
		Utility::DrawLine(CVector3D(i * 1.0f, 0, -5), CVector3D(i * 1.0f, 0, 5), CVector4D(0.2, 0.2, 0.2, 1));
	}
	if (GameData::m_item == 1) {
		//SetKill();
		Kill();
	}
}

BlackOut::BlackOut() :Task(TaskType::eUI)
	, m_gauge(0)
	,m_num(0)
{
	m_time = 0;
}

BlackOut::~BlackOut(){

}

void BlackOut::LateUpdate() {
	CVector4D color = CVector4D(0.0f, 0.0f, 0.0f, 1.0f);
	switch (m_num) {
	case 0:
		if (m_gauge <= 1080) {
			m_time++;
			m_gauge = 20.0f * (float)m_time;//m_gauge = (257.0f / 3.0f * (float)m_time);
		}
		else {
			m_num = 1;
		}
		break;
	case 1:
		if (m_gauge >= 0) {
			m_time--;
			m_gauge = 20.0f * (float)m_time;//m_gauge = (257.0f / 3.0f * (float)m_time);
		}
		else {
			m_num = 2;
		}
		break;
	case 2:
		Kill();
	}
	Utility::DrawQuad(CVector2D(0, 0), CVector2D(1920, m_gauge), color);
}