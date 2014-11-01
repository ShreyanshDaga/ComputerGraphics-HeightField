/////////////////////////////////////////////
//Name: Shreyansh Daga
//CSCI 420 Submission 1
//Date: 2/10/2014
//USC ID: 6375334833
//Email: sdaga@usc.edu
/////////////////////////////////////////////


#include"Main.h"
#include"glutWinObj.h"
#include"pic.h"
#include"Image.h"

using namespace std;

//Stores the prev value of mouse points
int iPrevX, iPrevY;

//Image Objects from main.cpp
extern Image *pimCurrent;
extern Image imSpiral;
extern Image imSantaMonica;
extern Image imOhioPyle;
extern Image imGrandTeton;
extern Image imEarth;

//Initializing all the GlutWinObj State variables
int GlutWinObj::iMenuID=0;
	
float GlutWinObj::fZMultiplier = 1;

float GlutWinObj::fTranX = 0;
float GlutWinObj::fTranY = 0;
float GlutWinObj::fTranZ = 0;

float GlutWinObj::fRotX = 0;
float GlutWinObj::fRotY = 0;
float GlutWinObj::fRotZ = 0;
	
float GlutWinObj::fScaleX = 1;
float GlutWinObj::fScaleY = 1;
float GlutWinObj::fScaleZ = 1;
	
unsigned int GlutWinObj::cRenderMode = 1;
int GlutWinObj::iMode = 0;

bool GlutWinObj::bToggleSun = true;
bool GlutWinObj::bToggleDefAnim = true;
bool GlutWinObj::bRecAnim = false;

//Ctor for GLutWinObj
GlutWinObj::GlutWinObj(int iW, int iH, int iX, int iY)
{
	//Fill up values
	this->iHeight = iH;
	this->iWidth = iW;
	this->iPosX = iX;
	this->iPosY = iY;

	//Init Handle to 0
	this->iWinHandle = 0;
	this->bGlutReady = false;
}

void GlutWinObj::InitGlut(int argc, char* argv[])
{
	//Perform all the initialization tasks
	glutInit(&argc,(char**)argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(this->iWidth, this->iHeight);
	glutInitWindowPosition(this->iPosX, this->iPosY);	
}

int GlutWinObj::GlutCreateWindow(const char *pszTitle)
{
	//Create window here and also set required flags
	this->iWinHandle = glutCreateWindow_ATEXIT_HACK(pszTitle);

	glClearColor(0.0,0.0,0.0,0.0);	
	glEnable(GL_DEPTH_TEST);	
	glShadeModel(GL_SMOOTH);

	//glutFullScreen();
	//Set this flag to be true, so that rendering can begin
	this->bGlutReady = true;

	return this->iWinHandle;
}

void GlutWinObj::StartGlutLoop()
{
	if(this->bGlutReady)
	{
		cout<<"\n* glut Main Loop started..";
		glutMainLoop();	
	}
}

void GlutWinObj::Reg_Callbacks_Prime()
{
	//Register all the callbacks
	this->RegCallback_RenderDisplay();
	this->RegCallback_HandleMouse();
	this->RegCallback_HandleMouseDrag();
	this->RegCallback_HandleMouseIdle();
	this->RegCallback_HandleReshape();
	this->RegCallback_Idle();
	this->RegCallback_HandleKeyboard();
	this->RegCallback_HandleSpecialKey();
	this->InitContextMenu();
}

void GlutWinObj::RegCallback_RenderDisplay()
{
	glutDisplayFunc(&GlutWinObj::DisplayFunc);
}

void GlutWinObj::RegCallback_HandleMouse()
{
	glutMouseFunc(GlutWinObj::MouseFunc);
}

void GlutWinObj::RegCallback_HandleMouseDrag()
{
	glutMotionFunc(GlutWinObj::MouseDragFunc);
}

void GlutWinObj::RegCallback_HandleMouseIdle()
{
	glutPassiveMotionFunc(GlutWinObj::MouseIdleFunc);
}

void GlutWinObj::RegCallback_HandleKeyboard()
{
	glutKeyboardFunc(GlutWinObj::KeyboardFunc);
}

void GlutWinObj::RegCallback_HandleSpecialKey()
{
	glutSpecialFunc(GlutWinObj::SpecialKeyFunc);
}

void GlutWinObj::RegCallback_Idle()
{
	glutIdleFunc(GlutWinObj::IdleFunc);
}

void GlutWinObj::RegCallback_HandleReshape()
{
	glutReshapeFunc(GlutWinObj::ReshapeFunc);
}

void GlutWinObj::InitContextMenu()
{
	iMenuID = glutCreateMenu(GlutWinObj::ContextMenuFunc);
	glutSetMenu(iMenuID);	
	glutAddMenuEntry("-------------------------",-1);
	glutAddMenuEntry("Spiral - 256",1);
	glutAddMenuEntry("Santa Monica Mountains - 256",2);
	glutAddMenuEntry("Ohio Pyle - 256",3);
	glutAddMenuEntry("Grand Tenot - 256",4);
	glutAddMenuEntry("Color Earth - 256",5);
	glutAddMenuEntry("-------------------------",-1);
	glutAddMenuEntry("Toggle Default Animation",6);
	glutAddMenuEntry("Record Animation",8);
	glutAddMenuEntry("Toggle Sun",7);
	glutAddMenuEntry("Toggle Shadow",9);
	glutAddMenuEntry("-------------------------",-1);
	glutAddMenuEntry("Reset Scene",10);
	glutAddMenuEntry("Quit",0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void GlutWinObj::DisplayFunc()
{		
	static float alpha = 0.0;	
	static int iFrameCount = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();	
	
	//Translate a bit behind so that the object is visible
	glTranslatef(0,0,-1.8);

	//Perform ops based on the GlutWinObj's state variables
	glTranslatef(fTranX,fTranY,fTranZ);
	glRotatef(-fRotX,1,0,0);
	glRotatef(-fRotY,0,1,0);
	glRotatef(-fRotZ,0,0,1);
	glScalef(fScaleX,fScaleY,fScaleZ);

	//This op is for the default animation which is rotation
	glRotatef(alpha,0,0,1);
	
	//Select mode based on the user input
	if(cRenderMode == 1)
		DisplayTriangles();
	else if(cRenderMode == 2)
		DisplayWireFrame();
	else if(cRenderMode == 4)
		DisplayVertex();
	else if(cRenderMode == 8)
		DisplayWireFrameOverlay();
	else if(cRenderMode == 16)
		DisplayTriangleStrip();


	if(bToggleSun)
		DrawSun();
	
	//Updates for default Animation
	if(!bToggleDefAnim)
		;
	else 
		alpha = alpha + 2;						

	//DisplayText(string::basic_string("Mode 1"));	

	glFlush();
	glutSwapBuffers();	
}

void GlutWinObj::MouseFunc(int iButton, int iState, int iX, int iY)
{	
	//Update Operation Mode based on the user input on mouse input

	switch(glutGetModifiers())
	{
		case GLUT_ACTIVE_CTRL:
			//Translate
			iMode = 1;
			break;

		case GLUT_ACTIVE_SHIFT:
			//Scale
			iMode = 2;
			break;

		case GLUT_ACTIVE_ALT:
			//Deform in the Z direction
			iMode = 4;
			break;

		default:
			//Rotation
			iMode = 3;
	}

	iPrevX = iX;
	iPrevY = iY;
}

void GlutWinObj::MouseDragFunc(int iX, int iY)
{
	int iDeltaX = iX - iPrevX, iDeltaY = iY - iPrevY;

	switch(iMode)
	{
		case 1:	//Translate in Z and X direction
			fTranZ += iDeltaY*0.01;
			fTranX += iDeltaX*0.01;
			break;

		case 3:	//Rotate in Y and X direction
			fRotX -= iDeltaY;
			fRotZ -= iDeltaX;
			break;

		case 2:	//Scale uniformly by just DeltaX
			fScaleX *= 1.0 + iDeltaX*0.01;
			fScaleY *= 1.0 + iDeltaX*0.01;//((float)iDeltaX)/sqrt(iDeltaX*iDeltaX)*sqrt(iDeltaX*iDeltaX + iDeltaY*iDeltaY)*0.01;
			fScaleZ *= 1.0 + iDeltaX*0.01;//((float)iDeltaX)/sqrt(iDeltaX*iDeltaX)*sqrt(iDeltaX*iDeltaX + iDeltaY*iDeltaY)*0.01;
			break;

		case 4:	//Deformation multiplier in the Z direction
			fZMultiplier *= 1.0 - iDeltaY*0.03;
			break;
	}

	//Capture Mouse movement
	iPrevX = iX;
	iPrevY = iY;
}

void GlutWinObj::MouseIdleFunc(int iX, int iY)
{
	//Capture Mouse movement
	iPrevX = iX;
	iPrevY = iY;
}

void GlutWinObj::KeyboardFunc(unsigned char cKey, int iX, int iY)
{	
	//Alternate ways of Translate and rotate in all three axes

	if(cKey == 'W')	//Shift + w
	{
		fRotX += 1;
	}
	else if(cKey == 'S')	//Shift + s
	{
		fRotX -= 1;
	}
	else if(cKey == 'A') // and like wise...
	{
		fRotY += 1;
	}
	else if(cKey == 'D')
	{
		fRotY -= 1;
	}
	else if(cKey == 'Q')
	{
		fRotZ += 1;
	}
	else if(cKey == 'E')
	{
		fRotZ -= 1;
	}
	else if(cKey == 'w')
	{
		fTranZ -= 0.1;
	}
	else if(cKey == 's')
	{
		fTranZ += 0.1;
	}
	else if(cKey == 'a')
	{
		fTranX -= 0.1;
	}
	else if(cKey == 'd')
	{
		fTranX += 0.1;
	}
	else if(cKey == 'q')
	{
		fTranY -= 0.1;
	}
	else if(cKey == 'e')
	{
		fTranY += 0.1;
	}	
	
	//Capture Mouse movement
	iPrevX = iX;
	iPrevY = iY;
}

void GlutWinObj::SpecialKeyFunc(int iKey, int iX, int iY)
{
	//Cyclicly rotate the rendering mode state variables

	switch(iKey)
	{
		case GLUT_KEY_RIGHT:
			//Switch Mode to right
			cRenderMode = cRenderMode << 1;
			if(cRenderMode > 16)
				cRenderMode = 1;
			break;
		
		case GLUT_KEY_LEFT:
			//Switch Mode to left
			cRenderMode = cRenderMode >> 1;
			if(cRenderMode < 1)
				cRenderMode = 16;
			break;

		case GLUT_KEY_END:
			//Start/Stop Animation
			if(bToggleDefAnim == true)
				bToggleDefAnim = false;
			else
				bToggleDefAnim = true;
			break;
	}

	//Capture Mouse movement
	iPrevX = iX;
	iPrevY = iY;
}

void GlutWinObj::IdleFunc()
{
	static int iFrameCount = 0;

	//Record the animation if user says to record the animations
	if(bRecAnim)
	{		
		if(iFrameCount < 300)
		{		
			char szTitle[10];

			sprintf(szTitle,"%d.jpg",iFrameCount);

			if(iFrameCount < 50)
			{
				pimCurrent = &imSpiral;
			}
			else if(iFrameCount > 50 && iFrameCount < 100)
			{
				pimCurrent = &imOhioPyle;
			}
			else if(iFrameCount > 100 && iFrameCount <150)
			{
				GlutWinObj::cRenderMode = 1;

				pimCurrent = &imSantaMonica;
			}
			else if(iFrameCount > 150 && iFrameCount <200)
			{
				GlutWinObj::cRenderMode = 2;
			}
			else if(iFrameCount > 200 && iFrameCount <250)
			{
				GlutWinObj::cRenderMode = 4;
				pimCurrent = &imEarth;
			}
			else if(iFrameCount > 250 && iFrameCount < 300)
			{
				GlutWinObj::cRenderMode = 8;
				pimCurrent = &imEarth;
			}

			SaveFrame(szTitle);
			iFrameCount++;
		}
	}

	glutPostRedisplay();
}

void GlutWinObj::ReshapeFunc(int iW, int iH)
{
	//Adjust according to new window size
	glViewport(0, 0, iW, iH);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(60, ((float)iW)/((float) iH), 0.1, 20);	
	//glFrustum(-1.0,1.0,-1.0,1.0,1,10);
	//glOrtho(-1.0,1.0,-1.0,1.0,1,100);
		
	glMatrixMode(GL_MODELVIEW);	
}

void GlutWinObj::ContextMenuFunc(int iVal)
{
	//Context menu selections
	switch(iVal)
	{
		case 0:
			exit(0);
			break;

			//THese change the image to be dispplayed 
		case 1:
			pimCurrent = &imSpiral;
			break;

		case 2:
			pimCurrent = &imSantaMonica;
			break;

		case 3:
			pimCurrent = &imOhioPyle;
			break;

		case 4:
			pimCurrent = &imGrandTeton;
			break;

		case 5:
			pimCurrent = &imEarth;
			break;

		case 6:
			bToggleDefAnim = (bToggleDefAnim == false) ? true : false;
			break;

		case 7:
			bToggleSun = (bToggleSun == false) ? true : false;
			break;

		case 8:
			bRecAnim = (bRecAnim == false) ? true : false;
			break;

		case 9:
			//Couldn implement shadow
			break;

		case 10:
			//This resets the scene to original parameters
			ResetGlutWinParam();
			break;
	}
}

void GlutWinObj::ResetGlutWinParam()
{
	//Reset all the parameters to original vaues
	
	iMenuID;
	
	fZMultiplier = 1.0;

	fTranX = 0;
	fTranY = 0;
	fTranZ = 0;
	
	fRotX = 0;
	fRotY = 0;
	fRotZ = 0;
	
	fScaleX = 1;
	fScaleY = 1;
	fScaleZ = 1;
	
	cRenderMode = 1;
	iMode;
}

void Cube()
{
	static float alpha = 0;

	glTranslatef(0, 0, -3);
	glRotatef(-30,1, 0,0);
	glRotatef(alpha, 0, 1, 0);

	glBegin(GL_QUADS);
		glColor3f(0, 0, 1); glVertex3f(-1, -1, -1);
		glColor3f(0, 0, 1); glVertex3f(-1, -1,  1);
		glColor3f(0, 0, 1); glVertex3f(-1,  1,  1);
		glColor3f(0, 0, 1); glVertex3f(-1,  1, -1);

		glColor3f(1, 0, 0); glVertex3f( 1, -1, -1);
		glColor3f(1, 0, 0); glVertex3f( 1, -1,  1);
		glColor3f(1, 0, 0); glVertex3f( 1,  1,  1);
		glColor3f(1, 0, 0); glVertex3f( 1,  1, -1);

		glColor3f(0, 1, 0); glVertex3f(-1, -1, -1);
		glColor3f(0, 1, 0); glVertex3f(-1, -1,  1);
		glColor3f(0, 1, 0); glVertex3f( 1, -1,  1);
		glColor3f(0, 1, 0); glVertex3f( 1, -1, -1);

		glColor3f(0, 1, 1); glVertex3f(-1,  1, -1);
		glColor3f(0, 1, 1); glVertex3f(-1,  1,  1);
		glColor3f(0, 1, 1); glVertex3f( 1,  1,  1);
		glColor3f(0, 1, 1); glVertex3f( 1,  1, -1);

		glColor3f(1, 1, 0); glVertex3f(-1, -1, -1);
		glColor3f(1, 1, 0); glVertex3f(-1,  1, -1);
		glColor3f(1, 1, 0); glVertex3f( 1,  1, -1);
		glColor3f(1, 1, 0); glVertex3f( 1, -1, -1);

		glColor3f(1, 0, 1); glVertex3f(-1, -1,  1);
		glColor3f(1, 0, 1); glVertex3f(-1,  1,  1);
		glColor3f(1, 0, 1); glVertex3f( 1,  1,  1);
		glColor3f(1, 0, 1); glVertex3f( 1, -1,  1);
	glEnd();	
}

void DisplayQuads()
{/*
	for(int i = 255;i>=0;i--)
		{
			for(int j = 0;j<255;j++)
			{
				glBegin(GL_QUADS);
				Pixel1 pxP = PIC_PIXEL(pImageData,i,j,0);
				float pZ = (float) pxP;

				glColor3f(pZ/256.00,pZ/256.00,1);
				glVertex3f(i/128.00 - 1,j/128.00-1,pZ/512.00);

				pxP = PIC_PIXEL(pImageData,i,j+1,0);
				pZ = (float) pxP;

				glColor3f(pZ/256.00,pZ/256.00,1);
				glVertex3f(i/128.00 - 1,(j+1)/128.00-1,pZ/512.00);

				pxP = PIC_PIXEL(pImageData,i+1,j+1,0);
				pZ = (float) pxP;

				glColor3f(pZ/256.00,pZ/256.00,1);
				glVertex3f((i+1)/128.00 - 1,(j+1)/128.00-1,pZ/512.00);

				pxP = PIC_PIXEL(pImageData,i+1,j,0);
				pZ = (float) pxP;

				glColor3f(pZ/256.00,pZ/256.00,1);
				glVertex3f((i+1)/128.00 - 1,j/128.00-1,pZ/512.00);

				glEnd();
			}
		}
*/}

void DisplayTriangleStrip()
{
	//Display the image in triangle strip mode
	//However it is not working correctly, i cant figure out why, i tried all combinations of the 4 vertices

	glBegin(GL_TRIANGLE_STRIP);	
	for(int i = 0;i<255;i++)
	{
		for(int j = 0;j<255;j++)
		{
			Pixel1 pxPT1 = pimCurrent->GetBW((int)i,(int)j);
			float fZ1 = ((float) pxPT1)/255.00;

			Pixel1 pxPT2 = pimCurrent->GetBW((int)i,(int)j+1);
			float fZ2 = ((float) pxPT2)/255.00;

			Pixel1 pxPT3 = pimCurrent->GetBW((int)i+1,(int)j);
			float fZ3 = ((float) pxPT3)/255.00;

			Pixel1 pxPT4 = pimCurrent->GetBW((int)i+1,(int)j+1);
			float fZ4 = ((float) pxPT4)/255.00;
			
			if(pimCurrent->GetType() == 1)
				glColor3f(fZ1,fZ1,1);	//Smooth gradient in the hue of blue
			else
				glColor3f(((float)(pimCurrent->GetR(i,j))/255.00),((float)(pimCurrent->GetG(i,j))/255.00),((float)(pimCurrent->GetB(i,j))/255.00));
			glVertex3f((float)i/128.00 - 1.0,(float)j/128.00 - 1.0,fZ1/4.00*GlutWinObj::fZMultiplier);								
			
			if(pimCurrent->GetType() == 1)
				glColor3f(fZ2,fZ2,1);	//Smooth gradient in the hue of blue
			else
				glColor3f(((float)(pimCurrent->GetR(i,j+1))/255.00),((float)(pimCurrent->GetG(i,j+1))/255.00),((float)(pimCurrent->GetB(i,j+1))/255.00));
			glVertex3f((float)i/128.00 - 1.0,(float)(j+1)/128.00 - 1.0,fZ2/4.00*GlutWinObj::fZMultiplier);

			if(pimCurrent->GetType() == 1)
				glColor3f(fZ3,fZ3,1);	//Smooth gradient in the hue of blue
			else
				glColor3f(((float)(pimCurrent->GetR(i+1,j))/255.00),((float)(pimCurrent->GetG(i+1,j))/255.00),((float)(pimCurrent->GetB(i+1,j))/255.00));
			glVertex3f((float)(i+1)/128.00 - 1.0,(float)j/128.00 - 1.0,fZ3/4.00*GlutWinObj::fZMultiplier);								

			if(pimCurrent->GetType() == 1)
				glColor3f(fZ4,fZ4,1);	//Smooth gradient in the hue of blue
			else
				glColor3f(((float)(pimCurrent->GetR(i+1,j+1))/255.00),((float)(pimCurrent->GetG(i+1,j+1))/255.00),((float)(pimCurrent->GetB(i+1,j+1))/255.00));
			glVertex3f((float)(i+1)/128.00 - 1.0,(float)(j+1)/128.00 - 1.0,fZ4/4.00*GlutWinObj::fZMultiplier);			
		}
	}						
	glEnd();
}

void DisplayTriangles()
{
	//Displays the scene in solid triangles

	glBegin(GL_TRIANGLES);

	for(int i = 0;i<255;i++)
	{
		for(int j = 0;j<255;j = j++)
		{
			Pixel1 pxPT1 = pimCurrent->GetBW((int)i,(int)j);
			float fZ1 = ((float) pxPT1)/255.00;

			Pixel1 pxPT2 = pimCurrent->GetBW((int)i,(int)j+1);
			float fZ2 = ((float) pxPT2)/255.00;

			Pixel1 pxPT3 = pimCurrent->GetBW((int)i+1,(int)j);
			float fZ3 = ((float) pxPT3)/255.00;

			Pixel1 pxPT4 = pimCurrent->GetBW((int)i+1,(int)j+1);
			float fZ4 = ((float) pxPT4)/255.00;
			
			if(pimCurrent->GetType() == 1)
				glColor3f(fZ1,fZ1,1);	//Smooth gradient in the hue of blue
			else
				glColor3f(((float)(pimCurrent->GetR(i,j))/255.00),((float)(pimCurrent->GetG(i,j))/255.00),((float)(pimCurrent->GetB(i,j))/255.00));
			glVertex3f((float)i/128.00 - 1.0,(float)j/128.00 - 1.0,fZ1/4.00*GlutWinObj::fZMultiplier);
			if(pimCurrent->GetType() == 1)
				glColor3f(fZ2,fZ2,1);
			else
				glColor3f(((float)(pimCurrent->GetR(i,j+1))/255.00),((float)(pimCurrent->GetG(i,j+1))/255.00),((float)(pimCurrent->GetB(i,j+1))/255.00));
			glVertex3f((float)i/128.00 - 1.0,(float)(j+1)/128.00 - 1.0,fZ2/4.00*GlutWinObj::fZMultiplier);
			if(pimCurrent->GetType() == 1)
				glColor3f(fZ3,fZ3,1);
			else
				glColor3f(((float)(pimCurrent->GetR(i+1,j))/255.00),((float)(pimCurrent->GetG(i+1,j))/255.00),((float)(pimCurrent->GetB(i+1,j))/255.00));
			glVertex3f((float)(i+1)/128.00 - 1.0,(float)j/128.00 - 1.0,fZ3/4.00*GlutWinObj::fZMultiplier);
			
			if(pimCurrent->GetType() == 1)
				glColor3f(fZ2,fZ2,1);
			else
				glColor3f(((float)(pimCurrent->GetR(i,j+1))/255.00),((float)(pimCurrent->GetG(i,j+1))/255.00),((float)(pimCurrent->GetB(i,j+1))/255.00));
			glVertex3f((float)i/128.00 - 1.0,(float)(j+1)/128.00 - 1.0,fZ2/4.00*GlutWinObj::fZMultiplier);
			if(pimCurrent->GetType() == 1)
				glColor3f(fZ3,fZ3,1);
			else
				glColor3f(((float)(pimCurrent->GetR(i+1,j))/255.00),((float)(pimCurrent->GetG(i+1,j))/255.00),((float)(pimCurrent->GetB(i+1,j))/255.00));
			glVertex3f((float)(i+1)/128.00 - 1.0,(float)j/128.00 - 1.0,fZ3/4.00*GlutWinObj::fZMultiplier);
			if(pimCurrent->GetType() == 1)
				glColor3f(fZ4,fZ4,1);
			else
				glColor3f(((float)(pimCurrent->GetR(i+1,j+1))/255.00),((float)(pimCurrent->GetG(i+1,j+1))/255.00),((float)(pimCurrent->GetB(i+1,j+1))/255.00));
			glVertex3f((float)(i+1)/128.00 - 1.0,(float)(j+1)/128.00 - 1.0,fZ4/4.00*GlutWinObj::fZMultiplier);
		}
	}
	glEnd();
}

void DisplayWireFrame()
{
	//Verical and horizontal wire mesh

	glBegin(GL_LINES);
	for(int i = 0;i<256;i++)
	{
		for(int j = 0;j<255;j++)
		{
			Pixel1 pxP1 = pimCurrent->GetBW(i,j);
			float fZ1 = ((float) pxP1)/255.00;

			Pixel1 pxP2 = pimCurrent->GetBW(i,j+1);
			float fZ2 = ((float) pxP2)/255.00;

			if(pimCurrent->GetType() == 1)
				glColor3f(fZ1,fZ1,1);//Smooth gradient in the hue of blue
			else
				glColor3f(((float)(pimCurrent->GetR(i,j))/255.00),((float)(pimCurrent->GetG(i,j))/255.00),((float)(pimCurrent->GetB(i,j))/255.00));
			glVertex3f((float)i/128.00 - 1.0, (float)j/128.00 - 1.0, fZ1/4.00*GlutWinObj::fZMultiplier);
			if(pimCurrent->GetType() == 1)
				glColor3f(fZ2,fZ2,1);
			else
				glColor3f(((float)(pimCurrent->GetR(i,j+1))/255.00),((float)(pimCurrent->GetG(i,j+1))/255.00),((float)(pimCurrent->GetB(i,j+1))/255.00));
			glVertex3f((float)i/128.00 - 1.0, (float)(j+1)/128.00 - 1.0, fZ2/4.00*GlutWinObj::fZMultiplier);
		}	
	}

	for(int i = 0;i<255;i++)
	{
		for(int j = 0;j<256;j++)
		{
			Pixel1 pxP1 = pimCurrent->GetBW(i,j);
			float fZ1 = ((float) pxP1)/255.00;

			Pixel1 pxP2 = pimCurrent->GetBW(i+1,j);
			float fZ2 = ((float) pxP2)/255.00;

			if(pimCurrent->GetType() == 1)
				glColor3f(fZ1,fZ1,1);
			else
				glColor3f(((float)(pimCurrent->GetR(i,j))/255.00),((float)(pimCurrent->GetG(i,j))/255.00),((float)(pimCurrent->GetB(i,j))/255.00));
			glVertex3f((float)i/128.00 - 1.0, (float)j/128.00 - 1.0, fZ1/4.00*GlutWinObj::fZMultiplier);
			if(pimCurrent->GetType() == 1)
				glColor3f(fZ2,fZ2,1);
			else
				glColor3f(((float)(pimCurrent->GetR(i+1,j))/255.00),((float)(pimCurrent->GetG(i+1,j))/255.00),((float)(pimCurrent->GetB(i+1,j))/255.00));
			glVertex3f((float)(i+1)/128.00 - 1.0, (float)(j)/128.00 - 1.0, fZ2/4.00*GlutWinObj::fZMultiplier);
		}
	}
	glEnd();
}

void DisplayWireFrameOverlay()
{
	glEnable (GL_POLYGON_OFFSET_FILL);	
	DisplayTriangles();		//First Display the solid triangles

	glPolygonOffset(1.0,1.0);

	glBegin(GL_LINES);		//Then display the overlay mesh with black edges
	glColor3f(0,0,0);			
	
	for(int i = 0;i<256;i++)
	{
		for(int j = 0;j<255;j++)
		{
			Pixel1 pxP1 = pimCurrent->GetBW(i,j);
			float fZ1 = ((float) pxP1)/255.00;

			Pixel1 pxP2 = pimCurrent->GetBW(i,j+1);
			float fZ2 = ((float) pxP2)/255.00;			
				
			glVertex3f((float)i/128.00 - 1.0, (float)j/128.00 - 1.0, fZ1/4.00*GlutWinObj::fZMultiplier);			
			glVertex3f((float)i/128.00 - 1.0, (float)(j+1)/128.00 - 1.0, fZ2/4.00*GlutWinObj::fZMultiplier);
		}	
	}

	for(int i = 0;i<255;i++)
	{
		for(int j = 0;j<256;j++)
		{
			Pixel1 pxP1 = pimCurrent->GetBW(i,j);
			float fZ1 = ((float) pxP1)/255.00;

			Pixel1 pxP2 = pimCurrent->GetBW(i+1,j);
			float fZ2 = ((float) pxP2)/255.00;
			
			glVertex3f((float)i/128.00 - 1.0, (float)j/128.00 - 1.0, fZ1/4.00*GlutWinObj::fZMultiplier);			
			glVertex3f((float)(i+1)/128.00 - 1.0, (float)(j)/128.00 - 1.0, fZ2/4.00*GlutWinObj::fZMultiplier);
		}
	}
	glEnd();
	glDisable(GL_POLYGON_OFFSET_FILL);
}

void DisplayVertex()
{
	//Display the scene in points

	Pixel1 pxP;
	float fZ = 0.00;
	
	glBegin(GL_POINTS);
	for(int i = 0;i<256;i++)
	{
		for(int j = 0;j<256;j++)
		{
			pxP = pimCurrent->GetBW(i,j);
			fZ = (float) pxP;

			if(pimCurrent->GetType() == 1)
				glColor3f(fZ/255.00,fZ/255.00,1.0);
			else
				glColor3f(((float)(pimCurrent->GetR(i,j))/255.00),((float)(pimCurrent->GetG(i,j))/255.00),((float)(pimCurrent->GetB(i,j))/255.00));
			glVertex3f((float)i/128.00 - 1.0,(float)j/128.00 - 1.0,(fZ/1024.00)*GlutWinObj::fZMultiplier);
		}
	}
	glEnd();
}

void DisplayFrustumVolume()
{
	glPushMatrix();
	glLoadIdentity();

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0,1.0,1.0);
	glVertex3f(-1,1,-1.1);
	glVertex3f(-1,-1,-1.1);
	glVertex3f(1,-1,-1.1);
	glVertex3f(1,1,-1.1);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(-1,1,-99.9);
	glVertex3f(-1,-1,-99.9);
	glVertex3f(1,-1,-99.9);
	glVertex3f(1,1,-99.9);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(1,1,-1.1);
	glVertex3f(1,1,-99.9);
	glVertex3f(1,-1,-99.9);
	glVertex3f(1,-1,-1.1);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(-1,1,-1.1);
	glVertex3f(-1,1,-99.9);
	glVertex3f(-1,-1,-99.9);
	glVertex3f(-1,-1,-1.1);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(-1,1,-1.1);
	glVertex3f(1,1,-1.1);
	glVertex3f(1,1,-99.9);
	glVertex3f(-1,1,-99.9);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(-1,-1,-1.1);
	glVertex3f(1,-1,-1.1);
	glVertex3f(1,-1,-99.9);
	glVertex3f(-1,-1,-99.9);
	glEnd();

	glPopMatrix();
}

void DrawSun()
{	
	glPushMatrix();
	glTranslatef(0.9,0.9,+0.4);
	glPushAttrib(GL_COLOR);
	glColor3f(1,1,0);
	
	if(GlutWinObj::cRenderMode == 1)
	{
		glutSolidSphere(0.07,10,10);
	}
	else if(GlutWinObj::cRenderMode == 2)
	{
		glutWireSphere(0.07,10,10);
	}
	else if(GlutWinObj::cRenderMode == 4)
	{
		glutWireSphere(0.07,100,100);
	}
	glPopMatrix();
	glPopAttrib();
}

void SaveFrame(char *pszTitle)
{
	//Saves the frame
	int i, j;
	Pic *in = NULL;

	 if (pszTitle == NULL)
		return;

	/* Allocate a picture buffer */
	in = pic_alloc(640, 480, 3, NULL);

	printf("File to save to: %s\n", pszTitle);

	for (i=479; i>=0; i--) 
	{
		glReadPixels(0, 479-i, 640, 1, GL_RGB, GL_UNSIGNED_BYTE, &in->pix[i*in->nx*in->bpp]);
	}

	if (jpeg_write(pszTitle, in))
		printf("File saved Successfully\n");
	else
		printf("Error in Saving\n");

	pic_free(in);
}

/*
void DisplayText(string strModeName)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	gluOrtho2D(0.0,640,0.00,480);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 0.0f);
    glRasterPos2i(40, 40);    
	void * font = GLUT_BITMAP_HELVETICA_18;

	for (std::string::iterator i = strModeName.begin(); i != strModeName.end(); ++i)
    {
        char c = *i;        
        glutBitmapCharacter(font, c);
    }

	glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	//glEnable(GL_TEXTURE_2D);
}*/
