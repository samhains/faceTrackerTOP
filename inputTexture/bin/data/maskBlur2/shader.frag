#version 150
uniform sampler2D tex;
uniform sampler2DRect mask;
uniform vec2 direction;
in vec2 texCoordVarying;
out vec4 outputColor;

uniform int k;
void main() {
	vec2 pos = texCoordVarying;
	vec2 posUN = vec2(pos.x*1280, 720- pos.y*720);
	vec4 sum = texture2D(tex, pos);
	int i;
	for(i = 1; i < k; i++) {
		vec2 curOffset = float(i) * direction;
		vec2 curOffsetNormalized = float(i) * vec2(0.0, .0008);
		vec4 leftMask = texture2DRect(mask, posUN - curOffset);
		vec4 rightMask = texture2DRect(mask, posUN + curOffset);
		bool valid = leftMask.r == 1. && rightMask.r == 1.;
		if(valid) {
			sum +=
				texture2D(tex, pos + curOffsetNormalized) +
				texture2D(tex, pos - curOffsetNormalized);
		} else {
			break;
		}
	}
	int samples = 1 + (i - 1) * 2;
	outputColor = sum / float(samples);
}