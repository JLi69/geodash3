#include "DeathParticle.h"

void Geodash3::DeathParticle::Update(float secToDrawFrame)
{
	position += movement * secToDrawFrame;
	float mag = sqrtf(movement.x * movement.x + movement.y * movement.y + movement.z * movement.z);	
	distTravelled += mag * secToDrawFrame;
}

Geodash3::DeathParticle::DeathParticle(glm::vec3 pos, glm::vec3 mov)
{
	position = pos;
	movement = mov;
	dimensions = glm::vec3(0.04f);
}
