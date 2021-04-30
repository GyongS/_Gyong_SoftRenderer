#ifndef LOADINGSCENE_H
#define LOADINGSCENE_H

#include "Scene.h"
#include "GameObject.h"
class CLoadingScene final : public Engine::CScene
{
	SMART_DELETER_REGISTER;
private:
	explicit								CLoadingScene		(void);
										   ~CLoadingScene		(void);
public:
	static			SHARED(Engine::CScene)	Create				(ESceneID eNextSceneID);

					void					Awake				(void) override;
					void					Start				(void) override;
																   
					_uint					FixedUpdate			(void) override;
					_uint					Update				(void) override;
					_uint					LateUpdate			(void) override;	
					
					void					OnDestroy			(void) override;

					void					OnEnable			(void) override;	 
					void					OnDisable			(void) override;

protected:
	virtual			void					InitLayers			(void) override;
	virtual			void					InitPrototypes		(void) override;

public:
	static unsigned __stdcall LoadDataFunc(LPVOID pArg);

private:
	HANDLE m_hThread;
	CRITICAL_SECTION m_CriticalSection;
	ESceneID m_eNextSceneID;

public:
	HRESULT Stage1Load();
	HRESULT Stage2Load();
	HRESULT Stage3Load();

private:
	_bool  m_LoadCheck;
};

#endif