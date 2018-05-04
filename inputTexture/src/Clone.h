#pragma once

#include "ofMain.h"

class Clone {
public:
	void setup(int width, int height, ofFbo::Settings settings, shared_ptr<ofGLProgrammableRenderer> _renderer);
	void setStrength(int strength);
	void update(ofTexture& src, ofTexture& dst, ofTexture& mask);
	ofTexture getTexture();
	ofTexture texture;
	ofTexture bufferTexture;
	shared_ptr<ofGLProgrammableRenderer> renderer;
protected:
	void maskedBlur(ofTexture& tex, ofTexture& mask, ofFbo& result);
	void simpleMask(ofTexture& tex, ofTexture& mask, ofTexture& dst);
	void maskedBlurNormalizedSrc(ofTexture& tex, ofTexture& mask, ofFbo& result);
	ofFbo buffer, srcBlur, dstBlur;
	ofShader maskBlurShader, maskBlurShader2, simpleMaskShader, cloneShader;
	int strength;
	bool toggleBlur = false;
};