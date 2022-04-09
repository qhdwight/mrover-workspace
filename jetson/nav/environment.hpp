#pragma once

#include <vector>

#include <eigen3/Eigen/Core>

#include "rover.hpp"
#include "filter.hpp"
#include "rover_msgs/Obstacle.hpp"
#include "rover_msgs/TargetList.hpp"

using Eigen::Vector2d;
using namespace rover_msgs;

class Rover;

class Environment {
private:
    Obstacle mObstacle{0.0, 0.0, -1.0};

    // The rover's current target information from computer
    // vision.
    Target mTargetLeft{-1.0, 0.0, 0};
    Target mTargetRight{-1.0, 0.0, 0};

    // Cached Target
    // Left means left in the pixel space
    Target mCacheTargetLeft{-1.0, 0.0, 0};
    Target mCacheTargetRight{-1.0, 0.0, 0};

    // Count of misses with cache
    int mCountLeftMisses = 0;
    int mCountRightMisses = 0;

    // Count hits for avoiding FPs
    int mCountLeftHits = 0;
    int mCountRightHits = 0;

    // Reference to config variables
    const rapidjson::Document& mRoverConfig;

    Filter<double> mLeftBearingFilter, mRightBearingFilter, mLeftDistanceFilter, mRightDistanceFilter;

    Odometry mLeftPost{}, mRightPost{};

    bool mHasLeftPost = false, mHasRightPost = false;

public:
    explicit Environment(const rapidjson::Document& config);

    Obstacle getObstacle();

    void setObstacle(Obstacle const& obstacle);

    Target getLeftTarget();

    Target getRightTarget();

    Odometry getLeftPostLocation();

    Odometry getRightPostLocation();

    Vector2d getLeftPostRelative();

    Vector2d getRightPostRelative();

    void setTargets(TargetList const& targets);

    [[nodiscard]] Target const& leftCacheTarget() const;

    [[nodiscard]] Target const& rightCacheTarget() const;

    [[nodiscard]] int getLeftMisses() const;

    [[nodiscard]] int getRightMisses() const;

    [[nodiscard]] int getLeftHits() const;

    [[nodiscard]] int getRightHits() const;

    void resetMisses();

    void updateTargets(std::shared_ptr<Rover> const& rover, std::shared_ptr<CourseProgress> const& course);

    [[nodiscard]] bool hasGateLocation() const;

    [[nodiscard]] bool areTargetFiltersReady() const;
};