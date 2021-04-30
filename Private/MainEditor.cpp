#include "stdafx.h"
#include "MainEditor.h"
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "ColliderManager.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "ShaderManager.h"
#include "EditorScene.h"
#include "FontManager.h"

CMainEditor::CMainEditor()
{
}


CMainEditor::~CMainEditor()
{
	OnDestroy();
}

SHARED(CMainEditor) CMainEditor::Create(void)
{
	SHARED(CMainEditor) pMainEditor(new CMainEditor, Engine::SmartDeleter<CMainEditor>);

	return pMainEditor;
}

void CMainEditor::Awake(void)
{
	Engine::CObjectFactory::GetInstance()->Awake();
	Engine::CGraphicsManager::GetInstance()->Awake();
	Engine::CInputManager::GetInstance()->Awake();
	Engine::CSceneManager::GetInstance()->Awake();
	Engine::CShaderManager::GetInstance()->Awake();
	Engine::CFontManager::GetInstance()->Awake();
}

void CMainEditor::Start(void)
{
	Engine::CObjectFactory::GetInstance()->Start();
	Engine::CGraphicsManager::GetInstance()->Start();
	Engine::CInputManager::GetInstance()->Start();
	Engine::CSceneManager::GetInstance()->Start();
	Engine::CSceneManager::GetInstance()->SceneChange(CEditorScene::Create());
	Engine::CShaderManager::GetInstance()->Start();
	Engine::CFontManager::GetInstance()->Start();

	int i = 10;
	float f = 20.1f;
	std::wstring str1 = std::to_wstring(f);

	Engine::ADD_TEXT(L"Test", str1, _float3(10.f, 10.f, 0.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	Engine::REWRITE_TEXT(L"Test", str1);
}

_uint CMainEditor::FixedUpdate(void)
{
	_uint event = NO_EVENT;

	if (event = Engine::CGraphicsManager::GetInstance()->FixedUpdate())	return event;
	if (event = Engine::CInputManager::GetInstance()->FixedUpdate())	return event;
	if (event = Engine::CSceneManager::GetInstance()->FixedUpdate())	return event;

	return event;
}

_uint CMainEditor::Update(void)
{
	_uint event = NO_EVENT;

	if (event = Engine::CGraphicsManager::GetInstance()->Update())	return event;
	if (event = Engine::CInputManager::GetInstance()->Update())		return event;
	if (event = Engine::CSceneManager::GetInstance()->Update())		return event;

	return event;
}

_uint CMainEditor::LateUpdate(void)
{
	_uint event = NO_EVENT;
	if (event = Engine::CGraphicsManager::GetInstance()->LateUpdate())	return event;
	if (event = Engine::CInputManager::GetInstance()->LateUpdate())		return event;
	if (event = Engine::CSceneManager::GetInstance()->LateUpdate())		return event;

	return event;
}

_uint CMainEditor::PreRender(void)
{
	_uint event = NO_EVENT;
	if (event = Engine::CGraphicsManager::GetInstance()->PreRender())	return event;
	

	return event;
}

_uint CMainEditor::Render(void)
{
	_uint event = NO_EVENT;
	if (event = Engine::CGraphicsManager::GetInstance()->Render())	return event;
	if (event = Engine::CFontManager::GetInstance()->Render())		return event;
	return event;
}

_uint CMainEditor::PostRender(void)
{
	_uint event = NO_EVENT;
	if (event = Engine::CGraphicsManager::GetInstance()->PostRender())	return event;

	return event;
}

void CMainEditor::OnDestroy(void)
{
	Engine::CGraphicsManager::GetInstance()->DestroyInstance();
	Engine::CInputManager::GetInstance()->DestroyInstance();
	Engine::CSceneManager::GetInstance()->DestroyInstance();
	Engine::CObjectFactory::GetInstance()->DestroyInstance();
	Engine::CShaderManager::GetInstance()->DestroyInstance();
	Engine::CColliderManager::GetInstance()->DestroyInstance();
	
}

void CMainEditor::OnEnable(void)
{
}

void CMainEditor::OnDisable(void)
{
}
