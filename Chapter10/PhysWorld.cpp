// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "PhysWorld.h"
#include <algorithm>
#include "BoxComponent.h"
#include <SDL/SDL.h>

PhysWorld::PhysWorld(Game* game)
	:mGame(game)
{
}

bool PhysWorld::SegmentCast(const LineSegment& l, CollisionInfo& outColl)
{
	bool collided = false;
	// Initialize closestT to infinity, so first
	// intersection will always update closestT
	float closestT = Math::Infinity;
	Vector3 norm;
	// Test against all boxes
	for (auto box : mBoxes)
	{
		float t;
		// Does the segment intersect with the box?
		if (Intersect(l, box->GetWorldBox(), t, norm))
		{
			// Is this closer than previous intersection?
			if (t < closestT)
			{
				closestT = t;
				outColl.mPoint = l.PointOnSegment(t);
				outColl.mNormal = norm;
				outColl.mBox = box;
				outColl.mActor = box->GetOwner();
				collided = true;
			}
		}
	}
	return collided;
}

void PhysWorld::TestPairwise(std::function<void(Actor*, Actor*)> f)
{
	// Naive implementation O(n^2)
	for (size_t i = 0; i < mBoxes.size(); i++)
	{
		// Don't need to test vs itself and any previous i values
		for (size_t j = i + 1; j < mBoxes.size(); j++)
		{
			BoxComponent* a = mBoxes[i];
			BoxComponent* b = mBoxes[j];
			if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				// Call supplied function to handle intersection
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
}

void PhysWorld::TestSweepAndPrune(std::function<void(Actor*, Actor*)> f)
{
	std::vector<class BoxComponent*> mBoxesX(mBoxes);
	std::vector<class BoxComponent*> mBoxesY;
	std::vector<class BoxComponent*> mBoxesZ;

	std::sort(mBoxesX.begin(), mBoxesX.end(),
		[](BoxComponent* a, BoxComponent* b) {
			return a->GetWorldBox().mMin.x <
				b->GetWorldBox().mMin.x;
	});

	//X 축 Intersect 체크
	for (size_t i = 0; i < mBoxesX.size(); i++)
	{
		// Get max.x for current box
		BoxComponent* a = mBoxesX[i];
		float max = a->GetWorldBox().mMax.x;
		for (size_t j = i + 1; j < mBoxesX.size(); j++)
		{
			BoxComponent* b = mBoxesX[j];
			// If AABB[j] min is past the max bounds of AABB[i],
			// then there aren't any other possible intersections
			// against AABB[i]
			if (b->GetWorldBox().mMin.x > max)
			{
				break;
			}
			else if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				mBoxesY.push_back(b);
			}
		}
	}
	
	//Y 축 Intersect 체크
	for (size_t i = 0; i < mBoxesY.size(); i++)
	{
		// Get max.x for current box
		BoxComponent* a = mBoxesY[i];
		float max = a->GetWorldBox().mMax.y;
		for (size_t j = i + 1; j < mBoxesY.size(); j++)
		{
			BoxComponent* b = mBoxesY[j];
			// If AABB[j] min is past the max bounds of AABB[i],
			// then there aren't any other possible intersections
			// against AABB[i]
			if (b->GetWorldBox().mMin.y > max)
			{
				break;
			}
			else if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				mBoxesZ.push_back(b);
			}
		}
	}
	
	//Z 축 Intersect 체크
	for (size_t i = 0; i < mBoxesZ.size(); i++)
	{
		// Get max.x for current box
		BoxComponent* a = mBoxesZ[i];
		float max = a->GetWorldBox().mMax.z;
		for (size_t j = i + 1; j < mBoxesZ.size(); j++)
		{
			BoxComponent* b = mBoxesZ[j];
			// If AABB[j] min is past the max bounds of AABB[i],
			// then there aren't any other possible intersections
			// against AABB[i]
			if (b->GetWorldBox().mMin.z > max)
			{
				break;
			}
			else if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				//다 통과하면 충돌 처리.
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
}

void PhysWorld::AddBox(BoxComponent* box)
{
	mBoxes.emplace_back(box);
}

void PhysWorld::RemoveBox(BoxComponent* box)
{
	{
		auto iter = std::find(mBoxes.begin(), mBoxes.end(), box);
		if (iter != mBoxes.end())
		{
			// Swap to end of vector and pop off (avoid erase copies)
			std::iter_swap(iter, mBoxes.end() - 1);
			mBoxes.pop_back();
		}
	}
}
