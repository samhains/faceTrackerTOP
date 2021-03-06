#pragma once
#include "ofMain.h"
#include "ofxCv.h"
#include "Clone.h"
#include "TargetMesh.h"
#include "ofxFaceTracker2.h"
#include "TouchParms.h"

class MultiFaceTracker {
public:
	ofTexture texture;
	ofTexture MultiFaceTracker::getTexture();
	void setup(ofFbo::Settings _settings, shared_ptr<ofGLProgrammableRenderer> _renderer, ofTexture _faceTexture);
	void update(ofTexture bgTexture, TouchParms touchParms, ofTexture _faceTexture);
	void stop();
	ofTexture faceTexture;
private:
	void loadFace();
	bool faceDetected;
	ofImage bgImg;
	ofImage faceAlphaImg;
	ofFbo::Settings settings;
	ofPixels pixels;
	ofVideoPlayer targetVideoPlayer;
	ofxFaceTracker2 srcTracker;
	ofxFaceTracker2 targetTracker;
	TargetMesh targetMesh;
	vector<ofVec2f> srcPoints;
	shared_ptr<ofGLProgrammableRenderer> renderer;
	Clone clone;
	ofFbo srcFbo, maskFbo, simpleFbo;

	ofDirectory faces;
};