#version 330 core

uniform    mat4     u_mvpMatrix;
layout(location=0)in         vec4       a_position;
layout(location=1)in         vec2       a_texCoord;

out        vec2        v_fragCoord;

void  main()
{
	gl_Position = u_mvpMatrix * a_position;
	v_fragCoord = a_texCoord*float(true);
}