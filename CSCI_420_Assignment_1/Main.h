/////////////////////////////////////////////
//Name: Shreyansh Daga
//CSCI 420 Submission 1
//Date: 2/10/2014
//USC ID: 6375334833
//Email: sdaga@usc.edu
/////////////////////////////////////////////


#ifndef _MAIN_H
#define _MAIN_H

	//Headers
	#include<stdio.h>
	#include<conio.h>
	#include<stdlib.h>
	#include<string.h>
	#include<iostream>
	#include<fstream>
	#include<Windows.h>
	#include<gl\GL.h>	
	#include<gl\glut.h>
	#include"pic.h"
	#include<string>
	
	using namespace std;
	
	//Prototypes for misc functions
	void DisplayVertex();
	void DisplayQuads();
	void DisplayTriangles();
	void DisplayTriangleStrip();
	void DisplayWireFrame();
	void DisplayWireFrameOverlay();
	void DisplayFrustumVolume();
	void Cube();
	void DrawSun();
	void SaveFrame(char *pszTitle);
	void DisplayText(string strModeName);

#endif