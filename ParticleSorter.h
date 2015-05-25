#include "Particle.h"

class ParticleSorter {
public:
	bool operator()(const Particle *p0, const Particle *p1) const
	//bool operator()(const shared_ptr<Particle> p0, const shared_ptr<Particle> p1) const
	{
		// Particle positions in world space
		const glm::vec3 &x0 = p0->getPosition();
		const glm::vec3 &x1 = p1->getPosition();
		// Particle positions in camera space
		glm::vec4 x0w = C * glm::vec4(x0, 1.0f);
		glm::vec4 x1w = C * glm::vec4(x1, 1.0f);
		return x0w.z < x1w.z;
	}
	
	glm::mat4 C; // current camera matrix
};
