#ifndef Layer_h__
#define Layer_h__
#include "stdHeaders.h"
#include "GameObject.h"
class CLayer
{
public:
	CLayer();
	virtual ~CLayer();
public:
	HRESULT Ready_Layer(void);
	int Update_Layer(const std::shared_ptr<CTimer> t);
	int LastUpdate_Layer(const std::shared_ptr<CTimer> t);
	void OnResize();

public:
	inline void Add_GameObject(shared_ptr<CGameObject> _Obj)
	{
		m_ObjList.push_back(_Obj);
	}
public:
	typedef list<shared_ptr<CGameObject>> OBJLIST;
private:
	OBJLIST m_ObjList;


private:
	void Free(void);

};


#endif // Layer_h__
