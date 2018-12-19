#include "Controller.h"

CController::CController()
	:m_mouseX(0)
	,m_mouseY(0)
	,m_mouseLastPosX(0)
	,m_mouseLastPosY(0)
	,m_btnState(NULL)
{

}

CController::~CController()
{
	
}

HRESULT CController::Init_Component()
{
	return S_OK;
}

void CController::Update_Component(const std::shared_ptr<CTimer> t)
{
	m_mouseX = m_mouseLastPosX;
	m_mouseY = m_mouseLastPosY;
}

std::shared_ptr<CComponent> CController::Clone()
{
	return this->shared_from_this();
}

void CController::OnResize()
{

}

void CController::Update_MouseXY(const int& _x, const int& _y)
{
	m_mouseLastPosX = m_mouseX;
	m_mouseLastPosY = m_mouseY;
	m_mouseX = _x;
	m_mouseY = _y;
	m_mousemoveX = m_mouseX - m_mouseLastPosX;
	m_mousemoveY = m_mouseY - m_mouseLastPosY;
}

void CController::Update_MouseBtnState(const WPARAM& _btnState)
{
	m_btnState = _btnState;
}

void CController::Get_MouseMoveXY(int& _x, int& _y)
{
	_x = m_mousemoveX;
	_y = m_mousemoveY;
	m_mousemoveX = 0;
	m_mousemoveY = 0;
}

void CController::Get_MousePosXY(int& _x, int& _y)
{
	_x = m_mouseX;
	_y = m_mouseY;
}

void CController::Get_MouseLastPosXY(int& _x, int& _y)
{
	_x = m_mouseLastPosX;
	_y = m_mouseLastPosY;
}

