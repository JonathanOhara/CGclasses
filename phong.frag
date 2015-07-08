# version 410
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

#version 400
in vec3 Position;
in vec3 Normal;
struct LightInfo {
vec4 position;
vec3 intensity;
};
uniform LightInfo Light;
uniform vec3 Kd; // Diffuse reflectivity
uniform vec3 Ka; // Ambient reflectivity
const int levels = 3;
const float scaleFactor = 1.0 / levels;
layout( location = 0 ) out vec4 FragColor;
vec3 toonShade( )
{
vec3 s = normalize( Light.position.xyz - Position.xyz );
float cosine = max( 0.0, dot( s, Normal ) );

vec3 diffuse = Kd * floor( cosine * levels ) *
scaleFactor;
return Light.intensity * (Ka + diffuse);
}
void main() {
FragColor = vec4(toonShade(), 1.0);
}
