#pragma once

class Voxel
{
	enum TYPE
	{
		NOTHING = 0,
		RED, GREEN, BLUE
	};
public:
	Voxel(Voxel::TYPE type = NOTHING, bool active = true);
	~Voxel();
	void activate();
	void deactivate();
	bool isActive();
private:
	bool active;
	Voxel::TYPE type;
};