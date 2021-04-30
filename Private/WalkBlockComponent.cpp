#include "stdafx.h"
#include "WalkBlockComponent.h"
#include "ColliderManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"


CWalkBlockComponent::CWalkBlockComponent() : m_hitCount(0), m_frameDelay(0.f)
{
}

CWalkBlockComponent::~CWalkBlockComponent()
{
}

SHARED(Engine::CComponent) CWalkBlockComponent::MakeClone(Engine::CGameObject * pObject)
{
	SHARED(CWalkBlockComponent) pClone(new CWalkBlockComponent);
	pClone->SetOwner(pObject);
	pClone->SetName(m_name);

	return pClone;
}

void CWalkBlockComponent::Awake(void)
{
	__super::Awake();

	m_componentID = (int)m_s_componentID;

	m_pOwner->SetLayerKey(L"WalkBlock");
	m_pOwner->SetObjectKey(L"WalkBlock");
}

void CWalkBlockComponent::Start(SHARED(CComponent) spThis)
{
	__super::Start(spThis);

	if (auto& pCC = m_pOwner->GetComponent<Engine::CColliderComponent>())
		pCC->SetColliderID((_int)EColliderID::Player);
}

_uint CWalkBlockComponent::FixedUpdate(SHARED(CComponent) spThis)
{
	return _uint();
}

_uint CWalkBlockComponent::Update(SHARED(CComponent) spThis)
{

	if (Engine::GET_CUR_SCENE->GetSceneEvent())
	{
		Move();
	}


	std::vector<Engine::CGameObject*> returnCollider;
	auto& pTransformComponent = m_pOwner->GetComponent<Engine::CTransformComponent>();

	bool crushCheck = Engine::CColliderManager::GetInstance()->OnColliderEnter(m_pOwner->GetComponent<Engine::CColliderComponent>(), returnCollider);
	SHARED(Engine::CColliderComponent) pCC = m_pOwner->GetComponent<Engine::CColliderComponent>();

	if (pCC->GetIsStarted())
		crushCheck = Engine::CColliderManager::GetInstance()->OnColliderEnter(pCC, returnCollider);

	if (crushCheck)
	{
		for (auto& col : returnCollider)
		{
			if (col->GetObjectKey() == L"RedBlock")
			{
				// »¡°£ ºí·°ÀÌ¶û Ãæµ¹ µÆÀ» ¶§ ÀÌº¥Æ®
				m_pOwner->GetComponent<Engine::CTransformComponent>()->SetPosition(_float3(3.f, 1.f, -1.f));
				//Engine::GET_CUR_SCENE->GetMainCamera()->GetTransform()->SetPositionZ(m_pOwner->GetComponent<Engine::CTransformComponent>()->GetPosition().z - 5.f);
			}			
			else if (col->GetObjectKey() == L"RhythmBlock")
			{
				m_frameDelay += GET_DT;
				if (m_frameDelay >= 0.16f)
				{
					m_hitCount++;
					m_frameDelay = 0.f;
				}
				if(m_hitCount > 1)
					m_pOwner->GetComponent<Engine::CTransformComponent>()->SetPosition(_float3(3.f, 1.f, -1.f));
				//Engine::GET_CUR_SCENE->GetMainCamera()->GetTransform()->SetPositionZ(m_pOwner->GetComponent<Engine::CTransformComponent>()->GetPosition().z - 5.f);
			}
			else
			{
				m_frameDelay = 0;
				m_hitCount = 0;
			}
		}
	}
	return _uint();
}

_uint CWalkBlockComponent::LateUpdate(SHARED(CComponent) spThis)
{
	return _uint();
}

void CWalkBlockComponent::OnDestroy(void)
{
}

void CWalkBlockComponent::OnEnable(void)
{
	__super::OnEnable();
}

void CWalkBlockComponent::OnDisable(void)
{
	__super::OnDisable();
}

void CWalkBlockComponent::Move()
{
	if (Engine::IMKEY_DOWN(KEY_W))
	{
		m_pOwner->GetComponent<Engine::CTransformComponent>()->AddPositionZ(1.f);
		//Engine::GET_CUR_SCENE->GetMainCamera()->GetTransform()->AddPositionZ(0.5f);
	}

	if (Engine::IMKEY_DOWN(KEY_A))
	{
		m_pOwner->GetComponent<Engine::CTransformComponent>()->AddPositionX(-1.f);
	}

	if (Engine::IMKEY_DOWN(KEY_S))
	{
		m_pOwner->GetComponent<Engine::CTransformComponent>()->AddPositionZ(-1.f);
		//Engine::GET_CUR_SCENE->GetMainCamera()->GetTransform()->AddPositionZ(-0.5f);
	}

	if (Engine::IMKEY_DOWN(KEY_D))
	{
	    m_pOwner->GetComponent<Engine::CTransformComponent>()->AddPositionX(1.f);
	}
}
