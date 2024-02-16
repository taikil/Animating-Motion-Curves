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

int DrivingSimulator::step(double time) // 0.01s
{
    double pos[3] = { 0.0, 0.0, 0.0 };
    m_object->getState(pos);


    // Use the Spline class to get the car's position along the spline
    glm::dvec3 carPosition = m_spline->getCarPosition(velocity * time); // 2m/s
    glm::dvec3 carRotation = m_spline->getTangents(velocity * time);

    if (glm::length(carPosition) > 0.0) {
        // Calculate the rotation angle in degrees based on the arctangent of the tangent vector components
        double rotationAngleDegrees = glm::degrees(atan2(carRotation.x, carRotation.y));

        // Rotate the car around the vertical axis based on the calculated angle
        static_cast<Car*>(m_object)->rotate(glm::dvec3(0, 0, 1), rotationAngleDegrees);
		animTcl::OutputMessage("Rotation: x: %.3f", rotationAngleDegrees);
    }

    // Update the car's position using the translate function
    static_cast<Car*>(m_object)->translate(carPosition);

    // Assuming the car has a setState function to update its internal state
    //m_object->setState(pos);

    return 0;

}