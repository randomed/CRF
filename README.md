CRF
===

CRF implementation in C++

issues:
rounding problems in the simulator unfixed because not sure how a real robot will handle it.
Currently does not work correctly when there is a occupied node in the edge of sensor range but the distance is rounded down due to being over the limit
this causes the sensor to report unoccupied space.
