#pragma once

#include "ofMain.h"
#include "TouchParms.h"

class Clone {
public:
	void setup(int width, int height, ofFbo::Settings settings, shared_ptr<ofGLProgrammableRenderer> _renderer);
	void setStrength(int strength);
	void update(ofTexture& src, ofTexture& dst, ofTexture& mask, TouchParms touchParms);
	void update(ofTexture& tex, ofTexture& mask);
	ofTexture getTexture();
	ofTexture texture;
	ofTexture bufferTexture;
	shared_ptr<ofGLProgrammableRenderer> renderer;
	void simpleMask(ofTexture& tex, ofTexture& mask, ofTexture& dst);
protected:
	void maskedBlur(ofTexture& tex, ofTexture& mask, ofFbo& result);
	void maskedBlurNormalizedSrc(ofTexture& tex, ofTexture& mask, ofFbo& result);
	ofFbo buffer, srcBlur, dstBlur;
	ofShader maskBlurShader, maskBlurShader2, simpleMaskShader, simpleMaskNoBgShader, cloneShader;
	int strength;
	bool toggleBlur = false;
};