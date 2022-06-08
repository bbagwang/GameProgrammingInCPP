// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "SpotLightComponent.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "Actor.h"

SpotLightComponent::SpotLightComponent(Actor* owner)
	:Component(owner)
{
	owner->GetGame()->GetRenderer()->AddSpotLight(this);
}

SpotLightComponent::~SpotLightComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveSpotLight(this);
}

void SpotLightComponent::Draw(Shader* shader, Mesh* mesh)
{
	// We assume, coming into this function, that the shader is active
	// and the sphere mesh is active

	// World transform is scaled to the outer radius (divided by the mesh radius)
	// and positioned to the world position
	Matrix4 scale = Matrix4::CreateScale(mOwner->GetScale() *
		mOuterRadius / mesh->GetRadius());
	Matrix4 rotation = Matrix4::CreateFromQuaternion(mRotation);
	Matrix4 trans = Matrix4::CreateTranslation(mOwner->GetPosition());
	Matrix4 worldTransform = scale * rotation * trans;
	shader->SetMatrixUniform("uWorldTransform", worldTransform);
	// Set point light shader constants
	shader->SetVectorUniform("uSpotLight.mWorldPos", mOwner->GetPosition());
	shader->SetVectorUniform("uSpotLight.mDirection", mOwner->GetRight());
	shader->SetVectorUniform("uSpotLight.mDiffuseColor", mDiffuseColor);
	shader->SetVectorUniform("uSpotLight.mSpecularColor", mSpecColor);
	shader->SetFloatUniform("uSpotLight.mInnerRadius", mInnerRadius);
	shader->SetFloatUniform("uSpotLight.mOuterRadius", mOuterRadius);

	// Draw the sphere
	glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->GetNumIndices(), 
		GL_UNSIGNED_INT, nullptr);
}
