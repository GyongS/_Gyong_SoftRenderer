#pragma once
#include "MainComponent.h"
class Engine::CGameObject;
class CNextPointComponent final : public Engine::CMainComponent
{
public:
	explicit CNextPointComponent();
	~CNextPointComponent();

public:
	SHARED(Engine::CComponent) MakeClone(Engine::CGameObject * pObject);
	virtual void Awake(void) override;
	virtual void Start(SHARED(CComponent) spThis) override;
	virtual _uint FixedUpdate(SHARED(CComponent) spThis) override;
	virtual _uint Update(SHARED(CComponent) spThis) override;
	virtual _uint LateUpdate(SHARED(CComponent) spThis) override;
	virtual void OnDestroy(void) override;
	virtual void OnEnable(void) override;
	virtual void OnDisable(void) override;

public:
	static const	EComponentID	m_s_componentID = EComponentID::Player;

protected:
	GETTOR(SHARED(Engine::CTransformComponent), m_pTransform, nullptr, Transform)
		GETTOR(SHARED(Engine::CGraphicsComponent), m_pGraphics, nullptr, Graphics)
};

