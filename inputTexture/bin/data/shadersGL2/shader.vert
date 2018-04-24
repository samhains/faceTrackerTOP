#version 120

// this is coming from our C++ code
uniform float xPos;

varying vec2 texCoordVarying;

void main()
{
    vec2 texcoord = gl_MultiTexCoord0.xy;

    // here we move the texture coordinates
    texCoordVarying = vec2(texcoord.x + xPos, texcoord.y);

    // send the vertices to the fragment shader
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;;
}