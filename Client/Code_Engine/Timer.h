//***************************************************************************************
// GameTimer.h by Frank Luna (C) 2011 All Rights Reserved.
// Converted RagNa 2018 All Rights Reserved.
//***************************************************************************************


#ifndef Timer_h__
#define Timer_h__
#include <memory>
typedef __int64 HINT;
class CTimer
{
public:
	CTimer();
	virtual ~CTimer();
public:
	float TotalTime()const; //seconds
	float DeltaTime()const; //seconds

	void Reset(); //Call before message loop
	void Start(); //Call When unpaused
	void Stop(); //Call when paused
	void Tick(); //Call every frames

private:
	double m_SecondsPerCount;
	double m_DeltaTime;
	HINT m_BaseTime;
	HINT m_PausedTime;
	HINT m_StopTime;
	HINT m_PrevTime;
	HINT m_CurrTime;

	bool m_Stopped;

//public:
//	std::shared_ptr<CTimer> GetTimer()
//	{
//		if (m_inst == nullptr)
//		{
//			m_inst = std::make_shared(this);
//		}
//
//		return m_inst;
//	}
//private:
//	std::shared_ptr<CTimer> m_inst;
};
#endif // Timer_h__
