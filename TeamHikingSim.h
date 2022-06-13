#ifndef __TEAMHIKINGSIM_H__
#define __TEAMHIKINGSIM_H__

#include <vector>
#include <iomanip>


#include "Scenario.h"

/*
hiker speed: form 
*/

class TeamHikingSim
{
	double m_guideMPF; // fastest time per foot
	double m_followersMPF; // total of all followers' minute per foot
	size_t m_groupSize;

protected:	
	double GetHikersMPF(TGroup::const_iterator itBegin, const TGroup::const_iterator itEnd, bool setGuideMPF = false);
    
public:
	virtual ~TeamHikingSim(){};

	void run(Scenario& sim);

	void setGroup(const TGroup& group);

	double crossBridge(const Bridge& bridge);
};


#endif // !__TEAMHIKINGSIM_H__

