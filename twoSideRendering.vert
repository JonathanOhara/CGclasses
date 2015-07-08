# version 410
in vec4 vPosition ;
in vec3 vNormal ;

uniform mat4 modelView ;
uniform mat4 projectionMatrix ;
uniform mat3 normalMatrix ;
uniform vec4 lightPosition ;

out vec3 fN ;
out vec3 fE ;
out vec3 fL ;

void computeEyeVectors ( out vec3 n , out vec4 p , out vec3 l , out vec3 e ) {
    n = normalize ( normalMatrix * vNormal ) ;
    p = modelView * vec4 ( vPosition ) ;
    l = lightPosition . xyz - p . xyz ;
    e = -p . xyz ;
}

void main () {
    vec4 eyePosition ;
    computeEyeVectors ( fN , eyePosition , fL , fE ) ;
    gl_Position = projectionMatrix * eyePosition ;
}
