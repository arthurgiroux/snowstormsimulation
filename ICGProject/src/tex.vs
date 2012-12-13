uniform mat4 worldcamera;
uniform mat4 projection;

void main()
{
	gl_Position = projection * worldcamera * gl_Vertex;
    
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
}
