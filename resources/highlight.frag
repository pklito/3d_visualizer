#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform vec4 color;
uniform float time;

// Function to generate hash values
float hash(float n) {
	return fract(sin(n) * 43758.5453);
}

// Function to generate Perlin noise
float noise(vec3 p) {
	vec3 i = floor(p);
	vec3 f = fract(p);
	f = f * f * (3.0 - 2.0 * f);

	float n = dot(i, vec3(1.0, 57.0, 113.0));
	return mix(mix(mix( hash(n +  0.0), hash(n +  1.0), f.x),
					mix( hash(n + 57.0), hash(n + 58.0), f.x), f.y),
				mix(mix( hash(n +113.0), hash(n +114.0), f.x),
					mix( hash(n +170.0), hash(n +171.0), f.x), f.y), f.z);
}

// Function to generate Perlin marble texture
vec3 marble(vec3 p) {
	float n = noise(p * 5.0);
	float m = sin(p.z + n * 10.0);
	return vec3(1.0) * 0.5 + vec3(m) * 0.5;
}

void main()
{
	vec3 p = vec3(texCoord, 1.0);
	vec3 marbleColor = marble(p) + vec3(time);
	if (marbleColor.r < 0.8) {
		discard;
	}
	FragColor = vec4(marbleColor, marbleColor.r);
}