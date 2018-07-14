#version 150
#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect src, srcBlur, dstBlur;
in vec2 texCoordVarying;
out vec4 outputColor;

void main() {
	vec2 pos = texCoordVarying;
	vec4 srcColorBlur = texture2DRect(srcBlur, pos);
	if(srcColorBlur.a > 0.) {
		vec3 srcColor = texture2DRect(src, pos).rgb;
		vec4 dstColorBlur = texture2DRect(dstBlur, pos);
		vec3 offset = (dstColorBlur.rgb - srcColorBlur.rgb);
		outputColor = vec4(srcColor + offset, 1.);
	} else {
		outputColor = vec4(0.);
	}
};