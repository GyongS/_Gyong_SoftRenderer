#pragma once
#include "Scene.h"
class CGameObject;
class CEventScene final : public Engine::CScene
{
	SMART_DELETER_REGISTER;
public:
	explicit CEventScene();
			~CEventScene();

public:
	static SHARED(Engine::CScene) Create();

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
	void SpawnNormalBlock();
	void SpawnPushBlock();
	void SpawnChessBoard();

	void ChangePosition();
	void ChangeRedPos();

private:
	HRESULT SceneLoad();

private:
	_bool m_isBGMStart;
	_bool m_isEventStart;
	_bool m_isEventEnd;
	_bool m_isCameraStart;
private:
	_float m_curPlaySoundTime;
	_float m_bpm;
	_float m_eventTimer;
	_float m_shakeTime;	
	_float m_countDown;

private:
	_int   m_count;
	_int   m_eventCount;	
	_int   m_startCount;


private:
	std::vector<SHARED(Engine::CGameObject)> m_vBlock;
	std::vector<SHARED(Engine::CGameObject)> m_vRedBlock;
	SHARED(Engine::CGameObject) m_pPlayer;
	SHARED(Engine::CGameObject) m_pSubPlayer;

};

