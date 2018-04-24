#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "TOP_CPlusPlusBase.h"
#include "MultiFaceTracker.h"

class OpenFrameworksTOP : public TOP_CPlusPlusBase
{

	ofAppNoWindow       myWindow;

public:
	OpenFrameworksTOP(const OP_NodeInfo *info);
	virtual ~OpenFrameworksTOP();

	virtual void		getGeneralInfo(TOP_GeneralInfo *) override;
	virtual bool		getOutputFormat(TOP_OutputFormat*) override;


	virtual void		execute(const TOP_OutputFormatSpecs*,
								OP_Inputs *inputs,
								TOP_Context *context) override;

	virtual void 		setupParameters(OP_ParameterManager* manager) override;

private:

	void				begin();
	void				end();

	void				setup();

	// We don't need to store this pointer, but we do for the example.
	// The OP_NodeInfo class store information about the node that's using
	// this instance of the class (like its name).
	const OP_NodeInfo	*myNodeInfo;

	ofShader 			shader;
	ofPlanePrimitive 	plane;
	ofFbo fbo;
    ofImage 			img;
	shared_ptr<ofGLProgrammableRenderer> renderer;
	ofVideoPlayer targetVideoPlayer;
	MultiFaceTracker faceTracker;

    bool 				isSetup;
};
