#include "stdafx.h"
#include "RhythmBlockComponent.h"
#include "GameObject.h"

CRhythmBlockComponent::CRhythmBlockComponent()
{
}


CRhythmBlockComponent::~CRhythmBlockComponent()
{
}

SHARED(Engine::CComponent) CRhythmBlockComponent::MakeClone(Engine::CGameObject * pObject)
{
	SHARED(CRhythmBlockComponent) pClone(new CRhythmBlockComponent);
	pClone->SetOwner(pObject);
	pClone->SetName(m_name);

	return pClone;
}

void CRhythmBlockComponent::Awake(void)
{
	__super::Awake();
	m_componentID = (_int)m_s_componentID;
	m_pOwner->SetObjectKey(L"RhythmBlock");

	std::wstring sectionKey = m_pOwner->GetLayerKey();
	std::wstring objectKey = m_pOwner->GetObjectKey();
}

void CRhythmBlockComponent::Start(SHARED(Engine::CComponent) spThis)
{
	__super::Start(spThis);
	auto& pTransformComponent = m_pOwner->GetComponent<Engine::CTransformComponent>();
	pTransformComponent->SetScale(_float3(1.f, 1.f, 1.f));
}

_uint CRhythmBlockComponent::FixedUpdate(SHARED(Engine::CComponent) spThis)
{
	return _uint();
}

_uint CRhythmBlockComponent::Update(SHARED(Engine::CComponent) spThis)
{
	return _uint();
}

_uint CRhythmBlockComponent::LateUpdate(SHARED(Engine::CComponent) spThis)
{
	return _uint();
}

void CRhythmBlockComponent::OnDestroy(void)
{
}

void CRhythmBlockComponent::OnEnable(void)
{
}

void CRhythmBlockComponent::OnDisable(void)
{
}

void CRhythmBlockComponent::LClicked(void)
{
}

void CRhythmBlockComponent::RClicked(void)
{
}
