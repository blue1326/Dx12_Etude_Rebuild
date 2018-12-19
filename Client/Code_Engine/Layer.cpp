#include "Layer.h"
CLayer::CLayer()
{
}

CLayer::~CLayer()
{
	Free();
}

HRESULT CLayer::Ready_Layer(void)
{
	for (const auto &j : m_ObjList)
	{
		j->Init_GameObject();
	}
	return S_OK;
}

int CLayer::Update_Layer(const std::shared_ptr<CTimer> t)
{
	int ExitCode = 0;

	for (const auto &j : m_ObjList)
	{
		ExitCode = j->Update_GameObject(t);
		if (ExitCode & 0x80000000)
			return ExitCode;
	}

	return 0;
}

int CLayer::LastUpdate_Layer(const std::shared_ptr<CTimer> t)
{
	int ExitCode = 0;

	for (const auto &j : m_ObjList)
	{
		ExitCode = j->LastUpdate_GameObject(t);
		if (ExitCode & 0x80000000)
			return ExitCode;
	}

	return 0;
}

void CLayer::OnResize()
{
	for (const auto &j : m_ObjList)
	{
		j->OnResize();
	}
}

void CLayer::Free(void)
{
	m_ObjList.clear();
}
