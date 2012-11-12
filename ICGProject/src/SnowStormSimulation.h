#ifndef MESH_VIEWER_HH
#define MESH_VIEWER_HH


//== INCLUDES =================================================================
#include <string>
#include <vector>

#include "../gl/TrackballViewer.h"
#include "../gl/shader.h"
#include "../gl/Mesh3D.h"
#include "../gl/Light3D.h"
#include "../gl/texture.h"
#include "../utils/StopWatch.h"

class SnowStormSimulation : public TrackballViewer
{
public:
    
	SnowStormSimulation(const char* _title, int _width, int _height);
    
	void load_mesh(const std::string& filenameObj);
	
    
protected:
    
	// overloaded GUI function
	virtual void init();
	virtual void keyboard(int key, int x, int y);
	virtual void special(int key, int x, int y);
	virtual void idle();
	
	virtual void draw_scene(DrawMode _draw_mode);
    
    void draw_object(Shader& sh, Mesh3D& mesh);
	void draw_object(Shader& sh, Mesh3D& mesh, bool showTexture);
    
protected:
	
	// mesh objects
	Mesh3D m_SnowFlake;
	
	// directional light
	Light3D m_light;
	
	// mesh shader
	Shader m_meshShaderDiffuse;
	Shader m_meshShaderPhong;
	Shader m_meshShaderTexture;
	
	//timer
	StopWatch watch;
	bool isWatchOn;
	
	float daysPerMiliSecond;
	float totalDaysElapsed;
	float currentTime;
};


//=============================================================================
#endif // MESH_VIEWER_HH defined
//=============================================================================
