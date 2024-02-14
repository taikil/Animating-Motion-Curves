#include "DrivingSimulator.h"

DrivingSimulator::DrivingSimulator(const std::string& name, BaseSystem* target) :
	BaseSimulator(name),
	m_object(target)
{
}

DrivingSimulator::~DrivingSimulator()
{
}

int DrivingSimulator::step(double time)
{
	double pos[3] = { 0.0, 0.0, 0.0 };
	m_object->getState(pos);

	double twoPIfreqTime = 2.0 * 3.14 * frequency * time;
	pos[1] = m_pos0[1] + amplitude * sin(twoPIfreqTime);

	glm::dvec3 translation = glm::dvec3(0.0, pos[1], 0.0); // Assuming y-axis represents the vertical direction
	static_cast<Car*>(m_object)->translate(translation);

	m_object->setState(pos);

	return 0;

}