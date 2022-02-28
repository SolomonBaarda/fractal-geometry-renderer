#ifndef TYPES_CL
/// @cond DOXYGEN_DO_NOT_DOCUMENT
#define TYPES_CL
/// @endcond



/// <summary>
/// A struct containing a position and normalised direction vector.
/// </summary>
typedef struct
{
	float3 position;
	float3 direction;
}
Ray;


/// <summary>
/// 
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
/// 
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

