#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform mediump vec4 color;
varying vec2 v_texcoord;

void main()
{
    gl_FragColor = color;
}
