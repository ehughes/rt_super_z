#include "eGFX_Config.h"
#include "eGFX_DataTypes.h"
#include "eGFX_PixelOps.h"
#include "eGFX_PrimitiveOps.h"
#include "eGFX_StringOps.h"
#include "eGFX_Blit.h"

#include "stdarg.h"
#include "stdio.h"

#ifndef eGFX_STRING_BUF_ALLOC
#error "eGFX_STRING_BUF_ALLOC needs defined in eGFX_Config.h"
#endif

#if (eGFX_STRING_BUF_ALLOC == 0)
static char eGFX_StringBuf[eGFX_MAX_PRINTF_BUF_LENGTH];
#endif

#ifdef __FREERTOS_PRESENT
#include "FreeRTOS_Global.h"
#endif

#ifdef __FREERTOS_PRESENT
	#include "FreeRTOS_Global.h"
#endif

int32_t eGFX_WEAK eGFX_GetStringWidth(char *String,
	const eGFX_Font *MyFont)
{
	uint8_t Ptr = 0;
	uint8_t NextChar;
	int32_t StringSize = 0;
	NextChar = String[Ptr];
	Ptr++;

	while ((NextChar != 0) && (Ptr < eGFX_MAX_STRING_LEN))
	{
		if (NextChar >= 0x20 && NextChar < 0x80)
		{
			//Remember that we don't encode the white space characters in the font table;
			StringSize += MyFont->CharacterSprites[NextChar - 0x20]->SizeX + MyFont->InterCharacterSpacing;
			NextChar = String[Ptr];

		}

		Ptr++;
	}

	return StringSize - MyFont->InterCharacterSpacing; //Remove the last intra character space!
}

int32_t eGFX_WEAK eGFX_DrawCharacter(eGFX_ImagePlane *Image,
	int32_t StartX,
	int32_t StartY,
	uint8_t Character,
	const eGFX_Font *MyFont)
{
	//eGFX_Font does not contain any of the ASCII control codes
	if (Character < 0x20)
		return StartX;

	Character = Character - 0x20;

	if (Character == 0x00)
	{
		return StartX + (MyFont->CharacterSprites[0]->SizeX);
	}
	else
	{
		eGFX_Blit(Image, StartX, StartY, MyFont->CharacterSprites[Character]);
	}

	StartX += MyFont->CharacterSprites[Character]->SizeX;

	return StartX;
}


int32_t eGFX_WEAK eGFX_ScaledDrawCharacter(eGFX_ImagePlane *Image,
	int32_t StartX,
	int32_t StartY,
	uint8_t Character,
	const eGFX_Font *MyFont,
	float ScaleFactor)

{
	//eGFX_Font does not contain any of the ASCII control codes
	if (Character < 0x20)
		return StartX;

	Character = Character - 0x20;

	if (Character == 0x00)
	{
		return StartX + (uint16_t)((MyFont->CharacterSprites[0]->SizeX * ScaleFactor) + 0.5f); // No need to render anything....
	}
	else
	{
		eGFX_ScaledBlit(Image, StartX, StartY, MyFont->CharacterSprites[Character], ScaleFactor);
	}

	StartX += MyFont->CharacterSprites[Character]->SizeX * (int16_t)(ScaleFactor + .5f);
	return StartX;
}



int32_t eGFX_WEAK eGFX_DrawCharacterFaded(eGFX_ImagePlane *Image,
	int32_t StartX,
	int32_t StartY,
	uint8_t Character,
	const eGFX_Font *MyFont,
	eGFX_PixelState PS)
{
	//eGFX_Font does not contain any of the ASCII control codes
	if (Character < 0x20)
		return StartX;

	Character = Character - 0x20;

	if (Character == 0x00)
	{
		return StartX + (MyFont->CharacterSprites[0]->SizeX);
	}
	else
	{
		eGFX_BlitFaded(Image, StartX, StartY, MyFont->CharacterSprites[Character], PS);
	}

	StartX += MyFont->CharacterSprites[Character]->SizeX;
	return StartX;
}


int32_t eGFX_WEAK eGFX_DrawCharacterColored(eGFX_ImagePlane *Image,
	int32_t StartX,
	int32_t StartY,
	uint8_t Character,
	const eGFX_Font *MyFont,
	eGFX_PixelState Color)
{
	//eGFX_Font does not contain any of the ASCII control codes
	if (Character < 0x20)
		return StartX;

	Character = Character - 0x20;

	if (Character == 0x00)
	{
		return StartX + (MyFont->CharacterSprites[0]->SizeX);
	}
	else
	{
		eGFX_BlitColored(Image, StartX, StartY, MyFont->CharacterSprites[Character], Color);
	}

	StartX += MyFont->CharacterSprites[Character]->SizeX;
	return StartX;
}


int32_t eGFX_WEAK eGFX_DrawCharacterShaded(eGFX_ImagePlane *Image,
	int32_t StartX,
	int32_t StartY,
	uint8_t Character,
	const eGFX_Font *MyFont,
	float Shading)
{
	//eGFX_Font does not contain any of the ASCII control codes
	if (Character < 0x20)
		return StartX;

	Character = Character - 0x20;

	if (Character == 0x00)
	{
		return StartX + (MyFont->CharacterSprites[0]->SizeX);
	}
	else
	{
		eGFX_BlitShaded(Image, StartX, StartY, MyFont->CharacterSprites[Character], Shading);
	}

	StartX += MyFont->CharacterSprites[Character]->SizeX;
	return StartX;
}




int32_t eGFX_WEAK eGFX_GetStringWidth_CustomSpacing(char *String,
	const eGFX_Font *MyFont,
	uint32_t Spacing)
{
	uint8_t Ptr = 0;
	uint8_t NextChar;
	int32_t StringSize = 0;
	NextChar = String[Ptr];
	Ptr++;

	while ((NextChar != 0) && (Ptr < eGFX_MAX_STRING_LEN))
	{
		if (NextChar >= 0x20 && NextChar < 0x80)
		{
			StringSize += MyFont->CharacterSprites[NextChar - 0x20]->SizeX + Spacing;
			NextChar = String[Ptr];
			Ptr++;
		}
	}

	return StringSize - Spacing; //Remove the last inter character space!
}


void eGFX_WEAK eGFX_DrawHorizontalCenteredString(eGFX_ImagePlane *Image,
	int32_t StartY,
	char *String,
	const eGFX_Font *MyFont)
{
	int16_t StartX = 0;
	StartX = ((int16_t)Image->SizeX - eGFX_GetStringWidth(String, MyFont)) >> 1;
	eGFX_DrawString(Image, String, StartX, StartY, MyFont);
}



int32_t eGFX_WEAK eGFX_DrawRightAlignedCenteredString_CustomSpacing(eGFX_ImagePlane *Image,
	int32_t StartY,
	char *String,
	const eGFX_Font *MyFont,
	uint32_t CustomCharacterSpacing,
	int32_t RightSideBuffer)
{
	int32_t StartX = 0;
	StartX = ((int16_t)Image->SizeX - eGFX_GetStringWidth_CustomSpacing(String, MyFont, CustomCharacterSpacing) - RightSideBuffer);
	eGFX_DrawString_CustomSpacing(Image, StartX, StartY, String, MyFont, CustomCharacterSpacing);
	return StartX;
}

void eGFX_WEAK eGFX_DrawHorizontalCenteredString_CustomSpacing(eGFX_ImagePlane *Image,
	int32_t StartY,
	char *String,
	const eGFX_Font *MyFont,
	uint32_t Spacing)
{
	int16_t StartX = 0;
	StartX = ((int16_t)Image->SizeX - eGFX_GetStringWidth_CustomSpacing(String, MyFont, Spacing)) >> 1;
	eGFX_DrawString_CustomSpacing(Image, StartX, StartY, String, MyFont, Spacing);
}


void eGFX_WEAK eGFX_ScaledDrawString(eGFX_ImagePlane *Image,
	int16_t StartX,
	int16_t StartY,
	char *String,
	const eGFX_Font *MyFont,
	float ScaleFactor)
{
	uint8_t Ptr = 0;
	uint8_t NextChar;
	NextChar = String[Ptr];

	while ((NextChar != 0) && (Ptr < eGFX_MAX_STRING_LEN))
	{
		StartX = eGFX_ScaledDrawCharacter(Image, StartX, StartY, NextChar, MyFont, ScaleFactor);
		StartX += MyFont->InterCharacterSpacing * (int16_t)(ScaleFactor + .5f);
		Ptr++;
		NextChar = String[Ptr];
	}
}


void eGFX_WEAK eGFX_ScaledDrawString_CustomSpacing(eGFX_ImagePlane *Image,
	int32_t StartX,
	int32_t StartY,
	char *String,
	const eGFX_Font *MyFont,
	float ScaleFactor,
	uint32_t Spacing)
{
	uint8_t Ptr = 0;
	uint8_t NextChar;
	NextChar = String[Ptr];

	while ((NextChar != 0) && (Ptr < eGFX_MAX_STRING_LEN))
	{
		StartX = eGFX_ScaledDrawCharacter(Image, StartX, StartY, NextChar, MyFont, ScaleFactor);
		StartX += (int16_t)((ScaleFactor * Spacing) + .5f);
		Ptr++;
		NextChar = String[Ptr];
	}
}

int32_t eGFX_WEAK eGFX_DrawString(eGFX_ImagePlane *Image,
	char *String,
	int32_t StartX,
	int32_t StartY,
	const eGFX_Font *MyFont)
{
	uint8_t Ptr = 0;
	uint8_t NextChar;
	NextChar = String[Ptr];

	while ((NextChar != 0) && (Ptr < eGFX_MAX_STRING_LEN))
	{
		StartX = eGFX_DrawCharacter(Image, StartX, StartY, NextChar, MyFont);
		StartX += MyFont->InterCharacterSpacing;
		Ptr++;
		NextChar = String[Ptr];
	}

	return StartX + 1;
}

int32_t eGFX_WEAK eGFX_DrawStringFaded(eGFX_ImagePlane *Image,
	char *String,
	int32_t StartX,
	int32_t StartY,
	const eGFX_Font *MyFont,
	eGFX_PixelState PS)
{
	uint8_t Ptr = 0;
	uint8_t NextChar;
	NextChar = String[Ptr];

	while ((NextChar != 0) && (Ptr < eGFX_MAX_STRING_LEN))
	{
		StartX = eGFX_DrawCharacterFaded(Image, StartX, StartY, NextChar, MyFont, PS);
		StartX += MyFont->InterCharacterSpacing;
		Ptr++;
		NextChar = String[Ptr];
	}

	return StartX + 1;
}

int32_t eGFX_WEAK eGFX_DrawStringShaded(eGFX_ImagePlane *Image,
	char *String,
	int32_t StartX,
	int32_t StartY,
	const eGFX_Font *MyFont,
	float Shading)
{
	uint8_t Ptr = 0;
	uint8_t NextChar;
	NextChar = String[Ptr];

	while ((NextChar != 0) && (Ptr < eGFX_MAX_STRING_LEN))
	{
		StartX = eGFX_DrawCharacterShaded(Image, StartX, StartY, NextChar, MyFont, Shading);
		StartX += MyFont->InterCharacterSpacing;
		Ptr++;
		NextChar = String[Ptr];
	}

	return StartX + 1;
}

int32_t eGFX_WEAK eGFX_DrawStringColored(eGFX_ImagePlane *Image,
	char *String,
	int32_t StartX,
	int32_t StartY,
	const eGFX_Font *MyFont,
	uint32_t Color)
{
	uint8_t Ptr = 0;
	uint8_t NextChar;
	NextChar = String[Ptr];

	while ((NextChar != 0) && (Ptr < eGFX_MAX_STRING_LEN))
	{
		StartX = eGFX_DrawCharacterColored(Image, StartX, StartY, NextChar, MyFont, Color);
		StartX += MyFont->InterCharacterSpacing;
		Ptr++;
		NextChar = String[Ptr];
	}

	return StartX + 1;
}


int32_t eGFX_WEAK eGFX_printf(eGFX_ImagePlane *Image,
	int32_t StartX,
	int32_t StartY,
	const eGFX_Font *MyFont,
	char *FormatString, ...)
{
	int16_t End;

#if (eGFX_STRING_BUF_ALLOC == 1)
	char eGFX_StringBuf[eGFX_MAX_PRINTF_BUF_LENGTH];
#endif

	va_list argptr;
	va_start(argptr, FormatString);

#ifdef WIN32
	vsnprintf_s((char *)eGFX_StringBuf, eGFX_MAX_PRINTF_BUF_LENGTH, eGFX_MAX_PRINTF_BUF_LENGTH, FormatString, argptr);
#else

#ifdef __FREERTOS_PRESENT
	if (xSemaphoreTake(SNPRINTF_Mutex, 1000) == pdFALSE)
	{
		eGFX_StringBuf[0] = 0;
	}
	else
	{
		vsnprintf((char *)eGFX_StringBuf, eGFX_MAX_PRINTF_BUF_LENGTH, FormatString, argptr);
		xSemaphoreGive(SNPRINTF_Mutex);
	}

#else

	vsnprintf((char *)eGFX_StringBuf, eGFX_MAX_PRINTF_BUF_LENGTH, FormatString, argptr);

#endif

#endif

	va_end(argptr);

	End = StartX + eGFX_GetStringWidth(eGFX_StringBuf, MyFont) + 1;
	eGFX_DrawString(Image, eGFX_StringBuf, StartX, StartY, MyFont);

	return End;
}

int32_t eGFX_WEAK eGFX_printf_Colored(eGFX_ImagePlane *Image,
	int16_t StartX,
	int16_t StartY,
	const eGFX_Font *MyFont,
	uint32_t Color,
	char *FormatString, ...)
{
	int32_t End;

#if (eGFX_STRING_BUF_ALLOC == 1)
	char eGFX_StringBuf[eGFX_MAX_PRINTF_BUF_LENGTH];
#endif

	va_list argptr;
	va_start(argptr, FormatString);

#ifdef WIN32
	vsnprintf_s((char *)eGFX_StringBuf, eGFX_MAX_PRINTF_BUF_LENGTH, eGFX_MAX_PRINTF_BUF_LENGTH, FormatString, argptr);
#else

#ifdef __FREERTOS_PRESENT
	if (xSemaphoreTake(SNPRINTF_Mutex, 1000) == pdFALSE)
	{
		eGFX_StringBuf[0] = 0;
	}
	else
	{
		vsnprintf((char *)eGFX_StringBuf, eGFX_MAX_PRINTF_BUF_LENGTH, FormatString, argptr);
		xSemaphoreGive(SNPRINTF_Mutex);
	}

#else

	vsnprintf((char *)eGFX_StringBuf, eGFX_MAX_PRINTF_BUF_LENGTH, FormatString, argptr);

#endif

#endif

	va_end(argptr);

	End = StartX + eGFX_GetStringWidth(eGFX_StringBuf, MyFont) + 1;

	eGFX_DrawStringColored(Image, eGFX_StringBuf, StartX, StartY, MyFont, Color);

	return End;
}

int32_t eGFX_WEAK eGFX_printf_HorizontalCentered_Colored(eGFX_ImagePlane *Image,
	int32_t StartY,
	const eGFX_Font *MyFont,
	uint32_t Color,
	char *FormatString, ...)
{
	int32_t End;
	int32_t StartX = 0;

#if (eGFX_STRING_BUF_ALLOC == 1)
	char eGFX_StringBuf[eGFX_MAX_PRINTF_BUF_LENGTH];
#endif

	va_list argptr;
	va_start(argptr, FormatString);

#ifdef WIN32
	vsnprintf_s((char *)eGFX_StringBuf, eGFX_MAX_PRINTF_BUF_LENGTH, eGFX_MAX_PRINTF_BUF_LENGTH, FormatString, argptr);
#else

#ifdef __FREERTOS_PRESENT
	if (xSemaphoreTake(SNPRINTF_Mutex, 1000) == pdFALSE)
	{
		eGFX_StringBuf[0] = 0;
	}
	else
	{
		vsnprintf((char *)eGFX_StringBuf, eGFX_MAX_PRINTF_BUF_LENGTH, FormatString, argptr);
		xSemaphoreGive(SNPRINTF_Mutex);
	}

#else

	vsnprintf((char *)eGFX_StringBuf, eGFX_MAX_PRINTF_BUF_LENGTH, FormatString, argptr);

#endif

#endif

	va_end(argptr);



	StartX = ((int16_t)Image->SizeX - eGFX_GetStringWidth(eGFX_StringBuf, MyFont)) >> 1;
	End = StartX + eGFX_GetStringWidth(eGFX_StringBuf, MyFont) + 1;

	eGFX_DrawStringColored(Image, eGFX_StringBuf, StartX, StartY, MyFont, Color);

	return End;
}


int32_t eGFX_printf_HorizontalCenteredInRegion_Colored(eGFX_ImagePlane *Image,
	int32_t StartY,
	int32_t X_Start,
	int32_t X_Stop,
	const eGFX_Font *MyFont,
	uint32_t Color,
	char *FormatString, ...)
{
	int32_t End;
	int32_t StartX = 0;

#if (eGFX_STRING_BUF_ALLOC == 1)
	char eGFX_StringBuf[eGFX_MAX_PRINTF_BUF_LENGTH];
#endif

	va_list argptr;
	va_start(argptr, FormatString);

#ifdef WIN32
	vsnprintf_s((char *)eGFX_StringBuf, eGFX_MAX_PRINTF_BUF_LENGTH, eGFX_MAX_PRINTF_BUF_LENGTH, FormatString, argptr);
#else

#ifdef __FREERTOS_PRESENT
	if (xSemaphoreTake(SNPRINTF_Mutex, 1000) == pdFALSE)
	{
		eGFX_StringBuf[0] = 0;
	}
	else
	{
		vsnprintf((char *)eGFX_StringBuf, eGFX_MAX_PRINTF_BUF_LENGTH, FormatString, argptr);
		xSemaphoreGive(SNPRINTF_Mutex);
	}

#else

	vsnprintf((char *)eGFX_StringBuf, eGFX_MAX_PRINTF_BUF_LENGTH, FormatString, argptr);

#endif

#endif

	va_end(argptr);



	StartX = (((X_Stop - X_Start) - (eGFX_GetStringWidth(eGFX_StringBuf, MyFont))) / 2) + X_Start;
	End = StartX + eGFX_GetStringWidth(eGFX_StringBuf, MyFont) + 1;

	eGFX_DrawStringColored(Image, eGFX_StringBuf, StartX, StartY, MyFont, Color);

	return End;
}


int32_t eGFX_WEAK eGFX_DrawString_CustomSpacing(eGFX_ImagePlane *Image,
	int32_t StartX,
	int32_t StartY,
	char *String,
	const eGFX_Font *MyFont,
	uint16_t Spacing)
{
	uint8_t Ptr = 0;
	uint8_t NextChar;
	NextChar = String[Ptr];

	while ((NextChar != 0) && (Ptr < eGFX_MAX_STRING_LEN))
	{
		StartX = eGFX_DrawCharacter(Image, StartX, StartY, NextChar, MyFont);
		StartX += Spacing;
		Ptr++;
		NextChar = String[Ptr];
	}

	return StartX + 1;
}
