#version 440

in vec3 tNorms;
in vec3 tPos;

out vec4 fragColor;

struct Lighting
{
	vec3 color;
	vec3 position;
	vec3 camera;
	float ambient;
	float diffuse;
	float specular;
};


uniform Lighting sunlight;


void main()
{
	vec3 color = vec3(0.0f, 1.0f, 0.0f);

	color = color * sunlight.color;
	vec3 amb = color * sunlight.ambient;
	vec3 dif = color * sunlight.diffuse * dot(normalize(sunlight.position-tPos), tNorms);
	vec3 spec = color * sunlight.specular * dot( normalize(-reflect(sunlight.position-tPos, tNorms)), normalize(sunlight.camera-tPos) );

	vec3 total = clamp(amb + dif + spec, 0.0, 1.0);
	fragColor = vec4(total, 1.0f);
};

