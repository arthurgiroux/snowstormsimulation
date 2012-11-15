//== INCLUDES =================================================================

#include "SnowStormSimulation.h"
#include "../utils/Mesh3DReader.h"


//== IMPLEMENTATION ==========================================================


SnowStormSimulation::
SnowStormSimulation(const char* _title, int _width, int _height)
: TrackballViewer(_title, _width, _height)
{
    init();
}


//-----------------------------------------------------------------------------


void
SnowStormSimulation::
init()
{
    // initialize parent
    TrackballViewer::init();
    
    // set camera to look at world coordinate center
    set_scene_pos(Vector3(0.0, 0.0, 0.0), 2.0);
	
	// load mesh shader
	m_meshShaderDiffuse.create("diffuse.vs", "diffuse.fs");
	m_meshShaderTexture.create("tex.vs","tex.fs");
    
	currentTime = 0.0;
	isWatchOn = false;
	
	daysPerMiliSecond = 1 / 180.0;
	totalDaysElapsed = 0;
    
    //init particle table
    init_particles();
    
    
}


//-------------------------------------------------------------------------------

void
SnowStormSimulation::
keyboard(int key, int x, int y)
{
	switch (key)
	{
		case 'h':
			printf("Help:\n");
			printf("'h'\t-\thelp\n");
			printf("'arrow keys\t-\tchange speed of rotation\n");
			break;
		case ' ':
			if(isWatchOn)
			{
				watch.stop();
				currentTime = 0.0;
			}
			else
			{
				watch.start();
			}
            
			isWatchOn = !isWatchOn;
			break;
		default:
			TrackballViewer::special(key, x, y);
			break;
	}
	
	glutPostRedisplay();
}

//-----------------------------------------------------------------------------


void
SnowStormSimulation::
special(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			daysPerMiliSecond += 0.001;
			if(daysPerMiliSecond > 0.1)
				daysPerMiliSecond = 0.1;
			break;
		case GLUT_KEY_DOWN:
			daysPerMiliSecond -= 0.001;
			if(daysPerMiliSecond < 0.001)
				daysPerMiliSecond = 0.001;
			break;
		default:
			TrackballViewer::special(key, x, y);
			break;
	}
	
	glutPostRedisplay();
}


//-----------------------------------------------------------------------------

void SnowStormSimulation::idle()
{
	if(isWatchOn)
	{
		float prevTime = currentTime;
		currentTime = watch.stop();
		float daysElapsed = daysPerMiliSecond * (currentTime-prevTime);
		totalDaysElapsed += daysElapsed;
		
		glutPostRedisplay();
	}
}

//-----------------------------------------------------------------------------

void
SnowStormSimulation::
draw_scene(DrawMode _draw_mode)
{

	// clear screen
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	
	m_meshShaderTexture.bind();
	
	// set parameters
	m_meshShaderTexture.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderTexture.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
    
    
    
    
    glPointSize(100.0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    double max = MAX_PARTICLES;
    for(int i = 0; i < max; i++)
    {
        Snowflake flake = particles[i];
        glVertex3d(flake.x, flake.y, flake.z);
        cout << flake.y << endl;
        flake.updatePosition();
        
    }

    glEnd();
    glFinish();

    
    
    
	
		
}


void SnowStormSimulation::draw_object(Shader& sh, Mesh3D& mesh)
{
	
	sh.setMatrix4x4Uniform("modelworld", mesh.getTransformation());
    
    
	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer( 3, GL_DOUBLE, 0, mesh.getVertexPointer() );
	glNormalPointer( GL_DOUBLE, 0, mesh.getNormalPointer() );
	glTexCoordPointer( 2, GL_DOUBLE, 0, mesh.getUvTextureCoordPointer() );
	
	for(unsigned int i = 0; i < mesh.getNumberOfParts(); i++)
	{
		glDrawElements( GL_TRIANGLES, mesh.getNumberOfFaces(i)*3, GL_UNSIGNED_INT, mesh.getVertexIndicesPointer(i) );
	}
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
     */
	
}


void SnowStormSimulation::init_particles()
{
    double max = MAX_PARTICLES;
    for(unsigned int j = 0; j < max; j++)
    {
        double x = rand() / (double) RAND_MAX;
        double y = rand() / (double) RAND_MAX;
        double z = rand() / (double) RAND_MAX;
        particles.push_back(Snowflake(x,y,z));
    }
    
    
}



//=============================================================================
