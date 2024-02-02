#ifndef BEZIER_H
#define BEZIER_H

#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

#include "shared/opengl.h"
#include <vector>



class Bezier : public BaseSystem
{ 

public:
	Bezier( const std::string& name, int numPoints);
	virtual void getState( double *p );
	virtual void setState( double  *p );
	void reset( double time );
	bool checkColinear();

	void display( GLenum mode = GL_RENDER );

	void readModel(char *fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char **argv) ;

protected:

	float m_sx;
	float m_sy;
	float m_sz;

	std::vector<Vector> m_pos ;

	int numPoints;

	GLMmodel m_model ;

} ;

#endif // BEZIER_H

