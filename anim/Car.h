#pragma once
#ifndef MY_CAR_H
#define MY_CAR_H


#include "BaseSystem.h"
#include <shared/defs.h>
#include <windows.h>
#include <util/util.h>
#include "animTcl.h"
#include "Spline.h"
#include <GLmodel/GLmodel.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include "shared/opengl.h"

// a sample system
class Car : public BaseSystem
{

public:
	Car(const std::string& name, Spline* spline);
	virtual void getState(double* p);
	virtual void setState(double* p);
	void reset(double time);

	void translate(glm::dvec3 translation);
	void rotate(glm::dvec3 axis, double angleDegrees);

	void display(GLenum mode = GL_RENDER);

    void readModel(const char* fname);
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char** argv);

protected:

	float m_sx;
	float m_sy;
	float m_sz;

	glm::quat m_rot; // Quaternion for rotation
	glm::dvec3 m_pos;

	GLMmodel m_model;
	Spline* m_spline;

};
#endif


