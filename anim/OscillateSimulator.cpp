#include "OscillateSimulator.h"

OscillateSimulator::OscillateSimulator(const std::string& name, BaseSystem* target) :
	BaseSimulator(name),
	m_object(target)
{
}

OscillateSimulator::~OscillateSimulator()
{
}

int OscillateSimulator::step(double time)
{
	Vector pos;
	m_object->getState(pos);

	double twoPIfreqTime = 2.0 * 3.14 * frequency * time;
	pos[1] = m_pos0[1] + amplitude * sin(twoPIfreqTime);

	m_object->setState(pos);

	return 0;

}