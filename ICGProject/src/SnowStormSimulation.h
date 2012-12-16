#ifndef MESH_VIEWER_HH
#define MESH_VIEWER_HH

#define MAX_PARTICLES 50000;

//== INCLUDES =================================================================
#include <string>
#include <vector>

#include "../gl/TrackballViewer.h"
#include "../gl/shader.h"
#include "../gl/Mesh3D.h"
#include "../gl/Light3D.h"
#include "../gl/texture.h"
#include "../utils/StopWatch.h"
#include "Snowflake.h"

class SnowStormSimulation : public TrackballViewer
{
public:
    
	SnowStormSimulation(const char* _title, int _width, int _height);
    ~SnowStormSimulation();
	void load_mesh(const std::string& filenameObj);
	
    
protected:
    
	// overloaded GUI function
	virtual void init();
	virtual void keyboard(int key, int x, int y);
	virtual void special(int key, int x, int y);
	virtual void idle();
	
	virtual void draw_scene(DrawMode _draw_mode);
    
    void draw_cube(Vector3 topfrontleft, Vector3 topfrontright, Vector3 bottomfrontleft,
                   Vector3 bottomfrontright, Vector3 topbackleft, Vector3 topbackright,
                   Vector3 bottombackleft, Vector3 bottombackright);
    
    void draw_object(Shader& sh, Mesh3D& mesh);
	void draw_object(Shader& sh, Mesh3D& mesh, bool showTexture);
    
    void draw_storms();
    void update_storms_positions(Vector3 minPos, Vector3 maxPos);
    float randomFloat(float a, float b);

    
    void moveCamera();
        
    //init particles

    void init_particles();
    
    void draw_sky();
    
protected:
	
	// mesh objects
	//Mesh3D m_SnowFlake;
    
    Mesh3D m_Scene;

	
	// directional light
	Light3D m_light;
	
	// mesh shader
	Shader m_meshShaderDiffuse;
	Shader m_meshShaderParticle;
    Shader m_meshShaderProj;
    Shader m_meshShaderSky;
	
	//timer
	StopWatch watch;
	bool isWatchOn;
	
	float daysPerMiliSecond;
	float totalDaysElapsed;
	float currentTime;
    
    float deltaTime;
    
    std::vector<Snowflake*> particles;
    
    std::vector<Cone*> storms;
    
    float walkingSpeed = 0.1;
    
    Texture* texture_snowflake;
    Texture texture_sky;
    
    int frame=0;
    int time;
    int timebase=0;
    float ratio = 1.0;
    float fps = 0;
    
    bool altMode = false;
    bool drawStorms = false;
    bool initPart = false;
    
};


//=============================================================================
#endif // MESH_VIEWER_HH defined
//=============================================================================
