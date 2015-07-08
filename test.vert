#version 330
uniform mat4 modelView ;
uniform mat4 projectionMatrix ;

in vec4 vPosition;
in vec3 vNormal;

out vec4 v2fcolor;
out vec4 vPosition2;


struct LightInfo {
    vec4 position;
    vec3 intensity;
};
uniform LightInfo light;

void main(){
    gl_Position = projectionMatrix * modelView * vPosition;

    v2fcolor = light.position;
    //v2fcolor = vec4(0.5, 0.5, 0.5, 1);

    vPosition2 = gl_Position;
}
