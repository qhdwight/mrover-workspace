#include "obstacleAvoidance.hpp"

//returns a bearing decision struct representing the desired NavState and bearing of the obstacle avoidance controller
BearingDecision ObstacleAvoidance::getDesiredBearingDecision(std::vector<Obstacle>& obstacles, Odometry roverOdom, Waypoint dest){
    //TODO implement
    return {NavState::Drive, -1.0};
}
    
//returns a vector of doubles representing clear bearings through a list of obstacles
//discretized in 1 degree increments, starting from -FOV to +FOV.
std::vector<double> ObstacleAvoidance::getClearBearings(std::vector<Obstacle>& obstacles){
    //TODO implement
    return {-1.0};
}

//returns a bearing that the rover should target to try to get to the destination while also getting around obstacles
//not latency adjusted
double ObstacleAvoidance::getIdealDesiredBearing(Odometry roverOdom, Waypoint dest, std::vector<double> clearBearings){
    //TODO: implement
    return -1.0;
}

//returns an adjusted target bearing based on latency specifications (thresholding on maximum allowable change in bearing)
double ObstacleAvoidance::getLatencyAdjustedDesiredBearing(Odometry roverOdom, double desiredBearing){
    //TODO: implement
    return -1.0;
}
