#version 150
//#extension GL_ARB_texture_rectangle : enable
uniform sampler2D tex;
uniform vec2 direction;
in vec2 texCoordVarying;
out vec4 outputColor;

uniform int k;
void main() {
	vec2 pos = texCoordVarying;
	vec2 posNormalized = vec2(pos.x, pos.y);
	vec4 sum = texture2D(tex, posNormalized);
	outputColor = sum;
}
