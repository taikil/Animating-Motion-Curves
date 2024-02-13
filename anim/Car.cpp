#include "Car.h"

Car::Car(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f),
	m_pos(0, 0, 0),
	m_rot(1, 0, 0, 0)
	//Rotation
{ 
	m_model.ReadOBJ("../Build/data/porsche.obj");
	glmUnitize(&m_model);
	glmFacetNormals(&m_model);
	glmVertexNormals(&m_model, 90);
}	// Car

void Car::getState(double* p)
{


}	// Car::getState

void Car::setState(double* p)
{


}	// Car::setState

void Car::reset(double time)
{


}	// Car::Reset


void Car::translate(glm::dvec3 translation) {
	m_pos = translation;
	//glTranslated(ranslation[0], translation[1], translation[2]);
}

void Car::rotate(glm::dvec3 axis, double angleDegrees)
{
	// Create a quaternion representing the rotation
	glm::quat rotationQuat = glm::angleAxis(glm::radians(angleDegrees), glm::normalize(axis));

	// Combine the new rotation with the current rotation
	m_rot= rotationQuat * m_rot;

	// Optionally normalize the quaternion to avoid floating-point precision issues
	m_rot = glm::normalize(m_rot);
}

void Car::readModel(const char* fname)
{
	m_model.ReadOBJ(fname);
	glmFacetNormals(&m_model);
	glmVertexNormals(&m_model, 90);
}

int Car::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "read") == 0)
	{
		if (argc == 2)
		{
			readModel(argv[1]);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: read <file_name>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "scale") == 0)
	{
		if (argc == 4)
		{
			m_sx = (float)atof(argv[1]);
			m_sy = (float)atof(argv[2]);
			m_sz = (float)atof(argv[3]);
		}
		else
		{
			animTcl::OutputMessage("Usage: scale <sx> <sy> <sz> ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "translate") == 0)
	{
		if (argc == 4)
		{
			translate(glm::dvec3(atof(argv[1]), atof(argv[2]), atof(argv[3])));
		}
		else
		{
			animTcl::OutputMessage("Usage: translate <x> <y> <z> ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "rotate") == 0)
	{
		if (argc == 5) // Need to pass axis and three rotation parameters
		{
			rotate(glm::dvec3(atof(argv[1]), atof(argv[2]), atof(argv[3])), atof(argv[4]));
		}
		else
		{
			animTcl::OutputMessage("Usage: rotate <axis_i> <axis_j> <axis_k> <angle_degrees>");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "flipNormals") == 0)
	{
		flipNormals();
		return TCL_OK;

	}
	else if (strcmp(argv[0], "reset") == 0)
	{
		double p[3] = { 0,0,0 };
		setState(p);
	}

	glutPostRedisplay();
	return TCL_OK;

}	// Car::command

void Car::display(GLenum mode)
{
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glTranslated(m_pos[0], m_pos[1], m_pos[2]);
	glScalef(m_sx, m_sy, m_sz);
	//glRotated(180, 0, 1, 0);
	//glRotated(270, 1, 0, 0);
	glm::mat4 rotationMatrix = glm::mat4_cast(m_rot);
	glMultMatrixf(glm::value_ptr(rotationMatrix));

	if (m_model.numvertices > 0)
		glmDraw(&m_model, GLM_SMOOTH | GLM_MATERIAL);
	else
		glutSolidSphere(1.0, 20, 20);

	glPopMatrix();
	glPopAttrib();

}	// Car::display

