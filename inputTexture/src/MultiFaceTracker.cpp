#include "MultiFaceTracker.h"

using namespace ofxCv;


void MultiFaceTracker::setup(ofFbo::Settings _settings, shared_ptr<ofGLProgrammableRenderer> _renderer) {
	settings = _settings;
	renderer = _renderer;
	//ofSetVerticalSync(true);
	clone.setup(1280, 720, settings, renderer);
	//targetVideoPlayer.load("movies/cut.mp4");
	//targetVideoPlayer.play();
	//targetVideoPlayer.setVolume(0);

	targetTracker.setup();
	srcTracker.setup();
	maskFbo.allocate(settings);
	srcFbo.allocate(settings);

	//faces.allowExt("jpg");
	//faces.allowExt("png");
	//faces.listDir("faces");
	loadFace("faces/src.jpg");
	//if (faces.size() != 0) {

	//	loadFace(faces.getPath(0));
	//}

}
void MultiFaceTracker::update(ofTexture bgTexture) {;
if (src.isAllocated()) {
		//ofLoadImage(bgTexture, "path");
		//renderer->bind(img);
		//bgTexture


		//ofPixels pixels;
		//bgTexture.readToPixels(pixels);
		//targetTracker.update(pixels);

		vector<ofxFaceTracker2Instance> instances = targetTracker.getInstances();
		vector<vector<ofVec2f>> targetPointsArr(instances.size());
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
			renderer->bind(src.getTexture(), 0);
			ofxFaceTracker2Instance camTarget = instances[i];
			std::vector<ofVec2f> targetPoints = camTarget.getLandmarks().getImagePoints();
			targetPointsArr[i] = targetPoints;
			targetMesh.update_vertices(targetPointsArr[i]);
			targetMesh.update_uvs(srcPoints);
			renderer->draw(targetMesh,OF_MESH_FILL);
			renderer->unbind(src.getTexture(), 0);
		}
		srcFbo.end();

		clone.setStrength(16);
		clone.update(srcFbo.getTextureReference(), bgTexture, maskFbo.getTextureReference());

		//texture = bgTexture;

	if (src.getWidth() > 0) {
		texture = clone.getTexture();
	}
	else {
		texture = bgTexture;
	}
}
else {
	std::cout << "NOT ALLOCATEED\n";
}

}

ofTexture MultiFaceTracker::getTexture() {
	return texture;
}
void MultiFaceTracker::stop() {
	srcTracker.stop();
	targetTracker.stop();
}

void MultiFaceTracker::loadFace(string face) {

	src.load(face);

	if (src.getWidth() > 0) {
		srcTracker.update(toCv(src));
		Sleep(1000);
		srcTracker.update(toCv(src));

		vector<ofxFaceTracker2Instance>  instances = srcTracker.getInstances();

		if (instances.size() > 0) {
			ofxFaceTracker2Instance instance = instances[0];
			srcPoints = instance.getLandmarks().getImagePoints();
			targetMesh.update_uvs(srcPoints);
		}
	}
	else {
		std::cout << "NOT ALLOCATEED\n";
	}
}

