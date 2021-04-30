#pragma once
#include "Scene.h"

class CEditorScene final : public Engine::CScene
{
	SMART_DELETER_REGISTER
public:
	explicit CEditorScene();
	~CEditorScene();

public:
	static			SHARED(Engine::CScene)	Create(void);

	virtual void Awake(void) override;
	virtual void Start(void) override;
	virtual _uint FixedUpdate(void) override;
	virtual _uint Update(void) override;
	virtual _uint LateUpdate(void) override;
	virtual void OnDestroy(void) override;
	virtual void OnEnable(void) override;
	virtual void OnDisable(void) override;
	virtual void InitLayers(void) override;
	virtual void InitPrototypes(void) override;
	

private:
	void BlockPicking();
	void SetAddFirstCubeToListBox(); // 처음 설치되어 있는 큐브를 ListBox에 추가. 첫 update 때 한 번만 실행함
	void ChangeCameraYaxis(); // z,x키를 눌러서 카메라 y축 변경 z가 위로, x가 아래로.
	void CreateCubeWhenClicked();
	void DeleteCubeWhenClicked();
	void SelectBlock(); // 피킹 모드가 아닐 때 큐브를 선택하게 됨

private:
	_bool m_isPickingMode = false;
	_bool m_AddFistCubeToListBox = true;
	class Engine::CGameObject* m_pPreSelectedObject = nullptr;
	class Engine::CGameObject* m_pCurSelectedObject = nullptr;

	class CMainFrame * m_pMain;
	class CEditorView* m_pEditorView;
	class CObjectListView* m_pListView;
};

