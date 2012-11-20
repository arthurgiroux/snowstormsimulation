uniform mat4 modelworld;
uniform mat4 worldcamera;
uniform mat4 projection;

void main()
{
	// transform vertex to camera coordinates
	gl_Position = projection * worldcamera * modelworld * gl_Vertex;
}
