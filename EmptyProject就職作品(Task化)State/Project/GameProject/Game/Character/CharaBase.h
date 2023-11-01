#pragma once
#include "../../TaskSystem/Task.h"
#include "../../TaskSystem/TaskManeger.h"
class NavNode;

//キャラクターのベース
class CharaBase :public Task {
protected:
	bool m_isGround;
	NavNode* m_navNode;
public:
	//コンストラクタ
	CharaBase(TaskType type);
	//デストラクタ
	virtual ~CharaBase();

	NavNode* GetNavNode() const;

	//更新
	void Uptate();

	// コリジョン描画
	void RenderCol();
};