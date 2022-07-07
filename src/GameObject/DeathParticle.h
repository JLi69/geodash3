#ifndef INCLUDE_GAMEOBJECT
#include "GameObject.h"
#endif

namespace Geodash3
{
	class DeathParticle : public GameObject
	{	
	public:
		float distTravelled = 0.0f;	

		//Update the particle
		void Update(float secToDrawFrame); 
	
		//Constructor
		DeathParticle(glm::vec3 pos, glm::vec3 mov);
	};
}
