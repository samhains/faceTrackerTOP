#version 150
//#extension GL_ARB_texture_rectangle : enable
uniform sampler2D dstBlur;
uniform sampler2DRect src, mask;
in vec2 texCoordVarying;
out vec4 outputColor;

void main() {
	vec2 pos = texCoordVarying;
	vec2 posUN = vec2(pos.x*1280, 720- pos.y*720);
	vec2 posNormalized = vec2(pos.x/1280.0, 1.0 - pos.y/720.0);
	vec4 maskTex = texture2DRect(mask, pos);
	vec4 srcTex = texture2DRect(src, pos);
	vec4 dstTex = texture2D(dstBlur, posNormalized);

	if(maskTex.r > 0.) {
		outputColor = srcTex;
	} else {
		outputColor = dstTex;
	}
};