#pragma once

#include "MainComponent.h"

class Engine::CGameObject;
class Engine::CGraphicsComponent;
class Engine::CTransformComponent;
class CCubeComponent final : public Engine::CMainComponent
{
public:
	explicit CCubeComponent();
	~CCubeComponent() = default;

public:
	SHARED(Engine::CComponent)	MakeClone(Engine::CGameObject * pObject);

	void						Awake(void) override;
	void						Start(SHARED(Engine::CComponent) spThis) override;

	_uint						FixedUpdate(SHARED(Engine::CComponent) spThis) override;
	_uint						Update(SHARED(Engine::CComponent) spThis) override;
	_uint						LateUpdate(SHARED(Engine::CComponent) spThis) override;

	void						OnDestroy(void) override;
	void						OnEnable(void) override;
	void						OnDisable(void) override;
private:
	void						Move(void);
protected:
	GETTOR(SHARED(Engine::CTransformComponent), m_pTransform, nullptr, Transform)
		GETTOR(SHARED(Engine::CGraphicsComponent), m_pGraphics, nullptr, Graphics)
		GETTOR_SETTOR(_float, m_moveSpeed, {}, MoveSpeed)
		GETTOR_SETTOR(_float, m_jumpForce, {}, JumpForce)

		SHARED(Engine::CTransformComponent) m_spCamTransform;

};

