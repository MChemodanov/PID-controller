#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>

float step = 0.1;
float t = 0;
float imitationEnd = 100;

float P_COEFF = 0;

void PrintSignal(float *signal, int size)
{
	float max=0;
	for (int i=0; i<size; i++)
	{
		if (signal[i]>max)
			max=signal[i];
	}
	float coef=35.0/max;
	int x=0;
	for (int i=0; i<size; i++)
	{
		int spaces = coef*signal[i];
		for (int j=0; j<spaces; j++)
		{
			printf (" ");
		}
		printf("*\n");
	}
}

struct Robot
{
	float mass;
	float force;
	float floatingForce;
	float speed;
	float depth;
	float dragCoeff;
	float desired_depth;
};

Robot robot;

void InitRobot(Robot*r)
{
	r->depth         = 30;
	r->mass          = 1;
	r->speed         = 0;
	r->dragCoeff     = 0.1;
	r->force         = 0;
	r->floatingForce = 1;
	r->desired_depth  = 20;
}

float P_regulate(float current_depth, float desired_depth)
{
	float diff = current_depth - desired_depth;
	float max_force = 20;
	float border_diff = 1;
	if (abs(diff) > border_diff)
		return max_force*(diff<0 ? 1 : -1);
	else
		return max_force * diff / border_diff;
}


float DragForce(Robot r)
{
	return r.speed * abs(r.speed) * r.dragCoeff;
}

float CalculateStep()
{
	float accel = 0;

	float forceSum = - robot.floatingForce - DragForce(robot) + P_COEFF*P_regulate(robot.depth, robot.desired_depth);
	accel = forceSum / robot.mass;
	robot.speed += accel * step;
	robot.depth += robot.speed * step;
	if (robot.depth <= 0)
	{
		robot.speed = 0;
		robot.depth = 0;
	}
	return robot.depth;
}

int main(int args, char ** params)
{
	sscanf(params[1], "%f", &P_COEFF);
	InitRobot(&robot);
	int steps = (int)(imitationEnd/step);
	float * vesult = (float*)malloc(steps*sizeof(float));
	for (int i = 0; i < steps; i++)
	{
		vesult[i] = CalculateStep();
		printf("%f\n", robot.depth);
	}
	return 0;
}