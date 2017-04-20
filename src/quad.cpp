#include "Quad.h"

namespace Taranis
{
	Quad::Quad()
	{
		vertexPos =
		{
			{ -1.0f,  1.0f, 0.0f, },
			{ -1.0f, -1.0f, 0.0f, },
			{ 1.0f,  1.0f, 0.0f, },
			{ 1.0f, -1.0f, 0.0f, },
		};

		vertexTexCoord = {
			{ 0.0f, 1.0f, },
			{ 0.0f, 0.0f, },
			{ 1.0f, 1.0f, },
			{ 1.0f, 0.0f, },
		};

		Topology = TRIANGLE_STRIP;

		Generate();

	}

	Quad::Quad(float width, float height)
	{
		vertexPos =
		{
			{ -width,  height, 0.0f, },
			{ -width, -height, 0.0f, },
			{ width,  height, 0.0f, },
			{ width, -height, 0.0f, },
		};

		vertexTexCoord = {
			{ 0.0f, 1.0f, },
			{ 0.0f, 0.0f, },
			{ 1.0f, 1.0f, },
			{ 1.0f, 0.0f, },
		};

		Topology = TRIANGLE_STRIP;

		Generate();
	}
}
