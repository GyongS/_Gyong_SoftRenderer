#include "EngineStdafx.h"
#include "Shader.h"
#include "DeviceManager.h"

USING(Engine)
CShader::CShader()
{
}


CShader::~CShader()
{
}

void CShader::Awake(void)
{
	__super::Awake();
}

void CShader::LoadShader(void)
{
	D3DXCreateEffectFromFile(GET_DEVICE, m_filePath.c_str(), 
							 nullptr, nullptr, 0, nullptr, &m_pShader, nullptr);

	if (m_pShader == nullptr)
		MSG_BOX(__FILE__, L"Create Effect failed in LoadShader");
}
