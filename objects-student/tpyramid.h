// =========================================================================================
//Computer Graphics, 2016
// 3D Engine Code
// =========================================================================================
//
//
// tpyramid.h
//
// This file declares the class for tpyramid objects, based on the
// generic 3D object


#ifndef _tpyramid_H
#define _tpyramid_H



// -----------------------------------------------------------------------------------------
// application includes
// -----------------------------------------------------------------------------------------
#include "face.h"
#include "object3d.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------

class tpyramid : public object3d
{
	public:
						tpyramid();
		virtual void		privateDraw(bool drawMode);
};


#endif // _tpyramid_H

