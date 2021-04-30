#include "stdafx.h"
#include "LoadingScene.h"
#include "WndApp.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "MainMenuScene.h"
#include "JiyongScene.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "Stage1Scene.h"
#include "Stage2Scene.h"
#include "Stage3Scene.h"
#include "DeviceManager.h"
#include "ColliderManager.h"

CLoadingScene::CLoadingScene() : m_LoadCheck(false)
{
	Engine::CSceneManager::GetInstance()->GetUnitInfo().reserve(1000);
	Engine::CSceneManager::GetInstance()->SetSceneName(L"LoadingScene");
}

CLoadingScene::~CLoadingScene()
{
}

SHARED(Engine::CScene) CLoadingScene::Create(ESceneID eNextSceneID)
{
	SHARED(CLoadingScene) pScene(new CLoadingScene, Engine::SmartDeleter<CLoadingScene>);
	pScene->m_eNextSceneID = eNextSceneID;
	return pScene;
}


void CLoadingScene::Awake(void)
{
	__super::Awake();

	InitLayers();
	InitializeCriticalSection(&m_CriticalSection);
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, CLoadingScene::LoadDataFunc, this, 0, nullptr);
}

void CLoadingScene::Start(void)
{
	__super::Start();

	m_name = GetCurClassName(this);
	SetWindowText(Engine::GET_HANDLE, m_name.c_str());

	SHARED(Engine::CGameObject) pObj = Engine::CObjectFactory::GetInstance()->AddClone(L"Default", L"Default", true);
	pObj->GetComponent<Engine::CTransformComponent>()->SetScale(_float3(800.0f, 600.0f, 1.0f));
	pObj->GetComponent<Engine::CTransformComponent>()->SetPosition(_float3(0, 0, 1));
	pObj->AddComponent<Engine::CUIComponent>()->SetTextureKey(L"LoadingUI");

	m_pMainCamera = Engine::ADD_CLONE(L"Camera", L"Camera", true)->GetComponent<Engine::CCameraComponent>();
}

_uint CLoadingScene::FixedUpdate(void)
{
	__super::FixedUpdate();

	_uint event = 0;
	if (event = __super::FixedUpdate())
		return event;


	return event;
}

_uint CLoadingScene::Update(void)
{
	__super::Update();

	_uint event = 0;
	if (event = __super::Update())
		return event;

	
	if (m_LoadCheck)
	{
			WaitForSingleObject(m_hThread, INFINITE);
			CloseHandle(m_hThread);
			DeleteCriticalSection(&m_CriticalSection);

			switch (m_eNextSceneID)
			{
			case ESceneID::Logo:
				break;
			case ESceneID::MainMenu:
				break;
			case ESceneID::Stage1:
				Engine::CSceneManager::GetInstance()->SceneChange(CStage1Scene::Create());
				break;
			case ESceneID::Stage2:
				Engine::CSceneManager::GetInstance()->SceneChange(CStage2Scene::Create());
				break;
			case ESceneID::Stage3:
				Engine::CSceneManager::GetInstance()->SceneChange(CStage3Scene::Create());
				break;
			default:
				break;
			}
			return SCENE_CHANGE;
	}
	return 0;
}

_uint CLoadingScene::LateUpdate(void)
{
	__super::LateUpdate();

	_uint event = 0;
	if (event = __super::LateUpdate())
		return event;


	return event;
}

void CLoadingScene::OnDestroy(void)
{
	__super::OnDestroy();
}

void CLoadingScene::OnEnable(void)
{
}

void CLoadingScene::OnDisable(void)
{
}

void CLoadingScene::InitLayers(void)
{
	AddLayer(L"Default");
	AddLayer(L"Camera");
}

void CLoadingScene::InitPrototypes(void)
{
	SHARED(Engine::CGameObject) pWhiteBlock = Engine::CGameObject::Create(L"NormalBlock", L"WhiteBlock", true);
	pWhiteBlock->AddComponent<CWhiteBlockComponent>();
	pWhiteBlock->AddComponent<Engine::CMeshComponent>();
	pWhiteBlock->AddComponent<Engine::CTextureComponent>();
	pWhiteBlock->AddComponent<Engine::CTransformComponent>();
	pWhiteBlock->AddComponent<Engine::CColliderComponent>()->AddCollider(Engine::CAabbCollider::Create(FLOAT3_ONE, FLOAT3_ZERO));
	pWhiteBlock->AddComponent<Engine::CGraphicsComponent>();
	Engine::CObjectFactory::GetInstance()->AddPrototype(pWhiteBlock);

	SHARED(Engine::CGameObject) pBlueBlock = Engine::CGameObject::Create(L"EventBlock", L"BlueBlock", true);
	pBlueBlock->AddComponent<CBlueBlockComponent>();
	pBlueBlock->AddComponent<Engine::CMeshComponent>();
	pBlueBlock->AddComponent<Engine::CTextureComponent>();
	pBlueBlock->AddComponent<Engine::CTransformComponent>();
	pBlueBlock->AddComponent<Engine::CGraphicsComponent>();
	pBlueBlock->AddComponent<Engine::CColliderComponent>()->AddCollider(Engine::CAabbCollider::Create(FLOAT3_ONE, FLOAT3_ZERO));
	Engine::CObjectFactory::GetInstance()->AddPrototype(pBlueBlock);

	SHARED(Engine::CGameObject) pRedBlock = Engine::CGameObject::Create(L"EventBlock", L"RedBlock", true);
	pRedBlock->AddComponent<CRedBlockComponent>();
	pRedBlock->AddComponent<Engine::CMeshComponent>();
	pRedBlock->AddComponent<Engine::CTextureComponent>();
	pRedBlock->AddComponent<Engine::CTransformComponent>()/*->SetScale()*/;
	pRedBlock->AddComponent<Engine::CGraphicsComponent>();
	pRedBlock->AddComponent<Engine::CColliderComponent>()->AddCollider(Engine::CAabbCollider::Create(_float3(1.f, 3.f, 1.f), FLOAT3_ZERO));
	Engine::CObjectFactory::GetInstance()->AddPrototype(pRedBlock);

	SHARED(Engine::CGameObject) pYellowBlock = Engine::CGameObject::Create(L"EventBlock", L"YellowBlock", true);
	pYellowBlock->AddComponent<CYellowBlockComponent>();
	pYellowBlock->AddComponent<Engine::CMeshComponent>();
	pYellowBlock->AddComponent<Engine::CTextureComponent>();
	pYellowBlock->AddComponent<Engine::CTransformComponent>();
	pYellowBlock->AddComponent<Engine::CGraphicsComponent>();
	pYellowBlock->AddComponent<Engine::CColliderComponent>()->AddCollider(Engine::CAabbCollider::Create(_float3(1.f, 3.f, 1.f), FLOAT3_ZERO));
	Engine::CObjectFactory::GetInstance()->AddPrototype(pYellowBlock);
}

unsigned CLoadingScene::LoadDataFunc(LPVOID pArg)
{
	CLoadingScene* pLoad = static_cast<CLoadingScene*>(pArg);

	HRESULT hr = 0;
	

	switch (pLoad->m_eNextSceneID)
	{
	case ESceneID::Logo:
		break;
	case ESceneID::MainMenu:
		break;
	case ESceneID::Stage1:
		hr = pLoad->Stage1Load();
		break;
	case ESceneID::Stage2:
		hr = pLoad->Stage2Load();
		break;
	case ESceneID::Stage3:
		hr = pLoad->Stage3Load();
		break;
	default:
		break;
	}


	if (FAILED(hr))
	{
		return SCENE_CHANGE;
	}

	return 0;
}

HRESULT CLoadingScene::Stage1Load()
{
	std::string filePath = "../../Data/Save_1.txt";
	std::ifstream ifsLoad(filePath.data());
	Engine::CSceneManager::GetInstance()->GetUnitInfo().clear();

	if (ifsLoad.is_open())
	{
		std::string line;

		while (!ifsLoad.eof())
		{
			std::string ObjectKey;

			if (std::getline(ifsLoad, line))
				ObjectKey = line;
			else
				break;

			float xPos = 0.f, yPos = 0.f, zPos = 0.f;
			float xRot = 0.f, yRot = 0.f, zRot = 0.f;


			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr(line);
				ssr >> xPos;
			}
			else
				break;

			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr2(line);
				ssr2 >> yPos;
			}
			else
				break;

			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr3(line);
				ssr3 >> zPos;
			}
			else
				break;
			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr4(line);
				ssr4 >> xRot;
			}
			else
				break;

			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr5(line);
				ssr5 >> yRot;
			}
			else
				break;

			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr6(line);
				ssr6 >> zRot;
			}
			else
				break;

			Engine::UNITINFO vInfo;


			if (ObjectKey == "m_objectKey=WhiteBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);
				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));

				vInfo.wsObjectKey = L"WhiteBlock";
			}
			else if (ObjectKey == "m_objectKey=RedBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);
				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));
				//vInfo.vScale = _float3(1.f, 3.f, 1.f);

				vInfo.wsObjectKey = L"RedBlock";
			}
			else if (ObjectKey == "m_objectKey=BlueBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);
				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));

				vInfo.wsObjectKey = L"BlueBlock";
			}
			else if (ObjectKey == "m_objectKey=YellowBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);
				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));
				vInfo.vScale = _float3(1.f, 3.f, 1.f);

				vInfo.wsObjectKey = L"YellowBlock";
			}
			else if (ObjectKey == "m_objectKey=GreenBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);
				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));

				vInfo.wsObjectKey = L"GreenBlock";
			}

			Engine::CSceneManager::GetInstance()->GetUnitInfo().emplace_back(vInfo);
		}

	}
	else
	{
		MessageBox(Engine::CWndApp::GetInstance()->GetHWnd(), L"Load Failed", L"Failed", MB_OK);
		return S_OK;

	}
	m_LoadCheck = true;
	return S_OK;
}

HRESULT CLoadingScene::Stage2Load()
{
	std::string filePath = "../../Data/Save_2.txt";
	std::ifstream ifsLoad(filePath.data());
	Engine::CSceneManager::GetInstance()->GetUnitInfo().clear();

	if (ifsLoad.is_open())
	{
		std::string line;

		while (!ifsLoad.eof())
		{
			std::string ObjectKey;

			if (std::getline(ifsLoad, line))
				ObjectKey = line;
			else
				break;

			float xPos = 0.f, yPos = 0.f, zPos = 0.f;
			float xRot = 0.f, yRot = 0.f, zRot = 0.f;


			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr(line);
				ssr >> xPos;
			}
			else
				break;

			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr2(line);
				ssr2 >> yPos;
			}
			else
				break;

			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr3(line);
				ssr3 >> zPos;
			}
			else
				break;
			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr4(line);
				ssr4 >> xRot;
			}
			else
				break;

			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr5(line);
				ssr5 >> yRot;
			}
			else
				break;

			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr6(line);
				ssr6 >> zRot;
			}
			else
				break;

			Engine::UNITINFO vInfo;


			if (ObjectKey == "m_objectKey=WhiteBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);
				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));
				vInfo.wsObjectKey = L"WhiteBlock";
			}
			else if (ObjectKey == "m_objectKey=RedBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);

				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));
				//vInfo.vScale = _float3(1.f, 3.f, 1.f);
				vInfo.wsObjectKey = L"RedBlock";
			}
			else if (ObjectKey == "m_objectKey=BlueBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);
				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));

				vInfo.wsObjectKey = L"BlueBlock";
			}
			else if (ObjectKey == "m_objectKey=YellowBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);
				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));
				vInfo.vScale = _float3(1.f, 3.f, 1.f);

				vInfo.wsObjectKey = L"YellowBlock";
			}
			else if (ObjectKey == "m_objectKey=GreenBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);
				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));

				vInfo.wsObjectKey = L"GreenBlock";
			}

			Engine::CSceneManager::GetInstance()->GetUnitInfo().emplace_back(vInfo);
		}
	}
	else
	{
		MessageBox(Engine::CWndApp::GetInstance()->GetHWnd(), L"Load Failed", L"Failed", MB_OK);
		return S_OK;
	}
	m_LoadCheck = true;
	return S_OK;
}

HRESULT CLoadingScene::Stage3Load()
{
	std::string filePath = "../../Data/Save_3.txt";
	std::ifstream ifsLoad(filePath.data());
	Engine::CSceneManager::GetInstance()->GetUnitInfo().clear();

	if (ifsLoad.is_open())
	{
		std::string line;

		while (!ifsLoad.eof())
		{
			std::string ObjectKey;

			if (std::getline(ifsLoad, line))
				ObjectKey = line;
			else
				break;

			float xPos = 0.f, yPos = 0.f, zPos = 0.f;
			float xRot = 0.f, yRot = 0.f, zRot = 0.f;


			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr(line);
				ssr >> xPos;
			}
			else
				break;

			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr2(line);
				ssr2 >> yPos;
			}
			else
				break;

			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr3(line);
				ssr3 >> zPos;
			}
			else
				break;
			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr4(line);
				ssr4 >> xRot;
			}
			else
				break;

			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr5(line);
				ssr5 >> yRot;
			}
			else
				break;

			if (std::getline(ifsLoad, line))
			{
				std::stringstream ssr6(line);
				ssr6 >> zRot;
			}
			else
				break;

			Engine::UNITINFO vInfo;


			if (ObjectKey == "m_objectKey=WhiteBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);
				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));
				vInfo.wsObjectKey = L"WhiteBlock";
			}
			else if (ObjectKey == "m_objectKey=RedBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);

				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));
				//vInfo.vScale = _float3(1.f, 3.f, 1.f);

				vInfo.wsObjectKey = L"RedBlock";
			}
			else if (ObjectKey == "m_objectKey=BlueBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);
				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));

				vInfo.wsObjectKey = L"BlueBlock";
			}
			else if (ObjectKey == "m_objectKey=YellowBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);
				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));
				vInfo.vScale = _float3(1.f, 3.f, 1.f);

				vInfo.wsObjectKey = L"YellowBlock";
			}
			else if (ObjectKey == "m_objectKey=GreenBlock")
			{
				vInfo.vPos = _float3(xPos, yPos, zPos);
				vInfo.vRot = _float3(D3DXToRadian(xRot), D3DXToRadian(yRot), D3DXToRadian(zRot));

				vInfo.wsObjectKey = L"GreenBlock";
			}

			Engine::CSceneManager::GetInstance()->GetUnitInfo().emplace_back(vInfo);
		}	
	}
	else
	{
		MessageBox(Engine::CWndApp::GetInstance()->GetHWnd(), L"Load Failed", L"Failed", MB_OK);
		return S_OK;

	}

	m_LoadCheck = true;
	return S_OK;
}


