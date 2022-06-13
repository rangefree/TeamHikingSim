#include "TeamHikingSim.h"

#include <cmath>
#include <iostream>

bool assumeEqual(double a, double b, double epsilon = std::numeric_limits<double>::epsilon())
{
	return fabs(a - b) <= (fmin(fabs(a), fabs(b)) * epsilon);
}


double TeamHikingSim::GetHikersMPF(TGroup::const_iterator itBegin, const TGroup::const_iterator itEnd, bool setGuideMPF)
{
	if (setGuideMPF && itBegin != itEnd)
	{
		if (assumeEqual(*itBegin, 0.0))
			throw std::exception("Possible division by 0. Speed must be above 0.");

		m_guideMPF = 1 / (*itBegin);
		++itBegin;
	}

	double followersMPF(0);
	double currentMPF(0);
	for (; itBegin != itEnd; ++itBegin)
	{
		if (assumeEqual(*itBegin, 0.0))
			throw std::exception("Possible division by 0. Speed must be above 0.");

		currentMPF = 1 / (*itBegin); // minute per foot

		if (currentMPF < m_guideMPF)
		{// current hiker is fastest
			followersMPF += m_guideMPF;
			if (setGuideMPF)
			{
				m_guideMPF = currentMPF; // save the fastest hiker
			}
		}
		else
		{
			followersMPF += currentMPF;
		}
	}
	return followersMPF;
}

void TeamHikingSim::run(Scenario& script)
{
	script.minTimes.clear();
	script.totalTime = 0;

	setGroup(script.group);

	for (const auto& bridge : script.bridges)
	{
		script.minTimes.push_back(crossBridge(bridge));
		script.totalTime += script.minTimes.back();
	}
}

void TeamHikingSim::setGroup(const TGroup& group)
{
	m_guideMPF = 0;
	m_followersMPF = 0;
	m_groupSize = group.size();

	if (m_groupSize == 0)
		throw std::exception("Empty group.");
	if (m_groupSize == 1)
	{
		m_guideMPF = 1 / group[0];
	}
	else
	{
		m_followersMPF = GetHikersMPF(group.begin(), group.end(), true);
	}
}

double TeamHikingSim::crossBridge(const Bridge& bridge)
{
	if (bridge.length == 0)
		throw std::exception("Wrong bridge size.");

	size_t newGroupSize(m_groupSize + bridge.hikers.size());

	double waitingHikersMPF(GetHikersMPF(bridge.hikers.begin(), bridge.hikers.end()));

	if (newGroupSize == 1)
		return bridge.length * m_guideMPF;

	if (newGroupSize == 2 && bridge.hikers.size() == 1)
		return bridge.length * waitingHikersMPF;

	return
		bridge.length * (m_followersMPF + waitingHikersMPF + ((newGroupSize - 2) * m_guideMPF));
}



