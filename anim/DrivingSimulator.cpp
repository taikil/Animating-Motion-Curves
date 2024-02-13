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
	Vector pos;
	m_object->getState(pos);

	double twoPIfreqTime = 2.0 * 3.14 * frequency * time;
	pos[1] = m_pos0[1] + amplitude * sin(twoPIfreqTime);

	m_object->setState(pos);

	return 0;

}