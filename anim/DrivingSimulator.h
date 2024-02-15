#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseSimulator.h"
#include "BaseSystem.h"
#include "Car.h"
#include "Spline.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



#include <string>

// a sample simulator

class DrivingSimulator : public BaseSimulator
{
public:

	DrivingSimulator(const std::string& name, BaseSystem* target, Spline* spline);
	~DrivingSimulator();

	int step(double time);
	int init(double time)
	{
		Vector pos;
		m_object->getState(pos);
		m_pos0 = glm::dvec3(pos[0], pos[1], pos[2]);

		// Set this to set the radius of the circle
		amplitude = 4.0;

		//Set this to change the frequency of the circular movement
		frequency = 0.2;

		return 0;
	};

	int command(int argc, myCONST_SPEC char** argv) { return TCL_OK; }

protected:

	glm::dvec3 m_pos0; // initial position
	glm::dvec3 m_vel0; // initial velocity
	glm::dvec3 m_pos;
	glm::dvec3 m_vel;

	double amplitude;
	double frequency;

	BaseSystem* m_object;
	Spline* m_spline;

};