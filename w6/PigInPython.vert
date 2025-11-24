#version 330 compatibility

uniform float uPigD;
uniform float uPigH;

out vec3  vL;
out vec3  vN;
out vec3  vE; 

const float PigW		= 6.;
const vec3 LightPosition	= vec3( 15., 15., 15. );

void
main( )
{
	vec4 MCvertex = gl_Vertex;


    float up   = smoothstep(uPigD - PigW, uPigD, MCvertex.x);
    float down = 1.0 - smoothstep(uPigD, uPigD + PigW, MCvertex.x);
    float pulse = up * down;
    float yzscale = 1.0 + uPigH * pulse;


	MCvertex.yz *= yzscale;

	vec4 ECposition = gl_ModelViewMatrix * MCvertex; // eye coordinate position

	vN = normalize( gl_NormalMatrix * gl_Normal ); // normal vector

	vL = LightPosition - ECposition.xyz; // vector from the point to the light position

	vE = vec3( 0., 0., 0. ) - ECposition.xyz; // vector from the point to the eye position		

	gl_Position = gl_ModelViewProjectionMatrix * MCvertex;
}