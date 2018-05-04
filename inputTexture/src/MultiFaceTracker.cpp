#include "MultiFaceTracker.h"

using namespace ofxCv;


void MultiFaceTracker::setup(ofFbo::Settings _settings, shared_ptr<ofGLProgrammableRenderer> _renderer, ofTexture faceTexture) {
	settings = _settings;
	renderer = _renderer;
	//ofSetVerticalSync(true);
	clone.setup(1280, 720, settings, renderer);
	targetTracker.setup();
	srcTracker.setup();
	maskFbo.allocate(settings);
	srcFbo.allocate(settings);
	loadFace(faceTexture);

}
void MultiFaceTracker::update(ofTexture bgTexture) {;
if (faceDetected) {

	ofPixels pixels;
	bgTexture.readToPixels(pixels);
	pixels.mirror(true, false);

	targetTracker.update(toCv(pixels));
	//targetTracker.update(toCv(targetVideoPlayer));

	vector<ofxFaceTracker2Instance> instances = targetTracker.getInstances();
	vector<vector<ofVec2f>> targetPointsArr(instances.size());
	ofDisableArbTex();
	maskFbo.begin();
	renderer->clear(0, 255);
	for (int i = 0; i < instances.size(); i++) {
		ofxFaceTracker2Instance camTarget = instances[i];
		std::vector<ofVec2f> targetPoints = camTarget.getLandmarks().getImagePoints();
		targetPointsArr[i] = targetPoints;
		targetMesh.update_vertices(targetPointsArr[i]);
		targetMesh.update_uvs(srcPoints);
		renderer->draw(targetMesh, OF_MESH_FILL);
	}
	maskFbo.end();

	srcFbo.begin();
	renderer->clear(0, 255);

	for (int i = 0; i < instances.size(); i++) {
		renderer->bind(faceTex, 0);
		ofxFaceTracker2Instance camTarget = instances[i];
		std::vector<ofVec2f> targetPoints = camTarget.getLandmarks().getImagePoints();
		targetPointsArr[i] = targetPoints;
		targetMesh.update_vertices(targetPointsArr[i]);
		targetMesh.update_uvs(srcPoints);
		renderer->draw(targetMesh, OF_MESH_FILL);
		renderer->unbind(faceTex, 0);
	}
	srcFbo.end();
	clone.setStrength(16);
	clone.update(srcFbo.getTextureReference(), bgTexture, maskFbo.getTextureReference());
	texture = clone.getTexture();
}
else {

	texture = bgTexture;
}

}


ofTexture MultiFaceTracker::getTexture() {
	return texture;
}
void MultiFaceTracker::stop() {
	srcTracker.stop();
	targetTracker.stop();
}

void MultiFaceTracker::loadFace(ofTexture faceTexture) {
	// how to load toCv compatible from texture instead of string

	ofPixels facePixels;
	faceTexture.readToPixels(facePixels);
	facePixels.mirror(true, false);
	ofImage image;
	image.setFromPixels(facePixels);
	faceTex = image.getTextureReference();


	if (facePixels.getWidth() > 0) {
		srcTracker.update(toCv(facePixels));
		Sleep(1000);
		srcTracker.update(toCv(facePixels));

		vector<ofxFaceTracker2Instance>  instances = srcTracker.getInstances();
		ofLog(OF_LOG_NOTICE, ofToString(instances.size()));

		if (instances.size() > 0) {
			ofxFaceTracker2Instance instance = instances[0];
			srcPoints = instance.getLandmarks().getImagePoints();
			targetMesh.update_uvs(srcPoints);
			faceDetected = true;
		}
		else {
			std::cout << "NOT ALLOCATEED\n";

		}
	}
}
