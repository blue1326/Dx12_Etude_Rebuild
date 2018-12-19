#ifndef Functor_h__
#define Functor_h__

class CTag_Finder
{
public:
	explicit CTag_Finder(const wchar_t* pString) : m_pString(pString) {}
	virtual ~CTag_Finder(void) {}
private:
	const wchar_t*			m_pString;
public:
	template <typename T>
	bool operator () (const T& Pair)
	{
		if (0 == lstrcmp(m_pString, Pair.first))
			return true;
		return false;
	}
};

#endif // Functor_h__
