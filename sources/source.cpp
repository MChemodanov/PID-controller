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
	r->depth         = 20;
	r->mass          = 10;
	r->speed         = 0;
	r->dragCoeff     = 2;
	r->force         = 0;
	r->floatingForce = -1;
	r->desired_depth  = 10;
}

float P_regulate(Robot*r)
{
	float diff = r->depth - r->desired_depth;
	float max_force = 20;

	return -1*max_force * diff * P_COEFF;
}


float DragForce(Robot r)
{
	return -1*r.speed * r.dragCoeff;
}

float CalculateStep()
{
	float accel = 0;

	float forceSum = robot.floatingForce + DragForce(robot) + P_regulate(&robot);
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
	if (args < 2)
	{
		printf("Need 1 param: p_coeff");
		return 0;
	}
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