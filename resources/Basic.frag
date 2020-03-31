#version 450

uniform sampler2D texture_diffuse;
uniform vec3 FragLightColor;
uniform vec3 FragLightPos;
uniform vec3 FragCamPos;

in vec3 FragNormal;
in vec3 FragPos;
 
in vec2 FragTextureCoord;
out vec4 frag_colour;
void main(){

float ambientStrenght=0.001;
vec3 ambient= ambientStrenght*FragLightColor;

vec3 normal= normalize(FragNormal);
vec3 lightDir = normalize(FragLightPos-FragPos);
float diff= max(dot(normal, lightDir),0.0);
vec3 diffuse= diff* FragLightColor;

float specularStrenght= 10f;
vec3 viewDir= normalize(FragCamPos- FragPos);
vec3 reflectDir= reflect(-lightDir,normal);
float spec= pow(max(dot(normal, reflectDir),0.0),32.0);
vec3 specular = vec3( specularStrenght* spec);

vec4 result=vec4(texture2D(texture_diffuse,FragTextureCoord).rgb* (ambient+diffuse+specular),1);

frag_colour=result;
}