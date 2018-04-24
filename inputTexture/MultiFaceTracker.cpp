#include "MultiFaceTracker.h"

using namespace ofxCv;


void MultiFaceTracker::setup(ofFbo::Settings _settings, shared_ptr<ofGLProgrammableRenderer> _renderer) {
	settings = _settings;
	renderer = _renderer;
	ofSetVerticalSync(true);
	clone.setup(1280, 720, settings, renderer);
	targetVideoPlayer.load("movies/cut.mp4");
	targetVideoPlayer.play();
	targetVideoPlayer.setVolume(0);

	targetTracker.setup();
	srcTracker.setup();
	maskFbo.allocate(settings);
	srcFbo.allocate(settings);

	faces.allowExt("jpg");
	faces.allowExt("png");
	faces.listDir("faces");
	if (faces.size() != 0) {
		loadFace(faces.getPath(0));
	}
}
void MultiFaceTracker::update() {
	targetVideoPlayer.update();

	if (targetVideoPlayer.isFrameNew()) {
		targetTracker.update(toCv(targetVideoPlayer));

		vector<ofxFaceTracker2Instance> instances = targetTracker.getInstances();
		vector<vector<ofVec2f>> targetPointsArr(instances.size());
		maskFbo.begin();
		ofClear(0, 255);
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
		ofClear(0, 255);

		for (int i = 0; i < instances.size(); i++) {
			src.bind();
			ofxFaceTracker2Instance camTarget = instances[i];
			std::vector<ofVec2f> targetPoints = camTarget.getLandmarks().getImagePoints();
			targetPointsArr[i] = targetPoints;
			targetMesh.update_vertices(targetPointsArr[i]);
			targetMesh.update_uvs(srcPoints);
			renderer->draw(targetMesh,OF_MESH_FILL);
			src.unbind();
		}
		srcFbo.end();




		clone.setStrength(16);
		clone.update(srcFbo.getTextureReference(), targetVideoPlayer.getTextureReference(), maskFbo.getTextureReference());

	}
}

void MultiFaceTracker::draw() {
	ofSetColor(255);

	if (src.getWidth() > 0) {
		texture = clone.draw(0, 0);
	}
	else {
		texture = targetVideoPlayer.getTexture();
	}


}

void MultiFaceTracker::loadFace(string face) {
	src.loadImage(face);
	if (src.getWidth() > 0) {
		while (srcPoints.size() == 0) {

			srcTracker.update(toCv(src));
			vector<ofxFaceTracker2Instance>  instances = srcTracker.getInstances();
			if (instances.size() > 0) {
				ofxFaceTracker2Instance instance = instances[0];
				srcPoints = instance.getLandmarks().getImagePoints();
				targetMesh.update_uvs(srcPoints);
			}
		}
	}
}

