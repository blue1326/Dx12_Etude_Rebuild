#include "ComponentHolder.h"

CComponentHolder::CComponentHolder()
{

}

CComponentHolder::~CComponentHolder()
{

}

bool CComponentHolder::InitComponents()
{
	for (const auto &j : m_OriginComponents)
	{
		if(!j.second->Init_Component())
		return false;
	}
	return true;
}

HRESULT CComponentHolder::Reserve_ComponentHolder(const unsigned int& iSize)
{
	return S_OK;
}

void CComponentHolder::AddOriginComponent(const std::string _ComponentName, std::shared_ptr<CComponent> _ComponentOrigin)
{	
	if (Find_Component(_ComponentName) == nullptr)
		m_OriginComponents.insert(MAPCOMPONENT::value_type(_ComponentName, move(_ComponentOrigin)));
	else
		return;
}

std::shared_ptr<CComponent> CComponentHolder::Clone_Component(const std::string _ComponentName)
{
	std::shared_ptr<CComponent> component = nullptr;
	component = Find_Component(_ComponentName);
	if (component != nullptr)
		return component->Clone();
	else
		return nullptr;
}

std::shared_ptr<CComponent> CComponentHolder::Get_Component(const std::string _ComponentName)
{
	std::shared_ptr<CComponent> component = nullptr;
	component = Find_Component(_ComponentName);
	if (component != nullptr)
		return component;
	else
		return nullptr;
}

std::shared_ptr<CComponent> CComponentHolder::Find_Component(const std::string _ComponentName)
{
	MAPCOMPONENT::iterator iter = m_OriginComponents.find(_ComponentName);
	if (iter == m_OriginComponents.end())
		return nullptr;
	else
		return iter->second;
}
