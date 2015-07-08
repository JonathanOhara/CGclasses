#version 330
uniform mat4 modelView ;
uniform mat4 projectionMatrix ;

in vec4 vPosition;
in vec3 vNormal;

out vec4 v2fcolor;
out vec4 vPosition2;

void main (){
    //v2fcolor = vec4(0.5, 0.5, 0.5, 1.0);
    v2fcolor = abs( vec4( vNormal.x, vNormal.y, vNormal.z, 1.0) );

    gl_Position = projectionMatrix * modelView * vPosition;

    vPosition2 = gl_Position;
}
