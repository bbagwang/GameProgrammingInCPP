#include "ArrowTarget.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Game.h"
#include "TargetComponent.h"

ArrowTarget::ArrowTarget(class Game* game) : Actor(game)
{
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Sphere.gpmesh");
	mc->SetMesh(mesh);
	new TargetComponent(this);
}
