#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;

int vec2 texCoord;

uniform sampled2D tex0;

void main()
{
	FragColor = texture(tex0, texCoord);
}