#include "Spline.h"

Spline::Spline(const std::string& name) :
	BaseSystem(name),
	points(),
	arcLengths(0)
{
}

void Spline::getState(double* p) {

}


void Spline::setState(double* p) {

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
	animTcl::OutputMessage("Initializing Hermite");
	// Ensure there are at least two control points
	if (points.size() < 2)
	{
		animTcl::OutputMessage("Error: Hermite spline requires at least two control points.");
		return;
	}

	// Iterate over each pair of consecutive control points
	for (size_t i = 0; i < points.size() - 1; ++i)
	{
		double stepSize = 1.0 / double(20);
		ControlPoint p0 = points[i];
		ControlPoint p1 = points[i + 1];
		glm::dvec3 samplePoints[20];
		//for each point, run
		for (size_t j = 0; j < 20; j++) {
			double t = j * stepSize;
			samplePoints[j][0] = evaluateCurve(0, t, p0, p1);
			samplePoints[j][1] = evaluateCurve(1, t, p0, p1);
			samplePoints[j][2] = evaluateCurve(2, t, p0, p1);
			animTcl::OutputMessage("Points %.3f / 20: x: %.3f, y: %.3f", static_cast<double>(j), samplePoints[j][0], samplePoints[j][1]);
		}
		points[i].setSamplePoints(samplePoints);

	}

	// Redisplay if needed
	glutPostRedisplay();
}

double Spline::evaluateCurve(int dimension, double t, ControlPoint p0, ControlPoint p1) {
	glm::dvec3 pos0, pos1, tan0, tan1;
	p0.getPos(pos0);
	p0.getTan(tan0);
	p1.getPos(pos1);
	p1.getTan(tan1);
	double a = (2 * pow(t, 3) - 3 * pow(t, 2) + 1) * pos0[dimension]; // (2t^3 -3t^2 + 1)y0
	double b = (-2 * pow(t, 3) + 3 * pow(t, 2)) * pos1[dimension]; // (-2t^3 + 3t^2)y1
	double c = (pow(t, 3) - 2 * pow(t, 2) + t) * tan0[dimension]; // (t^3 - 2t^2+ t)s0
	double d = (pow(t, 3) - pow(t, 2)) * tan1[dimension]; // (t^3 - t^2)s1

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

int Spline::setTangent(ControlPoint& a, ControlPoint& b)
{
	glm::dvec3 pos_A, pos_B;
	a.getTan(pos_A);
	b.getTan(pos_B);
	glm::dvec3 tangent = pos_B - pos_A;
	a.setTan(tangent);
	b.setTan(tangent);
	animTcl::OutputMessage("  New tangent: %.3f %.3f %.3f", tangent.x, tangent.y, tangent.z);

	return TCL_OK;
}

void Spline::reset(double time)
{
	points.clear();
}

double Spline::getArcLength(const ControlPoint& p0, const ControlPoint& p1, int numSamples) {
	double stepSize = 1.0 / double(numSamples);
	double arcLength = 0.0;

	for (int i = 0; i < numSamples; ++i) {
		double t0 = i * stepSize;
		double t1 = (i + 1) * stepSize;

		// Use a numerical integration method (e.g., trapezoidal rule)
		double deltaS = glm::length(glm::dvec3(
			evaluateCurve(0, t1, p0, p1) - evaluateCurve(0, t0, p0, p1),
			evaluateCurve(1, t1, p0, p1) - evaluateCurve(1, t0, p0, p1),
			evaluateCurve(2, t1, p0, p1) - evaluateCurve(2, t0, p0, p1)
		));

		arcLength += deltaS;
	}

	return arcLength;
}

void Spline::buildArcLengthLookupTable(int numSamples) {
	arcLengths.clear();

	for (size_t i = 0; i < points.size() - 1; ++i) {
		double segmentLength = getArcLength(points[i], points[i + 1], numSamples);
		double accumulatedLength = (i > 0) ? arcLengths[i - 1] : 0.0;

		arcLengths.push_back(segmentLength + accumulatedLength);
	}
}

glm::dvec3 Spline::getPointOnSpline(double parameter) {
	// Map parameter value to arc length using the lookup table
	double targetLength = parameter * arcLengths.back();

	// Find the curve segment that contains the target arc length
	size_t segmentIndex = 0;
	while (segmentIndex < arcLengths.size() && arcLengths[segmentIndex] < targetLength) {
		++segmentIndex;
	}

	// Interpolate within the segment to find the corresponding point
	double t = (targetLength - arcLengths[segmentIndex - 1]) / (arcLengths[segmentIndex] - arcLengths[segmentIndex - 1]);

	// Use evaluateCurve to get the point
	return glm::dvec3(
		evaluateCurve(0, t, points[segmentIndex - 1], points[segmentIndex]),
		evaluateCurve(1, t, points[segmentIndex - 1], points[segmentIndex]),
		evaluateCurve(2, t, points[segmentIndex - 1], points[segmentIndex])
	);
}



int Spline::load(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		animTcl::OutputMessage("Error: Unable to open file %s for reading.", filename.c_str());
		return TCL_ERROR;
	}

	// Clear existing points before loading
	points.clear();

	std::string splineName;
	int numPoints;
	file >> splineName >> numPoints;

	for (int i = 0; i < numPoints; ++i)
	{
		glm::dvec3 new_pos, new_tan;
		file >> new_pos.x >> new_pos.y >> new_pos.z >> new_tan.x >> new_tan.y >> new_tan.z;
		addPoint(new_pos, new_tan);
	}

	file.close();
	animTcl::OutputMessage("Spline loaded from %s.", filename.c_str());

	initHermite();

	return TCL_OK;
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
	else if (strcmp(argv[0], "cr") == 0)
	{
		if (argc == 1) {
			catMullRom();
			animTcl::OutputMessage("Initializing Catmull-Rom");
			return TCL_OK;
		}
		else {
			animTcl::OutputMessage("Usage: <name> cr");
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
		// Check if the correct number of arguments is provided
		if (argc != 8)
		{
			animTcl::OutputMessage("Usage: <name> add point <n> <p1[x] p1[y] p1[z] s1[x] s1[y] s1[z]> ... <pn[x] pn[y] pn[z] pn[x] sn[y] sn[z]>");
			return TCL_ERROR;
		}

		glm::dvec3 new_pos, new_tan;

		// Calculate the index for the current point
		int index = points.size();

		// Extract coordinates for the control point
		new_pos = glm::dvec3(atof(argv[2]), atof(argv[3]), atof(argv[4]));

		// Extract coordinates for the tangent
		new_tan = glm::dvec3(atof(argv[5]), atof(argv[6]), atof(argv[7]));

		// Add the point to the spline
		addPoint(new_pos, new_tan);

		// Output a success message
		animTcl::OutputMessage("Added point");
	}
	else if (strcmp(argv[0], "load") == 0)
	{
		if (argc == 2)
		{
			return load(argv[1]);
		}
		else
		{
			animTcl::OutputMessage("Usage: system <name> load \"<file name>\"");
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

void Spline::displaySampledCurve(float r) {
	glLineWidth(r);
	glm::dvec3 samplePoints[20];
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < points.size(); i++) {
		points[i].getPoints(samplePoints);
		for (int j = 0; j < 20; j++)
			glVertex3f(samplePoints[j][0], samplePoints[j][1], samplePoints[j][2]);
	}
	glEnd();
}

void Spline::displayPoints(float r) {
	// Render each control point as a dot
	glPointSize(r);
	glBegin(GL_POINTS);
	for (size_t i = 0; i < points.size(); ++i)
	{
		glm::dvec3 pos;
		points[i].getPos(pos);
		glVertex3f(pos.x, pos.y, pos.z);

	}
	glEnd();

}


void Spline::display(GLenum mode)
{
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_COLOR_MATERIAL);

	glTranslated(0, 0, 0);

	glColor3f(1.0f, 0.0f, 0.0f);  // Set dot color
	displayPoints(5.0f);

	glColor3f(0.3, 0.7, 0.1);
	displaySampledCurve(1.5);

	//if (m_model.numvertices > 0)
	//	glmDraw(&m_model, GLM_SMOOTH | GLM_MATERIAL);
	//else
		//glutSolidSphere(1.0, 20, 20);

	glPopMatrix();
	glPopAttrib();
}
