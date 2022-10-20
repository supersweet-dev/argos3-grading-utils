/*
 * AUTHOR: Carlo Pinciroli <cpinciro@ulb.ac.be>
 *
 * An example diffusion controller for the foot-bot.
 *
 * This controller makes the robots behave as gas particles. The robots
 * go straight until they get close enough to another robot, in which
 * case they turn, loosely simulating an elastic collision. The net effect
 * is that over time the robots diffuse in the environment.
 *
 * The controller uses the proximity sensor to detect obstacles and the
 * wheels to move the robot around.
 *
 * This controller is meant to be used with the XML files:
 *    experiments/diffusion_1.argos
 *    experiments/diffusion_10.argos
 */

#ifndef ASSIGNMENT2_H
#define ASSIGNMENT2_H

/*
 * Include some necessary headers.
 */

#include <argos3/core/utility/logging/argos_log.h>
/* Definition of the CCI_Controller class. */
#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/simulator/entity/floor_entity.h>
/* Definition of the differential steering actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
/* Definition of the foot-bot proximity sensor */
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>

#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <cmath>
#include <stack>
#include <time.h>
/*
 * All the ARGoS stuff in the 'argos' namespace.
 * With this statement, you save typing argos:: every time.
 */
using namespace argos;

/*
 * A controller is simply an implementation of the CCI_Controller class.
 */
class CFootBotDiffusion : public CCI_Controller {

public:

    //bool collisionFlag;
    CVector2 TargetPosition;
    Real TargetDistanceTolerance;
    CRadians TargetAngleTolerance;
    CRadians GetHeading();

    /* movement definition variables */
		struct Movement {
			size_t type;
			argos::Real magnitude;
		};
    Movement previous_movement;
    std::stack<Movement> MovementStack;

    // controller state variables
		enum MovementState {
			STOP    = 0,
			LEFT    = 1,
			RIGHT   = 2,
			FORWARD = 3,
			BACK    = 4
		} CurrentMovementState;

   void SetNextMovement();
   CVector2 GetPosition();
   bool IsAtTarget();


   /* Class constructor. */
   CFootBotDiffusion();

   /* Class destructor. */
   virtual ~CFootBotDiffusion() {}

   /*
    * This function initializes the controller.
    * The 't_node' variable points to the <parameters> section in the XML
    * file in the <controllers><assignment1_controller> section.
    */
   virtual void Init(TConfigurationNode& t_node);

   /*
    * This function is called once every time step.
    * The length of the time step is set in the XML file.
    */
    void Stop();
   void Move();
   void SetTarget(CVector2 t);
   virtual void ControlStep();

   /*
    * This function resets the controller to its state right after the
    * Init().
    * It is called when you press the reset button in the GUI.
    * In this example controller there is no need for resetting anything,
    * so the function could have been omitted. It's here just for
    * completeness.
    */
   virtual void Reset() {}

   /*
    * Called to cleanup what done by Init() when the experiment finishes.
    * In this example controller there is no need for clean anything up,
    * so the function could have been omitted. It's here just for
    * completeness.
    */
   virtual void Destroy() {}

   /*  time calculation functions */
   //size_t SimulationTick();
   size_t SimulationTicksPerSecond;
   //Real SimulationSecondsPerTick();
   //Real SimulationTimeInSeconds();

protected:
  //  base controller movement parameters
  Real RobotForwardSpeed;
  Real RobotRotationSpeed;
  Real TicksToWaitWhileMoving;

  CRange<argos::Real>   ForageRangeX;
	CRange<argos::Real>   ForageRangeY;


private:

  //CLoopFunctions& LF;

   /* Pointer to the differential steering actuator */
   CCI_DifferentialSteeringActuator* m_pcWheels;
   /* Pointer to the foot-bot proximity sensor */
   CCI_FootBotProximitySensor* m_pcProximity;
   CCI_PositioningSensor* m_compassSensor;

   /*
    * The following variables are used as parameters for the
    * algorithm. You can set their value in the <parameters> section
    * of the XML configuration file, under the
    * <controllers><assignment1_controller> section.
    */

   /* Maximum tolerance for the angle between
    * the robot heading direction and
    * the closest obstacle detected. */
   CDegrees m_cAlpha;
   /* Maximum tolerance for the proximity reading between
    * the robot and the closest obstacle.
    * The proximity reading is 0 when nothing is detected
    * and grows exponentially to 1 when the obstacle is
    * touching the robot.
    */
   Real m_fDelta;
   /* Wheel speed. */
   Real m_fWheelVelocity;
   /* Angle tolerance range to go straight.
    * It is set to [-alpha,alpha]. */
   CRange<CRadians> m_cGoStraightAngleRange;

   void SetTargetAngleDistance(Real newAngleToTurnInDegrees);
   void SetTargetTravelDistance(Real newTargetDistance);
   void SetLeftTurn(Real newTargetAngle);
   void SetRightTurn(Real newTargetAngle);
   void SetMoveForward(Real newTargetDistance);
   void SetMoveBack(Real newTargetDistance);
   void PushMovement(size_t moveType, Real moveSize);
   void PopMovement();
   void SetRandomLocation();


};

#endif
