#include "Component.h"


CComponent::CComponent()
{

}

CComponent::~CComponent()
{

}

HRESULT CComponent::Init_Component()
{
	return S_OK;
}

//HRESULT CComponent::Ready_Component()
//{
//	return 0;
//}

void CComponent::Update_Component(const std::shared_ptr<CTimer> t)
{

}


void CComponent::OnResize()
{
	
}
