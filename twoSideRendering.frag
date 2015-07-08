# version 410
in vec3 fN ;
in vec3 fE ;
in vec3 fL ;

uniform vec4 ambientProduct ;
uniform vec4 diffuseProduct ;
uniform vec4 specularProduct ;
uniform float shininess ;

out vec4 frag_color ;

vec4 Phong ( vec3 n ) {
    vec3 N = normalize ( n ) ;
    vec3 E = normalize ( fE ) ;
    vec3 L = normalize ( fL ) ;
    vec3 R = normalize (2.0 * dot (L , N ) * N - L ) ;
    float NdotL = dot (N , L ) ;
    float Kd = max ( NdotL , 0.0) ;
    float Ks = ( NdotL < 0.0) ? 0.0 : pow ( max ( dot (R , E ) , 0.0) , shininess ) ;
    vec4 diffuse = Kd * diffuseProduct ;
    vec4 specular = Ks * specularProduct ;
    vec4 ambient = ambientProduct ;

    return ambient + diffuse + specular ;
}

void main (){
    if( gl_FrontFacing ) {
        frag_color = vec4 ( Phong ( fN ) . xyz ,1) ;
    } else {
        frag_color = mix ( vec4 ( Phong ( - fN ) . xyz ,1) , vec4(0.0 ,2.0 ,0.0 ,1.0) , 0.7 ) ;
    }
}
