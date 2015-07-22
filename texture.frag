# version 330
uniform sampler2D colorTexture;
in vec2 ftexCoord;

out vec4 outColor;

void main ( void ) {
    outColor = texture( colorTexture , ftexCoord );
}
