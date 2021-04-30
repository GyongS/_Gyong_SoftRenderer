#include "stdafx.h"
#include "NextPointComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ColliderManager.h"
#include "LoadingScene.h"
CNextPointComponent::CNextPointComponent()
{
}


CNextPointComponent::~CNextPointComponent()
{
}

SHARED(Engine::CComponent) CNextPointComponent::MakeClone(Engine::CGameObject * pObject)
{
	SHARED(CNextPointComponent) pClone(new CNextPointComponent);
	pClone->SetOwner(pObject);
	pClone->SetName(m_name);

	return pClone;
}

void CNextPointComponent::Awake(void)
{
	__super::Awake();

	m_componentID = (int)m_s_componentID;
	m_pOwner->SetLayerKey(L"NextScenePoint");
	m_pOwner->SetObjectKey(L"NextScenePoint");
}

void CNextPointComponent::Start(SHARED(CComponent) spThis)
{
	if (auto& pCC = m_pOwner->GetComponent<Engine::CColliderComponent>())
		pCC->SetColliderID((_int)EColliderID::CheckingPlayer);
}

_uint CNextPointComponent::FixedUpdate(SHARED(CComponent) spThis)
{
	return _uint();
}

_uint CNextPointComponent::Update(SHARED(CComponent) spThis)
{
	std::vector<Engine::CGameObject*> returnCollider;
	auto& pTransformComponent = m_pOwner->GetComponent<Engine::CTransformComponent>();

	bool crushCheck = false;
	SHARED(Engine::CColliderComponent) pCC = m_pOwner->GetComponent<Engine::CColliderComponent>();

	if (pCC->GetIsStarted())
		crushCheck = Engine::CColliderManager::GetInstance()->OnColliderEnter(pCC, returnCollider);

	if (crushCheck)
	{
		for (auto& col : returnCollider)
		{
			if (col->GetObjectKey() == L"Player")
			{
				Engine::GET_CUR_SCENE->GetMainCamera()->SetCameraEvent(true);

				//m_pOwner->SetIsNeedToBeDeleted(true);
				/*if (Engine::GET_CUR_SCENE->GetName() == L"Stage3Scene")
					Engine::CSceneManager::GetInstance()->SceneChange(CLoadingScene::Create(ESceneID::Stage3));*/

			}
		}
	}
	return _uint();
}

_uint CNextPointComponent::LateUpdate(SHARED(CComponent) spThis)
{
	return _uint();
}

void CNextPointComponent::OnDestroy(void)
{
}

void CNextPointComponent::OnEnable(void)
{
}

void CNextPointComponent::OnDisable(void)
{
}
