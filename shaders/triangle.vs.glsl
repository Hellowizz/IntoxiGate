#version 330

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoords;

uniform mat4 uMVPMatrix;

out vec2 vTexCoords;

void main() {
	vTexCoords = aTexCoords;
    gl_Position = uMVPMatrix*vec4(aPosition, 0, 1);
}
