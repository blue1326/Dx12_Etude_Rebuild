#ifndef Scene_h__
#define Scene_h__
#include "stdHeaders.h"
#include "GameObject.h"
#include "Layer.h"
#include "DxDevice.h"
class CTimer;

class CScene
{
protected:
	explicit CScene(shared_ptr<DxDevice> _device);
	virtual ~CScene();
public :
	//active state setter and getter
	void SetActiveState(bool state);
	bool GetActiveState()
	{
		return isActive;
	}
	virtual HRESULT Ready_Scene(void);
	virtual int Update_Scene(const std::shared_ptr<CTimer> t);
	virtual int LastUpdate_Scene(const std::shared_ptr<CTimer> t);
	virtual void Render_Scene(void)PURE;

	//void AddGameObject(std::shared_ptr<CGameObject> obj);
public:
	HRESULT Add_Layer(const wchar_t* pLayerTag, shared_ptr<CLayer> _Layer);
	void OnResize();

protected:

private:
	shared_ptr<CLayer> Find_Layer(const wchar_t* pLayerTag);

public:
	typedef map<const wchar_t*, shared_ptr<CLayer>> MAPLAYER;

protected:
	shared_ptr<DxDevice> m_DxDevice;
	MAPLAYER m_mapLayer;
	bool isActive = false;
};
 
#endif // Scene_h__
