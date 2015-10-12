#include "Light3D.h"


namespace prop3
{
	Light3D::Light3D() :
		ambient( 0.2f, 0.2f, 0.2f, 1.0f),
		diffuse( 0.8f, 0.8f, 0.8f, 1.0f),
		specular(0.4f, 0.4f, 0.4f, 1.0f)
	{
	}

	DirectionnalLight3D::DirectionnalLight3D() :
		direction(0.0f, 0.0f, -1.0f, 0.0f)
	{
	}

	PointLight3D::PointLight3D() :
		position(0.0f, 0.0f, 0.0f, 1.0f),
        attenuationCoefs(1.0f, 0.0f, 0.0f, 0.0f)
	{
	}

	SpotLight3D::SpotLight3D() :
        direction(1, 0, 0, 0),
		cutoff(1.0f)
	{
	}
}
