precision  highp  float;
uniform    mat4   u_mvpMatrix;
uniform    mat4   u_modelMatrix;
layout(location=0)in       vec4        a_position;

out         vec3       v_position;
void    main()
{
     gl_Position = u_mvpMatrix * a_position;
     vec4 _position = u_modelMatrix * a_position
     v_position = _position.xyz;
}