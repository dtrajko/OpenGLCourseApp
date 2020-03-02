#version 330

layout (location = 0) in vec2 aPosition;

out vec4 clipSpace;
out vec2 textureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float tiling = 2.0;

void main()
{
	clipSpace = projection * view * model * vec4(aPosition.x, 0.0, aPosition.y, 1.0);
	gl_Position = clipSpace;
	textureCoords = vec2(aPosition.x / 2.0 + 0.5, aPosition.y / 2.0 + 0.5) * tiling;
}