uniform mat4 worldcamera;
uniform mat4 projection;

varying vec3 position;
void main()
{
    position = gl_Vertex.xyz;
	gl_Position = projection * worldcamera * gl_Vertex;
        
}
