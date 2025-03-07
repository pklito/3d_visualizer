#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

in vec4 normal;
// Gets the Texture Unit from the main function
uniform sampler2D tex0;

uniform mat4 cameraTransform;
uniform vec4 color;

void main()
{
	vec4 textureColor = color;
	
	float intensity = dot(normal, vec4(0.,0.,1.,0.));
	FragColor = min(max(intensity,0.1),1.) * textureColor;
}