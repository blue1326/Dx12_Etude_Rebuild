#ifndef Controller_h__
#define Controller_h__
#include "Component.h"

class CController : public CComponent
{
public:
	explicit CController();
	virtual ~CController();


private:
	int m_mouseX;
	int m_mouseY;
	int m_mouseLastPosX;
	int m_mouseLastPosY;
	int m_mousemoveX;
	int m_mousemoveY;
	
	WPARAM m_btnState;
public:
	virtual HRESULT Init_Component();
	virtual void Update_Component(const std::shared_ptr<CTimer> t);
	virtual std::shared_ptr<CComponent> Clone();
	virtual void OnResize();

public: 
	void Update_MouseXY(const int& _x, const int& _y);
	void Update_MouseBtnState(const WPARAM& _btnState);
	void Get_MouseMoveXY(int& _x, int& _y);
	void Get_MousePosXY(int& _x, int& _y);
	void Get_MouseLastPosXY(int& _x, int& _y);
};


#endif // Controller_h__
