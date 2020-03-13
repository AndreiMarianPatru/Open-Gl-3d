#version 450

uniform sampler2D texture_diffuse;
in vec2 FragTextureCoord;
out vec4 frag_colour;
void main(){

frag_colour=vec4(texture2D(texture_diffuse,FragTextureCoord).rgb,1);
}