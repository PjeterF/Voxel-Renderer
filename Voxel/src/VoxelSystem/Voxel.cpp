#include "Voxel.hpp"

Voxel::Voxel(Voxel::TYPE type, bool active)
{
	this->type = type;
	this->active = active;
}

Voxel::~Voxel()
{
}
