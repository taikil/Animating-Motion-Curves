#ifndef CONTROL_POINT_H
#define CONTROL_POINT_H 

/*

	This is a sample system. It accepts the command "read" followed by the 
	path to an OBJ model.

*/


#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

#include "shared/opengl.h"

// a sample system
class ControlPoint : public BaseSystem
{ 

public:
	ControlPoint( const std::string& name );
	virtual void getState( double *p );
	virtual void setState( double  *p );
	void reset( double time );

	void display( GLenum mode = GL_RENDER );

	void readModel(char *fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char **argv) ;

protected:
	Vector m_pos ;

	GLMmodel m_model ;

} ;
#endif
