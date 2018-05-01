#version 150

// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;
vec2 newTexCoord;

// this is something we're creating for this shader
out vec2 texCoordVarying;


void main()
{
    // here we move the texture coordinates
//	newTexCoord = texcoord;
//	newTexCoord.y = 1 - texcoord.y;
    texCoordVarying = vec2(texcoord.x, 1.0 - texcoord.y);

    // send the vertices to the fragment shader
	gl_Position = modelViewProjectionMatrix * position;
}