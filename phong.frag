#version 330
in vec3 fN ;
in vec3 fE ;
in vec3 fL ;

uniform vec4 ambientProduct ;
uniform vec4 diffuseProduct ;
uniform vec4 specularProduct ;

uniform float shininess ;

out vec4 frag_color ;

void main (){
    vec3 N = normalize ( fN ) ;
    vec3 E = normalize ( fE ) ;
    vec3 L = normalize ( fL ) ;
    vec3 R = normalize (2.0 * dot (L , N ) * N - L ) ;

    float NdotL = dot (N , L ) ;
    float Kd = max ( NdotL , 0.0) ;
    float Ks = ( NdotL < 0.0) ? 0.0 : pow ( max ( dot (R , E ) , 0.0) , shininess ) ;

    vec4 diffuse = Kd * diffuseProduct ;
    vec4 specular = Ks * specularProduct ;
    vec4 ambient = ambientProduct ;

    frag_color = ambient + diffuse + specular ;
    frag_color . a = 1.0;
}
