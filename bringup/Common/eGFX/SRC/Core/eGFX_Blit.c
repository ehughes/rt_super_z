#include "eGFX_Config.h"
#include "eGFX_DataTypes.h"
#include "eGFX_PixelOps.h"
#include "eGFX_Blit.h"
#include "eGFX_Plane.h"


void eGFX_WEAK eGFX_Blit(eGFX_ImagePlane *Destination,
	int32_t x,
	int32_t y,
	const eGFX_ImagePlane *Sprite)
{


	int32_t X1_DstRegion = x;
	int32_t Y1_DstRegion = y;
	int32_t X2_DstRegion = x + Sprite->SizeX - 1;
	int32_t Y2_DstRegion = y + Sprite->SizeY - 1;

	//Reject offscreen

	if (X1_DstRegion >= Destination->SizeX)
		return;

	if (X2_DstRegion < 0)
		return;

	if (Y1_DstRegion >= Destination->SizeY)
		return;

	if (Y2_DstRegion < 0)
		return;


	int SrcStartX = 0;
	int SrcStartY = 0;

	int SrcStopX = Sprite->SizeX - 1;
	int SrcStopY = Sprite->SizeY - 1;


	if (X1_DstRegion < 0)
	{
		SrcStartX -= X1_DstRegion;
	
	}

	if (X2_DstRegion >= Destination->SizeX)
	{
		SrcStopX -= (X2_DstRegion - Destination->SizeX) - 1;
		
	}


	if (Y1_DstRegion < 0)
	{
		SrcStartY -= Y1_DstRegion;
	
	}

	if (Y2_DstRegion >= Destination->SizeY)
	{
		SrcStopY -= (Y2_DstRegion - Destination->SizeY) - 1;
	
	}

	for (int y0 = SrcStartY; y0 <= SrcStopY; y0++)
	{
		for (int x0= SrcStartX; x0 <= SrcStopX; x0++)
		{
			eGFX_PutPixel(Destination, x + x0, y + y0, eGFX_GetPixel(Sprite, x0, y0));
		}
	}
}


/*
This is used to copy a section from equal sized image planes
when restoring a directy rectangle background image
*/
void eGFX_WEAK eGFX_BlitRestore(eGFX_ImagePlane *Destination,
								eGFX_Rect *Region,
							 eGFX_ImagePlane *Source)
{
	
	/*
		Clip the region
	*/
	if (Region->P1.X < 0)
		Region->P1.X = 0;
	
	if (Region->P1.Y < 0)
		Region->P1.Y = 0;

	if (Region->P2.X > (Destination->SizeX-1))
		Region->P2.X = Destination->SizeX-1;

	if (Region->P2.Y > (Destination->SizeY - 1))
		Region->P2.Y = (Destination->SizeY - 1);


	for (int x = Region->P1.X; x <= Region->P2.X; x++)
	{
		for (int y = Region->P1.Y; y <= Region->P2.Y; y++)
		{
			eGFX_PutPixel(Destination, x , y , eGFX_GetPixel(Source, x, y));
		}
	}
}


void eGFX_WEAK eGFX_BlitTransparentPixel(eGFX_ImagePlane *Destination,
	int32_t x,
	int32_t y,
	const eGFX_ImagePlane *Sprite,
	eGFX_PixelState TransparentPixel)
{
	uint32_t i, j;
	eGFX_PixelState PS;

	for (j = 0; j < Sprite->SizeY; j++)
	{
		for (i = 0; i < Sprite->SizeX; i++)
		{
			PS = eGFX_GetPixel(Sprite, i, j);
			if(PS!= TransparentPixel)
				eGFX_PutPixel(Destination, x + i, y + j, PS);
		}
	}
}


void eGFX_WEAK eGFX_BlitCentered(eGFX_ImagePlane *Destination,
                                 int32_t x,
                                 int32_t y,
                                 const eGFX_ImagePlane *Sprite)
{
        uint16_t i, j;

        uint16_t X_Offset;
        uint16_t Y_Offset;

        X_Offset = (Sprite->SizeX + 1) >> 1;
        Y_Offset = (Sprite->SizeY + 1) >> 1;

        for (j = 0; j<Sprite->SizeY; j++)
        {
                for (i = 0; i<Sprite->SizeX; i++)
                {
                    eGFX_PutPixel(Destination, x + i - X_Offset,
                                  y + j - Y_Offset,
                                  eGFX_GetPixel(Sprite, i, j));
                }

        }
}


//Note: This function only works with 4BPP Gray scale
void eGFX_WEAK eGFX_BlitFaded(eGFX_ImagePlane *Destination,
                              int32_t x,
                              int32_t y,
                              const eGFX_ImagePlane *Sprite,
                              eGFX_PixelState PS)
{
    uint32_t i,j;

    for(j=0; j<Sprite->SizeY; j++)
    {
        for(i=0; i<Sprite->SizeX; i++)
        {
             eGFX_PutPixel(Destination, x + i, y + j, (eGFX_GetPixel(Sprite, i, j) * PS)>>8);
        }

    }
}
//Note: This function only works with 4BPP Gray scale
void eGFX_WEAK eGFX_BlitShaded(eGFX_ImagePlane *Destination,
                               int32_t x,
                               int32_t y,
                               const eGFX_ImagePlane *Sprite,
                               float Shading)
{
        uint16_t i, j;

        if (Shading < 0.0f)
                Shading = 0.0f;
        if (Shading >1.0f)
                Shading = 1.0f;

        for (j = 0; j<Sprite->SizeY; j++)
        {
                for (i = 0; i<Sprite->SizeX; i++)
                {
                        eGFX_PutPixel(Destination, x + i, y + j,(eGFX_PixelState) (eGFX_GetPixel(Sprite, i, j) * Shading));
                }

        }
}

void eGFX_WEAK eGFX_BlitColored(eGFX_ImagePlane *Destination,
                                int16_t x,
                                int16_t y,
                                const eGFX_ImagePlane *Sprite,
                                uint32_t Color)
{
        uint32_t i, j;

        for (j = 0; j<Sprite->SizeY; j++)
        {
                for (i = 0; i<Sprite->SizeX; i++)
                {
					if(eGFX_GetPixel(Sprite, i, j))
						eGFX_PutPixel(Destination, x + i, y + j,Color);
                }

        }
}

void eGFX_WEAK eGFX_ScaledBlit(eGFX_ImagePlane *Destination,
                               int32_t x,
                               int32_t y,
                               const eGFX_ImagePlane *Sprite,
                               float ScaleFactor)
{
    uint32_t i,j;
    uint32_t DestinationSizeX;
    uint32_t DestinationSizeY;
    float StepX;
    float StepY;
    float SourcePixelX;
    float SourcePixelY;
    int32_t CenterOffsetX;
    int32_t CenterOffsetY;

    if (ScaleFactor <= 0.0f)
        return;
        
    DestinationSizeX = (uint16_t)((ScaleFactor * (float)Sprite->SizeX)+0.5f);
    DestinationSizeY = (uint16_t)((ScaleFactor * (float)Sprite->SizeY)+0.5f);

    CenterOffsetX = (uint16_t)(((ScaleFactor * (float)Sprite->SizeX)/2.0f) + 0.5f);
    CenterOffsetY = (uint16_t)(((ScaleFactor * (float)Sprite->SizeY)/2.0f) + 0.5f);

    StepX = (float)Sprite->SizeX / (float)DestinationSizeX;
    StepY = (float)Sprite->SizeY / (float)DestinationSizeY;

    SourcePixelY = 0;

    for(j=0; j<DestinationSizeY; j++)
    {
        SourcePixelX = 0;

        for(i=0; i<DestinationSizeX; i++)
        {
                        eGFX_PutPixel(Destination, 
                        
                        x + i - CenterOffsetX,
                        y + j - CenterOffsetY,
                        eGFX_GetPixel(Sprite,
                                     (int32_t)(SourcePixelX + 0.5f),
                                     (int32_t)(SourcePixelY + 0.5f)
                                     )
                        );
         
                        SourcePixelX += StepX;
        }
       
        SourcePixelY += StepY;
    }
}



