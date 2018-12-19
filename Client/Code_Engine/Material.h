#ifndef Material_h__
#define Material_h__
#include "Component.h"
#include "Structs.h"
#include "DxDevice.h"

class CMaterial : public CComponent
{
public:
	explicit CMaterial(const shared_ptr<DxDevice> _device);
	explicit CMaterial(const CMaterial& rhs);
	virtual ~CMaterial();


public:
	virtual HRESULT Init_Component()override;
	virtual void Update_Component(const std::shared_ptr<CTimer> t)override;

public:
	virtual std::shared_ptr<CComponent> Clone()override;

public:
	void SetUpMaterialState(const shared_ptr<Material> _Material);
	void SetUpTextureLocation(const wstring& _str);
private:
	void Init_Start();
	void Init_End();
private:
	void LoadTexture();
public:
	const shared_ptr<Material> GetMaterial()
	{
		return m_Material;
	}
	const shared_ptr<Texture> GetTexture()
	{
		return m_Texture;
	}
private:
	const shared_ptr<DxDevice> m_DxDevice;

private:
	shared_ptr<Material> m_Material;
	shared_ptr<Texture> m_Texture;
};

#endif // Material_h__
