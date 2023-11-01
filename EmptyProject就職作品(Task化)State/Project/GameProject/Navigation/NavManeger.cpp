#include "NavManeger.h"
#include <GLLibrary/Utility.h>
#include "Game/Stage/Stage.h"
#include "../Debug/DebugPrint.h"

//経路探索の管理クラスのインスタンス
NavManeger* NavManeger::ms_instance = nullptr;

//インスタンスの取得
NavManeger* NavManeger::Instance()
{
	//インスタンスがなければ
	if (ms_instance == nullptr) {
		//インスタンスを生成する
		ms_instance = new NavManeger();
	}
	return ms_instance;
}

//コンストラクタ
NavManeger::NavManeger()
	:m_addNodeCount(0)
	,m_isRender(false)
	,m_routeStartNode(nullptr)
{

}

//デストラクタ
NavManeger::~NavManeger()
{
	std::list<NavNode*>::iterator it = m_nodes.begin();
	while (it != m_nodes.end()) {
		NavNode* del = *it;
		it++;
		delete del;
	}
	m_nodes.clear();
}

//全ノードの状態をリセット
void NavManeger::Reset()
{
	for (NavNode* node : m_nodes) {
		node->Reset();
	}
}

//経路探索用のノードを追加
void NavManeger::AddNode(NavNode* node)
{
	node->m_no = m_addNodeCount++;
	m_nodes.push_back(node);
}

//経路探索用のノードを取り除く
void NavManeger::RemoveNode(NavNode* node)
{
	m_nodes.remove(node);
	for (NavNode* n : m_nodes) {
		n->RemoveConnect(node);
	}
	Reset();
}

//指定したノードからノードまでの経路を探索
NavNode* NavManeger::Navigate(NavNode* start, NavNode* goal)
{
	if (start == nullptr || goal == nullptr)return nullptr;

	//ノードの情報（移動コストや経路上の次のノードのポインタ）をリセット
	Reset();

	std::list<NavNode*> work1;
	std::list<NavNode*> work2;

	std::list<NavNode*>* currLevel = &work1;
	std::list<NavNode*>* nextLevel = &work2;
	std::list<NavNode*>* for_swap;

	//目的地から順番にコストを検索するため、
	//最初の検索リストには目的地のノードを追加
	goal->m_cost = 0.0f;
	currLevel->push_back(goal);

	std::list<NavNode*>::iterator itr;
	float nodeCost;
	//検索リストの最後までループする
	while (currLevel->size()) {
		for (itr = currLevel->begin(); itr != currLevel->end(); itr++) {
			//検索中のノードの接続リストを全て調べる
			for (NavNode::ConnectData& connect : (*itr)->m_connects) {
				//検索するノードの種類が
				//目的地としてのみ使用できるノードだった場合は、
				//目的地までの通過地点として扱わないため、
				//移動コストの計算はしない
				NavNode::NodeType type = connect.node->m_type;
				if (connect.node != start && connect.node != goal
					&& type == NavNode::NodeType::Destination)
				{
					continue;
				}

				//目的地から自身のノードまでの移動コストと
				//自身のノードから接続先のノードまでの移動コストを加算して、
				//目的地から接続先のノードまでに掛かるコストを求める
				nodeCost = (*itr)->m_cost + connect.cost;

				//初回の探索もしくは、既に登録されている移動コストより少ないならば、
				if (connect.node->m_cost < 0.0f ||
					nodeCost < connect.node->m_cost)
				{
					//移動コストと移動元のノードを設定
					connect.node->m_cost = nodeCost;
					connect.node->m_nextNodeToGoal = (*itr);
				}
				else {
					continue;
				}

				//次の検索リストに接続先のノードを追加
				nextLevel->push_back(connect.node);
			}
		}

		//検索リストを入れ替える
		for_swap = currLevel;
		currLevel = nextLevel;
		nextLevel = for_swap;
		nextLevel->clear();
	}

	//ルートの開発ノードを記憶しておく
	m_routeStartNode = start;

	//目的地のノードまで移動するための経路で
	//次に移動するノードを返す
	return start->m_nextNodeToGoal;
}

//指定された座標に一番近いノードを取得
NavNode* NavManeger::GetNearNavNode(const CVector3D& pos)
{
	//一番近いノードを参照するポインター
	NavNode* nearNode = nullptr;
	//一番近いノードまでの距離を格納する変数
	float nearDist = 0.0f;
	//全てのノードの距離を計測
	for (NavNode* node : m_nodes) {
		//目的地用のノードは検索対象にならない
		if (node->m_type == NavNode::NodeType::Destination) {
			continue;
		}

		//ノード同士の距離を取得
		float dist = (pos - node->m_pos).Length();
		if (dist < nearDist || nearNode == nullptr) {
			nearNode = node;
			nearDist = dist;
		}
	}
	return nearNode;
}

//指定された座標に一番近いノードを取得
NavNode* NavManeger::GetNearNavNode(NavNode* node)
{
	CVector3D pos = node->GetPos();

	//一番近いノードを参照するポインター
	NavNode* nearNode = nullptr;
	//一番近いノードまでの距離を格納する変数
	float nearDist = 0.0f;
	//全てのノードの距離を計測
	for (NavNode* findNode : m_nodes) {
		//調べるノードと同じ場合は、スルー
		if (node == findNode)continue;
		//目的地用のノードは検索対象にはならない
		if (node->m_type == NavNode::NodeType::Destination) {
			continue;
		}

		//ノード同士の距離を取得
		float dist = (pos - findNode->m_pos).Length();
		//今設定されているノードより近いまたは、一番最初のノードであれば、
		//一番近いノードとして登録
		if (dist < nearDist || nearNode == nullptr) {
			nearNode = findNode;
			nearDist = dist;
		}
	}

	//取得した一番近いノードを返す
	return nearNode;
}

//接続できるノードを検索
int NavManeger::FindConnectNavNodes(NavNode* node, float distance)
{
	//現在の接続先の情報をクリアにしておく
	node->ClearConnects();

	for (NavNode* n : m_nodes) {
		//自分自身はスルー
		if (n == node) continue;
		
		//指定された距離の範囲内か判定
		float dist = (n->m_pos - node->m_pos).Length();
		if (dist > distance) continue;

		//間に遮蔽物が存在しないか判定
		CVector3D hitPos, hitNormal;
		CVector3D start = node->m_pos;
		CVector3D end = n->m_pos;
		start.y = 1.0f;
		end.y = 1.0f;
		//まずレイを飛ばして、何かにヒットするかどうか
		if (Stage::CollisionRay(start, end, &hitPos, &hitNormal)) {
			//何かにヒットした場合は、ヒット位置までの距離と、
			//開始地点から終了地点までの距離を測り、
			//ヒット位置までの距離の方が小さい場合は、
			//相手のノードが遮蔽物に隠れているため、スルーする
			float hitDist = (hitPos - start).Length();
			if (dist > hitDist)continue;
		}
		//両方の条件を満たしたノードを接続リストに追加
		node->AddConnect(n);
	}

	return node->m_connects.size();
}

//全ノードを描画
void NavManeger::Render()
{
	//リスト内ノードを全て描画
	for (NavNode* node : m_nodes) {
		node->Render();
	}

	glDisable(GL_DEPTH_TEST);

	//地面と重ならないように少し上に上げる値
	float offsetY = 0.01f;
	NavNode* current = m_routeStartNode;
	while (current != nullptr) {
		NavNode* next = current->m_nextNodeToGoal;
		if (next == nullptr) break;

		//ルート上のノードかどうかで色を分ける
		CVector4D lineColor = CVector4D(0.25f, 1.0f, 1.0f, 1.0f);
		float lineWidth = 4.0f;
		//ラインを描画
		Utility::DrawLine
		(
			current->m_pos + CVector3D(0.0f, offsetY, 0.0f),
			next->m_pos + CVector3D(0.0f, offsetY, 0.0f),
			lineColor,
			lineWidth
		);

		//次のノードへ
		current = next;
	}

	glEnable(GL_DEPTH_TEST);
}
