#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//-----------------------------------------------
//Node Structure------------------------------
//-----------------------------------------------



struct Node {
	double mass;
	double x;
	double y;
	double z;
	double vX;
	double vY;
	double vZ;
};




//-----------------------------------------------
//Global Variables-----------------------------
//-----------------------------------------------



double timeDelta = 0.05;
double simTime = 50000;
int    nodeIndexes;
struct Node nodeData[28];
struct Node tempNodeData[28];
char *fileName = "nbody_initial_new.txt";
double forces[3];
double G = 6.67408E-11;
double x, y, z, mass, vX, vY, vZ;




//-----------------------------------------------
//Functions--------------------------------------
//-----------------------------------------------



//Read planet body parameters from a text file
double initializeNodes(){
	printf("Opening File \n");
	//point to file for reading
	FILE * file = fopen(fileName, "r");

	//skip first line
	char buffer[100];
	fgets(buffer, 100, file);

	int a = 0;
	//Scan file until end 
	while (fscanf(file, "%lf %lf %lf %lf %lf %lf %lf", &x, &y, &z, &mass, &vX, &vY, &vZ) > 0){ 
		//Set each respective parameter from file
		nodeData[a].x = x;
		nodeData[a].y = y;
		nodeData[a].z = z;
		nodeData[a].mass = mass;
		nodeData[a].vX = vX;
		nodeData[a].vY = vY;
		nodeData[a].vZ = vZ;
		printf("Node %i initilized: x = %e, y = %e, z = %e, mass = %e, vX = %e, vY = %e, vZ = %e \n", a, nodeData[a].x, nodeData[a].y, nodeData[a].z, nodeData[a].mass, nodeData[a].vX, nodeData[a].vY, nodeData[a].vZ);
		a++;
		nodeIndexes++; 
	}
	return 0.0;
}


//update all parameters on the node
double updateNode(){
	for(int a = 0; a < nodeIndexes; a++){
		//printf(" %i X = %e \n", a, nodeData[a].x);
		nodeData[a].x += (nodeData[a].vX * timeDelta) + tempNodeData[a].x;
		//printf(" %i X' = %e \n", a, nodeData[a].x);
		//printf(" %i Y = %e \n", a, nodeData[a].y);
		nodeData[a].y += (nodeData[a].vY * timeDelta) + tempNodeData[a].y;
		//printf(" %i Y' = %e \n", a, nodeData[a].y);
		//printf(" %i Z = %e \n", a, nodeData[a].z);
		nodeData[a].z += (nodeData[a].vZ * timeDelta) + tempNodeData[a].z;
		//printf(" %i Z' = %e \n", a, nodeData[a].z);
		nodeData[a].vX = nodeData[a].vX + tempNodeData[a].vX;
		nodeData[a].vY = nodeData[a].vY + tempNodeData[a].vY;
		nodeData[a].vZ = nodeData[a].vZ + tempNodeData[a].vZ;
	}
	//reset temporary node array to 0
	for(int a = 0; a < nodeIndexes; a++){
		tempNodeData[a].x = 0.00;
		tempNodeData[a].y  = 0.00;
		tempNodeData[a].z  = 0.00;
		tempNodeData[a].vX = 0.00;
		tempNodeData[a].vY = 0.00;
		tempNodeData[a].vZ = 0.00;
	}
	return 0.00;
}



double getForce(int b, int c, double radius){
	double force = (G * nodeData[b].mass * nodeData[c].mass) / (radius * radius);
	return force;
}



double updateVelocity(int b, double fx, double fy, double fz){
	tempNodeData[b].vX += fx/nodeData[b].mass * timeDelta;
	tempNodeData[b].vY += fy/nodeData[b].mass * timeDelta;
	tempNodeData[b].vZ += fz/nodeData[b].mass * timeDelta;
	return 0.00;
}



double calculateForce(int b, int c){
	double distance = sqrt(pow((nodeData[c].x - nodeData[b].x),2.00) + pow((nodeData[c].y - nodeData[b].y),2.00) + pow((nodeData[c].z - nodeData[b].z),2.00));
	if (distance < 1000.00){
		return 0.00;
	}else{
		double force = getForce(b,c,distance);
		double directionVectorX = (nodeData[c].x - nodeData[b].x) / distance;
		double directionVectorY = (nodeData[c].y - nodeData[b].y) / distance;
		double directionVectorZ = (nodeData[c].z - nodeData[b].z) / distance;
		double fx = directionVectorX * force;
		double fy = directionVectorY * force;
		double fz = directionVectorZ * force;
		tempNodeData[b].x += 0.5 * fx/nodeData[b].mass * timeDelta * timeDelta;
		tempNodeData[b].y += 0.5 * fy/nodeData[b].mass * timeDelta * timeDelta;
		tempNodeData[b].z += 0.5 * fz/nodeData[b].mass * timeDelta * timeDelta;
		updateVelocity(b, fx, fy, fz);
		return 0.00;
	}
}

double printResults(){
	for(int a = 0; a < nodeIndexes; a++){
		printf("----------Node %i Updated Positions: x=%e y=%e z=%e \n", a, nodeData[a].x, nodeData[a].y, nodeData[a].z);
	}
	return 0.00;
}



double startSimulation(){
	double time = 0.00;
	while(time < simTime){
		for(int b = 0; b < nodeIndexes; b++){
			for(int c = 0; c < nodeIndexes; c++){
				if(c != b){
					calculateForce(b, c);
				}
			}
		}
		updateNode();
		time += timeDelta;
	}
	printResults();
	return 0.0;
}



int main(int argc, char *argv[]) {
	simTime = atof(argv[1]);
	fileName = argv[2];
	initializeNodes();
	startSimulation();
	return(0);
}


