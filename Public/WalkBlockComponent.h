#pragma once
#include "MainComponent.h"
class Engine::CGameObject;
class CWalkBlockComponent : public Engine::CMainComponent
{
public:
	CWalkBlockComponent();
	~CWalkBlockComponent();

public:
	SHARED(Engine::CComponent) MakeClone(Engine::CGameObject * pObject);
	void Awake(void) override;
	void Start(SHARED(CComponent) spThis) override;
	_uint FixedUpdate(SHARED(CComponent) spThis) override;
	_uint Update(SHARED(CComponent) spThis) override;
	_uint LateUpdate(SHARED(CComponent) spThis) override;
	void OnDestroy(void) override;
	void OnEnable(void) override;
	void OnDisable(void) override;

public:
	static const	EComponentID	m_s_componentID = EComponentID::Player;

private:
	void Move();

private:
	_float m_frameDelay;

private:
	_int m_hitCount;

protected:
	GETTOR(SHARED(Engine::CTransformComponent), m_pTransform, nullptr, Transform)
		GETTOR(SHARED(Engine::CGraphicsComponent), m_pGraphics, nullptr, Graphics)


};

