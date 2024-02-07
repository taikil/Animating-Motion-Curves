#pragma once
#ifndef SPLINE_H
#define SPLINE_H

#include "animTcl.h"
#include "BaseSystem.h"
#include "ControlPoint.h"
#include <GLmodel/GLmodel.h>
#include <shared/defs.h>
#include <util/util.h>

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
    //    void initializeCatmullRom();
    int setTangent(ControlPoint a, ControlPoint b);
    void Spline::addPoint(const glm::dvec3& pos, const glm::dvec3& tan);
    bool checkColinear();
    void initHermite();

    void display(GLenum mode = GL_RENDER);

    void readModel(const char* fname) { m_model.ReadOBJ(fname); }
    void flipNormals(void) { glmReverseWinding(&m_model); }
    int command(int argc, myCONST_SPEC char** argv);

protected:
    float m_sx;
    float m_sy;
    float m_sz;

    std::vector<ControlPoint> points;

    GLMmodel m_model;
};

#endif // SPLINE_H
