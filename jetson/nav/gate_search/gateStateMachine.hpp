#ifndef GATE_STATE_MACHINE_HPP
#define GATE_STATE_MACHINE_HPP

#include <deque>
#include <memory>

#include "../rover.hpp"
#include "rover_msgs/Odometry.hpp"


using namespace rover_msgs;

class StateMachine;

class GateStateMachine {
public:
    /*************************************************************************/
    /* Public Member Functions */
    /*************************************************************************/
    GateStateMachine(std::weak_ptr<StateMachine> stateMachine, const rapidjson::Document& roverConfig);

    virtual ~GateStateMachine();

    NavState run();

    virtual void initializeSearch() = 0;

    /*************************************************************************/
    /* Public Member Variables */
    /*************************************************************************/
    /* saved last known location of first tag of a gate */
    Waypoint lastKnownRightPost;

    /* saved last known location of second tag of a gate */
    Waypoint lastKnownLeftPost;

    // Queue of search points
    std::deque<Odometry> mGateSearchPoints;

protected:
    /*************************************************************************/
    /* Protected Member Variables */
    /*************************************************************************/

    // Pointer to rover State Machine to access member functions
    std::weak_ptr<StateMachine> mStateMachine;

private:
    /*************************************************************************/
    /* Private Member Functions */
    /*************************************************************************/
    NavState executeGateSpin();

    NavState executeGateSpinWait();

    NavState executeGateTurn();

    NavState executeGateDrive();

    NavState executeGateTurnToCentPoint();

    NavState executeGateDriveToCentPoint();

    NavState executeGateFace();

    NavState executeGateTurnToFarPost();

    NavState executeGateDriveToFarPost();

    NavState executeGateTurnToGateCenter();;

    NavState executeGateDriveThrough();

    void updatePost2Info();

    void calcCenterPoint();

    /*************************************************************************/
    /* Private Member Variables */
    /*************************************************************************/

    // Reference to config variables
    const rapidjson::Document& mRoverConfig;

    // Points in frnot of center of gate
    Odometry centerPoint1;
    Odometry centerPoint2;

    // Check if we drove through the correct direction
    bool isCorrectGateDir;

    // Minor adjustment to gate targeting, due to issue
    // of driving through a post when driving through the
    // wrong direction
    double gateAdjustmentDist;
};

std::shared_ptr<GateStateMachine>
GateFactory(std::weak_ptr<StateMachine> stateMachine, const rapidjson::Document& roverConfig);

#endif //GATE_STATE_MACHINE_HPP
