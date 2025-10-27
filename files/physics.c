//Copyright (c) 2025 Neil Kirby not for distribution without written permission
// This file has also been edited by Samay Patel

// system libraries
#include <stdio.h>
#include <math.h>

// custom libraries
#include "debug.h"

// file specific header file
#include "physics.h"

// calculates distance between two points on a cartesian plane
double distance(double x1, double y1, double x2, double y2)
{
        double dx = x2 - x1;
        double dy = y2 - y1;
        double separation = sqrt(dx * dx + dy * dy);
        return separation;
}

// calculates the new velocity for item "a" in an inelastic collision
double inelastic_v_final(double v_a, double mass_a, double v_b, double mass_b)
{
        double total_p = v_a * mass_a + v_b * mass_b;
        double total_mass = mass_a + mass_b;

        return total_p / total_mass;
}

// calculates the final velocity for "a" in an elastic condition
double elastic_va_final(double v_a, double mass_a, double v_b, double mass_b)
{
        double total_mass = mass_a + mass_b;

        double new_v_a = (
                v_a *(mass_a - mass_b) + (2*mass_b)* v_b
                ) /total_mass;
        return new_v_a;

}

double split_offset(double p1, double p2, double dist, double radius)
{
	double ratio = (p1-p2) / dist;
	double offset = ratio * radius / 2.0;

	if(DEBUG)fprintf(stderr,
"split_offset: p1 %.2lf, p2 %.2lf, dist %.2lf, radius %.2lf: \n\t ratio %.2lf, offset %.2lf\n",
		p1, p2, dist, radius, ratio, offset);

	return offset;

}
