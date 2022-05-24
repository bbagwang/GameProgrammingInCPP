// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "UIScreen.h"
#include <vector>

struct RadarBlipData
{
public:
	RadarBlipData():Position2D(),bIsUpper(false){}
	RadarBlipData(const Vector2& InPosition2D, bool bInIsUpper)
		: Position2D(InPosition2D)
		, bIsUpper(bInIsUpper){}
	
public:
	Vector2 Position2D;
	bool bIsUpper;
};

class HUD : public UIScreen
{
public:
	// (Lower draw order corresponds with further back)
	HUD(class Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;
	
	void AddTargetComponent(class TargetComponent* tc);
	void RemoveTargetComponent(class TargetComponent* tc);
protected:
	void UpdateCrosshair(float deltaTime);
	void UpdateRadar(float deltaTime);
	void UpdateArrow(float deltaTime);
	
	class Texture* mHealthBar;
	class Texture* mRadar;
	class Texture* mCrosshair;
	class Texture* mCrosshairEnemy;
	class Texture* mBlipTex;
	class Texture* mBlipUpTex;
	class Texture* mBlipDownTex;
	class Texture* mRadarArrow;
	class Texture* mArrow;
	
	// All the target components in the game
	std::vector<class TargetComponent*> mTargetComps;
	// 2D offsets of blips relative to radar
	std::vector<RadarBlipData> mBlips;
	// Adjust range of radar and radius
	float mRadarRange;
	float mRadarRadius;
	// Whether the crosshair targets an enemy
	bool mTargetEnemy;

	float mArrowTargetRotation;
};
