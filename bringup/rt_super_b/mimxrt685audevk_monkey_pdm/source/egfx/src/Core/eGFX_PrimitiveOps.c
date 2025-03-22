#include "eGFX_Config.h"
#include "eGFX_DataTypes.h"
#include "eGFX_PixelOps.h"
#include "eGFX_PrimitiveOps.h"

void eGFX_WEAK eGFX_DrawH_Line(eGFX_ImagePlane *Image,
                               int32_t X_Start,
                               int32_t X_Stop,
                               int32_t Y,
                               eGFX_PixelState PS)
{
    int32_t LineStart;
    int32_t LineStop;
    int i;

    if((Y<Image->SizeY)  && (Y>=0))
    {
        if(X_Start>X_Stop)
        {
            LineStart = X_Stop;
            LineStop = X_Start;
        }
        else
        {
            LineStart = X_Start;
            LineStop = X_Stop;
        }

        if(LineStart<0)
        {
            LineStart = 0;
        }

        if(LineStop>Image->SizeX)
        {
            LineStop =  Image->SizeX-1;
        }

        if(LineStart == LineStop)
        {
            eGFX_PutPixel(Image,LineStart,Y,PS);
        }
        else
        {
            for(i=LineStart; i<=LineStop ; i++)
            {
                eGFX_PutPixel(Image,i,Y,PS);
            }
        }
    }
}

void eGFX_WEAK eGFX_DrawV_Line(eGFX_ImagePlane *Image,
                              int32_t YStart,
                              int32_t YStop,
                              int32_t X,
                              eGFX_PixelState PS)
{
    int32_t LineStart;
    int32_t LineStop;
    int32_t i;

    if((X<Image->SizeX) && (X>=0))
    {
        if(YStart>YStop)
        {
            LineStart = YStop;
            LineStop = YStart;
        }
        else
        {
            LineStart = YStart;
            LineStop = YStop;
        }

        if(LineStart<0)
        {
            LineStart = 0;
        }

        if(LineStop>Image->SizeY)
        {
            LineStop =  Image->SizeY-1;
        }

        for(i=LineStart; i<=LineStop ; i++)
        {
            eGFX_PutPixel(Image,X,i,PS);
        }
    }
}

void eGFX_WEAK eGFX_DrawFilledBox(eGFX_ImagePlane *Image,
                                  eGFX_Box *Box,
                                  eGFX_PixelState PS)
{
    int32_t i;

    for(i=Box->P1.Y ; i<Box->P2.Y+1 ; i++)
    {
        eGFX_DrawH_Line(Image, Box->P1.X, Box->P2.X,i,PS);
    }
}


void eGFX_WEAK eGFX_DrawBox(eGFX_ImagePlane *Image,
                            eGFX_Box *Box,
                            eGFX_PixelState PS)
{
    eGFX_DrawH_Line(Image, Box->P1.X, Box->P2.X,Box->P1.Y,PS);
    eGFX_DrawH_Line(Image, Box->P1.X, Box->P2.X,Box->P2.Y,PS);
    eGFX_DrawV_Line(Image, Box->P1.Y, Box->P2.Y,Box->P1.X,PS);
    eGFX_DrawV_Line(Image, Box->P1.Y, Box->P2.Y,Box->P2.X,PS);
}

void eGFX_WEAK eGFX_DrawSolidRectangle(eGFX_ImagePlane *Image,
	eGFX_Rect * R,
	eGFX_PixelState PS)
{
	for (int i = R->P1.Y; i < R->P2.Y + 1; i++)
	{
		eGFX_DrawH_Line(Image, R->P1.X, R->P2.X, i, PS);
	}
}


void eGFX_WEAK eGFX_DrawLine(eGFX_ImagePlane *Image,
                             int32_t X1,
                             int32_t Y1,
                             int32_t X2,
                             int32_t Y2,
                             eGFX_PixelState PS)
{
    //A simple Implementation of Bresenham's line Algorithm
    int32_t StartX,StopX,StartY,StopY;
    int32_t dX,dY;
    int32_t Y_Numerator;
    int32_t X_Numerator;
    int32_t Y;
    int32_t X;
    int32_t i;
    uint32_t YDir = 0;

    //First Make sure that it is left to right
    //If not them flop them
    if(X2>X1)
    {
        StartX = X1;
        StopX = X2;
        StartY = Y1;
        StopY = Y2;
    }
    else
    {
        StartX = X2;
        StopX = X1;
        StartY = Y2;
        StopY = Y1;
    }

    eGFX_PutPixel(Image, StopX,StopY, PS);

    if(StopY>=StartY)
    {
        dY = StopY - StartY;
        YDir = 0;
    }
    else
    {
        dY = StartY - StopY;
        YDir = 1;
    }

    dX = StopX - StartX;

    //Now, if the slope is less greater than one,  we need to swap all X/Y operations
    if(dY<=dX)
    {
        //Slope is less than one, proceed at normal and step along the x axis
        Y=StartY;   //start the whole part of the Y value at the starting pixeel.
        X=StartX;
        //We need to start the numerator of the fraction half way through the fraction so evertyhing rounds at
        //fraction midpoint
        Y_Numerator = dX>>1;   //The fraction demonimator is assumeed to be dX

        // out fixed point Y value is  Y + (Y_Numerator / dX)
        //Every time we step the X coordinate by one, we need to step
        //out Y coordinate by dY/dX.  We do this by just adding dY to our
        //numerator.  When the numerator gets bigger than the
        //denomiator, the increment the whole part by one and decrement the numerator
        //by the denominator
        for(i=0; i<dX; i++)
        {
            eGFX_PutPixel(Image,X,Y,PS);
            X++;
            //Now do all the fractional stuff
            Y_Numerator += dY;

            if(Y_Numerator >= dX)
            {
                Y_Numerator-=dX;

                if(StopY > StartY)
                {
                    Y++;
                }
                else
                {
                    Y--;
                }
            }
        }
    }
    else
    {
        //Same as before by step along the y axis.
        Y=StartY;
        X=StartX;
        X_Numerator = dY>>1;

        for(i=0; i<dY; i++)
        {
            eGFX_PutPixel(Image,X,Y,PS);

            //Now do all the fractional stuff
            if(YDir)
            {
                Y--;
            }
            else
            {
                Y++;
            }

            X_Numerator += dX;

            if(X_Numerator >= dY)
            {
                X_Numerator-=dY;

                if(StopX > StartX)
                {
                    X++;
                }
                else
                {
                    X--;
                }
            }
        }
    }
}

// midpoint circle algorithm
void eGFX_WEAK eGFX_DrawCircle(eGFX_ImagePlane *Image,
                               int32_t x0,
                               int32_t y0,
                               int32_t radius,
                               eGFX_PixelState PS)
{
    int32_t f = 1 - radius;
    int32_t ddF_x = 1;
    int32_t ddF_y = -2 * radius;
    int32_t x = 0;
    int32_t y = radius;

        if (radius <= 0)
                return;

    eGFX_PutPixel(Image, x0, y0 + radius, PS);
    eGFX_PutPixel(Image, x0, y0 - radius, PS);
    eGFX_PutPixel(Image, x0 + radius, y0, PS);
    eGFX_PutPixel(Image, x0 - radius, y0, PS);


    while(x < y)
    {
        // ddF_x == 2 * x + 1;
        // ddF_y == -2 * y;
        // f == x*x + y*y - radius*radius + 2*x - y + 1;
        if(f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x;
        eGFX_PutPixel(Image, x0 + x, y0 + y, PS);
        eGFX_PutPixel(Image, x0 - x, y0 + y, PS);
        eGFX_PutPixel(Image, x0 + x, y0 - y, PS);
        eGFX_PutPixel(Image, x0 - x, y0 - y, PS);
        eGFX_PutPixel(Image, x0 + y, y0 + x, PS);
        eGFX_PutPixel(Image, x0 - y, y0 + x, PS);
        eGFX_PutPixel(Image, x0 + y, y0 - x, PS);
        eGFX_PutPixel(Image, x0 - y, y0 - x, PS);
    }
}

// midpoint circle algorithm
void eGFX_WEAK eGFX_DrawCircleFromBackground(eGFX_ImagePlane *Image,
	eGFX_ImagePlane *Background,
	int32_t x0,
	int32_t y0,
	int32_t radius)
{
	int32_t f = 1 - radius;
	int32_t ddF_x = 1;
	int32_t ddF_y = -2 * radius;
	int32_t x = 0;
	int32_t y = radius;

	if (radius <= 0)
		return;

	
	eGFX_PutPixel(Image, x0, y0 + radius, eGFX_GetPixel(Background, x0, y0 + radius));
	eGFX_PutPixel(Image, x0, y0 - radius, eGFX_GetPixel(Background, x0, y0 - radius));
	eGFX_PutPixel(Image, x0 + radius, y0, eGFX_GetPixel(Background, x0 + radius, y0));
	eGFX_PutPixel(Image, x0 - radius, y0, eGFX_GetPixel(Background, x0 - radius, y0));


	while (x < y)
	{
		// ddF_x == 2 * x + 1;
		// ddF_y == -2 * y;
		// f == x*x + y*y - radius*radius + 2*x - y + 1;
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;
		eGFX_PutPixel(Image, x0 + x, y0 + y, eGFX_GetPixel(Background, x0 + x, y0 + y));
		eGFX_PutPixel(Image, x0 - x, y0 + y, eGFX_GetPixel(Background, x0 - x, y0 + y));
		eGFX_PutPixel(Image, x0 + x, y0 - y, eGFX_GetPixel(Background, x0 + x, y0 - y));
		eGFX_PutPixel(Image, x0 - x, y0 - y, eGFX_GetPixel(Background, x0 - x, y0 - y));
		eGFX_PutPixel(Image, x0 + y, y0 + x, eGFX_GetPixel(Background, x0 + y, y0 + x));
		eGFX_PutPixel(Image, x0 - y, y0 + x, eGFX_GetPixel(Background, x0 - y, y0 + x));
		eGFX_PutPixel(Image, x0 + y, y0 - x, eGFX_GetPixel(Background, x0 + y, y0 - x));
		eGFX_PutPixel(Image, x0 - y, y0 - x, eGFX_GetPixel(Background, x0 - y, y0 - x));
	}
}

void eGFX_WEAK eGFX_DrawFilledCircle(eGFX_ImagePlane *Image,
                                    int16_t x0,
                                    int16_t y0,
                                    int16_t radius,
                                    eGFX_PixelState PS)
{
        int16_t f = 1 - radius;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * radius;
        int16_t x = 0;
        int16_t y = radius;

        if (radius == 0)
                return;

        eGFX_DrawH_Line(Image, x0 - radius, x0 + radius , y0, PS);

        eGFX_DrawV_Line(Image, y0 - radius , y0 + radius , x0, PS);

        while (x < y)
        {
                // ddF_x == 2 * x + 1;
                // ddF_y == -2 * y;
                // f == x*x + y*y - radius*radius + 2*x - y + 1;
                if (f >= 0)
                {
                        y--;
                        ddF_y += 2;
                        f += ddF_y;
                }

                x++;
                ddF_x += 2;
                f += ddF_x;
                
                eGFX_DrawH_Line(Image, x0 - x, x0 + x, y0 + y, PS);

                eGFX_DrawH_Line(Image, x0 - x, x0 + x, y0 - y, PS);

                eGFX_DrawH_Line(Image, x0 - y, x0 + y, y0 + x, PS);

                eGFX_DrawH_Line(Image, x0 - y, x0 + y, y0 - x, PS);


        }
}

