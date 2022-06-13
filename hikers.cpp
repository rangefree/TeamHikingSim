
#include <iostream>

#include "TeamHikingSim.h"
#include "Simulation.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cout << " Usage: hikers.exe file_name [file_name]\n";
        return -1;
    }

    try
    {
        for (int i = 1; i < argc; ++i)
        {
            YAML::Node sims = YAML::LoadFile(argv[i]); 
            if (!sims)
            {
                cout << "Faile to load simulation scenario from file (" << argv[i] << ").\n";
                return -1;
            }
 
            Simulation simulation;
            TeamHikingSim simulator;
            Scenario script;
            for (const auto& node : sims)
            {
                script = node.as<Scenario>();
                if (simulation.run(simulator, script))
                    cout << script << endl;
                else
                    cout << "Emulation failed (wrong script)." << endl;
            }
        }
    }
    catch (YAML::Exception& e)
    {
        std::cerr << "YAML processing failed.\n  " << e.what() << "\n";
        return -2;
    }
    catch (std::exception& e)
    {
        std::cerr << "Failed with exception: " << e.what() << "\n";
        return -2;
    }

    return 0;
}
