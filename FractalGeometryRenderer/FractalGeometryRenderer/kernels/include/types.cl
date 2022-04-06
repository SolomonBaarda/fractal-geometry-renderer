#ifndef TYPES_CL
/// @cond DOXYGEN_DO_NOT_DOCUMENT
#define TYPES_CL
/// @endcond

/// <summary>
/// A struct representing a line in 3D space.
/// </summary>
typedef struct
{
	float3 position;
	float3 direction;
}
Ray;

/// <summary>
/// A struct representing a geometry material, for use with the Phong reflection model.
/// </summary>
typedef struct
{
	float3 ambient;
	float3 diffuse;
	float3 specular;
	float shininess;
}
Material;

/// <summary>
/// A struct representing a light, for use with the phong illumination model.
/// </summary>
typedef struct
{
	float3 position;
	float3 ambient;
	float3 diffuse;
	float3 specular;
}
Light;

#endif
