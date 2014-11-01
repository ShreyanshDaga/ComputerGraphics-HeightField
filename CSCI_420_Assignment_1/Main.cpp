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

//Global Image Objects - this encapsulates the Pic structure
Image *pimCurrent;
Image imSpiral(string::basic_string("spiral.jpg"),256,256,1);
Image imSantaMonica(string::basic_string("SantaMonicaMountains-256.jpg"),256,256,1);
Image imOhioPyle(string::basic_string("OhioPyle-256.jpg"),256,256,1);
Image imGrandTeton(string::basic_string("GrandTeton-256.jpg"),256,256,1);
Image imEarth(string::basic_string("color-256.jpg"),256,256,3);

//Entry Point
int main(int argc, char* argv[])
{	
	//Read all the Image files
	imSpiral.ReadImage();
	imSantaMonica.ReadImage();
	imOhioPyle.ReadImage();
	imGrandTeton.ReadImage();
	imEarth.ReadImage();

	GlutWinObj glWin1(640,480,100,100);

	pimCurrent = &imSpiral; //Assign current image, which is to be displayed to be the spiral image

	//Init the GlutWinObj and register all the callbacks
	glWin1.InitGlut(argc,argv);
	glWin1.GlutCreateWindow("OPENGL_ASSIGNMENT !");		
	glWin1.Reg_Callbacks_Prime();

	//Main Loop start
	glWin1.StartGlutLoop();	

	cout<<"\nOpenGL Code exited the program..";
	return 0;
}