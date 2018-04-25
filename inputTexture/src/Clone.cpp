#include "Clone.h"

char maskBlurShaderSource[] =
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect tex, mask;\
uniform vec2 direction;\
uniform int k;\
void main() {\
	vec2 pos = gl_TexCoord[0].st;\
	vec4 sum = texture2DRect(tex, pos);\
	int i;\
	for(i = 1; i < k; i++) {\
		vec2 curOffset = float(i) * direction;\
		vec4 leftMask = texture2DRect(mask, pos - curOffset);\
		vec4 rightMask = texture2DRect(mask, pos + curOffset);\
		bool valid = leftMask.r == 1. && rightMask.r == 1.;\
		if(valid) {\
			sum +=\
				texture2DRect(tex, pos + curOffset) +\
				texture2DRect(tex, pos - curOffset);\
		} else {\
			break;\
		}\
	}\
	int samples = 1 + (i - 1) * 2;\
	gl_FragColor = sum / float(samples);\
}";

char cloneShaderSource[] = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect src, srcBlur, dstBlur;\
void main() {\
	vec2 pos = gl_TexCoord[0].st;	\
	vec4 srcColorBlur = texture2DRect(srcBlur, pos);\
	if(srcColorBlur.a > 0.) {\
		vec3 srcColor = texture2DRect(src, pos).rgb;\
		vec4 dstColorBlur = texture2DRect(dstBlur, pos);\
		vec3 offset = (dstColorBlur.rgb - srcColorBlur.rgb);\
		gl_FragColor = vec4(srcColor + offset, 1.);\
	} else {\
		gl_FragColor = vec4(0.);\
	}\
}";

void Clone::setup(int width, int height, ofFbo::Settings settings, shared_ptr<ofGLProgrammableRenderer> _renderer) {
	renderer = _renderer;
	
	buffer.allocate(settings);
	srcBlur.allocate(settings);
	dstBlur.allocate(settings);
	
	// this might be a problem
	//solution - shader.load("shadersGL2/shader");
	//maskBlurShader.load(maskBlurShaderSource);
	//cloneShader.load(cloneShaderSource);
	maskBlurShader.setupShaderFromSource(GL_FRAGMENT_SHADER, maskBlurShaderSource);
	cloneShader.setupShaderFromSource(GL_FRAGMENT_SHADER, cloneShaderSource);
	maskBlurShader.linkProgram();
	cloneShader.linkProgram();
	
	strength = 0;
}

void Clone::maskedBlur(ofTexture& tex, ofTexture& mask, ofFbo& result) {
	int k = strength;
	
	buffer.begin();
	renderer->bind(maskBlurShader);
	maskBlurShader.setUniformTexture("tex", tex, 1);
	maskBlurShader.setUniformTexture("mask", mask, 2);
	maskBlurShader.setUniform2f("direction", 1, 0);
	maskBlurShader.setUniform1i("k", k);
	renderer->draw(tex, 0, 0, 0, 1280, 720, 0, 0, 1280, 720);
	renderer->unbind(maskBlurShader);
	buffer.end();
	
	result.begin();
	renderer->bind(maskBlurShader);
	maskBlurShader.setUniformTexture("tex", buffer, 1);
	maskBlurShader.setUniformTexture("mask", mask, 2);
	maskBlurShader.setUniform2f("direction", 0, 1);
	maskBlurShader.setUniform1i("k", k);
	bufferTexture = buffer.getTexture();
	renderer->draw(bufferTexture, 0, 0, 0, 1280, 720, 0, 0, 1280, 720);
	renderer->unbind(maskBlurShader);
	result.end();
}

void Clone::setStrength(int strength) {
	this->strength = strength;
}

void Clone::update(ofTexture& src, ofTexture& dst, ofTexture& mask) {
	maskedBlur(src, mask, srcBlur);
	maskedBlur(dst, mask, dstBlur);
	
	buffer.begin();
	renderer->pushStyle();
	renderer->setBlendMode(OF_BLENDMODE_ALPHA);
	renderer->draw(dst, 0, 0, 0, 1280, 720, 0, 0, 1280, 720);
	renderer->bind(cloneShader);
		cloneShader.setUniformTexture("src", src, 1);
		cloneShader.setUniformTexture("srcBlur", srcBlur, 2);
		cloneShader.setUniformTexture("dstBlur", dstBlur, 3);
		renderer->draw(dst, 0, 0, 0, 1280, 720, 0, 0, 1280, 720);
	renderer->unbind(cloneShader);
	renderer->setBlendMode(OF_BLENDMODE_DISABLED);
	renderer->popStyle();
	buffer.end();
}

ofTexture Clone::getTexture() {
	return buffer.getTexture();
}
