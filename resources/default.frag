#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;


void main()
{
	vec4 textureColor = texture(tex0, texCoord);
	vec4 appliedColor = vec4(textureColor.r * color.r, textureColor.g * color.g, textureColor.b * color.b, 1);
	FragColor = appliedColor;
}