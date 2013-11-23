#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>

float step = 0.1;
float t = 0;
float imitationEnd = 100;

float P_COEFF = 0;
float I_COEFF = 0;


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
	memset(r->buffer,0,I_BUFFER_SIZE*sizeof(float));
}

float P_regulate(Robot*r)
{
	float diff = r->depth - r->desired_depth;
	float max_force = 20;

	return -1*max_force * diff * P_COEFF;
}

float BufferSum(float * buffer, int size)
{
	float result = 0;
	for (int i =0; i< size; i++)
		result += buffer[i];
	return result;
}

float I_regulate(Robot*r)
{
	static float integrated = 0;
	static int iCounter = 0;
	integrated  += r->desired_depth - r->depth;

	return integrated*I_COEFF;
}

float DragForce(Robot r)
{
	return -1*r.speed * r.dragCoeff;
}

float CalculateStep()
{
	float accel = 0;

	float forceSum = robot.floatingForce + DragForce(robot) + P_regulate(&robot)+I_regulate(&robot);;
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
	if (args < 3)
	{
		printf("Need 2 params, p_coeff and i_coeff");
		return 0;
	}
	sscanf(params[1], "%f", &P_COEFF);
	sscanf(params[2], "%f", &I_COEFF);
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