#version 430 core

layout (vertices = 3) out;

in VS_OUT {
	vec4 color;
} tc_in;

out VS_OUT {
	vec4 color;
} tc_out;

void main( void ) {
	if ( gl_InvocationID == 0 ) {
		gl_TessLevelInner[0] = 5.0;
		gl_TessLevelOuter[0] = 5.0;
		gl_TessLevelOuter[1] = 5.0;
		gl_TessLevelOuter[2] = 5.0;
	}
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	tc_out.color = tc_in.color;
}