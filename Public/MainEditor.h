#pragma once
class CMainEditor final
{
	SMART_DELETER_REGISTER;
protected:
	explicit						CMainEditor(void);
	~CMainEditor(void);


public:

	static		SHARED(CMainEditor)	Create(void);

	void				Awake(void);
	void				Start(void);

	_uint				FixedUpdate(void);
	_uint				Update(void);
	_uint				LateUpdate(void);

	_uint				PreRender(void);
	_uint				Render(void);
	_uint				PostRender(void);

	void				OnDestroy(void);

	void				OnEnable(void);
	void				OnDisable(void);

};

