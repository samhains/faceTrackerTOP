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
	void setup(ofFbo::Settings _settings, shared_ptr<ofGLProgrammableRenderer> _renderer);
	void update();
private:
	void loadFace(string face);
	ofFbo::Settings settings;
	ofVideoPlayer targetVideoPlayer;
	ofxFaceTracker2 srcTracker;
	ofxFaceTracker2 targetTracker;
	ofImage src;
	TargetMesh targetMesh;
	vector<ofVec2f> srcPoints;
	shared_ptr<ofGLProgrammableRenderer> renderer;
	Clone clone;
	ofFbo srcFbo, maskFbo;

	ofDirectory faces;
};