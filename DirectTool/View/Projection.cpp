#include "stdafx.h"
#include "Projection.h"



Projection::Projection()
{
	this->viewProjectionBuffer = Buffers->FindShaderBuffer<ViewProjectionBuffer>();
}


Projection::~Projection()
{
}
