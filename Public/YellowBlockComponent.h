#pragma once

#include "EventBlockComponent.h"
class CYellowBlockComponent final : public CEventBlockComponent
{
public:
	explicit CYellowBlockComponent();
	~CYellowBlockComponent() = default;

	virtual SHARED(Engine::CComponent) MakeClone(Engine::CGameObject * pObject) override;
	virtual void Awake(void) override;
	virtual void Start(SHARED(Engine::CComponent) spThis) override;
	virtual _uint FixedUpdate(SHARED(Engine::CComponent) spThis) override;
	virtual _uint Update(SHARED(Engine::CComponent) spThis) override;
	virtual _uint LateUpdate(SHARED(Engine::CComponent) spThis) override;
	virtual void OnDestroy(void) override;
	virtual void OnEnable(void) override;
	virtual void OnDisable(void) override;
	virtual void LClicked(void) override;
	virtual void RClicked(void) override;

public:
	static const	EComponentID		m_s_componentID = EComponentID::YellowBlock;
	_float3 GetDirectionOfMovement(_float3 vAt, _float3 vRotation);
	_bool CheckBlocksForRayCollision(); // 광선으로 블럭과 충돌하는지 체크. 충돌하면 true 반환

private:
	//_bool   m_isCubeMoveStart = false;
	//_bool   m_isCubeMoveEnd = false;
	//_int    m_iCnt = 0;
	//_int    m_iPreCnt = 0;
	static _uint m_yellowBlockCnt; // 노랑 블럭의 총 개수. 이를 이용해서 노랑 블럭 생성시 그룹 번호와 그룹 내 순서 번호를 부여한다.
	_float3 m_initialPosition;
	_float3 m_activationPosition;
	Engine::CGameObject* m_arrBlocks[3] = { nullptr }; // 노랑 블럭을 담을 배열. 증가시킬 순서대로 담음

	//SHARED(Engine::CGameObject) y0;
	//SHARED(Engine::CGameObject) y1;
	//SHARED(Engine::CGameObject) y2;

	GETTOR_SETTOR(_uint, m_groupNumber, 0, GroupNumber); // 동일한 그룹 숫자를 가지는 블럭끼리 움직임
	GETTOR_SETTOR(_uint, m_orderWithinGroup, 0, OrderWithinGroup); // 그룹 내에서 순서
	GETTOR_SETTOR(_bool, m_isInteractBlockWithLBtn, false, CheckInteractBlockWithLBtn); // 블럭 상호작용으로 클릭 여부
	GETTOR_SETTOR(_bool, m_isInteractBlockWithRBtn, false, CheckInteractBlockWithRBtn); // 블럭 상호작용으로 클릭 여부

//private:
//	// 큐브가 레이케스트에 충돌되었을 때 실행 함수
//	void FirstCubePicking();
//	void SecondCubePicking();
//	void ThirdCubePicking();
//
//	// 큐브를 원래대로 돌려놓는 함수
//	void FirstCubeInit();
//	void SecondCubeInit();
//	void ThirdCubeInit();
};

