#include "Scene.h"
#include "Layer.h"


CScene::CScene(shared_ptr<DxDevice> _device)
	:m_DxDevice(_device)
{

}

CScene::~CScene()
{

}

void CScene::SetActiveState(bool state)
{
	isActive = state;
}

HRESULT CScene::Ready_Scene(void)
{
	return S_OK;
}

int CScene::Update_Scene(const std::shared_ptr<CTimer> t)
{
	int ExitCode = 0;

	for (const auto &j : m_mapLayer)
	{
		ExitCode = j.second->Update_Layer(t);
		if (ExitCode & 0x80000000)
			return ExitCode;
	}

	return 0;
}

int CScene::LastUpdate_Scene(const std::shared_ptr<CTimer> t)
{
	int ExitCode = 0;

	for (const auto &j : m_mapLayer)
	{
		ExitCode = j.second->LastUpdate_Layer(t);
		if (ExitCode & 0x80000000)
			return ExitCode;
	}
	return 0;
}

HRESULT CScene::Add_Layer(const wchar_t* pLayerTag, shared_ptr<CLayer> _Layer)
{
	shared_ptr<CLayer> FoundedLayer = Find_Layer(pLayerTag);

	if (nullptr != FoundedLayer)
		return E_FAIL;

	m_mapLayer.insert(pair<const wchar_t*, shared_ptr<CLayer>>(pLayerTag, _Layer));
	return S_OK;
}

//void CScene::AddGameObject(std::shared_ptr<CGameObject> obj)
//{
//	m_Objects.push_back(obj);
//}

void CScene::OnResize()
{
	for (const auto &j : m_mapLayer)
	{
		j.second->OnResize();
		
	}
}

std::shared_ptr<CLayer> CScene::Find_Layer(const wchar_t* pLayerTag)
{
	MAPLAYER::iterator findIter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));
	if (findIter == m_mapLayer.end())
		return nullptr;
	return findIter->second;
}

