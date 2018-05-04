#include "Clone.h"

void Clone::setup(int width, int height, ofFbo::Settings settings, shared_ptr<ofGLProgrammableRenderer> _renderer) {
	renderer = _renderer;
	 
	buffer.allocate(settings);
	srcBlur.allocate(settings);
	dstBlur.allocate(settings);
	maskBlurShader.load("maskBlur/shader");
	maskBlurShader2.load("maskBlur2/shader");
	simpleMaskShader.load("simpleMask/shader");
	cloneShader.load("cloneShader/shader");
	
	strength = 0;
}

void Clone::simpleMask(ofTexture& tex, ofTexture& mask, ofTexture& dst) {
	
	buffer.begin();
	renderer->pushStyle();
	renderer->setBlendMode(OF_BLENDMODE_ALPHA);
	renderer->bind(simpleMaskShader);
	simpleMaskShader.setUniformTexture("src", tex, 1);
	simpleMaskShader.setUniformTexture("mask", mask, 2);
	simpleMaskShader.setUniformTexture("dstBlur", dst, 3);
	renderer->draw(tex, 0, 0, 0, 1280, 720, 0, 0, 1280, 720);
	renderer->unbind(simpleMaskShader);
	renderer->setBlendMode(OF_BLENDMODE_DISABLED);
	renderer->popStyle();
	buffer.end();
	
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

void Clone::maskedBlurNormalizedSrc(ofTexture& tex, ofTexture& mask, ofFbo& result) {
	int k = strength;
	
	buffer.begin();
	//renderer->draw(mask, 0, 0, 0, 1280, 720, 0, 0, 1280, 720);
	renderer->bind(maskBlurShader2);
	maskBlurShader2.setUniformTexture("tex", tex, 1);
	maskBlurShader2.setUniformTexture("mask", mask, 2);
	maskBlurShader2.setUniform2f("direction", 1, 0);
	maskBlurShader2.setUniform1i("k", k);
	renderer->draw(tex, 0, 0, 0, 1280, 720, 0, 0, 1280, 720);
	renderer->unbind(maskBlurShader2);
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
	//maskedBlur(src, mask, srcBlur);
	//maskedBlurNormalizedSrc(dst, mask, dstBlur);
	if (toggleBlur) {
		maskedBlur(src, mask, srcBlur);
		maskedBlurNormalizedSrc(dst, mask, dstBlur);
	}
	else {
		simpleMask(src, mask, dst);
	}


	buffer.begin();
	renderer->pushStyle();
	renderer->setBlendMode(OF_BLENDMODE_ALPHA);
	renderer->bind(cloneShader);
	cloneShader.setUniformTexture("src", src, 1);
	cloneShader.setUniformTexture("srcBlur", srcBlur, 2);
	cloneShader.setUniformTexture("dstBlur", dstBlur, 3);
	renderer->draw(src, 0, 0, 0, 1280, 720, 0, 0, 1280, 720);
	renderer->unbind(cloneShader);
	renderer->setBlendMode(OF_BLENDMODE_DISABLED);
	renderer->popStyle();
	buffer.end();
}

ofTexture Clone::getTexture() {
	return buffer.getTexture();
}
