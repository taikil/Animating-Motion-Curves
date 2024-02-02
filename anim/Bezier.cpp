#include "Bezier.h"

Bezier::Bezier( const std::string& name, int numPoints ):
	BaseSystem( name ),
	m_sx( 1.0f ),
	m_sy( 1.0f ),
	m_sz( 1.0f ),
	numPoints( numPoints )
{ 

	for (size_t i = 0; i < numPoints; ++i) {
		setVector(m_pos[i],  static_cast<double>(rand() % 1000),  static_cast<float>(rand() % 1000),  static_cast<float>(rand() % 1000));
	}
	
}	// Bezier

void Bezier::getState( double* p )
{ 

	VecCopy( p, m_pos[0]);

}	// Bezier::getState

void Bezier::setState( double  *p )
{ 

	VecCopy(m_pos[0], p);

}	// Bezier::setState

void Bezier::reset( double time ) 
{ 
	
	for (size_t i = 0; i < numPoints; ++i) {
		setVector(m_pos[i], 0, 0, 0);
	}
	
}	// Bezier::Reset

bool Bezier::checkColinear() {
	// Check if the control points are co-linear in 3D space
	for (size_t i = 0; i < m_pos.size() - 1; ++i) {
		Vector crossProduct;
		VecCrossProd(m_pos[i], m_pos[i+1], crossProduct);
		// If the cross product is too small, points are considered collinear
		if (abs(crossProduct[0]) < 0.001 && abs(crossProduct[1]) < 0.001 && abs(crossProduct[2]) < 0.001) {
			return true;
		}
	}
	return false;
}


int Bezier::command(int argc, myCONST_SPEC char **argv) 
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
	else if( strcmp(argv[0], "pos") == 0 )
	{
		if( argc == 4 )
		{
			m_pos[atof(argv[1])][0] = atof(argv[2]) ;
			m_pos[atof(argv[1])][0] = atof(argv[2]) ;
			m_pos[atof(argv[1])][2] = atof(argv[4]);
		}
		else
		{
			animTcl::OutputMessage("Usage: pos <i> <x> <y> <z> ") ;
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
		double p[3] = {0,0,0} ;
		setState(p) ;
	}
    
    glutPostRedisplay() ;
	return TCL_OK ;

}	// Bezier::command

void Bezier::display( GLenum mode ) 
{
	glEnable(GL_LIGHTING) ;
	glMatrixMode(GL_MODELVIEW) ;
	glPushMatrix() ;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glTranslated(m_pos[0][0],m_pos[0][1],m_pos[0][2]) ;
	glScalef(m_sx,m_sy,m_sz) ;

	if( m_model.numvertices > 0 )
		glmDraw(&m_model, GLM_SMOOTH | GLM_MATERIAL);
	else
		glutSolidSphere(1.0,20,20) ;

	glPopMatrix();
	glPopAttrib();

}	// Bezier::display
