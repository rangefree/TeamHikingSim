#ifndef __SCENARIO_H__
#define __SCENARIO_H__

#include <iostream>
#include <iomanip>
#include <vector>

#include "yaml.h"

using TGroup = std::vector<double>;
using TTimes = std::vector<double>;

struct Bridge
{
    uint32_t length = 0;
    TGroup hikers;
};

using TBridges = std::vector<Bridge>;

struct Scenario
{
    TGroup group;
    TBridges bridges;

    TTimes minTimes;
    long double totalTime = 0;

    typedef std::pair<double, double> TSpeedLimit;
    typedef std::pair<uint32_t, uint32_t> TLengthLimit;

    TSpeedLimit speedLimit = { 0.0001, 10000.0 };   // feet per minute (max ~113mph)
    TLengthLimit lengthLimit = { 1, 1000000 };        // feet (max ~189 miles)

    bool valid();
};

std::ostream& operator<< (std::ostream& stream, const Scenario& script);


namespace YAML {

    template<>
    struct convert<TGroup> {
        static Node encode(const TGroup& rhs) {
            Node node;
            for (auto& i : rhs)
                node.push_back(i);
            return node;
        }

        static bool decode(const Node& node, TGroup& rhs) {
            if (!node.IsSequence()) {
                return false;
            }

            rhs.clear();
            for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
                rhs.push_back(it->as<double>());

            return true;
        }
    };

    template<>
    struct convert<Bridge> {
        static Node encode(const Bridge& rhs) {
            Node node;
            node["Length"] = rhs.length;
            node["Hikers"] = rhs.hikers;
            return node;
        }

        static bool decode(const Node& node, Bridge& rhs) {
            if (!node.IsMap() || node.size() != 2) {
                return false;
            }

            rhs.length = node["Length"].as<uint32_t>();
            rhs.hikers = node["Hikers"].as<TGroup>();
            return true;
        }
    };


    template<>
    struct convert<TBridges> {
        static Node encode(const TBridges& rhs) {
            Node node;

            for (auto& i : rhs)
                node.push_back(i);
            return node;
        }

        static bool decode(const Node& node, TBridges& rhs) {
            if (!node.IsSequence() || node.size() == 0) {
                return false;
            }

            rhs.clear();
            for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
                rhs.push_back(it->as<Bridge>());
            return true;
        }
    };

    template<>
    struct convert<Scenario> {
        static Node encode(const Scenario& rhs) {
            Node node;
            node["Group"] = rhs.group;
            node["Bridges"] = rhs.bridges;
            return node;
        }

        static bool decode(const Node& node, Scenario& rhs) {
            if (!node.IsMap() ||
                !node["Group"].IsDefined() ||
                !node["Bridges"].IsDefined() ||
                node["Group"].size() == 0 ||
                node["Bridges"].size() == 0)
            {
                return false;
            }

            rhs.group = node["Group"].as<TGroup>();
            rhs.bridges = node["Bridges"].as<TBridges>();
            return true;
        }
    };
}

#endif !__SCENARIO_H__
