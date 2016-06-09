// createZoneLUT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../IL/il.h"
#include <cstdlib>
#include <memory>

#define RGBA(v) (((v)<<24) | ((v)<<16) | ((v)<<8) | (v))

const int zlutSize = 32;

int main(int argc, char* argv[])
{
	ilInit();

	unsigned int *pData = (unsigned int*)malloc(zlutSize*zlutSize*4);
	memset(pData, 0, zlutSize*zlutSize*4);

	// zone layout
	// *-----------*
	// | 0 / | \ 3 |
	// | / 4 | 7 \ |
	// |-----|-----|
	// | \ 5 | 6 / |
	// | 1 \ | / 2 |
	// *-----------*
	//

	for(int i=0; i<zlutSize; ++i)
	{
		for(int j=0; j<zlutSize; ++j)
		{
			// get uv.
			float u = (float(j)+0.5f)/zlutSize;
			float v = (float(i)+0.5f)/zlutSize;
			
			// compute the zone index.
			int index;
			float uu, vv;
			if(u < 0.5f)
			{
				if(v < 0.5f)
				{
					index = 0;
					uu = -u + 0.5f;
					vv = v;
					if(uu < vv) index += 4;
				}
				else
				{
					index = 1;
					uu = -u;
					vv = -v + 0.5f;
					if(uu < vv) index += 4;
				}
			}
			else
			{
				if(v < 0.5f)
				{
					index = 3;
					uu = u - 0.5f;
					vv = v;
					if(uu < vv) index += 4;
				}
				else
				{
					index = 2;
					uu = u - 1.0f;
					vv = -v + 0.5f;
					if(uu < vv) index += 4;
				}
			}
			
			// write the zone index.
			unsigned int value = 256 * index / 8;
			pData[i*zlutSize+j] = RGBA(value);
		}
	}

	// output as a targa image by using DevIL.
	ILuint iid;
	ilGenImages(1, &iid);
	ilBindImage(iid);
	ilTexImage(zlutSize, zlutSize, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, pData);

	ilSave(IL_TGA, "zlut.tga");

	ilDeleteImages(1, &iid);

	free(pData);

	return 0;
}
