#include "Scenario.h"

bool Scenario::valid()
{
    if (!group.size() || !bridges.size())
        return false;

    auto checkGroup = [](const TGroup& g, const TSpeedLimit& speedLimit)
    {
        for (const auto& i : g)
        {
            if (i < speedLimit.first || i > speedLimit.second)
                return false;
        }
        return true;
    };

    if (!checkGroup(group, speedLimit))
        return false;

    for (const auto& b : bridges)
    {
        if (b.length < lengthLimit.first || b.length > lengthLimit.second || !checkGroup(b.hikers, speedLimit))
            return false;
    }
    return true;
}

std::ostream& operator<< (std::ostream& stream, const Scenario& script)
{
    stream << "Simulation for group of " << script.group.size() << " hiker(s) and " << script.bridges.size() << " bridge(s)." << std::endl;
    stream << "Hiker's speed should be in range [" << script.speedLimit.first << "," << script.speedLimit.second << "] feet per minute." << std::endl
        << "Bridge's length should be in range [" << script.lengthLimit.first << "," << script.lengthLimit.second << "] feet." << std::endl;
    long double t(0);
    const int bridgeW(12), minTimeW(28);
    stream << std::setprecision(std::numeric_limits< double >::max_digits10);

    stream << std::left << std::setw(bridgeW) << "Bridge [ft]" << std::setw(minTimeW) << "Minimum time [min]" << std::endl;

    for (size_t i(0); i < script.minTimes.size(); ++i)
    {
        t += script.minTimes.at(i);
        stream << std::left << std::setw(bridgeW) << script.bridges.at(i).length << std::setw(minTimeW) << script.minTimes.at(i)
            << "  " << script.minTimes.at(i) << std::endl;
    }
    stream << "Total time[min]: " << script.totalTime << std::endl;
    return stream;
}

