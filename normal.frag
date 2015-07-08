#version 330
in vec4 v2fcolor;
in vec4 vPosition2;
out vec4 myfragcolor;

float rand(vec2 n){
  return 0.5 + 0.5 *
     fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

void main (){
    /*
    float x = rand(vPosition2.xx);
    float y = rand(vPosition2.yy);
    float z = rand(vPosition2.zz);

    myfragcolor = vec4(x, y, z, 1.0);
    */
    myfragcolor = v2fcolor;
}
