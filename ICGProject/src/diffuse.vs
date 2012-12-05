uniform mat4 modelworld;
uniform mat4 worldcamera;
uniform mat4 projection;

uniform mat3 worldcameraNormal;
uniform mat3 modelworldNormal;
uniform vec3 lightposition;

varying vec3 normal, lightDir;
void main()
{
	gl_Position = projection * worldcamera * modelworld * gl_Vertex;
    
    normal = normalize( worldcameraNormal * modelworldNormal * gl_Normal );
	
	vec3 vertex = vec3( worldcamera * modelworld * gl_Vertex );
	
    lightDir = (lightposition - vertex);

}
