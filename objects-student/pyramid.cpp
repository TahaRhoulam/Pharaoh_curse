// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2018
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: HXP
//
// pyramid.cpp
//
// This file defines the class for pyramid objects



// -----------------------------------------------------------------------------------------
// application includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "pyramid.h"
#include "utility.h"




// -----------------------------------------------------------------------------------------
// globals
// -----------------------------------------------------------------------------------------
extern vector<object3d*> gShapeVector;



// -----------------------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------------------

pyramid::pyramid()
{
	setName("pyramid composite");
	
	// Initialise the object's state
	setColour(0.15, .85, .35, 1.0);					// default colour

	vertexCount = 5;
	faceCount = 5;

	// allocate memory for the vertex and face arrays
	vertices.resize(vertexCount);
	faces.resize(faceCount);


	// Initialise the pyramid's vertices to create a pyramid centered around the origin
	vertices[0].set( 0,  1,  0);				// apex
	vertices[1].set(-1, -1,  1);				// front, top, right
	vertices[2].set( 1, -1,  1);				// front, bot, right
	vertices[3].set( 1, -1, -1);				// front, bot, left
	vertices[4].set(-1, -1, -1);				// back,  top, left


	// now set up the faces - note that the vertex order is always
	// specified counter-clockwise when that face is viewed front on
	faces[0].init(0, 1, 2);
	faces[1].init(0, 2, 3);
	faces[2].init(0, 3, 4);
	faces[3].init(0, 4, 1);
	faces[4].init(1, 4, 3, 2);
	
	// calculate the face normals for flat lighting
	calculateNormals();
	
	
	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);

}



