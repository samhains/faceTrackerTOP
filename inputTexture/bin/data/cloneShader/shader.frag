#version 150
uniform sampler2D src, srcBlur, dstBlur;
in vec2 texCoordVarying;
out vec4 outputColor;

void main() {
	vec2 pos = texCoordVarying;
	vec4 srcColorBlur = texture2D(srcBlur, pos);
	if(srcColorBlur.a > 0.) {
		vec3 srcColor = texture2D(src, pos).rgb;
		vec4 dstColorBlur = texture2D(dstBlur, pos);
		vec3 offset = (dstColorBlur.rgb - srcColorBlur.rgb);
		outputColor = vec4(srcColor + offset, 1.);
	} else {
		outputColor = vec4(0.);
	}
};
