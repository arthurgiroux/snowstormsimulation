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
    
    
    cameraPosX = 0;
    cameraPosY = 2;
    cameraPosZ = 0;
    
    // set camera to look at world coordinate center
    set_scene_pos(Vector3(cameraPosX, cameraPosY, cameraPosZ), 1.0);
	
	// load mesh shader
    m_meshShaderDiffuse.create("diffuse.vs", "diffuse.fs");
    m_meshShaderParticle.create("particle.vs", "particle.fs");
	//m_meshShaderTexture.create("tex.vs","tex.fs");
    
    m_light.translateWorld(Vector3(-3, 5, -10));
    m_Scene.translateWorld(Vector3(1.0, 0.0, 0.0));
    
	currentTime = 0.0;
    
    secondsElapsed = 0.0;
	isWatchOn = false;
    
    //init particle table
    init_particles();
    
    glEnable(GL_DEPTH_TEST);   
    
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
        case 'f':
            cout << "fps " << fps << endl;
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
            m_camera.translateWorld(Vector3(sin(m_camera.getAngleY())*walkingSpeed, 0, -cos(m_camera.getAngleY())*walkingSpeed));
			break;
		case GLUT_KEY_DOWN:
            m_camera.translateWorld(Vector3(-sin(m_camera.getAngleY())*walkingSpeed, 0, cos(m_camera.getAngleY())*walkingSpeed));
			break;
		case GLUT_KEY_LEFT:
            m_camera.translateObject(Vector3(-walkingSpeed, 0, 0));
			break;
		case GLUT_KEY_RIGHT:
            m_camera.translateObject(Vector3(walkingSpeed, 0, 0));
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
    frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
        fps = frame*1000.0/(time-timebase);
	 	timebase = time;
		frame = 0;
	}
}

//-----------------------------------------------------------------------------

void
SnowStormSimulation::
draw_scene(DrawMode _draw_mode)
{

	// clear screen
    glEnable(GL_PROGRAM_POINT_SIZE_EXT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
	//glDisable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

	
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
    m_meshShaderDiffuse.setVector3Uniform("lightcolor", 0.2, 0.8, 0.2);
	
    draw_object(m_meshShaderDiffuse, m_Scene);
    
    m_meshShaderDiffuse.unbind();
    
    
    
    double max = MAX_PARTICLES;
    // Unit box in world coordinates
    Vector3 unitBox[8];
    int boxHeight = 5;
    int boxWidth = 5;
    int boxHeightStart = -1;
    int boxFarPlane = -5;
    int boxNearPlane = 5;
    unitBox[0] = m_camera.getTransformation() * Vector3(-boxWidth, boxHeight, boxNearPlane); // top front left
    unitBox[1] = m_camera.getTransformation() * Vector3(boxWidth, boxHeight, boxNearPlane); // top front right
    unitBox[2] = m_camera.getTransformation() * Vector3(-boxWidth, boxHeightStart, boxNearPlane); // bottom front left
    unitBox[3] = m_camera.getTransformation() * Vector3(boxWidth, boxHeightStart, boxNearPlane); // bottom front right
    unitBox[4] = m_camera.getTransformation() * Vector3(-boxWidth, boxHeight, boxFarPlane); // top back left
    unitBox[5] = m_camera.getTransformation() * Vector3(boxWidth, boxHeight, boxFarPlane); // top back right
    unitBox[6] = m_camera.getTransformation() * Vector3(-boxWidth, boxHeightStart, boxFarPlane); // bottom back left
    unitBox[7] = m_camera.getTransformation() * Vector3(boxWidth, boxHeightStart, boxFarPlane); // bottom back right
    
    
    glColor3f(1.0f, 1.0f, 1.0f);
    /*draw_cube(m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[0],
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[1],
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[2],
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[3],
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[4],
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[5],
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[6],
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[7]);*/
    Vector3 minPos = unitBox[0];
    Vector3 maxPos = unitBox[0];
    
    // Get the maximum / minimum for x, y and z
    for (int i=0; i < 8; ++i) {
        if (unitBox[i].x < minPos.x) {
            minPos.x = unitBox[i].x;
        }
        if (unitBox[i].x > maxPos.x) {
            maxPos.x = unitBox[i].x;
        }
        if (unitBox[i].y < minPos.y) {
            minPos.y = unitBox[i].y;
        }
        if (unitBox[i].y > maxPos.y) {
            maxPos.y = unitBox[i].y;
        }
        if (unitBox[i].z < minPos.z) {
            minPos.z = unitBox[i].z;
        }
        if (unitBox[i].z > maxPos.z) {
            maxPos.z = unitBox[i].z;
        }
    }
    
    // (Vector3 topfrontleft, Vector3 topfrontright, Vector3 bottomfrontleft, Vector3 bottomfrontright, Vector3 topbackleft, Vector3 topbackright, Vector3 bottombackleft, Vector3 bottombackright)
    /*draw_cube(m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(minPos.x, maxPos.y, minPos.z),
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(maxPos.x, maxPos.y, minPos.z),
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(minPos.x, minPos.y, minPos.z),
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(maxPos.x, minPos.y, minPos.z),
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(minPos.x, maxPos.y, maxPos.z),
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(maxPos.x, maxPos.y, maxPos.z),
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(minPos.x, minPos.y, maxPos.z),
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(maxPos.x, minPos.y, maxPos.z));*/
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    Vector3 tmp;
    tmp = m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(0, 1, 0);
    glVertex3d(tmp.x, tmp.y, tmp.z);
    tmp = m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(2, 1, 0);
    glVertex3d(tmp.x, tmp.y, tmp.z);
    
    glColor3f(0.0f, 1.0f, 0.0f);
    tmp = m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(0, 1, 0);
    glVertex3d(tmp.x, tmp.y, tmp.z);
    tmp = m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(0, 2, 0);
    glVertex3d(tmp.x, tmp.y, tmp.z);
    
    
    glColor3f(0.0f, 0.0f, 1.0f);
    tmp = m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(0, 1, 0);
    glVertex3d(tmp.x, tmp.y, tmp.z);
    tmp = m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(0, 1, 1);
    glVertex3d(tmp.x, tmp.y, tmp.z);
    
    glEnd();
    
    m_meshShaderParticle.bind();
    m_meshShaderParticle.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderParticle.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
    m_meshShaderParticle.setVector4Uniform("cameraorigin", m_camera.origin().x, m_camera.origin().y, m_camera.origin().z, 0);
    cout << m_camera.origin().x << " " << m_camera.origin().y << " " << m_camera.origin().z << endl;
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glBegin(GL_POINTS);
    for (int i = 0; i < max; i++)
    {
        Snowflake* flake = particles[i];
        //m_meshShaderParticle.setVector3Attribute("velocity", flake->velocity.x, flake->velocity.y, flake->velocity.z);
        //m_meshShaderParticle.setVector3Attribute("force", 0, -0.009f, 0);
        flake->updatePosition(Vector3(0, -0.009f, 0), minPos, maxPos);
        //m_meshShaderParticle.setVector3Attribute("initPos", flake->x, flake->y, flake->z);
        //cout << fabs(flake->pos.z - m_camera.origin().z) << endl;
        glVertex3d(flake->pos.x, flake->pos.y, flake->pos.z);
    }
    glEnd();
    m_meshShaderParticle.unbind();
    glFinish();
}

void SnowStormSimulation::draw_cube(Vector3 topfrontleft, Vector3 topfrontright, Vector3 bottomfrontleft, Vector3 bottomfrontright, Vector3 topbackleft, Vector3 topbackright, Vector3 bottombackleft, Vector3 bottombackright) {
    glBegin(GL_LINES);
    glVertex3d(topfrontleft.x, topfrontleft.y, topfrontleft.z);
    glVertex3d(topfrontright.x, topfrontright.y, topfrontright.z);
    glVertex3d(topfrontleft.x, topfrontleft.y, topfrontleft.z);
    glVertex3d(bottomfrontleft.x, bottomfrontleft.y, bottomfrontleft.z);
    glVertex3d(topfrontleft.x, topfrontleft.y, topfrontleft.z);
    glVertex3d(topbackleft.x, topbackleft.y, topbackleft.z);
    
    glVertex3d(bottomfrontright.x, bottomfrontright.y, bottomfrontright.z);
    glVertex3d(topfrontright.x, topfrontright.y, topfrontright.z);
    glVertex3d(bottomfrontright.x, bottomfrontright.y, bottomfrontright.z);
    glVertex3d(bottomfrontleft.x, bottomfrontleft.y, bottomfrontleft.z);
    glVertex3d(bottomfrontright.x, bottomfrontright.y, bottomfrontright.z);
    glVertex3d(bottombackright.x, bottombackright.y, bottombackright.z);
    
    glVertex3d(topbackright.x, topbackright.y, topbackright.z);
    glVertex3d(topfrontright.x, topfrontright.y, topfrontright.z);
    glVertex3d(topbackright.x, topbackright.y, topbackright.z);
    glVertex3d(bottombackright.x, bottombackright.y, bottombackright.z);
    glVertex3d(topbackright.x, topbackright.y, topbackright.z);
    glVertex3d(topbackleft.x, topbackleft.y, topbackleft.z);
    
    
    glVertex3d(bottombackleft.x, bottombackleft.y, bottombackleft.z);
    glVertex3d(topbackleft.x, topbackleft.y, topbackleft.z);
    glVertex3d(bottombackleft.x, bottombackleft.y, bottombackleft.z);
    glVertex3d(bottomfrontleft.x, bottomfrontleft.y, bottomfrontleft.z);
    glVertex3d(bottombackleft.x, bottombackleft.y, bottombackleft.z);
    glVertex3d(bottombackright.x, bottombackright.y, bottombackright.z);
    
    glEnd();
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
