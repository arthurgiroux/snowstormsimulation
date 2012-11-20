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
    set_scene_pos(Vector3(0.0, 0.0, 0.0), 10.0);
	
	// load mesh shader
    m_meshShaderDiffuse.create("diffuse.vs", "diffuse.fs");
	//m_meshShaderTexture.create("tex.vs","tex.fs");
    
    m_light.translateWorld(Vector3(-3, 5, -10));
    m_Scene.translateWorld(Vector3(1.0, 0.0, 0.0));
    
	currentTime = 0.0;
    
    secondsElapsed = 0.0;
	isWatchOn = false;
    
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
		secondsElapsed =  (currentTime-prevTime) / 1000.0;		
		glutPostRedisplay();
	}
}

//-----------------------------------------------------------------------------

void
SnowStormSimulation::
draw_scene(DrawMode _draw_mode)
{

	// clear screen
    //glEnable(GL_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glDisable(GL_CULL_FACE);
	//glEnable(GL_MULTISAMPLE);
	
	m_meshShaderDiffuse.bind();
	
	// set parameters
	m_meshShaderDiffuse.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderDiffuse.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());

	//scene
	Vector3 lightPosInCamera = m_camera.getTransformation().Inverse() * m_light.origin();
	
	m_meshShaderDiffuse.setMatrix3x3Uniform("worldcameraNormal", m_camera.getTransformation().Transpose());
    m_meshShaderDiffuse.setMatrix3x3Uniform("modelworldNormal", m_Scene.getTransformation().Inverse().Transpose());
	m_meshShaderDiffuse.setVector3Uniform("lightposition", lightPosInCamera.x, lightPosInCamera.y, lightPosInCamera.z );
	m_meshShaderDiffuse.setVector3Uniform("diffuseColor", 0.5, 0.5, 0.5);
    m_meshShaderDiffuse.setVector3Uniform("lightcolor", 0.6, 0.2, 0.4);
	
    draw_object(m_meshShaderDiffuse, m_Scene);

    
    glPointSize(2);
    
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();
    
    m_meshShaderDiffuse.unbind();
    
    glPointSize(2);
    
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    double max = MAX_PARTICLES;
    for(int i = 0; i < max; i++)
    {
        Snowflake* flake = particles[i];
        Vector3 proj = Vector3(flake->x, flake->y, flake->z);
        Vector3 tmp = m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * proj;
        cout << tmp.x << " " << tmp.y << " " << tmp.z << endl;
        glVertex3d(tmp.x, tmp.y, tmp.z);
        flake->updatePosition(Vector3(0, -0.009f, 0));
    }

    glEnd();
    glFinish();
    cout << secondsElapsed << endl;
    secondsElapsed = 0;
    
    
    
	
		
}

void SnowStormSimulation::load_mesh(const std::string& filenameObj) {
    Mesh3DReader::read( filenameObj, m_Scene);
    
    // calculate normals
    if(!m_Scene.hasNormals())
        m_Scene.calculateVertexNormals();
    
    m_Scene.translateWorld(Vector3(0, 0, 0));
    
}

void SnowStormSimulation::draw_object(Shader& sh, Mesh3D& mesh)
{
	
	sh.setMatrix4x4Uniform("modelworld", mesh.getTransformation() );
	
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
	
}


void SnowStormSimulation::init_particles()
{
    double max = MAX_PARTICLES;
    for(unsigned int j = 0; j < max; j++)
    {
        particles.push_back(new Snowflake());
    }
    
    
}



//=============================================================================
