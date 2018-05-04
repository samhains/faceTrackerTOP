#pragma once
#include "ofMain.h"
#include "ofxCv.h"
#include "Clone.h"
#include "Coord.h"
#include "TargetMesh.h"
#include "ofxFaceTracker2.h"

class MultiFaceTracker {
public:
	ofTexture texture;
	ofTexture MultiFaceTracker::getTexture();
	void setup(ofFbo::Settings _settings, shared_ptr<ofGLProgrammableRenderer> _renderer, ofTexture faceTexture);
	void update(ofTexture bgTexture);
	void stop();
private:
	void loadFace(ofTexture faceTexture);
	bool faceDetected = false;
	ofImage bgImg;
	ofFbo::Settings settings;
	ofPixels pixels;
	ofTexture tex2;
	ofVideoPlayer targetVideoPlayer;
	ofxFaceTracker2 srcTracker;
	ofxFaceTracker2 targetTracker;
	ofTexture faceTex;
	TargetMesh targetMesh;
	vector<ofVec2f> srcPoints;
	shared_ptr<ofGLProgrammableRenderer> renderer;
	Clone clone;
	ofFbo srcFbo, maskFbo;

	ofDirectory faces;
};