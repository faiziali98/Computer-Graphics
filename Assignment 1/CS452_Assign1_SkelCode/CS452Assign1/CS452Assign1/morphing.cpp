#include <iostream>
#include <fstream>
#include "stdafx.h"
#include "CS452Assign1.h"

using namespace std;
FIBITMAP *amb,*img, *img1,*img2,*newImg,*dib,*newImg1,*result;
unsigned pamb, pimg, pimg1, pimg2;
BYTE *bamb, *bimg, *bimg1, *bimg2;

void Initialize()
{
	setWindowSize(320, 280);
	img2 = FreeImage_Load(FIF_BMP, "both.BMP", 0);
	img1= FreeImage_Load(FIF_BMP, "right.BMP", 0);
	img = FreeImage_Load(FIF_BMP, "left.BMP", 0);
	amb = FreeImage_Load(FIF_BMP, "amb.BMP", 0);
	newImg = FreeImage_Allocate(315, 239, FreeImage_GetBPP(img));
	newImg1 = FreeImage_Allocate(315, 239, FreeImage_GetBPP(img));
	result = FreeImage_Allocate(315, 239, FreeImage_GetBPP(img));
	dib = img;
}

void Render(int mouseX, int mouseY)
{
	unsigned width = FreeImage_GetWidth(newImg);
	unsigned height = FreeImage_GetHeight(newImg);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			RGBQUAD value;
			RGBQUAD vl;
			FreeImage_GetPixelColor(img1, x, y, &value);
			FreeImage_GetPixelColor(img, x, y, &vl);
			RGBQUAD temp;
			temp.rgbBlue = value.rgbBlue*0.5;
			temp.rgbGreen = value.rgbGreen*0.2;
			temp.rgbRed = value.rgbRed*0.3;
			RGBQUAD temp1;
			temp1.rgbBlue = vl.rgbBlue*0.3;
			temp1.rgbGreen = vl.rgbGreen*0.5;
			temp1.rgbRed = vl.rgbRed*0.3;

			//assign pixel the RGB values
			FreeImage_SetPixelColor(newImg, x, y, &temp);
			FreeImage_SetPixelColor(newImg1, x, y, &temp1);
		}
	}

	

	pimg1 = FreeImage_GetPitch(newImg);
	pimg2 = FreeImage_GetPitch(newImg1);
	unsigned pnewimg = FreeImage_GetPitch(result);

	bimg1 = (BYTE*)FreeImage_GetBits(newImg);
	bimg2 = (BYTE*)FreeImage_GetBits(newImg1);
	BYTE *bnewimg = (BYTE*)FreeImage_GetBits(result);

	for (unsigned y = 0; y < height; y++) {
		BYTE *pxnewimg = (BYTE*)bnewimg;
		BYTE *pximg = (BYTE*)bimg1;
		BYTE *pximg1 = (BYTE*)bimg2;
		for (unsigned x = 0; x < width; x++) {
			double weight = double(mouseX) / width;
			pxnewimg[FI_RGBA_RED] = (pximg[FI_RGBA_RED] * weight) + (pximg1[FI_RGBA_RED] * (1 - weight));
			pxnewimg[FI_RGBA_GREEN] = (pximg[FI_RGBA_GREEN] * weight) + (pximg1[FI_RGBA_GREEN] * (1 - weight));
			pxnewimg[FI_RGBA_BLUE] = (pximg[FI_RGBA_BLUE] * weight) + (pximg1[FI_RGBA_BLUE] * (1 - weight));

			pxnewimg += 3;
			pximg += 3;
			pximg1 += 3;
		}
		// next line
		bnewimg += pnewimg;
		bimg1 += pimg1;
		bimg2 += pimg2;
	}

	renderImage(result, 1);
}

void Destroy()
{
	FreeImage_Unload(img);
	FreeImage_Unload(img1);
	FreeImage_Unload(img2);
	FreeImage_Unload(amb);
}