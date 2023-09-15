#pragma once

class Voxel
{
public:
	enum TYPE
	{
		NOTHING = 0,
		RED, GREEN, BLUE
	};
	Voxel(Voxel::TYPE type = NOTHING, bool active = true);
	~Voxel();
	void activate();
	void deactivate();
	bool isActive();
	Voxel::TYPE type;
private:
	bool active;
};