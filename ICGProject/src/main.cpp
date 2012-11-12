//=============================================================================
//                                                                            
//   Exercise code for the lecture
//                                                                            
//=============================================================================

#include "SnowStormSimulation.h"



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	
	SnowStormSimulation window("Snow Storm Simulation", 800, 800);
	
	glutMainLoop();
}
