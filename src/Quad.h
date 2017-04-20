#pragma once

#include "Mesh.h"
#include <glm\glm.hpp>

namespace Taranis
{
	class Quad : public Mesh
	{
	public:

		// Default 1x1

		Quad();
		Quad(float width, float height);

	};
}