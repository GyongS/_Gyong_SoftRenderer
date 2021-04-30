#include "stdafx.h"
#include "EditorScene.h"
#include "GameObject.h"
#include "ObjectFactory.h"

#include "InputManager.h"
#include "TextureStore.h"
#include "Layer.h"
#include "MainFrm.h"
#include "ObjectListView.h"
#include "EditorView.h"
#include "SceneManager.h"

CEditorScene::CEditorScene()
{
}

CEditorScene::~CEditorScene()
{
}

SHARED(Engine::CScene) CEditorScene::Create(void)
{
	SHARED(CEditorScene) pCLogoScene(new CEditorScene, Engine::SmartDeleter<CEditorScene>);

	return pCLogoScene;
}

void CEditorScene::Awake(void)
{
	__super::Awake();
	InitLayers();
	InitPrototypes();
}

void CEditorScene::Start(void)
{
	__super::Start();

	m_pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	m_pEditorView = dynamic_cast<CEditorView*>(m_pMain->m_mainSplitter.GetPane(0, 0));
	m_pListView = dynamic_cast<CObjectListView*>(m_pMain->m_uiSplitter.GetPane(0, 0));

	m_pMainCamera = Engine::ADD_CLONE(L"Camera", L"Camera", false)->GetComponent<Engine::CCameraComponent>();
	

	SHARED(Engine::CGameObject) pObj = Engine::ADD_CLONE(L"NormalBlock", L"WhiteBlock", false);

	m_pEditorView->Set_CubeData(pObj);
}

_uint CEditorScene::FixedUpdate(void)
{
	return _uint();
}

_uint CEditorScene::Update(void)
{
	_uint event = NO_EVENT;
	if (event = __super::Update())
		return event;

	m_pMainCamera->CameraMove();
	
	if (Engine::IMKEY_PRESS(KEY_RBUTTON))
	{
		if (Engine::IMKEY_DOWN(KEY_RBUTTON))
		{
			POINT curPt;
			GetCursorPos(&curPt);
			m_pMainCamera->SetCenterPt(curPt);
			
		}
		m_pMainCamera->CameraRotation();
	}
	
	

	BlockPicking();
	SetAddFirstCubeToListBox();
}

_uint CEditorScene::LateUpdate(void)
{
	_uint event = NO_EVENT;
	if (event = __super::LateUpdate())
		return event;


	return event;
}

void CEditorScene::OnDestroy(void)
{
	__super::OnDestroy();

}

void CEditorScene::OnEnable(void)
{
}

void CEditorScene::OnDisable(void)
{
}

void CEditorScene::InitLayers(void)
{
	AddLayer(L"Camera");
	AddLayer(L"NormalBlock");
	AddLayer(L"EventBlock");
}

void CEditorScene::InitPrototypes(void)
{
	SHARED(Engine::CGameObject) pCamera = Engine::CGameObject::Create(L"Camera", L"Camera", false);
	pCamera->AddComponent<Engine::CTransformComponent>();
	pCamera->GetComponent<Engine::CTransformComponent>()->SetPosition(_float3(0, 0, -5));
	pCamera->AddComponent<Engine::CCameraComponent>();
	Engine::CObjectFactory::GetInstance()->AddPrototype(pCamera);

	SHARED(Engine::CGameObject) pWhiteBlock = Engine::CGameObject::Create(L"NormalBlock", L"WhiteBlock", false);
	pWhiteBlock->AddComponent<Engine::CMeshComponent>();
	pWhiteBlock->AddComponent<Engine::CTextureComponent>();
	pWhiteBlock->AddComponent<Engine::CTransformComponent>();
	pWhiteBlock->AddComponent<Engine::CGraphicsComponent>();
	Engine::CObjectFactory::GetInstance()->AddPrototype(pWhiteBlock);

	SHARED(Engine::CGameObject) pRedBlock = Engine::CGameObject::Create(L"EventBlock", L"RedBlock", false);
	pRedBlock->AddComponent<Engine::CMeshComponent>();
	pRedBlock->AddComponent<Engine::CTextureComponent>();
	pRedBlock->AddComponent<Engine::CTransformComponent>();
	pRedBlock->AddComponent<Engine::CGraphicsComponent>();
	Engine::CObjectFactory::GetInstance()->AddPrototype(pRedBlock);

	SHARED(Engine::CGameObject) pBlueBlock = Engine::CGameObject::Create(L"EventBlock", L"BlueBlock", false);
	pBlueBlock->AddComponent<Engine::CMeshComponent>();
	pBlueBlock->AddComponent<Engine::CTextureComponent>();
	pBlueBlock->AddComponent<Engine::CTransformComponent>();
	pBlueBlock->AddComponent<Engine::CGraphicsComponent>();
	Engine::CObjectFactory::GetInstance()->AddPrototype(pBlueBlock);

	SHARED(Engine::CGameObject) pYellowBlock = Engine::CGameObject::Create(L"EventBlock", L"YellowBlock", false);
	pYellowBlock->AddComponent<Engine::CMeshComponent>();
	pYellowBlock->AddComponent<Engine::CTextureComponent>();
	pYellowBlock->AddComponent<Engine::CTransformComponent>();
	pYellowBlock->AddComponent<Engine::CGraphicsComponent>();
	Engine::CObjectFactory::GetInstance()->AddPrototype(pYellowBlock);

	SHARED(Engine::CGameObject) pGreenBlock = Engine::CGameObject::Create(L"EventBlock", L"GreenBlock", false);
	pGreenBlock->AddComponent<Engine::CMeshComponent>();
	pGreenBlock->AddComponent<Engine::CTextureComponent>();
	pGreenBlock->AddComponent<Engine::CTransformComponent>();
	pGreenBlock->AddComponent<Engine::CGraphicsComponent>();
	Engine::CObjectFactory::GetInstance()->AddPrototype(pGreenBlock);
}


void CEditorScene::BlockPicking()
{
	_float3 intersection = {};
	
	if (m_pListView->m_wsBlockColor == L"")
		return;

	if (m_pListView->m_wsBlockColor == L"WhiteBlock")
	{
		m_pListView->m_wsBlockType = L"NormalBlock";
	}
	else
	{
		m_pListView->m_wsBlockType = L"EventBlock";
	}

	if (Engine::CInputManager::GetInstance()->KeyDown(KEY_TAB) && !m_isPickingMode)
	{
		std::cout << "picking mode on" << std::endl;
		m_isPickingMode = true;
	}
	else if (Engine::CInputManager::GetInstance()->KeyDown(KEY_TAB) && m_isPickingMode)
	{
		std::cout << "picking mode off" << std::endl;
		m_isPickingMode = false;
	}

	if (!m_isPickingMode)
	{
		SelectBlock();
	}

	ChangeCameraYaxis();

	// 큐브 생성
	if (Engine::CInputManager::GetInstance()->KeyDown(MOUSE_LEFT) && m_isPickingMode)
	{
		CreateCubeWhenClicked();
	}
	// 큐브 삭제
	else if (Engine::CInputManager::GetInstance()->KeyDown(KEY_DELETE) && m_isPickingMode)
	{
		DeleteCubeWhenClicked();
	}
	return;
}

void CEditorScene::SetAddFirstCubeToListBox()
{
	if (m_AddFistCubeToListBox)
	{
		std::wstring defaultCubeKey = m_pEditorView->GetGameObjects().front()->GetObjectKey();
		m_pListView->m_ListBox.AddString(defaultCubeKey.c_str());
		m_AddFistCubeToListBox = false;
	}
}

void CEditorScene::ChangeCameraYaxis()
{
	if (Engine::CInputManager::GetInstance()->KeyDown(KEY_Z))
	{
		auto& cameraTransform = Engine::GET_CUR_SCENE->GetMainCamera()->GetTransform();
		_float3 camPos = cameraTransform->GetPosition();

		cameraTransform->SetPositionY(camPos.y + 5.f);
	}
	else if (Engine::CInputManager::GetInstance()->KeyDown(KEY_X))
	{
		auto& cameraTransform = Engine::GET_CUR_SCENE->GetMainCamera()->GetTransform();
		_float3 camPos = cameraTransform->GetPosition();

		cameraTransform->SetPositionY(camPos.y - 5.f);
	}
}

void CEditorScene::CreateCubeWhenClicked()
{
	_float3 intersection = { 0.f, 0.f, 0.f };

	Engine::CGameObject* target = Engine::CInputManager::GetInstance()->MousePicking(L"NormalBlock", intersection);

	if (!target)
	{
		target = Engine::CInputManager::GetInstance()->MousePicking(L"EventBlock", intersection);
	}

	if (!target)
	{
		return;
	}

	_float3 targetPos = target->GetComponent<Engine::CTransformComponent>()->GetPosition();
		if (!target)
		{
			target = Engine::CInputManager::GetInstance()->MousePicking(L"EventBlock", intersection);
			
		}
		if (!target)
		{
			return;
		}
		targetPos = target->GetComponent<Engine::CTransformComponent>()->GetPosition();

	std::cout << "======================================" << std::endl;
	std::cout << "objX: " << targetPos.x << " objY: " << targetPos.y << " objZ: " << targetPos.z << std::endl;
	std::cout << "x: " << intersection.x << " y: " << intersection.y << " z: " << intersection.z << std::endl;

	// 큐브 크기가 다르므로 그에 따른 코드 수정필요함
	_float3 newPos = targetPos;

	if (targetPos.x + 0.49f <= intersection.x)
		newPos.x = targetPos.x + 1.f;
	else if (targetPos.x - 0.49f >= intersection.x)
		newPos.x = targetPos.x - 1.f;

	if (targetPos.y + 0.49f <= intersection.y)
		newPos.y = targetPos.y + 1.f;

	else if (targetPos.y - 0.49f >= intersection.y)
		newPos.y = targetPos.y - 1.f;

	if (targetPos.z + 0.49f <= intersection.z)
		newPos.z = targetPos.z + 1.f;

	else if (targetPos.z - 0.49f >= intersection.z)
		newPos.z = targetPos.z - 1.f;


	std::cout << "newX: " << newPos.x << " newY: " << newPos.y << " newZ: " << newPos.z << std::endl;
	std::cout << "======================================" << std::endl;

	// 이미 위치에 큐브가 존재한다면 생성안함
	auto& vBlocks = m_mLayers[target->GetLayerKey()]->GetGameObjects();


	for (auto& iter = vBlocks.begin(); iter != vBlocks.end();)
	{
		if (iter->get() == target)
		{
			if (target->GetComponent<Engine::CTransformComponent>()->GetPosition() == newPos)
			{
				std::cout << "Cube already exists.." << std::endl;
				return;
			}
			break;
		}
		else
			++iter;
	}

	SHARED(Engine::CGameObject) pObj = Engine::ADD_CLONE(m_pListView->m_wsBlockType, m_pListView->m_wsBlockColor, false);
	pObj->GetComponent<Engine::CTransformComponent>()->SetPosition(newPos);

	m_pEditorView->Set_CubeData(pObj);

	m_pListView->m_ListBox.AddString(m_pEditorView->GetGameObjects().back()->GetObjectKey().c_str());
}

void CEditorScene::DeleteCubeWhenClicked()
{
	_float3 intersection = { 0.f, 0.f, 0.f };

	// 내가 우클릭 했을 때 LayerKey로 넘긴 가장 가까운 큐브가 target으로 들어옴
	// 즉, 빨간 큐브가 있을 때 그 뒤에 하얀 큐브가 있다고 합시다.
	// 그리고 카메라가 빨간 큐브를 바라보게 두고, 빨간 큐브를 지우려고 클릭 했을 때.
	// 이 때 MousePicking 매개변수로 layerkey를 하얀 큐브를 빨간 큐브보다 먼저 넣었다면 하얀 큐브가 지워짐
	// 그래서 교차점을 이용해 지울 대상이 내가 클릭한 대상이 맞는지 확인하는 절차가 필요함

	// layerkey에 해당하는 가장 가까운 블럭을 변수에 넣음
	// 광선에 걸리는 layerkey가 없다면 nullptr을 넣음
	Engine::CGameObject* target = nullptr;
	Engine::CGameObject* normalBlock = Engine::CInputManager::GetInstance()->MousePicking(L"NormalBlock", intersection);
	Engine::CGameObject* eventBlock = Engine::CInputManager::GetInstance()->MousePicking(L"EventBlock", intersection);

	// 두 layerkey가 광선에 걸렸다면 교차점에서 더 가까운 블럭을 target에 넣음
	if (normalBlock && eventBlock)
	{
		_float3 normalPos = normalBlock->GetComponent<Engine::CTransformComponent>()->GetPosition();
		_float3 eventPos = eventBlock->GetComponent<Engine::CTransformComponent>()->GetPosition();

		if (normalPos - intersection > eventPos - intersection)
		{
			target = eventBlock;
		}
		else
		{
			target = normalBlock;
		}
	}
	// 둘 중 하나의 layerkey만 걸린다면 걸린 블럭만 target에 넣음
	else if (!normalBlock)
	{
		target = eventBlock;
	}
	else if (!eventBlock)
	{
		target = normalBlock;
	}

	// target이 비었다면 반환
	if (!target)
	{
		return;

	}

	m_pCurSelectedObject = target;

	// 대상을 맵에서 찾아서 지움	
	std::cout << "Cube Deleted" << std::endl;
	
	for (size_t i = 0; i < m_pListView->m_ListBox.GetCount(); i++)
	{
		if (m_pEditorView->GetGameObjects()[i]->GetComponent<Engine::CTransformComponent>()->GetPosition() ==
			m_pCurSelectedObject->GetComponent<Engine::CTransformComponent>()->GetPosition())
		{
			m_pListView->m_ListBox.DeleteString(i);
			m_pEditorView->GetGameObjects()[i]->SetIsNeedToBeDeleted(true);

			if (!m_pEditorView->GetGameObjects().empty())
			{
				for (auto& it = m_pEditorView->GetGameObjects().begin(); it != m_pEditorView->GetGameObjects().end();)
				{
					if ((*it)->GetIsNeedToBeDeleted() && it->get() == m_pCurSelectedObject)
					{
						it = m_pEditorView->GetGameObjects().erase(it);
						return;
					}
					else
						++it;
				}
			}
			else
				return;
		}
	} 
	
}

void CEditorScene::SelectBlock()
{
	_float3 intersection = {};

	if (Engine::CInputManager::GetInstance()->KeyDown(MOUSE_LEFT))
	{
		Engine::CGameObject* target = Engine::CInputManager::GetInstance()->MousePicking(L"EventBlock", intersection);

		if (!target)
		{
			target = Engine::CInputManager::GetInstance()->MousePicking(L"NormalBlock", intersection);
		}

		if (!target)
		{
			return;
		}

		// 대상의 정보를 콘솔에 표시
		std::wstring message_w = target->GetObjectKey();
		std::string message_s;
		message_s.assign(message_w.begin(), message_w.end());

		std::cout << "target: " << message_s.c_str() << std::endl;
		std::cout << "targetX: " << target->GetComponent<Engine::CTransformComponent>()->GetPosition().x << std::endl;
		std::cout << "targetY: " << target->GetComponent<Engine::CTransformComponent>()->GetPosition().y << std::endl;
		std::cout << "targetZ: " << target->GetComponent<Engine::CTransformComponent>()->GetPosition().z << std::endl;
		std::cout << "/*****************************************/ " << std::endl;
		target->GetComponent<Engine::CGraphicsComponent>()->SetRenderID(Engine::ERenderID::WireFrame);

		if (m_pPreSelectedObject)
		{
			m_pPreSelectedObject->GetComponent<Engine::CGraphicsComponent>()->SetRenderID(Engine::ERenderID::Base);
		}
		m_pPreSelectedObject = target;
	}
}
