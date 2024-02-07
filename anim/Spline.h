#pragma once
#ifndef SPLINE_H 
#define SPLINE_H

#include "BaseSystem.h"
#include "ControlPoint.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

#include "shared/opengl.h"
#include <vector>



class Spline : public BaseSystem
{ 

public:
	Spline( const std::string& name);
	void reset( double time );
//	void initializeCatmullRom();
	int setTangent(ControlPoint a, ControlPoint b );
	bool checkColinear();

	void display( GLenum mode = GL_RENDER );

	void readModel(char *fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char **argv) ;

protected:

	float m_sx;
	float m_sy;
	float m_sz;

	std::vector<ControlPoint> points ;
 
	GLMmodel m_model ;

} ;

#endif // SPLINE_H 


