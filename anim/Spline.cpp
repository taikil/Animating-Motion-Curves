#include "Spline.h"

Spline::Spline( const std::string& name, int numPoints ):
	BaseSystem( name ),
	m_sx( 1.0f ),
	m_sy( 1.0f ),
	m_sz( 1.0f ),
	numPoints( numPoints )
{ 

	for (size_t i = 0; i < numPoints; ++i) {
		setVector(points[i],  static_cast<double>(rand() % 1000),  static_cast<float>(rand() % 1000),  static_cast<float>(rand() % 1000));
	}
	
}	// Spline


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

int Spline::setTangent(int index, double x, double y, double z) {
	if (index < 0 || index >= numPoints) {
		return TCL_ERROR; // Invalid index
	}

	m_tangent[index][0] = x;
	m_tangent[index][1] = y;
	m_tangent[index][2] = z;

	// Update the model after modifying tangents
	updateModel();

	return TCL_OK;
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
	else if( strcmp(argv[0], "pos") == 0 )
	{
		if( argc == 4 )
		{
			points[atof(argv[1])][0] = atof(argv[2]) ;
			points[atof(argv[1])][0] = atof(argv[2]) ;
			points[atof(argv[1])][2] = atof(argv[4]);
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

}	// Spline::command

void Spline::display( GLenum mode ) 
{
	glEnable(GL_LIGHTING) ;
	glMatrixMode(GL_MODELVIEW) ;
	glPushMatrix() ;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glTranslated(points[0][0],points[0][1],points[0][2]) ;
	glScalef(m_sx,m_sy,m_sz) ;

	if( m_model.numvertices > 0 )
		glmDraw(&m_model, GLM_SMOOTH | GLM_MATERIAL);
	else
		glutSolidSphere(1.0,20,20) ;

	glPopMatrix();
	glPopAttrib();

}	// Spline::display
