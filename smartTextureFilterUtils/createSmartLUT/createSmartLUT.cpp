// createSmartLUT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../IL/il.h"
#include <cstdlib>
#include <memory>

#define RGBA(r,g,b,a) (((a)<<24) | ((b)<<16) | ((g)<<8) | (r))

const int w = 8;
const int h = 16;

int main(int argc, char* argv[])
{
	ilInit();

	unsigned int *pData = (unsigned int*)malloc(w*h*4);
	memset(pData, 0, w*h*4);

	// overall layout
	// R-----------A
	// | 0 / | \ 3 |
	// | / 4 | 7 \ |
	// |-----|-----|
	// | \ 5 | 6 / |
	// | 1 \ | / 2 |
	// G-----------B
	//

	// 0
	// *-------*
	// |       |
	// |       |
	// |       |
	// *-------*
	//
	pData[w*0+0] = RGBA(255,255,255,255);
	pData[w*0+1] = RGBA(255,255,255,255);
	pData[w*0+2] = RGBA(255,255,255,255);
	pData[w*0+3] = RGBA(255,255,255,255);
	pData[w*0+4] = RGBA(255,255,255,255);
	pData[w*0+5] = RGBA(255,255,255,255);
	pData[w*0+6] = RGBA(255,255,255,255);
	pData[w*0+7] = RGBA(255,255,255,255);

	// 1
	// *-------*
	// |       |
	// |-------|
	// |       |
	// *-------*
	//
	pData[w*1+0] = RGBA(255,0,0,255);
	pData[w*1+1] = RGBA(0,255,255,0);
	pData[w*1+2] = RGBA(0,255,255,0);
	pData[w*1+3] = RGBA(255,0,0,255);
	pData[w*1+4] = RGBA(255,0,0,255);
	pData[w*1+5] = RGBA(0,255,255,0);
	pData[w*1+6] = RGBA(0,255,255,0);
	pData[w*1+7] = RGBA(255,0,0,255);

	// 2
	// *-------*
	// |   |   |
	// |   |   |
	// |   |   |
	// *-------*
	//
	pData[w*2+0] = RGBA(255,255,0,0);
	pData[w*2+1] = RGBA(255,255,0,0);
	pData[w*2+2] = RGBA(0,0,255,255);
	pData[w*2+3] = RGBA(0,0,255,255);
	pData[w*2+4] = RGBA(255,255,0,0);
	pData[w*2+5] = RGBA(255,255,0,0);
	pData[w*2+6] = RGBA(0,0,255,255);
	pData[w*2+7] = RGBA(0,0,255,255);

	// 3
	// *-------*
	// |       |
	// |      /|
	// |    /  |
	// *-------*
	//
	pData[w*3+0] = RGBA(255,255,0,255);
	pData[w*3+1] = RGBA(255,255,0,255);
	pData[w*3+2] = RGBA(0,0,255,0);
	pData[w*3+3] = RGBA(255,255,0,255);
	pData[w*3+4] = RGBA(255,255,0,255);
	pData[w*3+5] = RGBA(255,255,0,255);
	pData[w*3+6] = RGBA(255,255,0,255);
	pData[w*3+7] = RGBA(255,255,0,255);

	// 4
	// *-------*
	// |       |
	// |\      |
	// |  \    |
	// *-------*
	//
	pData[w*4+0] = RGBA(255,0,255,255);
	pData[w*4+1] = RGBA(0,255,0,0);
	pData[w*4+2] = RGBA(255,0,255,255);
	pData[w*4+3] = RGBA(255,0,255,255);
	pData[w*4+4] = RGBA(255,0,255,255);
	pData[w*4+5] = RGBA(255,0,255,255);
	pData[w*4+6] = RGBA(255,0,255,255);
	pData[w*4+7] = RGBA(255,0,255,255);

	// 5
	// *-------*
	// |  /    |
	// |/      |
	// |       |
	// *-------*
	//
	pData[w*5+0] = RGBA(255,0,0,0);
	pData[w*5+1] = RGBA(0,255,255,255);
	pData[w*5+2] = RGBA(0,255,255,255);
	pData[w*5+3] = RGBA(0,255,255,255);
	pData[w*5+4] = RGBA(0,255,255,255);
	pData[w*5+5] = RGBA(0,255,255,255);
	pData[w*5+6] = RGBA(0,255,255,255);
	pData[w*5+7] = RGBA(0,255,255,255);

	// 6
	// *-------*
	// |    \  |
	// |      \|
	// |       |
	// *-------*
	//
	pData[w*6+0] = RGBA(255,255,255,0);
	pData[w*6+1] = RGBA(255,255,255,0);
	pData[w*6+2] = RGBA(255,255,255,0);
	pData[w*6+3] = RGBA(0,0,0,255);
	pData[w*6+4] = RGBA(255,255,255,0);
	pData[w*6+5] = RGBA(255,255,255,0);
	pData[w*6+6] = RGBA(255,255,255,0);
	pData[w*6+7] = RGBA(255,255,255,0);

	// 7
	// *-------*
	// |   |   |
	// |-------|
	// |       |
	// *-------*
	//
	pData[w*7+0] = RGBA(255,0,0,0);
	pData[w*7+1] = RGBA(0,255,255,0);
	pData[w*7+2] = RGBA(0,255,255,0);
	pData[w*7+3] = RGBA(0,0,0,255);
	pData[w*7+4] = RGBA(255,0,0,0);
	pData[w*7+5] = RGBA(0,255,255,0);
	pData[w*7+6] = RGBA(0,255,255,0);
	pData[w*7+7] = RGBA(0,0,0,255);
	
	// 8
	// *-------*
	// |       |
	// |-------|
	// |   |   |
	// *-------*
	//
	pData[w*8+0] = RGBA(255,0,0,255);
	pData[w*8+1] = RGBA(0,255,0,0);
	pData[w*8+2] = RGBA(0,0,255,0);
	pData[w*8+3] = RGBA(255,0,0,255);
	pData[w*8+4] = RGBA(255,0,0,255);
	pData[w*8+5] = RGBA(0,255,0,0);
	pData[w*8+6] = RGBA(0,0,255,0);
	pData[w*8+7] = RGBA(255,0,0,255);

	// 9
	// *-------*
	// |   |   |
	// |---|   |
	// |   |   |
	// *-------*
	//
	pData[w*9+0] = RGBA(255,0,0,0);
	pData[w*9+1] = RGBA(0,255,0,0);
	pData[w*9+2] = RGBA(0,0,255,255);
	pData[w*9+3] = RGBA(0,0,255,255);
	pData[w*9+4] = RGBA(255,0,0,0);
	pData[w*9+5] = RGBA(0,255,0,0);
	pData[w*9+6] = RGBA(0,0,255,255);
	pData[w*9+7] = RGBA(0,0,255,255);

	// 10
	// *-------*
	// |   |   |
	// |   |---|
	// |   |   |
	// *-------*
	//
	pData[w*10+0] = RGBA(255,255,0,0);
	pData[w*10+1] = RGBA(255,255,0,0);
	pData[w*10+2] = RGBA(0,0,255,0);
	pData[w*10+3] = RGBA(0,0,0,255);
	pData[w*10+4] = RGBA(255,255,0,0);
	pData[w*10+5] = RGBA(255,255,0,0);
	pData[w*10+6] = RGBA(0,0,255,0);
	pData[w*10+7] = RGBA(0,0,0,255);

	// 11
	// *-------*
	// |    \  |
	// |\     \|
	// |  \    |
	// *-------*
	//
	pData[w*11+0] = RGBA(255,0,255,0);
	pData[w*11+1] = RGBA(0,255,0,0);
	pData[w*11+2] = RGBA(255,0,255,0);
	pData[w*11+3] = RGBA(0,0,0,255);
	pData[w*11+4] = RGBA(255,0,255,0);
	pData[w*11+5] = RGBA(255,0,255,0);
	pData[w*11+6] = RGBA(255,0,255,0);
	pData[w*11+7] = RGBA(255,0,255,0);

	// 12
	// *-------*
	// |  /    |
	// |/     /|
	// |    /  |
	// *-------*
	//
	pData[w*12+0] = RGBA(255,0,0,0);
	pData[w*12+1] = RGBA(0,255,0,255);
	pData[w*12+2] = RGBA(0,0,255,0);
	pData[w*12+3] = RGBA(0,255,0,255);
	pData[w*12+4] = RGBA(0,255,0,255);
	pData[w*12+5] = RGBA(0,255,0,255);
	pData[w*12+6] = RGBA(0,255,0,255);
	pData[w*12+7] = RGBA(0,255,0,255);

	// 13
	// *-------*
	// |   |   |
	// |---|---|
	// |   |   |
	// *-------*
	//
	pData[w*13+0] = RGBA(255,0,0,0);
	pData[w*13+1] = RGBA(0,255,0,0);
	pData[w*13+2] = RGBA(0,0,255,0);
	pData[w*13+3] = RGBA(0,0,0,255);
	pData[w*13+4] = RGBA(255,0,0,0);
	pData[w*13+5] = RGBA(0,255,0,0);
	pData[w*13+6] = RGBA(0,0,255,0);
	pData[w*13+7] = RGBA(0,0,0,255);

	// 14
	// *-------*
	// |  / \  |
	// |/     /|
	// | \  /  |
	// *-------*
	// R == B && G == A 
	//
	pData[w*14+0] = RGBA(255,0,255,0);
	pData[w*14+1] = RGBA(0,255,0,255);
	pData[w*14+2] = RGBA(255,0,255,0);
	pData[w*14+3] = RGBA(255,255,255,255);
	pData[w*14+4] = RGBA(255,255,255,255);
	pData[w*14+5] = RGBA(255,255,255,255);
	pData[w*14+6] = RGBA(255,255,255,255);
	pData[w*14+7] = RGBA(255,255,255,255);

	// output as a targa image by using DevIL.
	ILuint iid;
	ilGenImages(1, &iid);
	ilBindImage(iid);
	ilTexImage(w, h, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, pData);

	ilSave(IL_TGA, "slut.tga");

	ilDeleteImages(1, &iid);

	free(pData);

	return 0;
}
