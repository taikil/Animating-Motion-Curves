#pragma once
#ifndef SPLINE_H
#define SPLINE_H

#include <windows.h>
#include "animTcl.h"
#include "BaseSystem.h"
#include "ControlPoint.h"
#include <GLmodel/GLmodel.h>
#include <shared/defs.h>
#include <util/util.h>
#include <fstream>

#include "shared/opengl.h"
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Spline : public BaseSystem
{

public:
	Spline(const std::string& name);
	void reset(double time);
	int setTangent(ControlPoint& a, ControlPoint& b);
	void catMullRom();
	void addPoint(const glm::dvec3& pos, const glm::dvec3& tan);
	bool checkColinear();
	void initHermite();
	double evaluateCurve(int dimension, double t, ControlPoint p0, ControlPoint p1);
	double getArcLength(const ControlPoint& p0, const ControlPoint& p1, int numSamples);
	void buildArcLengthLookupTable(int numSamples);
	glm::dvec3 Spline::getPointOnSpline(double parameter);
	void displaySampledCurve(float r);
	void displayPoints(float r);
	void display(GLenum mode = GL_RENDER);
	int load(const std::string& filename);
	void readModel(const char* fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char** argv);

protected:
	std::vector<ControlPoint> points;
	std::vector<double> arcLengths;

	GLMmodel m_model;
};

#endif // SPLINE_H
