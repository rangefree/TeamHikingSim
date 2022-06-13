REQUIREMENTS:
The team encounters a series of narrow bridges along the way. At each bridge they may meet additional hikers who need their help to cross the bridge.The narrow bridge can only hold two people at a time. They have one torch and because it's night, the torch has to be used when crossing the bridge. Each hiker can cross the bridge at different speeds. When two hikers cross the bridge together, they must move at the slower person's pace.
Determine the fastest time that the hikers can cross each bridge and the total time for all crossings. The input to the program will be a yaml file that describes the speeds of each person, the bridges encountered, their length, and the additional hikers encountered along the way. Your solution should assume any number of people and bridges can be provided as inputs.
Demonstrate the operation of your program using the following inputs: the hikers cross 3 bridges, at the first bridge (100 ft long) 4 hikers cross (hiker A can cross at 100 ft/minute, B at 50 ft/minute, C at 20 ft/minute, and D at 10 ft/minute), at the second bridge (250 ft long) an additional hiker crosses with the team (E at 2.5 ft/minute), and finally at the last bridge (150 ft long) two hikers are encountered (F at 25 ft/minute and G at 15 ft/minute). 

Based on the above requirements we have constrains:
1. Fastest in the initial team is Leader. Leader will cross bridge with every member of the team.
2. Hiker speed provided in foot per minute value and can be represented as floating point value with double precision.
3. There should be as minimum 1 bridge in simulation and 1 hiker in initial team.
4. Bridge is defined by length in feet and group of 1 or no additional hikers. 
5. Bridge size should be above 0 and should be unsigned integer (unless, there is unknown reason why it should be floating point).
6. During bridge crossing we should help other hikers (not opposite). As a result, our group's leader will be used for all crossings (as for initial as for additional groups).
7. Hikers cross bridge in couples (if there are 2 or more hikers).
8. Hikers should cross bridge at the slower person's pace.

As a result of the above, next crossings are possible:
1. Group of 1 [1] : 
	1 crossing at speed 1.
2. Group of 2 [1,2]: 
	1 crossing at speed of 1(slowest).
3. Group of 3 [1,2, 3]: 
	1 crossing at speed of 1(slowest) + 
	1 crossing at speed of 3(Leader back way) + 
	1 crossing at speed of 2(second slowest)
...

We need to measure min time which group will spend to cross the bridge. If we know total of minute per foot (MPF) performance of all “following” hikers (not Leader) then we know how much time they spent to cross the bridge. Summarize all “following” hikers' MPFs (with respect of constrains #6, 7, 8) and multiply result by bridge length. 
Now, we have to add Leader's back time to it and result is ready. Leader's back time should be calculated as follow:
IF total group size > 2
THEN
	return (total group size-2) * Leader's MPF *  bridge length.

Final bridge crossing formula would be:
IF total group size == 1 THEN return Tmpf(leader) ] * bridge length;
ELSE IF total group size == 2 && bridge group size == 1 THEN return Tmpf(bridge_group);
ELSE 
	return 
		[	
			(Tmpf(original_group) + 
			Tmpf(bridge_group) + 
			(total group size -2)*Tmpf(leader) 
		] * bridge length.


--------------------------

Design reason:
1. We have very base requirements, so design and implementation should be flexible enough to accommodate space for lots of changes in final use.
2. Format of our scenario or script files are not specified, so I decided to go with the basic approach.
3. For simplicity of usage code should be able to load one of more yaml files, specified in command line and execute them.
4. Output is in free format, so should be isolated and will be changed later most likely.
5. Test case is basic and as a result I added some constrains to the values in the yaml files in order to avoid very extreme cases. Those limits are hardcoded in the script body but can be loaded from yaml files also (if required).

Main function will go through command line parameters and for each will try to load correspondent scenario yaml file. Scenario file (scrips) has 1 or more simulation scenarios. Each simulation scenario consists of array of speeds of initial group of hikers and array of bridges to cross. Each bridge consists of length and array of waiting hikers' speeds.

Each scenario file (script) will be given to simulator through simulation object.
And results of simulation will be printed on screen.

	Simulation simulation;
	TeamHikingSim simulator;
	Scenario script(node.as<Scenario>());
	if (simulation.run(simulator, script))
		cout << script << endl;
	else
		cout << "Emulation failed (wrong script)." << endl;

Simulator itself, represent algorithm of group definition, single threaded run() function and bridge crossing function which can be used in multi-threading approach, where groups of bridges will be assigned to the certain threads to process.

Scenario has parameters, which are used by simulator and designed to provide simple validation function to make sure that scenario has correct values before simulation will be started. Results of operation are also stored there.

Simulation is a binder for scenario and simulator. It controls how simulator will be used. Current implementation is a sample single threaded approach. If processing of the huge amounts of bridges is necessary then strategy should be implemented here. 
