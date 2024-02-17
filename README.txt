Tai Kilpatrick - V00943861

To run Part 1:

part1
system hermite load "gradingTest3524.txt"

system hermite getArcLength 1
system hermite set point 18 0 0 0
system hermite set tangent 0 5 5 0
system hermite getArcLength 1


Part 2:

part2
system objectpath load "gradingTest3525.txt"

Part1 Completion:

[3] Hermite spline class with a maximum of 40 control points:
 - Complete - Spline.cpp
[3] Catmull-Rom initialization with second-order accurate boundary conditions:
 - Complete - catMullRom() in Spline.cpp
[3] Manipulation of control points and tangents using the console:
 - Complete
[5] Arc Length parameterization using a piecewise linear approximation and lookup table
 - Complete - buildArcLengthLookupTable() - Followed formula for len(u) - Secant Method for inverse(u)
[3] Input/output from/to a file.
> system <name> load "<file name>"
> system <name> export "<file name>"
 - Complete

Part2 Completion:

[1] Loading the path for the object:
> system objectpath load "<spline file>"
 - Complete
[4] Align the major axis of the object with the tangent of the path as it moves:
 - Complete
[4] The moving object should start and stop in an easy-in easy-out way:
 - Semi Complete, EaseIn/Out implemented with translated spline, but it causes issues with velocity when resetting and causes jumpy behavior
[1] When the user presses ‘r’ the object should assume the position and velocity it had when the simulation started.
 - Semi Complete, EaseIn/Out created issues
[1] Quality. Make sure that the scale of your objects is appropriate for the tests provided. Speed over sections of movement should be reasonable so animation, ease-in/out, is clearly visible.
 - Complete

Part3 Completion:
[1] Part1 command: 
 - Complete? part1 is a dummy command but the other commands behave as normal
[1] Part2 command: 
 - Complete? part2 is a dummy command but the other commands behave as normal