#include <windows.h>
#include "Spline.h"

Spline::Spline(const std::string& name) :
	BaseSystem(name),
	points()
{
}

bool Spline::checkColinear()
{
	// Check if the control points are co-linear in 3D space
	for (size_t i = 0; i < points.size() - 1; ++i)
	{
		glm::dvec3 a, b, crossProduct;
		points[i].getPos(a);
		points[i + 1].getPos(b);

		crossProduct = glm::cross(a, b);

		// If the cross product is too small, points are considered collinear
		if (glm::length(crossProduct) < 0.001)
		{
			return true;
		}
	}
	return false;
}


void Spline::initHermite()
{
	// Ensure there are at least two control points
	if (points.size() < 2)
	{
		animTcl::OutputMessage("Error: Hermite spline requires at least two control points.");
		return;
	}


	// Iterate over each pair of consecutive control points
	for (size_t i = 0; i < points.size() - 1; ++i)
	{
		double stepSize = 1.0 / double(numSamples);
		ControlPoint& p0 = points[i];
		ControlPoint& p1 = points[i + 1];

	}

	// Redisplay if needed
	glutPostRedisplay();
}

double Spline::evaluateCurve(int d, double t, ControlPoint p0, ControlPoint p1) {
	glm::dvec3 pos0, pos1, tan0, tan1;
	p0.getPos(pos0);
	p0.getTan(tan0);
	p1.getPos(pos1);
	p1.getTan(tan1);
	double a = (2 * pow(t, 3) - 3 * pow(t, 2) + 1) * pos0[d]; // (2t^3 -3t^2 + 1)y0
	double b = (-2 * pow(t, 3) + 3 * pow(t, 2)) * pos1[d]; // (-2t^3 + 3t^2)y1
	double c = (pow(t, 3) - 2 * pow(t, 2) + t) * tan0[d]; // (t^3 - 2t^2+ t)s0
	double d = (pow(t, 3) - pow(t, 2)) * tan1[d]; // (t^3 - t^2)s1

	return (a + b + c + d);

}

void Spline::addPoint(const glm::dvec3& pos, const glm::dvec3& tan) {
	if (points.size() < 40) {
		ControlPoint new_point("Default");
		new_point.setPos(pos);
		new_point.setTan(tan);
		points.push_back(new_point);

		// Output characteristics of the added point
		animTcl::OutputMessage("Added Point:");
		animTcl::OutputMessage("  Position: %.3f %.3f %.3f", pos.x, pos.y, pos.z);
		animTcl::OutputMessage("  Tangent: %.3f %.3f %.3f", tan.x, tan.y, tan.z);
	}
	else {
		animTcl::OutputMessage("Error: Maximum number of control points reached (40).");
	}
}

void Spline::catMullRom() {
	for (size_t i = 0; i < points.size() - 1; i++) {
		ControlPoint& p0 = points[i];
		ControlPoint& p1 = points[i + 1];
		setTangent(p0, p1);
	}

	// Redisplay if needed
	glutPostRedisplay();
}

int Spline::setTangent(ControlPoint a, ControlPoint b)
{
	glm::dvec3 pos_A, pos_B;
	a.getTan(pos_A);
	b.getTan(pos_B);
	glm::dvec3 tangent = pos_B - pos_A;
	a.setTan(tangent);
	b.setTan(tangent);

	return TCL_OK;
}

void Spline::reset(double time)
{
	points.clear();
}

int Spline::command(int argc, myCONST_SPEC char** argv)
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
			glmFacetNormals(&m_model);
			glmVertexNormals(&m_model, 90);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: read <file_name>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "set") == 0 && strcmp(argv[1], "point") == 0)
	{
		if (argc == 6)
		{
			glm::dvec3 new_pos;
			int index = atoi(argv[2]);
			if (index >= points.size())
			{
				animTcl::OutputMessage(" Error: Index out of range");
				return TCL_ERROR;
			}
			new_pos = glm::dvec3(atof(argv[3]), atof(argv[4]), atof(argv[5]));
			points[index].setPos(new_pos);
		}
		else
		{
			animTcl::OutputMessage("Usage: <name> set point <index> <x y z>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "set") == 0 && strcmp(argv[1], "tangent") == 0)
	{
		if (argc == 6)
		{
			glm::dvec3 new_tan;
			int index = atoi(argv[2]);
			if (index >= points.size())
			{
				animTcl::OutputMessage(" Error: Index out of range");
				return TCL_ERROR;
			}
			new_tan = glm::dvec3(atof(argv[4]), atof(argv[5]), atof(argv[6]));
			points[index].setTan(new_tan);
		}
		else
		{
			animTcl::OutputMessage("Usage: <name> set point <index> <x y z>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "add") == 0 && strcmp(argv[1], "point") == 0)
	{
		if (argc == 8)
		{
			glm::dvec3 new_pos, new_tan;
			ControlPoint new_point("Default");
			new_pos = glm::dvec3(atof(argv[2]), atof(argv[3]), atof(argv[4]));
			new_tan = glm::dvec3(atof(argv[5]), atof(argv[6]), atof(argv[7]));
			addPoint(new_pos, new_tan);
		}
		else
		{
			animTcl::OutputMessage("Usage: <name> add point <x y z> <xt yt zt>");
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
		reset(0);
	}

	glutPostRedisplay();
	return TCL_OK;
}

void Spline::display(GLenum mode)
{
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_COLOR_MATERIAL);

	glTranslated(0, 0, 0);

	// Render each control point as a dot
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f);  // Set dot color
	for (size_t i = 0; i < points.size(); ++i)
	{
		glm::dvec3 pos;
		points[i].getPos(pos);
		glVertex3f(pos.x, pos.y, pos.z);

	}
	glEnd();

	//if (m_model.numvertices > 0)
	//	glmDraw(&m_model, GLM_SMOOTH | GLM_MATERIAL);
	//else
		//glutSolidSphere(1.0, 20, 20);

	glPopMatrix();
	glPopAttrib();
}
