#version 150
uniform sampler2D mask;
uniform sampler2D tex;
uniform vec2 direction;
in vec2 texCoordVarying;
out vec4 outputColor;

uniform int k;
void main() {
	vec2 pos = texCoordVarying;
	vec2 blurPos = vec2(pos.x, 1.0 - pos.y);
	vec4 sum = texture2D(tex, pos);
	int i;
	for(i = 1; i < k; i++) {
		vec2 curOffset = float(i) * vec2(0.0, .0008);
		vec2 curOffsetNormalized = float(i) * direction;
		vec4 leftMask = texture2D(mask, blurPos - curOffset);
		vec4 rightMask = texture2D(mask, blurPos + curOffset);
//		vec4 leftMask = texture2D(mask, blurPos - curOffsetNormalized);
//		vec4 rightMask = texture2D(mask, blurPos + curOffsetNormalized);
//		vec4 leftMask = texture2D(mask, pos);
//		vec4 rightMask = texture2D(mask, pos);
		bool valid = leftMask.r == 1. && rightMask.r == 1.;
//		if(leftMask.r>0.5){
//			outputColor = vec4(1.0);
//		} else {
//			outputColor = vec4(0.0, vec3(1.0));
//		}
//
		if(valid) {
			sum +=
				texture2D(tex, pos + curOffset) +
				texture2D(tex, pos - curOffset);
		} else {
			break;
		}
	}
////	}
	int samples = 1 + (i - 1) * 2;
	outputColor = sum;
}