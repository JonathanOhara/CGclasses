# version 400
in vec4 vPosition ;
in vec3 vNormal ;

uniform mat4 modelView ;
uniform mat4 projectionMatrix ;
uniform mat3 normalMatrix ;
uniform vec4 ambientProduct ;
uniform vec4 diffuseProduct ;
uniform vec4 specularProduct ;
uniform float shininess ;
uniform vec4 lightPosition ;

out vec3 fN ;
out vec3 fE ;
out vec3 fL ;

out vec4 pos;

void main (){
    vec4 eyePosition = modelView * vPosition ;
    fN = normalMatrix * vNormal ;
    fL = lightPosition . xyz - eyePosition . xyz ;
    fE = - eyePosition . xyz ;
    gl_Position = projectionMatrix * eyePosition ;
    pos = gl_Position;
}
