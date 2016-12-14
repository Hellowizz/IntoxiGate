#version 330

in vec2 vTexCoords;

uniform sampler2D uTexture;

out vec3 fFragColor;

void main() {
    fFragColor = (texture(uTexture, vTexCoords)).xyz; //vec3(vTexCoords*2, 0);
}
