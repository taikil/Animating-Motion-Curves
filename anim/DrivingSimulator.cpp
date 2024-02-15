#include "DrivingSimulator.h"

DrivingSimulator::DrivingSimulator(const std::string& name, BaseSystem* target, Spline* spline) :
	BaseSimulator(name),
	m_object(target),
	m_spline(spline)  // Add a member variable to store the spline
{
}

DrivingSimulator::~DrivingSimulator()
{
}

int DrivingSimulator::step(double time)
{
    double pos[3] = { 0.0, 0.0, 0.0 };
    m_object->getState(pos);

    // Use the Spline class to get the car's position along the spline
    glm::dvec3 carPosition = m_spline->getCarPosition(time);

    // Update the car's position using the translate function
    static_cast<Car*>(m_object)->translate(carPosition);

    // Assuming the car has a setState function to update its internal state
    m_object->setState(pos);

	return 0;

}