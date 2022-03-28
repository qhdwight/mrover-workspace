#include <vector>
#include "rover_msgs/Obstacle.hpp"
#include "rover_msgs/Odometry.hpp"
#include "rover.hpp"

class ObstacleAvoidance{
    public:
        struct BearingDecision{
            NavState obstacleControllerOutputState;
            double desiredBearing;
        };

        //returns a bearing decision struct representing the desired NavState and bearing of the obstacle avoidance controller
        BearingDecision getDesiredBearingDecision(std::vector<Obstacle>& obstacles, Odometry roverOdom, Odometry dest);
    
    private:
        //returns a vector of doubles representing clear bearings through a list of obstacles
        //discretized in 1 degree increments, starting from -FOV to +FOV.
        std::vector<double> getClearBearings(std::vector<Obstacle>& obstacles);
        
        //returns a bearing that the rover should target to try to get to the destination while also getting around obstacles
        //not latency adjusted
        double getIdealDesiredBearing(Odometry roverOdom, Odometry dest, std::vector<double> clearBearings);

        //returns an adjusted target bearing based on latency specifications (thresholding on maximum allowable change in bearing)
        double getLatencyAdjustedDesiredBearing(Odometry roverOdom, double desiredBearing);
      
};