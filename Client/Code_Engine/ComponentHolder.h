#ifndef ComponentHolder_h__
#define ComponentHolder_h__

#include "Component.h"

#include "Templates.h"

	class CComponentHolder : public CSingleton<CComponentHolder>
	{
	public:
		CComponentHolder();
		~CComponentHolder();
	protected:

	public:
		bool InitComponents();
		HRESULT Reserve_ComponentHolder(const unsigned int& iSize);
		void AddOriginComponent(const std::string _ComponentName, std::shared_ptr<CComponent> _ComponentOrigin);
		std::shared_ptr<CComponent> Clone_Component(const std::string _ComponentName);
		std::shared_ptr<CComponent> Get_Component(const std::string _ComponentName);
		typedef std::map<const std::string, std::shared_ptr<CComponent>> MAPCOMPONENT;
	private:
		std::shared_ptr<CComponent> Find_Component(const std::string _ComponentName);
	private:
		MAPCOMPONENT m_OriginComponents;
	};



/*class CComponentHolder
{
private:
	CComponentHolder() {}
	~CComponentHolder() {}
	CComponentHolder(const CComponentHolder& s) {}
	CComponentHolder operator = (const CComponentHolder& s);
public:
	void CreateComponent();
	void CloneComponent();
private:
	std::map<std::string, std::unique_ptr<CComponent>> m_OriginComponents;
private:
	static CComponentHolder* m_pInstance;
private:
	static void Destroy()
	{
		if (m_pInstance != nullptr)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
public:
	static CComponentHolder* GetInstance(void)
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CComponentHolder();
			atexit(Destroy);
		}
		return m_pInstance;
	}
};
*/
#endif // ComponentHolder_h__
