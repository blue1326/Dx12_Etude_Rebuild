#include "GameObject.h"
#include "Structs.h"
#include "dxException.h"
#include "BufferUtility.h"
CGameObject::CGameObject(shared_ptr<DxDevice> _device)
	:m_DxDevice(_device)
{

}

CGameObject::~CGameObject()
{

}

void CGameObject::Free(void)
{

}
