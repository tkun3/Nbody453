#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Global Variables


struct Node {
	double mass;
	double x;
	double y;
	double z;
	double vX;
	double vY;
	double vZ;
};

double timeDelta = 0.05;
double currentSimTime;
double simTime = 50000;
int    nodeIndexes;
struct Node nodeData[9];
struct Node nodeDataResult[9];
char *fileName = "nbody_initial_new.txt";

double forces[3];

double G = 6.67408E-11;


double x, y, z, mass, vX, vY, vZ;


double initializeNodes(){
	printf("Opening File \n");
	FILE * file = fopen(fileName, "r");

	int a = 0;

	while (fscanf(file, "%lf %lf %lf %lf %lf %lf %lf", &x, &y, &z, &mass, &vX, &vY, &vZ) > 0){ 
		//printf("Initializing Node %i", a);
		nodeData[a].x = x;
		nodeData[a].y = y;
		nodeData[a].z = z;
		nodeData[a].mass = mass;
		nodeData[a].vX = vX;
		nodeData[a].vY = vY;
		nodeData[a].vZ = vZ;
		printf("Node %i initilized: x = %e, y = %e, z = %e, mass = %e, vX = %e, vY = %e, vZ = %e \n", a, x, y, z, mass, vX, vY, vZ);
		a++; 
	}

	nodeIndexes = a;
	return 0.0;
}


//calculates initial S + Velocity increment
double updatePositionVelocity(int a){
	nodeDataResult[a].x = nodeData[a].x + (nodeData[a].vX * timeDelta);
	nodeDataResult[a].y = nodeData[a].y + (nodeData[a].vY * timeDelta);
	nodeDataResult[a].z = nodeData[a].z + (nodeData[a].vZ * timeDelta);
	//printf("Node %i Position (VELOCITY APPLIED): x=%e y=%e z=%e \n", a, nodeDataResult[a].x, nodeDataResult[a].y, nodeDataResult[a].z);
	return 0.0;
}

double calculateForce(int node1, int node2){

	double node1X = nodeData[node1].x;
	double node1Y = nodeData[node1].y;
	double node1Z = nodeData[node1].z;

	double node2X = nodeData[node2].x;
	double node2Y = nodeData[node2].y;
	double node2Z = nodeData[node2].z;

	double radius = sqrt(pow((nodeData[node2].x - nodeData[node1].x),2.00) + pow((nodeData[node2].y - nodeData[node1].y),2.00) + pow((nodeData[node2].z - nodeData[node1].z),2.00));

	if (radius < 1000.00){
		forces[0] = 0.00;
		forces[1] = 0.00;
		forces[2] = 0.00;
		return 0.00;
	}

	double xDvector = (node1X - node2X)/radius;
	double yDvector = (node1Y - node2Y)/radius;
	double zDvector = (node1Z - node2Z)/radius;

	double force = (G * nodeData[node1].mass * nodeData[node2].mass)/(radius*radius);

	forces[0] = force * xDvector;
	forces[1] = force * yDvector;
	forces[2] = force * zDvector;

	return 0.00;
}

//calculates and adds accleration to velocity
double updateNodeVelocity(int node1, double fX, double fY, double fZ){
	double vX = nodeDataResult[node1].vX;
	double vY = nodeDataResult[node1].vY;
	double vZ = nodeDataResult[node1].vZ;
	nodeDataResult[node1].vX = vX + (fX/nodeData[node1].mass) * timeDelta;
	nodeDataResult[node1].vY = vY + (fY/nodeData[node1].mass) * timeDelta;
	nodeDataResult[node1].vZ = vZ + (fZ/nodeData[node1].mass) * timeDelta;
	return 0.0;
}

//calculate Fx Fy Fz by setting r to the distance between N1x and N2x etc.
double updatePositionForces(int node1, int node2){
	//Force node2 applies to node1
	calculateForce(node1, node2);
	double fX = forces[0];
	double fY = forces[1];
	double fZ = forces[2];

	//printf("forceX=%e, node%i\n", forces[0], node1);
	//printf("forceY=%e, node%i\n", forces[1], node1);
	//printf("forceZ=%e, node%i\n", forces[2], node1);
	double x = nodeDataResult[node1].x;
	double y = nodeDataResult[node1].y;
	double z = nodeDataResult[node1].z;

	nodeDataResult[node1].x = x + fX/nodeData[node1].mass * timeDelta * timeDelta *0.5;
	nodeDataResult[node1].y = y + fY/nodeData[node1].mass * timeDelta * timeDelta *0.5;
	nodeDataResult[node1].z = z + fZ/nodeData[node1].mass * timeDelta * timeDelta *0.5;
	//exit(1);


	updateNodeVelocity(node1, fX, fY, fZ);	

	//printf("Node %i Updated Positions: x=%e y=%e z=%e \n", node1, nodeDataResult[node1].x, nodeDataResult[node1].y, nodeDataResult[node1].z);
	return 0.0;
}


//update the parameters after calculations are done
double updateNodeParameters(){
	int a;
	for(a = 0; a < nodeIndexes; a++){
		nodeData[a].x = nodeDataResult[a].x;
		nodeData[a].y = nodeDataResult[a].y;
		nodeData[a].z = nodeDataResult[a].z;
		double vX = nodeData[a].vX;
		double vY = nodeData[a].vY; 
		double vZ = nodeData[a].vZ; 
		nodeData[a].vX = vX + nodeDataResult[a].vX;
		nodeData[a].vY = vY + nodeDataResult[a].vY;
		nodeData[a].vZ = vZ + nodeDataResult[a].vZ;
		//printf("----------Node %i Updated Positions: x=%e y=%e z=%e \n", a, nodeData[a].x, nodeData[a].y, nodeData[a].z);
	}
	return 0.0;
}

double printResults(){
	int a;
	for(a = 0; a < nodeIndexes; a++){
		printf("----------Node %i Updated Positions: x=%e y=%e z=%e \n", a, nodeData[a].x, nodeData[a].y, nodeData[a].z);
	}

	return 0.00;
}

double startSimulation(){

	int b;
	int c;

	while(simTime > currentSimTime){
		for(b = 0; b < nodeIndexes; b++){
			updatePositionVelocity(b);
			for(c = 0; c < nodeIndexes; c++){
				if(c != b){
					updatePositionForces(b, c);
				}
			}
		}
		updateNodeParameters();
		currentSimTime += timeDelta;
	}

	printResults();
	return 0.0;
}


int main(int argc, char *argv[]) {
	//simTime = atof(argv[0]);
	//fileName = argv[1];
	initializeNodes();
	startSimulation();
	return(0);
}


