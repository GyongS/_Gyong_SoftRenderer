#include "stdafx.h"
#include "YellowBlockComponent.h"
#include "GameObject.h"
#include "ColliderManager.h"
#include "InputManager.h"
#include "ObjectFactory.h"
#include "DataStore.h"
#include "GameObject.h"
#include "Layer.h"
#include "SceneManager.h"
#include "SoundManager.h"

_uint CYellowBlockComponent::m_yellowBlockCnt = 0;

CYellowBlockComponent::CYellowBlockComponent()
{

}

SHARED(Engine::CComponent) CYellowBlockComponent::MakeClone(Engine::CGameObject * pObject)
{
	SHARED(CYellowBlockComponent) pClone(new CYellowBlockComponent);
	pClone->SetOwner(pObject);
	pClone->SetName(m_name);

	pClone->m_groupNumber = m_yellowBlockCnt / 3;
	pClone->m_orderWithinGroup = m_yellowBlockCnt % 3;
	++m_yellowBlockCnt;

	return pClone;
}

void CYellowBlockComponent::Awake(void)
{
	__super::Awake();
	m_componentID = (_int)m_s_componentID;
	m_pOwner->SetObjectKey(L"YellowBlock");

	std::wstring sectionKey = m_pOwner->GetLayerKey();
	std::wstring objectKey = m_pOwner->GetObjectKey();

	Engine::GET_VALUE(true, sectionKey, objectKey, L"m_activatedLv", m_activatedLv);
	Engine::GET_VALUE(true, sectionKey, objectKey, L"m_maxActivatedLv", m_maxActivatedLv);
}

void CYellowBlockComponent::Start(SHARED(Engine::CComponent) spThis)
{
	__super::Start(spThis);
	auto& pTransformComponent = m_pOwner->GetComponent<Engine::CTransformComponent>();
	m_initialPosition = pTransformComponent->GetPosition();
	m_activationPosition = m_initialPosition;
	pTransformComponent->SetScale({ 1.f, 3.f, 1.f });
}

_uint CYellowBlockComponent::FixedUpdate(SHARED(Engine::CComponent) spThis)
{
	return _uint();
}

_uint CYellowBlockComponent::Update(SHARED(Engine::CComponent) spThis)
{
	if (m_lButton)
	{
		LClicked();
	}
	else if (m_rButton)
	{
		RClicked();
	}

	return _uint();
}

_uint CYellowBlockComponent::LateUpdate(SHARED(Engine::CComponent) spThis)
{
	return _uint();
}

void CYellowBlockComponent::OnDestroy(void)
{
}

void CYellowBlockComponent::OnEnable(void)
{
}

void CYellowBlockComponent::OnDisable(void)
{
}

void CYellowBlockComponent::LClicked(void)
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
			if (col->GetName() == L"Player")
			{
				_float3& playerPos = col->GetComponent<Engine::CTransformComponent>()->GetPosition();
				_float3& blockPos = pTransformComponent->GetPosition();

				if (pTransformComponent->GetPosition() != m_activationPosition)
				{
					_float3 nextPlayerPos = playerPos + FLOAT3_UP;
					col->GetComponent<Engine::CTransformComponent>()->Lerp(playerPos, nextPlayerPos, 1);
				}
			}
		}
	}

	pTransformComponent->Lerp(pTransformComponent->GetPosition(), m_activationPosition, 1);

	if (m_maxActivatedLv <= m_activatedLv) // ?????? ????????? ??????????????? ??????
	{
		return;
	}
	else if (0 != m_activatedLv) // ????????? ?????? ????????? ?????? ????????? ???????????? ?????? ????????????
	{
		// ?????? ?????? ?????? ????????? ????????? ????????? ??????????????? ????????? ????????? ????????? ????????? ???????????????
		++m_activatedLv;
		m_activationPosition += GetDirectionOfMovement(FLOAT3_UP, pTransformComponent->GetRotation());
	}

	if (m_isInteractBlockWithLBtn)
	{
		//if (m_arrBlocks[0] != nullptr && CheckBlocksForRayCollision())
		//{
		//	m_isInteractBlockWithLBtn = false;
		//	return;
		//}

		Engine::CSoundManager::GetInstance()->StartSound(L"BoxPush.wav", Engine::CHANNELID::BOX);

		// ????????? ?????? ?????? ??????
		Engine::CLayer* pLayer = Engine::GET_CUR_SCENE->GetLayers()[L"EventBlock"].get();
		_uint index = 1;

		m_arrBlocks[0] = m_pOwner; // ????????? ????????? ??? ????????? ??????

		for (auto& object : pLayer->GetGameObjects())
		{
			// ????????? ?????? ??? ???????????? ?????? ?????? ?????????.
			if (object->GetObjectKey() != L"YellowBlock")
				continue;

			auto& objCom = object->GetComponent<CYellowBlockComponent>();
			_uint order = objCom->GetOrderWithinGroup(); // ?????? ??? ????????? ??????

			// ????????? ?????? ??? ?????? ????????? ????????? ?????? ??????
			// ????????? ????????? ????????? ?????? ?????? ?????? ????????? ??????
			if (m_groupNumber == objCom->GetGroupNumber() && 
				m_orderWithinGroup != order)
			{
				// ??? ?????? ????????? ?????? ???????????? ????????? ????????? ????????? ???????????? ????????? ????????????
				// ????????? ????????? ??????????????? ????????? ?????? ????????? ?????? ????????? ?????? ??????
				if (1 != m_orderWithinGroup)
				{
					if (1 == order)
					{
						m_arrBlocks[1] = object.get();
						++index;
					}
					else if (0 == order || 2 == order)
					{
						m_arrBlocks[2] = object.get();
					}
				}
				else // ?????? ????????? ????????? ??????????????? ????????? ????????? ????????? ???????????? ?????????
				{
					m_arrBlocks[index] = object.get();
					++index;
				}
			}
		}

		// ?????? ??? ???????????? ?????? ???????????? ?????? ???????????? ?????????
		auto& secondBlockCom = m_arrBlocks[1]->GetComponent<CYellowBlockComponent>();
		auto& thirdBlockCom = m_arrBlocks[2]->GetComponent<CYellowBlockComponent>();

		secondBlockCom->SetLButton(true);
		secondBlockCom->SetRButton(false);

		thirdBlockCom->SetLButton(true);
		thirdBlockCom->SetRButton(false);

		memcpy(secondBlockCom->m_arrBlocks, m_arrBlocks, sizeof(Engine::CGameObject*) * 3);
		memcpy(thirdBlockCom->m_arrBlocks, m_arrBlocks, sizeof(Engine::CGameObject*) * 3);

		if (1 == m_orderWithinGroup)
		{
			m_activatedLv += 2; // ????????? ????????? ??????????????? 2?????? ???????????? ?????????
			// ?????? ????????? ??? ?????? ???????????????
			secondBlockCom->SetActivatedLv(2);
			thirdBlockCom->SetActivatedLv(2);
		}
		else
		{
			++m_activatedLv;
			// ?????? ???????????? 1?????? ?????? ??????????????? ??????
			secondBlockCom->SetActivatedLv(1);
			thirdBlockCom->SetActivatedLv(2);
		}

		m_activationPosition += GetDirectionOfMovement(FLOAT3_UP, pTransformComponent->GetRotation());
		m_isInteractBlockWithLBtn = false;
	}
}

void CYellowBlockComponent::RClicked(void)
{
	std::vector<Engine::CGameObject*> returnCollider;
	bool crushCheck = Engine::CColliderManager::GetInstance()->OnColliderEnter(m_pOwner->GetComponent<Engine::CColliderComponent>(), returnCollider);
	auto& pTransformComponent = m_pOwner->GetComponent<Engine::CTransformComponent>();
	if (crushCheck)
	{
		for (auto& col : returnCollider)
		{
			if (col->GetName() == L"Player")
			{
				auto& playerCom = col->GetComponent<Engine::CTransformComponent>();
				_float3& playerPos = playerCom->GetPosition();
				_float3& blockPos = pTransformComponent->GetPosition();

				// ?????? ??????????????? ????????? ????????? ????????????
				if (pTransformComponent->GetPosition() != m_activationPosition)
				{
					// ????????? ???????????? y = ????????? y + (??????????????? ??????) + (????????? ?????? / 2)
					playerPos.y = blockPos.y + (playerCom->GetScale().y) + (pTransformComponent->GetScale().y * 0.5f);
				}
			}
		}
	}

	pTransformComponent->Lerp(pTransformComponent->GetPosition(), m_activationPosition,1);

	if (0 == m_activatedLv)
		return;
	else if (0 != m_activatedLv) // ????????? ?????? ????????? ?????? ????????? ???????????? ?????? ????????????
	{
		--m_activatedLv;
		m_activationPosition += GetDirectionOfMovement(FLOAT3_DOWN, pTransformComponent->GetRotation());
	}

	if (m_isInteractBlockWithRBtn)
	{
		// ???????????? 3?????? ?????? ??? ?????? ?????? ????????? ?????? ?????? ????????? ???????????????
		// ???????????? ????????? ???????????? ?????? ????????? ?????? ?????? ????????? ????????? ???????????? ?????????
		// ????????? m_arrBlock ????????? ????????? ?????? ???????????? ?????? ????????? ?????????

		// ?????? ??? ???????????? ?????? ???????????? ?????? ???????????? ?????????
		auto& firstBlockCom = m_arrBlocks[0]->GetComponent<CYellowBlockComponent>();
		auto& secondBlockCom = m_arrBlocks[1]->GetComponent<CYellowBlockComponent>();
		auto& thirdBlockCom = m_arrBlocks[2]->GetComponent<CYellowBlockComponent>();

		firstBlockCom->SetLButton(false);
		firstBlockCom->SetRButton(true);

		secondBlockCom->SetLButton(false);
		secondBlockCom->SetRButton(true);

		thirdBlockCom->SetLButton(false);
		thirdBlockCom->SetRButton(true);

		if (1 != firstBlockCom->GetOrderWithinGroup())
		{
			secondBlockCom->GetActivatedLv() -= 1;
			thirdBlockCom->GetActivatedLv() -= 2;
		}
		else
		{
			firstBlockCom->GetActivatedLv() -= 1;
			secondBlockCom->GetActivatedLv() -= 2;
			thirdBlockCom->GetActivatedLv() -= 2;
		}

		memset(firstBlockCom->m_arrBlocks, sizeof(Engine::CGameObject) * 3, 0);
		memset(secondBlockCom->m_arrBlocks, sizeof(Engine::CGameObject) * 3, 0);
		memset(thirdBlockCom->m_arrBlocks, sizeof(Engine::CGameObject) * 3, 0);
		
		m_isInteractBlockWithRBtn = false;
	}
}

_float3 CYellowBlockComponent::GetDirectionOfMovement(_float3 vAt, _float3 vRotation)
{
	_float3 vOut = {};
	_mat rotate;

	D3DXMatrixRotationYawPitchRoll(&rotate, vRotation.y, vRotation.x, vRotation.z);
	D3DXVec3TransformCoord(&vOut, &vAt, &rotate);

	return vOut;
}

_bool CYellowBlockComponent::CheckBlocksForRayCollision()
{
	for (int i = 0; i < 3; ++i)
	{
		auto& blockTf = m_arrBlocks[i]->GetComponent<Engine::CTransformComponent>();

		_float3 rayPos = blockTf->GetPosition();
		_float3 rayDir = GetDirectionOfMovement(FLOAT3_UP, blockTf->GetRotation());

		if (Engine::CInputManager::GetInstance()->RayCast(rayPos, rayDir, 3.1f, L"NormalBlock"))
		{
			return true;
		}
		if (Engine::CInputManager::GetInstance()->RayCast(rayPos, rayDir, 3.1f, L"EventBlock"))
		{
			return true;
		}
	}

	return false;
}

//void CYellowBlockComponent::FirstCubePicking()
//{
	//if (m_vBlocks[0]->GetComponent<Engine::CTransformComponent>()->GetPosition().y <= 2.f)
	//{
	//	m_vBlocks[0]->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, 2.5f * GET_DT, 0.f));
	//}
	//else if (m_vBlocks[0]->GetComponent<Engine::CTransformComponent>()->GetPosition().y >= 2.f)
	//{
	//	if (m_vBlocks[1]->GetComponent<Engine::CTransformComponent>()->GetPosition().y <= 1.f)
	//		m_vBlocks[1]->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, 2.5f * GET_DT, 0.f));

	//	else if (m_vBlocks[1]->GetComponent<Engine::CTransformComponent>()->GetPosition().y >= 1.f)
	//	{
	//		if (m_vBlocks[2]->GetComponent<Engine::CTransformComponent>()->GetPosition().y <= 0.f)
	//			m_vBlocks[2]->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, 2.5f * GET_DT, 0.f));

	//		else if (m_vBlocks[2]->GetComponent<Engine::CTransformComponent>()->GetPosition().y >= 0.f)
	//			m_isCubeMoveEnd = true;
	//	}
	//}

	
//}

//void CYellowBlockComponent::SecondCubePicking()
//{
//	if (y0->GetComponent<Engine::CTransformComponent>()->GetPosition().y <= 0.f && y2->GetComponent<Engine::CTransformComponent>()->GetPosition().y <= 0.f)
//	{
//		y0->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, 2.5f * GET_DT, 0.f));
//		y2->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, 2.5f * GET_DT, 0.f));
//	}
//	else if (y0->GetComponent<Engine::CTransformComponent>()->GetPosition().y >= 0.f && y2->GetComponent<Engine::CTransformComponent>()->GetPosition().y >= 0.f)
//	{
//		if (y1->GetComponent<Engine::CTransformComponent>()->GetPosition().y <= 1.f)
//			y1->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, 2.5f * GET_DT, 0.f));
//
//		if (y1->GetComponent<Engine::CTransformComponent>()->GetPosition().y >= 1.f)
//			m_isCubeMoveEnd = true;
//	}
//}
//
//void CYellowBlockComponent::ThirdCubePicking()
//{
//	if (y2->GetComponent<Engine::CTransformComponent>()->GetPosition().y <= 2.f)
//	{
//		y2->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, 2.5f * GET_DT, 0.f));
//	}
//	else if (y2->GetComponent<Engine::CTransformComponent>()->GetPosition().y >= 2.f)
//	{
//		if (y1->GetComponent<Engine::CTransformComponent>()->GetPosition().y <= 1.f)
//			y1->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, 2.5f * GET_DT, 0.f));
//
//		else if (y1->GetComponent<Engine::CTransformComponent>()->GetPosition().y >= 1.f)
//		{
//			if (y0->GetComponent<Engine::CTransformComponent>()->GetPosition().y <= 0.f)
//				y0->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, 2.5f * GET_DT, 0.f));
//
//			if (y0->GetComponent<Engine::CTransformComponent>()->GetPosition().y >= 0.f)
//				m_isCubeMoveEnd = true;
//		}
//	}
//}
//
//void CYellowBlockComponent::FirstCubeInit()
//{
//	if (y0->GetComponent<Engine::CTransformComponent>()->GetPosition().y > -1.f)
//	{
//		y0->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, -2.5f * GET_DT, 0.f));
//	}
//	else if (y0->GetComponent<Engine::CTransformComponent>()->GetPosition().y <= 0.f)
//	{
//		y0->GetComponent<Engine::CTransformComponent>()->SetPositionY(-1);
//		if (y1->GetComponent<Engine::CTransformComponent>()->GetPosition().y > -1.f)
//			y1->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, -2.5f * GET_DT, 0.f));
//
//		else if (y1->GetComponent<Engine::CTransformComponent>()->GetPosition().y < 0.f)
//		{
//			y1->GetComponent<Engine::CTransformComponent>()->SetPositionY(-1);
//			if (y2->GetComponent<Engine::CTransformComponent>()->GetPosition().y > -1.f)
//				y2->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, -2.5f * GET_DT, 0.f));
//
//			if (y2->GetComponent<Engine::CTransformComponent>()->GetPosition().y <= -1.f)
//			{
//				y2->GetComponent<Engine::CTransformComponent>()->SetPositionY(-1);
//				m_isCubeMoveStart = false;
//				m_isCubeMoveEnd = false;
//			}
//
//		}
//	}
//}
//
//void CYellowBlockComponent::SecondCubeInit()
//{
//	if (y1->GetComponent<Engine::CTransformComponent>()->GetPosition().y < 2.f)
//	{
//		if (y1->GetComponent<Engine::CTransformComponent>()->GetPosition().y > -1.f)
//			y1->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, -2.5f * GET_DT, 0.f));
//
//		else if (y1->GetComponent<Engine::CTransformComponent>()->GetPosition().y < 0.f)
//		{
//			y1->GetComponent<Engine::CTransformComponent>()->SetPositionY(-1);
//
//			y0->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, -2.5f * GET_DT, 0.f));
//			y2->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, -2.5f * GET_DT, 0.f));
//
//			if (y0->GetComponent<Engine::CTransformComponent>()->GetPosition().y < -1.f && y2->GetComponent<Engine::CTransformComponent>()->GetPosition().y < -1.f)
//			{
//				y0->GetComponent<Engine::CTransformComponent>()->SetPositionY(-1);
//				y2->GetComponent<Engine::CTransformComponent>()->SetPositionY(-1);
//				m_isCubeMoveStart = false;
//				m_isCubeMoveEnd = false;
//			}
//
//		}
//
//	}
//}
//
//void CYellowBlockComponent::ThirdCubeInit()
//{
//	if (y2->GetComponent<Engine::CTransformComponent>()->GetPosition().y > -1.f)
//	{
//		y2->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, -2.5f * GET_DT, 0.f));
//	}
//	else if (y2->GetComponent<Engine::CTransformComponent>()->GetPosition().y <= -1.f)
//	{
//		y2->GetComponent<Engine::CTransformComponent>()->SetPositionY(-1);
//
//		if (y1->GetComponent<Engine::CTransformComponent>()->GetPosition().y > -1.f)
//			y1->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, -2.5f * GET_DT, 0.f));
//
//		else if (y1->GetComponent<Engine::CTransformComponent>()->GetPosition().y <= -1.f)
//		{
//			y1->GetComponent<Engine::CTransformComponent>()->SetPositionY(-1);
//			if (y0->GetComponent<Engine::CTransformComponent>()->GetPosition().y > -1.f)
//				y0->GetComponent<Engine::CTransformComponent>()->Translate(_float3(0.f, -2.5f * GET_DT, 0.f));
//
//			if (y0->GetComponent<Engine::CTransformComponent>()->GetPosition().y <= -1.f)
//			{
//				y0->GetComponent<Engine::CTransformComponent>()->SetPositionY(-1);
//				m_isCubeMoveStart = false;
//				m_isCubeMoveEnd = false;
//			}
//		}
//	}
//}