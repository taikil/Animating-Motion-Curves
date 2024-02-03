#include "Spline.h"

Spline::Spline(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f),
	numPoints(0),
	points() { 
}


bool Spline::checkColinear() {
	// Check if the control points are co-linear in 3D space
	for (size_t i = 0; i < points.size() - 1; ++i) {
		Vector a,b, crossProduct;
		points[i].getState(a);
		points[i + 1].getState(a);

		VecCrossProd(a, b, crossProduct);
		// If the cross product is too small, points are considered collinear
		if (abs(crossProduct[0]) < 0.001 && abs(crossProduct[1]) < 0.001 && abs(crossProduct[2]) < 0.001) {
			return true;
		}
	}
	return false;
}

int Spline::setTangent(ControlPoint a, ControlPoint b ) {
	Vector pos_A, pos_B;
	a.getTan(pos_A);
	b.getTan(pos_B);
	Vector tangent;
	VecSubtract(pos_A, pos_B, tangent);
	a.setTan(tangent);
	b.setTan(tangent);


	return TCL_OK;
}

void Spline::reset(double time) {
	points.clear();
	numPoints = 0;
}


int Spline::command(int argc, myCONST_SPEC char **argv) 
{
	if( argc < 1 )
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str()) ;
		return TCL_ERROR ;
	}
	else if( strcmp(argv[0], "read") == 0 )
	{
		if( argc == 2 )
		{
			m_model.ReadOBJ(argv[1]) ;
			glmFacetNormals(&m_model) ;
			glmVertexNormals(&m_model, 90) ;
			return TCL_OK ;
		}
		else 
		{
			animTcl::OutputMessage("Usage: read <file_name>") ;
			return TCL_ERROR ;
		}
	}
	else if( strcmp(argv[0], "scale") == 0 )
	{
		if( argc == 4 )
		{
			m_sx = (float)atof(argv[1]) ;
			m_sy = (float)atof(argv[2]) ;
			m_sz = (float)atof(argv[3]) ;
		}
		else
		{
			animTcl::OutputMessage("Usage: scale <sx> <sy> <sz> ") ;
			return TCL_ERROR ;

		}
	}
	else if (strcmp(argv[1], "set") == 0  && strcmp(argv[2], "point") == 0) {
		if( argc == 6 )
		{
			Vector new_pos;
			setVector(new_pos, atof(argv[4]), atof(argv[5]), atof(argv[6]));
			points[atof(argv[3])].setPos(new_pos) ;
		}
		else
		{
			animTcl::OutputMessage("Usage: <name> set point <index> <x y z>") ;
			return TCL_ERROR ;

		}
	} 
	else if (strcmp(argv[1], "set") == 0  && strcmp(argv[2], "tangent") == 0) {
		if( argc == 7 )
		{
			Vector new_tan;
			setVector(new_tan, atof(argv[4]), atof(argv[5]), atof(argv[6]));
			points[atof(argv[3])].setTan(new_tan) ;
		}
		else
		{
			animTcl::OutputMessage("Usage: <name> set point <index> <x y z>") ;
			return TCL_ERROR ;

		}
	}
	else if (strcmp(argv[1], "add") == 0  && strcmp(argv[2], "point") == 0) {
		if( argc == 9 )
		{
			Vector new_pos;
			Vector new_tan;
			ControlPoint new_point("Default");
			setVector(new_pos, atof(argv[4]), atof(argv[5]), atof(argv[6]));
			setVector(new_pos, atof(argv[7]), atof(argv[8]), atof(argv[9]));
			points.push_back(new_point);
		}
		else
		{
			animTcl::OutputMessage("Usage: <name> set point <index> <x y z>") ;
			return TCL_ERROR ;

		}
	}

	else if( strcmp(argv[0], "flipNormals") == 0 )
	{
		flipNormals() ;
		return TCL_OK ;
		
	}
	else if( strcmp(argv[0], "reset") == 0)
	{
		reset(0);
	}
    
    glutPostRedisplay() ;
	return TCL_OK ;

}	// Spline::command

void Spline::display( GLenum mode ) 
{
	glEnable(GL_LIGHTING) ;
	glMatrixMode(GL_MODELVIEW) ;
	glPushMatrix() ;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	Vector p;
	points[0].getPos(p);
	glTranslated(p[0],p[1],p[2]) ;
	glScalef(m_sx,m_sy,m_sz) ;

	if( m_model.numvertices > 0 )
		glmDraw(&m_model, GLM_SMOOTH | GLM_MATERIAL);
	else
		glutSolidSphere(1.0,20,20) ;

	glPopMatrix();
	glPopAttrib();

}	// Spline::display

