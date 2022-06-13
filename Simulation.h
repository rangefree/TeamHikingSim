#ifndef __SUMULATION_H__
#define __SUMULATION_H__

struct Simulation
{
    virtual ~Simulation() {};

    template<typename MySimulator, typename MyScenario>
    bool run(MySimulator& sim, MyScenario& script)
    {
        if (script.valid())
        {
            sim.run(script);
            return true;
        }
        return false;
    }
};

#endif !__SUMULATION_H__

