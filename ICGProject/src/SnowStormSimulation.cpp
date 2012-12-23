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


SnowStormSimulation::~SnowStormSimulation()
{
    for(std::vector<Cone*>::iterator it = storms.begin(); it != storms.end(); ++it) {
        delete * it;
    }
    
    for(std::vector<Snowflake*>::iterator it = particles.begin(); it != particles.end(); ++it) {
        delete * it;
    }
    
    delete texture_snowflake;
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
    m_meshShaderProj.create("proj.vs", "proj.fs");
    m_meshShaderSky.create("sky.vs", "sky.fs");
    
    m_light.translateWorld(Vector3(-10, 10, 20));
    m_Scene.translateWorld(Vector3(1.0, 0.0, 0.0));
    
	currentTime = 0.0;
    
    deltaTime = 0.0;
	isWatchOn = false;
    
    //init particle table
    init_particles();
    
    
    texture_snowflake = new Texture();
    texture_snowflake->create("../data/snowflakereal.tga");
    
    texture_sky.create_cubemap("../data/sky/sky");
        
    glEnable(GL_DEPTH_TEST);
}


//-------------------------------------------------------------------------------

void
SnowStormSimulation::
keyboard(int key, int x, int y)
{
	switch (key)
	{
        case 's':
            drawStorms = !drawStorms;
            break;
        case 'x':
            altSky = !altSky;
            if (altSky) {
                texture_sky.create_cubemap("../data/sky/skyalt");
            }
            else {
                texture_sky.create_cubemap("../data/sky/sky");
            }
            break;
		case ' ':
			if (isWatchOn)
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
        case 'c':
            altMode = !altMode;
            if (!altMode) {
                ratio = 1.0;
                texture_snowflake->create("../data/snowflakereal.tga");
            }
            else {
                ratio = 2.0;
                texture_snowflake->create("../data/cage.tga");
            }
            break;
        case 'p':
            altMode = !altMode;
            if (!altMode) {
                ratio = 1.0;
                texture_snowflake->create("../data/snowflakereal.tga");
            }
            else {
                ratio = 2.0;
                texture_snowflake->create("../data/pauly.tga");
            }
            break;
            
        case 'o':

            storms.push_back(new Cone(Vector3(0, 0, 0), randomFloat(1,2), randomFloat(2,5), Vector3(0, 1, 0), Vector3(randomFloat(-0.001, 0.001),0,randomFloat(-0.0005, 0.0005)),Vector3(randomFloat(-0.00005, 0.00005), 0, randomFloat(-0.00005, 0.00005))));
            break;
            
        case 'i':
            if (storms.size() > 0) {
                storms.pop_back();
            }
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
		deltaTime =  currentTime-prevTime;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
	glEnable(GL_MULTISAMPLE);
    
    draw_sky();
    
	
	m_meshShaderDiffuse.bind();
	
	// draw the mountain
	m_meshShaderDiffuse.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderDiffuse.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());

	Vector3 lightPosInCamera = m_camera.getTransformation().Inverse() * m_light.origin();
	
	m_meshShaderDiffuse.setMatrix3x3Uniform("worldcameraNormal", m_camera.getTransformation().Transpose());
    m_meshShaderDiffuse.setMatrix3x3Uniform("modelworldNormal", m_Scene.getTransformation().Inverse().Transpose());
    m_Scene.getMaterial(0).m_diffuseTexture.bind();
    m_meshShaderDiffuse.setIntUniform("texture", m_Scene.getMaterial(0).m_diffuseTexture.getLayer());
	m_meshShaderDiffuse.setVector3Uniform("lightposition", lightPosInCamera.x, lightPosInCamera.y, lightPosInCamera.z );
	m_meshShaderDiffuse.setVector3Uniform("diffuseColor", 0.5, 0.5, 0.5);
    m_meshShaderDiffuse.setVector3Uniform("lightcolor", 0.8, 0.8, 0.8);
	
    draw_object(m_meshShaderDiffuse, m_Scene);
    
    m_meshShaderDiffuse.unbind();
    
    // Unit box in world coordinates
    Vector3 unitBox[8];
    int boxHeight = 5;
    int boxWidth = 5;
    int boxHeightStart = -3;
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
    
    
    // Draw the camera-aligned box
    /*glColor3f(1.0, 0.0, 0.0);
    draw_cube(m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[0],
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[1],
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[2],
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[3],
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[4],
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[5],
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[6],
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * unitBox[7]);*/
    
    // Compute the min/max value for the world aligned box
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
    
    // Draw the world-aligned box
    
    //glColor3f(1.0, 1.0, 1.0);
    
    // (Vector3 topfrontleft, Vector3 topfrontright, Vector3 bottomfrontleft, Vector3 bottomfrontright, Vector3 topbackleft, Vector3 topbackright, Vector3 bottombackleft, Vector3 bottombackright)
    /*draw_cube(m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(minPos.x, maxPos.y, minPos.z),
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(maxPos.x, maxPos.y, minPos.z),
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(minPos.x, minPos.y, minPos.z),
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(maxPos.x, minPos.y, minPos.z),
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(minPos.x, maxPos.y, maxPos.z),
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(maxPos.x, maxPos.y, maxPos.z),
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(minPos.x, minPos.y, maxPos.z),
              m_camera.getProjectionMatrix() * m_camera.getTransformation().Inverse() * Vector3(maxPos.x, minPos.y, maxPos.z));
     */
    
    // Draw the storms
    if (drawStorms) {
        draw_storms();
    }
    
    // Draw the axis
    /*
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
    
    glEnd();*/
    

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (initPart) {
        m_meshShaderParticle.bind();
        Object3D* billboard = new Object3D();
        billboard->rotateObject(Vector3(0, 1, 0), -m_camera.getAngleY());
        billboard->rotateObject(Vector3(1, 0, 0), -m_camera.getAngleX());
        m_meshShaderParticle.setMatrix4x4Uniform("billboard", billboard->getTransformation());
        m_meshShaderParticle.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
        m_meshShaderParticle.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
        
        delete billboard;
        texture_snowflake->bind();
        m_meshShaderParticle.setIntUniform("texture",texture_snowflake->getLayer());
        
    
        if(isWatchOn)
        {
            for (unsigned int i = 0; i < storms.size(); ++i) {
                storms[i]->updatePosition(minPos, maxPos, deltaTime);
            }
        }
    
        double max = MAX_PARTICLES;
        for (int i = 0; i < max; i++)
        {
            Snowflake* flake = particles[i];
            // Update flake's position if the watch is on
            if (isWatchOn)
            {
                flake->updatePosition(deltaTime, minPos, maxPos, storms);
            }
            m_meshShaderParticle.setVector3Attribute("pos", flake->pos.x, flake->pos.y, flake->pos.z);
            // Draw the flakes
            glBegin(GL_QUADS);
            if (altMode) {
                glTexCoord2f(0, 1);
            } else {
                glTexCoord2f((flake->texture % 4) * 0.25, 1.0 - (flake->texture / 4) * 0.25);
            }
            glVertex3f(-flake->size*ratio, flake->size*ratio, 0);
            if (altMode) {
                glTexCoord2f(0, 0);
            } else {
                glTexCoord2f((flake->texture % 4) * 0.25,  0.75 - (flake->texture / 4) * 0.25);
            }
            glVertex3f(-flake->size*ratio, -flake->size*ratio, 0);
            if (altMode) {
                glTexCoord2f(1, 0);
            } else {
                glTexCoord2f((flake->texture % 4) * 0.25 + 0.25, 0.75 - (flake->texture / 4) * 0.25);
            }
            glVertex3f(flake->size*ratio, -flake->size*ratio, 0);
            if (altMode) {
                glTexCoord2f(1, 1);
            } else {
                glTexCoord2f((flake->texture % 4) * 0.25 + 0.25, 1.0 - (flake->texture / 4) * 0.25);
            }
            glVertex3f(flake->size*ratio, flake->size*ratio, 0);
            glEnd();
        }
        texture_snowflake->unbind();
        m_meshShaderParticle.unbind();
        
    }
    glDisable(GL_BLEND);
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

void SnowStormSimulation::draw_storms()
{
    for (unsigned int i = 0; i < storms.size(); ++i) {
        Object3D* tmp = new Object3D();
        tmp->translateWorld(storms[i]->pos);
        tmp->rotateObject(Vector3(1, 0, 0), -M_PI/2.0);
        m_meshShaderProj.bind();
        m_meshShaderProj.setMatrix4x4Uniform("modelworld", tmp->getTransformation());
        m_meshShaderProj.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
        m_meshShaderProj.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
        GLUquadric* params = gluNewQuadric();
        gluQuadricDrawStyle(params,GLU_LINE);
        gluCylinder(params, 0, storms[i]->radius, storms[i]->height, 20, 1);
        m_meshShaderProj.unbind();
        delete tmp;
    }

}

float SnowStormSimulation::randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


void SnowStormSimulation::update_storms_positions(Vector3 minPos, Vector3 maxPos)
{
    for(unsigned int i = 0; i < storms.size(); ++i){
        Cone *storm = storms[i];
        storm->pos = storm->pos + deltaTime * storm->velocity + 0.5 * storm->acceleration * deltaTime*deltaTime;
        if(storm->pos.x > maxPos.x || storm->pos.x < minPos.x ||
           storm->pos.z > maxPos.z || storm->pos.z < minPos.z)
        {
            storm->pos = Vector3(randomFloat(minPos.x, maxPos.x), 0, randomFloat(minPos.z, maxPos.z));
        }

    }
}
void SnowStormSimulation::load_mesh(const std::string& filenameObj) {
    Mesh3DReader::read( filenameObj, m_Scene);
    
    // calculate normals
    if(!m_Scene.hasNormals())
        m_Scene.calculateVertexNormals();
    
    m_Scene.translateWorld(Vector3(0, 0, 0));
    
    m_Scene.scaleTextureCoord(7);
    
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
    initPart = true;
    double max = MAX_PARTICLES;
    for (unsigned int j = 0; j < max; j++)
    {
        particles.push_back(new Snowflake());
    }
}



void
SnowStormSimulation::draw_sky()
{
    float size = 50;
    // front quad
    m_meshShaderSky.bind();
    m_meshShaderSky.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
    m_meshShaderSky.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
    texture_sky.bind();
    m_meshShaderSky.setIntUniform("cubeMap",texture_sky.getLayer());
    
    glBegin(GL_QUADS);
    glVertex3f(  -size, size, -size );
    glVertex3f( -size, -size, -size );
    glVertex3f( size,  -size, -size );
    glVertex3f(  size,  size, -size );
    
    // Render the left quad
    glVertex3f(  -size, size, size );
    glVertex3f(  -size,  -size, size );
    glVertex3f( -size, -size, -size );
    glVertex3f( -size, size, -size );
    
    // Render the back quad
    glVertex3f(  -size, size, size );
    glVertex3f( -size, -size, size );
    glVertex3f( size,  -size, size );
    glVertex3f(  size,  size, size );
    
    
    // Render the right quad
    glVertex3f(  size, size, -size );
    glVertex3f( size, -size, -size );
    glVertex3f( size,  -size, size );
    glVertex3f(  size,  size, size );
    
    // Render the top quad
    glVertex3f(  -size, size, -size );
    glVertex3f( -size, size, size );
    glVertex3f( size,  size, size );
    glVertex3f(  size,  size, -size );
 
    glEnd();
    
    texture_sky.unbind();
    m_meshShaderSky.unbind();

    
}

//=============================================================================
