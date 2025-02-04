#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 vPosition;
// Colors
layout (location = 1) in vec3 vNormal;
// Texture Coordinates
layout (location = 2) in vec2 vTexture;


// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

out vec4 normal;

// Imports the camera matrix from the main function
uniform mat4 cameraTransform;
uniform mat4 modelTransform;
uniform mat4 normalTransform;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = cameraTransform * (modelTransform * vec4(vPosition, 1.0));

	normal = normalize((normalTransform * vec4(vNormal,1.0)));
	// Assigns the colors from the Vertex Data to "color"
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = vTexture;
}