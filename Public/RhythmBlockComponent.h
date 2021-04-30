#pragma once

#include "EventBlockComponent.h"

class CRhythmBlockComponent final : public CEventBlockComponent
{
public:
	explicit CRhythmBlockComponent();
	~CRhythmBlockComponent();

public:
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
	static const	EComponentID		m_s_componentID = EComponentID::RhythmBlock;

};