#include "Box.h"

CBox::CBox()
{

}

CBox::~CBox()
{

}

bool CBox::Init_Component()
{
	return true;
}

void CBox::Update_Component()
{
	
}

std::shared_ptr<CComponent> CBox::Clone()
{
	std::shared_ptr<CComponent> instance = std::shared_ptr<CComponent>(new CBox);
	CopyComponentSettings(instance);
	return instance;
}

void CBox::CopyComponentSettings(std::shared_ptr<CComponent> CopyInst)
{

}

void CBox::OnResize()
{
	
}

