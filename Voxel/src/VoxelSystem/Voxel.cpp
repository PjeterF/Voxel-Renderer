#include "Voxel.hpp"

Voxel::Voxel(Voxel::TYPE type, bool active)
{
	this->type = type;
	this->active = active;
}

Voxel::~Voxel()
{
}

void Voxel::activate()
{
	active = true;
}

void Voxel::deactivate()
{
	active = false;
}

bool Voxel::isActive()
{
	return active;
}
