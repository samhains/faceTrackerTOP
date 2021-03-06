#include "OpenFrameworksTOP.h"

#include <windows.h>
#include <gl/glew.h>

#include <cstdio>
#include <assert.h>
#include <stdio.h>
#include <string.h>

// These functions are basic C function, which the DLL loader can find
// much easier than finding a C++ Class.
// The DLLEXPORT prefix is needed so the compile exports these functions from the .dll
// you are creating
extern "C"
{

	DLLEXPORT
		int
		GetTOPAPIVersion(void)
	{
		// Always return TOP_CPLUSPLUS_API_VERSION in this function.
		return TOP_CPLUSPLUS_API_VERSION;
	}

	DLLEXPORT
		TOP_CPlusPlusBase*
		CreateTOPInstance(const OP_NodeInfo* info, TOP_Context *context)
	{
		// Return a new instance of your class every time this is called.
		// It will be called once per TOP that is using the .dll
		return new OpenFrameworksTOP(info);
	}

	DLLEXPORT
		void
		DestroyTOPInstance(TOP_CPlusPlusBase* instance, TOP_Context *context)
	{
		// Delete the instance here, this will be called when
		// Touch is shutting down, when the TOP using that instance is deleted, or
		// if the TOP loads a different DLL
		delete (OpenFrameworksTOP*)instance;
	}

};

OpenFrameworksTOP::OpenFrameworksTOP(const OP_NodeInfo* info) : myNodeInfo(info), renderer(make_shared<ofGLProgrammableRenderer>(&myWindow))
{
	isSetup = false;

	// ofAppNoWindow doesn't provide a usable renderer so calling any interfacing
	// functions that use a renderer (ie. ofShader::begin) will either crash or do absolutely nothing.
	// Instead make our own renderer and call the renderer's functions directly.

	// In the below example my installed TouchDesigner executable is in C:/Program Files/Derivative/TouchDesigner088/bin/ and 
	// my data directory is in C:/of_v0.9.3_vs_release/apps/myApps/inputTexture/bin/data/
	// So below is the relative path from the installed TouchDesigner executable to my data directory:
	// ofSetDataPathRoot("../../../../of_v0.9.3_vs_release/apps/myApps/inputTexture/bin/data/");
	ofSetDataPathRoot("C:/Users/samhains/C++/of_v0.9.8_vs_release/apps/inputTexture/inputTexture/bin/data");
}

OpenFrameworksTOP::~OpenFrameworksTOP()
{
	faceTracker.stop();
	//delete renderer;
}

void OpenFrameworksTOP::setup()
{
	glewInit();
	renderer->setup(3, 2);
	targetVideoPlayer.load("movies/cut.mp4");
	targetVideoPlayer.play();
	targetVideoPlayer.setVolume(0);

	ofFbo::Settings settings(renderer);
	settings.width = 1280;
	settings.height = 720;
	settings.numSamples = 0;
	settings.internalformat = GL_RGBA;
	faceTracker.setup(settings, renderer, faceTexture);


	isSetup = true;
}

void
OpenFrameworksTOP::getGeneralInfo(TOP_GeneralInfo* ginfo)
{
	// Uncomment this line if you want the TOP to cook every frame even
	// if none of it's inputs/parameters are changing.
	ginfo->cookEveryFrame = true;
}

bool
OpenFrameworksTOP::getOutputFormat(TOP_OutputFormat* format)
{
	// In this function we could assign variable values to 'format' to specify
	// the pixel format/resolution etc that we want to output to.
	// If we did that, we'd want to return true to tell the TOP to use the settings we've
	// specified.
	// In this example we'll return false and use the TOP's settings
	return false;
}

void
OpenFrameworksTOP::begin()
{
	renderer->startRender();
	renderer->setupScreen();
	renderer->pushMatrix();
}

void
OpenFrameworksTOP::end()
{
	renderer->popMatrix();
	renderer->finishRender();
}
void OpenFrameworksTOP::updateParameters(OP_Inputs* inputs) {

	const OP_TOPInput *topInput = inputs->getInputTOP(0);
	bool toggleBackground = inputs->getParInt("Background");
	bool toggleBlur = inputs->getParInt("Blur");
	bool toggleActive = inputs->getParInt("Active");
	bool pulseReload = inputs->getParInt("Reload");

	touchParms.toggleBackground = toggleBackground;
	touchParms.toggleBlur = toggleBlur;
	touchParms.toggleActive = toggleActive;
	touchParms.pulseReload = pulseReload;
}

void OpenFrameworksTOP::setTexturesFromInput(OP_Inputs* inputs) {

	const OP_TOPInput *topInput = inputs->getInputTOP(0);

	bgTexture.setUseExternalTextureID(topInput->textureIndex);
	bgTexture.texData.width = topInput->width;
	bgTexture.texData.height = topInput->height;
	bgTexture.texData.tex_w = topInput->width;
	bgTexture.texData.tex_h = topInput->height;
	bgTexture.texData.tex_t = 1.0f;
	bgTexture.texData.tex_u = 1.0f;
	bgTexture.texData.textureTarget = topInput->textureType;
	bgTexture.texData.bFlipTexture = true;

	const OP_TOPInput *topInput2 = inputs->getInputTOP(1);
	faceTexture.setUseExternalTextureID(topInput2->textureIndex);
	faceTexture.texData.width = topInput2->width;
	faceTexture.texData.height = topInput2->height;
	faceTexture.texData.tex_w = topInput2->width;
	faceTexture.texData.tex_h = topInput2->height;
	faceTexture.texData.tex_t = 1.0f;
	faceTexture.texData.tex_u = 1.0f;
	faceTexture.texData.textureTarget = topInput2->textureType;
	faceTexture.texData.bFlipTexture = true;


}

void
OpenFrameworksTOP::execute(const TOP_OutputFormatSpecs* outputFormat,
	OP_Inputs* inputs,
	TOP_Context *context)

{
		updateParameters(inputs);
		if (touchParms.toggleActive) {
			setTexturesFromInput(inputs);
			int width = outputFormat->width;
			int height = outputFormat->height;

		// Use the first input TOP (here we assume it exists but in reality it might not)

		// Need to use a ofAppNoWindow so that openFrameworks doesn't create a conflicting
		// OpenGL context. We want to use TouchDesigner's context in ::execute
		ofSetupOpenGL(&myWindow, width, height, OF_WINDOW);

		context->beginGLCommands();
		if (!isSetup)
		{
			setup();
		}
		faceTracker.update(bgTexture, touchParms, faceTexture);
		texture = faceTracker.getTexture();

		glBindFramebuffer(GL_FRAMEBUFFER, context->getFBOIndex());
		begin();

		renderer->draw(texture, 0, 0, 0, 1280, 720, 0, 0, 1280, 720);

		end();

		context->endGLCommands();


	}

}

void
OpenFrameworksTOP::setupParameters(OP_ParameterManager* manager)
{

	OP_NumericParameter	np;
	np.name = "Background";
	np.label = "Background";


	OP_NumericParameter	np2;
	np2.name = "Blur";
	np2.label = "Blur Subject";

	OP_NumericParameter	np3;
	np3.name = "Active";
	np3.label = "Tracking Active";

	OP_NumericParameter	np4;
	np4.name = "Reload";
	np4.label = "Reload Face";

	ParAppendResult res3 = manager->appendToggle(np3);
	ParAppendResult res = manager->appendToggle(np);
	ParAppendResult res2 = manager->appendToggle(np2);
	ParAppendResult res4 = manager->appendToggle(np4);
	assert(res == PARAMETER_APPEND_SUCCESS);
	assert(res2 == PARAMETER_APPEND_SUCCESS);
	assert(res3 == PARAMETER_APPEND_SUCCESS);
	assert(res4 == PARAMETER_APPEND_SUCCESS);

}
