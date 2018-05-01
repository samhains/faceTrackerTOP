#version 150
#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect mask;
uniform sampler2D tex;
uniform vec2 direction;
in vec2 texCoordVarying;
out vec4 outputColor;

uniform int k;
void main() {
	vec2 pos = texCoordVarying;
	vec4 sum = texture2D(tex, pos);
	int i;
	for(i = 1; i < k; i++) {
		vec2 curOffset = float(i) * direction;
		vec4 leftMask = texture2DRect(mask, pos - curOffset);
		vec4 rightMask = texture2DRect(mask, pos + curOffset);
		bool valid = leftMask.r == 1. && rightMask.r == 1.;
		if(leftMask.r > 0) {
			outputColor = vec4(1.0); 
//			sum +=
//				texture2D(tex, pos + curOffset) +
//				texture2D(tex, pos - curOffset);
		} else {
			outputColor = vec4(0.0, vec3(1.0)); 
			break;
		}
	}
	int samples = 1 + (i - 1) * 2;
//	outputColor = sum/ float(samples) ;
//	outputColor = vec4(1.0, 0.0, 0.0, 1.0);
}
