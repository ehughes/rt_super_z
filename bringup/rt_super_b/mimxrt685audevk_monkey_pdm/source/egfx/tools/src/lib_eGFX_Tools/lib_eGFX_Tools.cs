using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using JsonExSerializer;
using System.IO;
using System.Drawing;
using System.Drawing.Text;
using System.Security.Cryptography;
using ServiceStack.Text;

namespace lib_eGFX_Tools
{
    public class FontBuilderConfig
    {
        string _CombinedOutputName = "";
     
	    int _SpacesPerTab;
	    int _InterCharacterSpacing;

        int _Ascent;
        int _Descent;
        int _LineSpacing;

        public int Ascent
        {
            get { return _Ascent; }
            set { _Ascent = value; }
        }

        public int Descent
        {
            get { return _Descent; }
            set { _Descent = value; }
        }

        public int LineSpacing
        {
            get { return _LineSpacing; }
            set { _LineSpacing = value; }
        }

        public int SpacesPerTab
        {
            get { return _SpacesPerTab; }
            set { _SpacesPerTab = value; }
        }

        public int InterCharacterSpacing
        {
            get { return _InterCharacterSpacing; }
            set { _InterCharacterSpacing = value; }
        }
        public string CombinedOutputName
        {
            get { return _CombinedOutputName; }
            set { _CombinedOutputName = value; }
        }
    }

    public class SliceDiceConfig
    {
        string _FontImage = "";

        int _FontElementWidth;
        int _FontElementHeight;
        int _StartingASCII_Character;
        int _Rows;
        int _CharactersPerRow;
        int _PixelsBetweenElements;
        bool _UseASCII_Naming;
        string _Prefix;

        public string Prefix
        {
            get { return _Prefix; }
            set { _Prefix = value; }
        }

        public bool UseASCII_Naming
        {
            get { return _UseASCII_Naming; }
            set { _UseASCII_Naming = value; }
        }

        public string FontImage
        {
            get {return _FontImage;}
            set {_FontImage = value; }
        }

        public int FontElementWidth
        {
             get { return _FontElementWidth; }
             set { _FontElementWidth = value; }
        }
        public int FontElementHeight
        {
              get { return _FontElementHeight; }
             set { _FontElementHeight = value; }
        }
        public int StartingASCII_Character
        {
              get { return _StartingASCII_Character; }
             set { _StartingASCII_Character = value; }
        }
        public int Rows
        {  get { return _Rows; }
             set { _Rows = value; }

        }
        public int CharactersPerRow
        {
              get { return _CharactersPerRow; }
             set { _CharactersPerRow = value; }
        }
        public int PixelsBetweenElements
        {
              get { return _PixelsBetweenElements; }
             set { _PixelsBetweenElements = value; }
        }

    }

    public enum eGFX_ImagePlaneType
    {
        eGFX_IMAGE_PLANE_1BPP            = 0x0100,
        eGFX_IMAGE_PLANE_3BPP_BGR        = 0x0300,
        eGFX_IMAGE_PLANE_4BPP            = 0x0400,
        eGFX_IMAGE_PLANE_4BPP_XBGR       = 0x0401,
        eGFX_IMAGE_PLANE_8BPP            = 0x0800,
        eGFX_IMAGE_PLANE_8BPP_XRGB222    = 0x0801,
        eGFX_IMAGE_PLANE_16BPP           = 0x1000,
        eGFX_IMAGE_PLANE_16BPP_RGB565    = 0x1001,
        eGFX_IMAGE_PLANE_16BPP_BGR565    = 0x1002,
        eGFX_IMAGE_PLANE_16BPP_GBRG3553  = 0x1003,
        eGFX_IMAGE_PLANE_16BPP_GRBG3553  = 0x1004,
        eGFX_IMAGE_PLANE_24BPP           = 0x1800,
        eGFX_IMAGE_PLANE_24BPP_RGB888    = 0x1801,
        eGFX_IMAGE_PLANE_32BPP           = 0x2000,
        eGFX_IMAGE_PLANE_32BPP_XRGB888   = 0x2001,
    }

    public class eGFX_ImagePlane
    {
        public int SizeX;
        public int SizeY;
        public eGFX_ImagePlaneType Type;
        public byte[] Data;
        
        public int eGFX_CALCULATE_1BPP_BUFFER_ROW_BYTE_SIZE(int x)		{ return ((x+7)>>3); } //We add 8 to round up to the next even byte boundary
        public int eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(int x)		{ return ((x+1)>>1); } //We add 1 to round up to the next even byte boundary
        public int eGFX_CALCULATE_8BPP_BUFFER_ROW_BYTE_SIZE(int x)	 	{ return (x); }
        public int eGFX_CALCULATE_16BPP_BUFFER_ROW_BYTE_SIZE(int x)     { return (x * 2); }
        public int eGFX_CALCULATE_24BPP_BUFFER_ROW_BYTE_SIZE(int x)      { return (x * 3); }
        public int eGFX_CALCULATE_32BPP_BUFFER_ROW_BYTE_SIZE(int x)     { return (x * 4); }


        public int eGFX_CALCULATE_1BPP_IMAGE_STORAGE_SPACE_SIZE(int x, int y)   { return (eGFX_CALCULATE_1BPP_BUFFER_ROW_BYTE_SIZE(x)	* y);}
        public int eGFX_CALCULATE_3BPP_IMAGE_STORAGE_SPACE_SIZE(int x, int y)           { return (((x * y * 3)) / 8); } //This is a special type  for sharp 8 color memory LCDs.   Packing is done in the data buffer so the dump to the screen is simple
        public int eGFX_CALCULATE_4BPP_IMAGE_STORAGE_SPACE_SIZE(int x, int y)   { return (eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(x)	* y);}
        public int eGFX_CALCULATE_8BPP_IMAGE_STORAGE_SPACE_SIZE(int x, int y)   { return (eGFX_CALCULATE_8BPP_BUFFER_ROW_BYTE_SIZE(x)	* y);}
        public int eGFX_CALCULATE_16BPP_IMAGE_STORAGE_SPACE_SIZE(int x, int y)  { return (eGFX_CALCULATE_16BPP_BUFFER_ROW_BYTE_SIZE(x)  * y); }
        public int eGFX_CALCULATE_24BPP_IMAGE_STORAGE_SPACE_SIZE(int x, int y)  { return (eGFX_CALCULATE_24BPP_BUFFER_ROW_BYTE_SIZE(x) * y); }
        public int eGFX_CALCULATE_32BPP_IMAGE_STORAGE_SPACE_SIZE(int x, int y) { return (eGFX_CALCULATE_32BPP_BUFFER_ROW_BYTE_SIZE(x) * y); }

        static public string GetImagePlaneTypePrefix(eGFX_ImagePlaneType IPT)
         {

            return IPT.ToString().Replace("eGFX_IMAGE_PLANE", "");

         }

        public eGFX_ImagePlane(int x, int y, eGFX_ImagePlaneType IPT)
        {
            SizeX = x;
            SizeY = y;
            
            Type = IPT;

            int BPP = (int)IPT >> 8;
            switch(BPP)
            {
                case 1:
                    Data = new byte[eGFX_CALCULATE_1BPP_BUFFER_ROW_BYTE_SIZE(x) * y];
                    break;

                case 3:
                    Data = new byte[eGFX_CALCULATE_3BPP_IMAGE_STORAGE_SPACE_SIZE(x,y)];
                    break;

                case 4:
                    Data = new byte[eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(x) * y];
                    break;


                case 8:
                    Data = new byte[eGFX_CALCULATE_8BPP_BUFFER_ROW_BYTE_SIZE(x) * y];
                    break;

                case 16:
                    this.Data = new byte[this.eGFX_CALCULATE_16BPP_BUFFER_ROW_BYTE_SIZE(x) * y];
                    break;

                case 24:
                    this.Data = new byte[this.eGFX_CALCULATE_24BPP_BUFFER_ROW_BYTE_SIZE(x) * y];
                    break;

                case 32:
                    this.Data = new byte[this.eGFX_CALCULATE_32BPP_BUFFER_ROW_BYTE_SIZE(x) * y];
                    break;
            }
           
 
            Clear();
        }

        public void Clear()
        {
            for(int i=0;i<Data.Length;i++)
            {
                Data[i] = 0;
            }
        }


        void BitStreamPut(UInt32 Position, UInt32 PixelState)
        {
            UInt32 BaseAddress = Position >> 3;
            UInt32 Offset = Position & 0x7;

            if (PixelState>0)
            {
                Data[BaseAddress] |= (byte)(1 << (Int32)Offset);
            }
            else
            {
                Data[BaseAddress] &= (byte)(~((1 << (Int32)Offset)));
            }
        }

        byte BitStreamGet(UInt32 Position)
        {

            UInt32 BaseAddress = Position >> 3;
            UInt32 Offset = Position & 0x7;

            return (byte)(Data[BaseAddress] & (byte)((byte)1 << (byte)Offset));

        }



        public void PutPixel(int x, int y, UInt32 PS)
        {
                int Offset;
                int Mask;
                int MemWidthInBytes;

                int BPP = (int)Type >> 8;
                //if((x<SizeX) && (y<SizeY) && (x>=0) && (y>=0))
                {
                    switch(BPP)
                    {
                        case  1:
                            //Properly Implement Rounding  for odd bit plane sizes
				            MemWidthInBytes = eGFX_CALCULATE_1BPP_BUFFER_ROW_BYTE_SIZE(SizeX);

                            Offset = (y * (MemWidthInBytes)) + (x>>3);
                            Mask = 0x01 << (x & 0x07);

                            if(PS > 0)
                            {
                                Data[Offset] |= (byte)Mask;
                            }
                            else
                            {
                                Data[Offset] &= (byte)~Mask;
                            }

                            break;


                    case 3:

                        {
                            //This image plane type is an odd ball case for the 3-color sharp memory LCD.
                            UInt32 BitStreamPosition = (UInt32)(x * 3 + y * (this.SizeX * 3));
                            BitStreamPut(BitStreamPosition++, PS & (UInt32)0x01);
                            BitStreamPut(BitStreamPosition++, PS & (UInt32)0x02);
                            BitStreamPut(BitStreamPosition,  PS & (UInt32)0x04);
                        }


                        break;

                    case  4:

                        //Properly Implement Rounding  for odd bit plane sizes
                        MemWidthInBytes = eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(SizeX);

                        Offset = (y * (MemWidthInBytes)) + (x >> 1);

                        byte b = (byte)(PS >> 4);

                        if ((x & 0x01) > 0)
                        {
                             Data[Offset] &= (byte)(0xF0);
                             Data[Offset] |= ((byte)(b & 0xF));

                        }
                        else
                        {

                            Data[Offset] &= (byte)(0x0F);
                            Data[Offset] |= ((byte)((b<<4)&0xF0));

                        }
                    
                        break;

      

                        case  8:
                            MemWidthInBytes  = SizeX;
                            Offset = (y * MemWidthInBytes) + x;
                            Data[Offset] = ((byte)PS);
                            break;

                        case 16:

                            MemWidthInBytes = SizeX * 2;
                            Offset = (y * MemWidthInBytes) + (x*2);
                            Data[Offset] = ((byte)PS);
                            Data[Offset + 1] = ((byte)(PS>>8));
                            break;



                    case 24:
                            MemWidthInBytes = SizeX * 3;
                            Offset = (y * MemWidthInBytes) + (x * 3);
                            Data[Offset] = ((byte)PS);
                            Data[Offset + 1] = ((byte)(PS >> 8));
                            Data[Offset + 2] = ((byte)(PS >> 16));

                            break;

                        case 32:
                            MemWidthInBytes = SizeX * 4;
                            Offset = (y * MemWidthInBytes) + (x * 4);
                            Data[Offset] = ((byte)PS);
                            Data[Offset + 1] = ((byte)(PS >> 8));
                            Data[Offset + 2] = ((byte)(PS >> 16));
                            Data[Offset + 3] = ((byte)(PS >> 24));

                            break;


                        default:
                            break;
                    }
                }


        }

        public UInt32 Get(int x,int y)
        {
            int Offset;
            int Mask;
            int MemWidthInBytes;
	         
            UInt32 PS = 0;

            int BPP = (int)Type >> 8;
           // if((x<SizeX) && (y<SizeY) && (x>=0) && (y>=0))
            {
                switch(BPP)
                {
                    case  1:
                        //Properly Implement Rounding  for odd bit plane sizes
				        MemWidthInBytes = eGFX_CALCULATE_1BPP_BUFFER_ROW_BYTE_SIZE(SizeX);

                        Offset = (y * (MemWidthInBytes)) + (x>>3);
                        Mask = 0x01 << (x & 0x07);

                        if((Data[Offset] & Mask) > 0)
                            PS = 0xFFFFFFFF;
                        else
                            PS = 0;

                        break;



                    case 3:

                        
                            //This image plane type is an odd ball case for the 3-color sharp memory LCD.
                            UInt32 BitStreamPosition = (UInt32)(x * 3 + y * this.SizeX * 3);
                            if (BitStreamGet(BitStreamPosition++)>0)
                                PS |= 0x01;
                            if (BitStreamGet(BitStreamPosition++)>0)
                                PS |= 0x02;
                            if (BitStreamGet(BitStreamPosition++)>0)
                                PS |= 0x04;

                        break;

                    case 4:

                        MemWidthInBytes = eGFX_CALCULATE_4BPP_BUFFER_ROW_BYTE_SIZE(SizeX);

                        Offset = (y * (MemWidthInBytes)) + (x >> 1);

                        if ((x & 0x01) > 0)
                            PS = (UInt32)Data[Offset] & 0x0F;
                        else
                            PS = (UInt32)(((Data[Offset]) >> 4) & 0x0F);

                        break;


                    case  8:
                        MemWidthInBytes  = SizeX;
                        Offset = (y * MemWidthInBytes) + x;
                        PS = Data[Offset];
                        break;

                    case 16:

                        MemWidthInBytes = this.SizeX * 2;
                        Offset = (y * MemWidthInBytes) + (x * 2);
                        PS = (uint)Data[Offset] + ((uint)Data[Offset + 1] << 8);
                        break;


                    case 24:
                        MemWidthInBytes = this.SizeX * 3;
                        Offset = (y * MemWidthInBytes) + (x * 3);
                        PS = (uint)Data[Offset] +
                             ((uint)Data[Offset + 1] << 8) +
                             ((uint)Data[Offset + 2] << 16);
                       
                        break;

                    case 32:
                        MemWidthInBytes = this.SizeX * 4;
                        Offset = (y * MemWidthInBytes) + (x * 4);
                        PS = (uint)Data[Offset] + 
                             ((uint)Data[Offset + 1] << 8)  +
                             ((uint)Data[Offset + 2] << 16) +
                             ((uint)Data[Offset + 3] << 24) ;

                        break;

                    default:
                        break;
                }
            }

          return PS;
        }
    }

    public delegate void PostMessage(String Msg);

    public class eGFX_Tools
    {

        public static int eGFX_API_LEVEL = 1;
        public static int eGFX_REVISION = 1;

        public event PostMessage MessagePost;
 
        const byte STARTING_ASCII_FONT_INDEX = 0x20;
        const byte ENDING_ASCII_FONT_INDEX = 0x80;

        public eGFX_Tools(PostMessage PCM)
        {
            MessagePost += PCM;
        }

        public void GenerateSprites(String SpriteDirectory,eGFX_ImagePlaneType IPT,string SECTION_MACRO_ID, string SectionDecorator,string OutputFolder)
        {
           string OutputBaseName;

           String Code = "";
           String Header = "";
           String CodeOutput = "";
           String HeaderOutput = "";
           String SpritePrefixName = "Sprite" + eGFX_ImagePlane.GetImagePlaneTypePrefix(IPT);

           if (Directory.Exists(SpriteDirectory) == false)
           {
               MessagePost(SpriteDirectory + " does not exist\r\n");
               return;
           }

           MessagePost("Scanning " + SpriteDirectory + " for files");

           string[] files = System.IO.Directory.GetFiles(SpriteDirectory, "*.png", System.IO.SearchOption.AllDirectories);
           

           if(files.Length == 0)
           {
               MessagePost("No image files specified in " + SpriteDirectory);
               return;
           }


           #region Translate Bitmaps

          for (int i = 0; i < files.Length; i++)
           {

               MessagePost("Translating " + files[i] + "\r\n");

               TranslateImage(files[i], ref Code, ref Header, SpritePrefixName+"_", IPT, SECTION_MACRO_ID);

              CodeOutput += Code;
              HeaderOutput += Header;
           }

           #endregion

           #region Write Code Files

               OutputBaseName = "Sprites";
               
               OutputBaseName += eGFX_ImagePlane.GetImagePlaneTypePrefix(IPT);

               MessagePost("output folder :" + OutputFolder);
               string OutputPath_C = Path.Combine(OutputFolder, OutputBaseName) + ".c";
               string OutputPath_H = Path.Combine(OutputFolder, OutputBaseName) + ".h";


               MessagePost("Writing sprites source file to " + OutputPath_C);
               MessagePost("Writing sprites header to " + OutputPath_H);

               TextWriter CodeTW = new StreamWriter(OutputPath_C, false);
               TextWriter HeaderTW = new StreamWriter(OutputPath_H, false);

               CodeTW.Write("#include \"eGFX.h\" \r\n");
  
               CodeTW.Write("#include \"" + OutputBaseName + ".h\" \r\n");

               HeaderTW.Write("#include \"eGFX.h\" \r\n\r\n");

               HeaderTW.Write("#ifndef _" + OutputBaseName.ToUpperInvariant() + "_H\r\n");
               HeaderTW.Write("#define _" + OutputBaseName.ToUpperInvariant() + "_H\r\n");

               HeaderTW.Write("\r\n");
               HeaderTW.Write("#ifndef "     + SECTION_MACRO_ID + "\r\n");
               HeaderTW.Write("    #define " + SECTION_MACRO_ID + " const " + SectionDecorator.Trim() + "\r\n");
               HeaderTW.Write("#endif " + "\r\n");
               HeaderTW.Write("\r\n");

               CodeTW.Write(CodeOutput);
               HeaderTW.Write(HeaderOutput);
              
               HeaderTW.WriteLine("\r\n#endif\r\n");

               HeaderTW.Close();
               CodeTW.Close();
          
           #endregion

               MessagePost("\r\n\r\nAll Done!\r\n\r\n");
        }

        public void InitFont(String PathToFontFolder, int Width, int Height)
        {
            if (MessagePost == null)
                MessagePost += SelfMessagePost;

            CheckFontDirectory(PathToFontFolder);
            
            CheckFontConfig(PathToFontFolder,null);
       
            GenerateFontSpriteSet(PathToFontFolder, Width, Height);
        }

        static public Bitmap BMP_Copy(Bitmap srcBitmap, Rectangle section)
            {
                // Create the new bitmap and associated graphics object
                Bitmap bmp = new Bitmap(section.Width, section.Height);
                Graphics g = Graphics.FromImage(bmp);



                for (int x = 0; x < section.Width; x++)
                {
                    for (int y = 0; y < section.Height; y++)
                    {
                        bmp.SetPixel(x, y,
                         srcBitmap.GetPixel(x + section.X, y + section.Y));

                    }
                }

                // Return the bitmap
                return bmp;
            }

        public void SliceDice(string ConfigFile)
        {
           
            SliceDiceConfig SDC;



            if(File.Exists(ConfigFile) == false)
            {
                PostToConsole("Could not find " + ConfigFile);    
                return;
            }
            

              Serializer MyConfigSerializer = new Serializer(typeof(SliceDiceConfig));
              StreamReader SxR = new StreamReader(ConfigFile);
              SDC = ( SliceDiceConfig)MyConfigSerializer.Deserialize(SxR);
              SxR.Close();


            //Do some basic image Analsysis
            Bitmap B;


            SDC.FontImage = Path.Combine(Path.GetDirectoryName(ConfigFile),SDC.FontImage);
            try
            {
                if(File.Exists(SDC.FontImage) == false)
                {
                    PostToConsole(SDC.FontImage + "not found.");
                }
                
                B = new Bitmap(SDC.FontImage);
            }
            catch (Exception Ex)
            {
                PostToConsole(Ex.Message);
                return;
            }

            int CalculatedNumberOfCharacters = (SDC.Rows * SDC.CharactersPerRow);

            PostToConsole("Scanning " + SDC.FontImage + " for " + " Elements.");


            int CalculatedImageHeight = ((SDC.FontElementHeight + SDC.PixelsBetweenElements) * SDC.Rows) - 1;  //  The pixel buffer isn't need at the edges of teh image.  Take 1 pixel from the talley
            int CalculatedImageWidth = ((SDC.FontElementWidth + SDC.PixelsBetweenElements) * SDC.CharactersPerRow) - 1;  //  The pixel buffer isn't need at the edges of teh image.  Take 1 pixel from the talley


            PostToConsole("Your BMP dimensions should be " +CalculatedImageWidth + "w x " + CalculatedImageHeight + "h ");

        
            PostToConsole("BMP Dimensions are " + B.Width + "w x " +  B.Height + "h");

            if(B.Width == CalculatedImageWidth && B.Height == CalculatedImageHeight)
            {
                PostToConsole("Geometry is OK.");
            }
            else
            {
               PostToConsole("Your Geometry sucks... try again");
                return;
            }


            for (int i = 0; i < CalculatedNumberOfCharacters; i++)
            {
                int x = i % SDC.CharactersPerRow;
                int y = i / SDC.CharactersPerRow;

                
                Bitmap FE = BMP_Copy(B, new Rectangle(x * (SDC.FontElementWidth + SDC.PixelsBetweenElements), y * (SDC.FontElementHeight + SDC.PixelsBetweenElements), SDC.FontElementWidth, SDC.FontElementHeight));

                if (SDC.UseASCII_Naming == true)
                {
                    FE.Save(Path.Combine(Path.GetDirectoryName(ConfigFile), MapASCII_CodeToString((byte)(i + SDC.StartingASCII_Character)) + ".png"));
                }
                else
                {
                    FE.Save(Path.Combine(Path.GetDirectoryName(ConfigFile),SDC.Prefix +  ( i + SDC.StartingASCII_Character) + ".png"));
                
                }
           }
        }

        public void CheckFontDirectory(String PathToFontFolder)
        {
            if (Directory.Exists(PathToFontFolder) == false)
            {
                Directory.CreateDirectory(PathToFontFolder);
                MessagePost(PathToFontFolder + " does not exist... Creating....");
            }
        }

        public void CheckFontConfig(String PathToFontFolder,Font F)
        {

            FontBuilderConfig MyFontBuilderConfig;


            if (File.Exists(Path.Combine(PathToFontFolder, "FontConfiguration.jsx")) == false)
            {
                MessagePost("Building Configuration...");

                MyFontBuilderConfig = new FontBuilderConfig();
                FileInfo fInfo = new FileInfo(Path.Combine(PathToFontFolder, "FontConfiguration.jsx"));
                MyFontBuilderConfig.CombinedOutputName = fInfo.Directory.Name;

                Directory.CreateDirectory(Path.Combine(PathToFontFolder, "SrcImages"));

                MyFontBuilderConfig.InterCharacterSpacing = 1;
                MyFontBuilderConfig.SpacesPerTab = 4;

                if (F != null)
                {
                    MyFontBuilderConfig.Ascent = (int)((double)F.Size * (double)F.FontFamily.GetCellAscent(F.Style) / (double)F.FontFamily.GetEmHeight(F.Style))*256;
                    MyFontBuilderConfig.Descent = (int)((double)F.Size * (double)F.FontFamily.GetCellDescent(F.Style) / (double)F.FontFamily.GetEmHeight(F.Style))*256;
                    MyFontBuilderConfig.LineSpacing = (int)((double)F.Size * (double)F.FontFamily.GetLineSpacing(F.Style) / (double)F.FontFamily.GetEmHeight(F.Style))*256;
                }
                else
                {
                    MyFontBuilderConfig.Ascent = 0;
                    MyFontBuilderConfig.Descent = 0;
                    MyFontBuilderConfig.LineSpacing = 0;
                }

                Serializer MySerializer = new Serializer(typeof(FontBuilderConfig));
                StreamWriter SR = new StreamWriter(Path.Combine(PathToFontFolder, "FontConfiguration.jsx"));
                MySerializer.Serialize(MyFontBuilderConfig, SR);
                SR.Close();
            }
            else
            {
                MessagePost("Font configuration file found...");
            }
        }

        public void GenerateFontSpriteSet(String PathToFontFolder, int Width, int Height)
        {
            for (byte i = STARTING_ASCII_FONT_INDEX; i < ENDING_ASCII_FONT_INDEX; i++)
            {
                string FileName = MapASCII_CodeToString(i) + ".png";

                if (File.Exists(Path.Combine(PathToFontFolder, "SrcImages", FileName)) == false)
                {

                    Bitmap B = new Bitmap(Width, Height);
                    Graphics g = Graphics.FromImage(B);
                 
                    B.Save(Path.Combine(PathToFontFolder, "SrcImages", FileName));
                }
            }
        }

        public void Remap(String PathToFontBuilder)
        {
            for (byte i = STARTING_ASCII_FONT_INDEX; i < ENDING_ASCII_FONT_INDEX; i++)
            {
                
                if(File.Exists(Path.Combine(PathToFontBuilder,"SrcImages","0x" + i.ToString("X2") + ".png")) == true)
                {
                    File.Move(Path.Combine(PathToFontBuilder,"SrcImages","0x" + i.ToString("X2") + ".png"),
                              Path.Combine(PathToFontBuilder,"SrcImages",MapASCII_CodeToString(i) + ".png"));
                    Console.WriteLine(Path.Combine(PathToFontBuilder, "SrcImages", "0x" + i.ToString("X2") + ".png") + " ---> " +
                                      Path.Combine(PathToFontBuilder, "SrcImages", MapASCII_CodeToString(i) + ".png"));
                
                }
            }

        }

        public int[] GetFirstPassCharacterWidths(Font F,TextRenderingHint TRH)
        {
            int[] FPCW = new int[ENDING_ASCII_FONT_INDEX + 1];

            //We need a graphics class to measure the string
            Bitmap TempB = new Bitmap((int)F.GetHeight(), (int)F.GetHeight());
            Graphics TempG = Graphics.FromImage(TempB);
            TempG.TextRenderingHint = TRH;

            
            for (int i = STARTING_ASCII_FONT_INDEX; i <= ENDING_ASCII_FONT_INDEX; i++)
            {
                string S = char.ConvertFromUtf32(i);

                SizeF Size = TempG.MeasureString(S, F);

                FPCW[i] = (int)Math.Ceiling(Size.Width);

            }

            return FPCW;

        }

        public Bitmap[] GenerateFirstPassBitmaps(Font F, int[] FirstPassCharacterWidths, TextRenderingHint TRH)
        {
            Bitmap[] B = new Bitmap[ENDING_ASCII_FONT_INDEX + 1];

            for (int i = STARTING_ASCII_FONT_INDEX; i <= ENDING_ASCII_FONT_INDEX; i++)
            {
                B[i] = new Bitmap(FirstPassCharacterWidths[i], (int)Math.Ceiling(F.GetHeight()) + 1);
                Graphics G = Graphics.FromImage(B[i]);
                G.Clear(Color.Black);
                string S = char.ConvertFromUtf32(i);
                G.TextRenderingHint = TRH;
                G.DrawString(S, F, Brushes.White, 0f, 0f);
            }

            return B;
        }

        public bool GetVerticleClipBounds(Bitmap B, ref int VSI, ref int VEI)
        {

            int VerticalStartIndex = 0;
            int VerticalEndIndex = B.Height - 1;
            bool VerticalStartIndexFound = false;
            bool VerticalEndIndexFound = false;


            while (VerticalStartIndex < VerticalEndIndex)
            {
                for (int i = 0; i < B.Width; i++)
                {
                    if (B.GetPixel(i, VerticalStartIndex) != Color.FromArgb(0, 0, 0))
                        VerticalStartIndexFound = true;
                }

                if (VerticalStartIndexFound == true)
                    break;
                else
                    VerticalStartIndex++;
            }

            while (VerticalEndIndex > -1)
            {
                for (int i = 0; i < B.Width; i++)
                {
                    if (B.GetPixel(i, VerticalEndIndex) != Color.FromArgb(0, 0, 0))
                        VerticalEndIndexFound = true;
                }

                if (VerticalEndIndexFound == true)
                    break;
                else
                    VerticalEndIndex--;
            }

            VSI = VerticalStartIndex;
            VEI = VerticalEndIndex;

            return VerticalStartIndexFound;
        }

        public bool GetHorizontalClipBounds(Bitmap B, ref int HSI, ref int HEI)
        {

            int HorizontalStartIndex = 0;
            int HorizontalEndIndex = B.Width - 1;
            bool HorizontalStartIndexFound = false;
            bool HorizontalEndIndexFound = false;

            while (HorizontalStartIndex < HorizontalEndIndex)
            {
                for (int i = 0; i < B.Height; i++)
                {
                    if (B.GetPixel(HorizontalStartIndex, i) != Color.FromArgb(0, 0, 0))
                        HorizontalStartIndexFound = true;
                }

                if (HorizontalStartIndexFound == true)
                    break;
                else
                    HorizontalStartIndex++;
            }

            while (HorizontalEndIndex > -1)
            {
                for (int i = 0; i < B.Height; i++)
                {
                    if (B.GetPixel(HorizontalEndIndex, i) != Color.FromArgb(0, 0, 0))
                        HorizontalEndIndexFound = true;
                }

                if (HorizontalEndIndexFound == true)
                    break;
                else
                    HorizontalEndIndex--;
            }

            HSI = HorizontalStartIndex;
            HEI = HorizontalEndIndex;

            return HorizontalStartIndexFound;
        }

        public Bitmap[] TrimFirstPassBitMaps(Bitmap[] B)
        {
            int VerticalStartIndex = B[STARTING_ASCII_FONT_INDEX].Height - 1;
            int VerticalEndIndex = 0;

            int TmpVerticalStartIndex = B[STARTING_ASCII_FONT_INDEX].Height - 1;
            int TmpVerticalEndIndex = 0;


            int HorizontalStartIndex = B[STARTING_ASCII_FONT_INDEX].Height - 1;
            int HorizontalEndIndex = 0;

            int TmpHorizontalStartIndex = B[STARTING_ASCII_FONT_INDEX].Height - 1;
            int TmpHorizontalEndIndex = 0;

            for (int i = STARTING_ASCII_FONT_INDEX; i <= ENDING_ASCII_FONT_INDEX; i++)
            {
                // if we detect data in the bitmap (i.e. it is not a white space character) then continue finding the min/max
                if (GetVerticleClipBounds(B[i], ref TmpVerticalStartIndex, ref TmpVerticalEndIndex) == true)
                {
                    if (TmpVerticalStartIndex < VerticalStartIndex)
                        VerticalStartIndex = TmpVerticalStartIndex;
                    if (TmpVerticalEndIndex > VerticalEndIndex)
                        VerticalEndIndex = TmpVerticalEndIndex;
                }
            }

            Bitmap[] RB = new Bitmap[ENDING_ASCII_FONT_INDEX + 1];

            for (int i = STARTING_ASCII_FONT_INDEX; i <= ENDING_ASCII_FONT_INDEX; i++)
            {
                if (GetHorizontalClipBounds(B[i], ref TmpHorizontalStartIndex, ref TmpHorizontalEndIndex) == true)
                {
                    HorizontalEndIndex = TmpHorizontalEndIndex;
                    HorizontalStartIndex = TmpHorizontalStartIndex;
                }
                else
                {
                    HorizontalStartIndex = 0;
                    HorizontalEndIndex = B[i].Width - 1;
                }

                RB[i] = B[i].Clone(new Rectangle(HorizontalStartIndex,
                                                 VerticalStartIndex,
                                                 HorizontalEndIndex - HorizontalStartIndex + 1,
                                                 VerticalEndIndex - VerticalStartIndex + 1)
                                                 , B[i].PixelFormat);
            }

            return RB;
        }

        public void GenerateFontConfigFile(String PathToFontFolder, Font F, TextRenderingHint TRH)
        {
            FontBuilderConfig MyFontBuilderConfig;

                MessagePost("Building Configuration...");

                MyFontBuilderConfig = new FontBuilderConfig();
                FileInfo fInfo = new FileInfo(Path.Combine(PathToFontFolder, "FontConfiguration.jsx"));
                MyFontBuilderConfig.CombinedOutputName = fInfo.Directory.Name;

                MyFontBuilderConfig.InterCharacterSpacing = 1;
       
                MyFontBuilderConfig.SpacesPerTab = 4;

                Serializer MySerializer = new Serializer(typeof(FontBuilderConfig));
                StreamWriter SR = new StreamWriter(Path.Combine(PathToFontFolder, "FontConfiguration.jsx"));
                MySerializer.Serialize(MyFontBuilderConfig, SR);
                SR.Close();
            
        }

        public void GenerateTT_FontSpriteSet(String PathToFontFolder, Font F, TextRenderingHint TRH)
        {

            int[] FirstPassCharacterWidths = new int[ENDING_ASCII_FONT_INDEX + 1];
            Bitmap[] FirstPassBitmaps = new Bitmap[ENDING_ASCII_FONT_INDEX + 1];
            Bitmap[] FinalBitmaps = new Bitmap[ENDING_ASCII_FONT_INDEX + 1];

            FirstPassCharacterWidths = GetFirstPassCharacterWidths(F,TRH);

            FirstPassBitmaps = GenerateFirstPassBitmaps(F,FirstPassCharacterWidths, TRH);

            FinalBitmaps = TrimFirstPassBitMaps(FirstPassBitmaps);

            for (byte i = STARTING_ASCII_FONT_INDEX; i <= ENDING_ASCII_FONT_INDEX; i++)
            {
                string FileName = MapASCII_CodeToString(i) + ".png";

                try
                {
                    File.Delete(Path.Combine(PathToFontFolder, "SrcImages", FileName));
                }

                catch
                {
                }

                FinalBitmaps[i].Save(Path.Combine(PathToFontFolder, "SrcImages", FileName));
            }

            //Write the config file....



        }

        public void InitFontFromTrueType(String PathToFontFolder, Font F, TextRenderingHint TRH)
        {
            if (MessagePost == null)
                MessagePost += SelfMessagePost;

            CheckFontDirectory(PathToFontFolder);

            MessagePost("Cleaning " + PathToFontFolder);

            System.IO.DirectoryInfo DI = new DirectoryInfo(PathToFontFolder);

            foreach (FileInfo file in DI.GetFiles())
            {

                try
                {
                    file.Delete();
                }
                catch (Exception Ex)
                {
                    MessagePost("Could note delete" + file.Name + ":" + Ex.Message);
                }
            }
            foreach (DirectoryInfo dir in DI.GetDirectories())
            {
                try
                {
                    dir.Delete(true);
                }
                catch (Exception Ex)
                {
                    MessagePost("Could note delete" + dir.Name + ":" + Ex.Message);
                }
            }

            CheckFontConfig(PathToFontFolder,F);

            GenerateTT_FontSpriteSet(PathToFontFolder, F, TRH);

            MessagePost("Init TT  font operation complete");
        }

        public void FontBuilder(String PathToFontFolder, string OutputFolder,  eGFX_ImagePlaneType IPT, string SECTION_MACRO_ID, string SectionDecorator)
        {
            if (MessagePost == null)
                 MessagePost += SelfMessagePost;


            #region Read In Configuration

              FontBuilderConfig MyFontBuilderConfig;
              Serializer MyConfigSerializer = new Serializer(typeof(FontBuilderConfig));
              StreamReader SxR = new StreamReader(Path.Combine(PathToFontFolder, "FontConfiguration.jsx"));
              MyFontBuilderConfig = (FontBuilderConfig)MyConfigSerializer.Deserialize(SxR);
              SxR.Close();
            #endregion
        
            #region Translate Bitmaps

            String Code ="";
            String Header = "";

            String CodeOutput = "";
            String HeaderOutput = "";



            for (byte i = STARTING_ASCII_FONT_INDEX; i < ENDING_ASCII_FONT_INDEX; i++)
            {
                string FileName = MapASCII_CodeToString(i) +  ".png";
                TranslateImage(Path.Combine(PathToFontFolder, "SrcImages", FileName), ref Code, ref Header, MyFontBuilderConfig.CombinedOutputName+"_"+"FontElement_",IPT, SECTION_MACRO_ID);
                CodeOutput += Code;
            }

            CodeOutput += "\r\n\r\n";
            CodeOutput += SECTION_MACRO_ID + " " + "eGFX_Font " + MyFontBuilderConfig.CombinedOutputName + " = { \r\n";
            CodeOutput += "\t\t\t\t\t\t\t.CharacterSprites = {\r\n";

            for (byte i = STARTING_ASCII_FONT_INDEX; i < ENDING_ASCII_FONT_INDEX; i++)
            {

                if (i >= 0x30 && i <= 0x39)
                {
                    //in this case the translate bit map we rename 0.png to _0.png to avoid C names starting with a number.  add in an extra _ to make everything happy
                    CodeOutput += "\t\t\t\t\t\t\t\t(const eGFX_ImagePlane *)&" + MyFontBuilderConfig.CombinedOutputName + "_" + "FontElement_" + "_" + MapASCII_CodeToString(i);
                }
                else
                {
                    CodeOutput += "\t\t\t\t\t\t\t\t(const eGFX_ImagePlane *)&" + MyFontBuilderConfig.CombinedOutputName + "_" + "FontElement_" + MapASCII_CodeToString(i);
                }
                if(i!= ENDING_ASCII_FONT_INDEX-1)
                {
                    CodeOutput += ",\r\n";
                }
                else
                {
                    CodeOutput += "\r\n";

                }

            }

            CodeOutput += "\t\t\t\t\t\t\t},\r\n";


            CodeOutput += "\t\t\t\t\t\t\t.Ascent = " + MyFontBuilderConfig.Ascent+ ", //TT Metric : Ascent - Scaled up by 65536 (q15.16) \r\n";
            CodeOutput += "\t\t\t\t\t\t\t.Descent = " + MyFontBuilderConfig.Descent + ", //TT Metric : Descent - Scaled up by 65536 (q15.16)\r\n";
            CodeOutput += "\t\t\t\t\t\t\t.LineSpacing = " + MyFontBuilderConfig.LineSpacing + ", //TT Metric : Linespacing -Scaled up by 65536 (q15,16)\r\n";
            CodeOutput += "\t\t\t\t\t\t\t.SpacesPerTab = " + MyFontBuilderConfig.SpacesPerTab + ", //Spaces per tab\r\n";
            CodeOutput += "\t\t\t\t\t\t\t.InterCharacterSpacing = " + MyFontBuilderConfig.InterCharacterSpacing + ", //InterCharacterSpacing\r\n";
            CodeOutput += "\t\t\t\t\t\t\t.Padding = {0,0} //Padding Bytes\r\n";
            
            CodeOutput += "\t\t\t\t\t\t};\r\n";


            HeaderOutput += "\r\n#ifndef " + SECTION_MACRO_ID + "\r\n";
            HeaderOutput += "    #define " + SECTION_MACRO_ID + " const " + SectionDecorator.Trim() +"\r\n";
            HeaderOutput += "#endif\r\n\r\n";

            HeaderOutput += "extern " + SECTION_MACRO_ID + " eGFX_Font " + MyFontBuilderConfig.CombinedOutputName + ";\r\n\r\n";

            #endregion

            #region Write Code Files

         
            if (MyFontBuilderConfig.CombinedOutputName == "" || MyFontBuilderConfig.CombinedOutputName == null)
                {
                    MyFontBuilderConfig.CombinedOutputName = "MyFont";
                }
                TextWriter CodeTW = new StreamWriter(Path.Combine(OutputFolder, MyFontBuilderConfig.CombinedOutputName + ".c"));
                TextWriter HeaderTW = new StreamWriter(Path.Combine(OutputFolder, MyFontBuilderConfig.CombinedOutputName + ".h"));

                CodeTW.Write("#include \"stdint.h\" \r\n");
                CodeTW.Write("#include \"eGFX.h\" \r\n");
                CodeTW.Write("#include \"" + MyFontBuilderConfig.CombinedOutputName + ".h\" \r\n");

                HeaderTW.Write("#include \"eGFX.h\" \r\n\r\n");


                HeaderTW.Write("#ifndef _" + MyFontBuilderConfig.CombinedOutputName.ToUpperInvariant() + "_H\r\n");
                HeaderTW.Write("#define _" +MyFontBuilderConfig.CombinedOutputName.ToUpperInvariant() + "_H\r\n\r\n");

                CodeTW.Write(CodeOutput);

                HeaderTW.Write(HeaderOutput);
                
                HeaderTW.WriteLine("#endif\r\n");

                HeaderTW.Close();

                CodeTW.Close();
            
           
            #endregion
           
        }

        UInt32 Luma(Color PixelColor)
        {
            
            UInt32 L = (UInt32)((PixelColor.R * 0.299) + (PixelColor.G * 0.587) + (PixelColor.B * 0.114));

            if(L>255)
            {
                L = 255;
            }

            return L;

        }

        UInt32 RemapPixelColor(eGFX_ImagePlaneType IPT, Color PixelColor)
        {

            UInt32 PS = 0;
           

            switch (IPT)
            {
                case eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_1BPP:

                    PS = Luma(PixelColor);

                    if (PS > 0x7f)
                    {
                        PS = 0xFFFFFFFF;  //median cut
                    }
                    else
                    {
                        PS = 0;
                    }

                    break;


                case eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_3BPP_BGR:

                    if (PixelColor.R > 127)
                        PS |= 0x01;

                    if (PixelColor.G > 127)
                        PS |= 0x02;

                    if (PixelColor.B > 127)
                        PS |= 0x04;

                    break;

                case eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_4BPP_XBGR:

                    if (PixelColor.R > 127)
                        PS |= 0x01;

                    if (PixelColor.G > 127)
                        PS |= 0x02;

                    if (PixelColor.B > 127)
                        PS |= 0x04;

                    break;

                case eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_4BPP:

                    PS = Luma(PixelColor);
                  
                    break;

                case eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_8BPP:

                    PS = Luma(PixelColor); 

                    break;


                case eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_8BPP_XRGB222:

                    PS = (uint)(((PixelColor.R >> 6) << 4) +
                             ((PixelColor.G >> 6) << 2) +
                             ((PixelColor.B >> 6)));



                    break;


                case eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_16BPP_RGB565:
           
                    PS = (uint)(((int)PixelColor.R >> 3 << 11) + ((int)PixelColor.G >> 2 << 5) + ((int)PixelColor.B >> 3));
           
                    break;
                
                case eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_16BPP_BGR565:
                   
                    PS = (uint)(((int)PixelColor.B >> 3 << 11) + ((int)PixelColor.G >> 2 << 5) + ((int)PixelColor.R >> 3));
                    
                    break;


                case eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_16BPP_GBRG3553:


                    uint R5_GBRG = (uint)((((int)PixelColor.R) >> 3));
                    uint B6_GBRG = (uint)((((int)PixelColor.B) >> 3));
                    uint G5_GBRG = (uint)((((int)PixelColor.G) >> 2));

                    PS = R5_GBRG << 3;

                    PS |= B6_GBRG << 8;

                    PS |= (G5_GBRG >> 3);

                    PS |= (G5_GBRG & 0x7) << 13;

                   
                    break;

                case eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_16BPP_GRBG3553:

                    uint R5_GRBG = (uint)((((int)PixelColor.R) >> 3));
                    uint B6_GRBG = (uint)((((int)PixelColor.B) >> 3));
                    uint G5_GRBG = (uint)((((int)PixelColor.G) >> 2));

                    PS = R5_GRBG << 8;

                    PS |= B6_GRBG << 3;

                    PS |= (G5_GRBG >> 3);

                    PS |= (G5_GRBG & 0x7) << 13;

                    break;

                case eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_24BPP:
                case eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_24BPP_RGB888:

                    PS = (uint)(((int)PixelColor.R) +
                                  ((int)PixelColor.G << 8) +
                                  ((int)PixelColor.B << 24));

                    break;

                case eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_32BPP:
                case eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_32BPP_XRGB888:

                    PS = (uint)(((int)PixelColor.B) +
                            ((int)PixelColor.G << 8) +
                            ((int)PixelColor.R << 16) +
                            ((int)0xFF << 24));

                    break;


            }



            return PS;
        }

        void TranslateImage(string ImageFileName, ref string CodeOutput, ref string HeaderOutput,string SpriteStructNamePrefix, eGFX_ImagePlaneType IPT,string SECTION_MACRO_ID)
        {
            CodeOutput = "";
            HeaderOutput = "";

            if (File.Exists(ImageFileName) == false)
            {
                MessagePost("Could not open " + ImageFileName);
                return;
            }

            Bitmap NextBmp = new Bitmap(ImageFileName);

            MessagePost(ImageFileName + " is " + NextBmp.Width + " x " + NextBmp.Height + " pixels");

            string SpriteNameBase = Path.GetFileName(ImageFileName);
            string [] Splits = SpriteNameBase.Split('.');
            string SpriteName = Splits[0];

            SpriteName = SpriteName.Replace(" ", "_");
            SpriteName = SpriteName.Replace("@", "_");
            SpriteName = SpriteName.Replace("#", "_");
            SpriteName = SpriteName.Replace("$", "_");
            SpriteName = SpriteName.Replace("%", "_");
            SpriteName = SpriteName.Replace("^", "_");
            SpriteName = SpriteName.Replace("&", "_");
            SpriteName = SpriteName.Replace("*", "_");
            SpriteName = SpriteName.Replace("(", "_");
            SpriteName = SpriteName.Replace(")", "_");

            SpriteName = SpriteName.Replace("[", "_");
            SpriteName = SpriteName.Replace("]", "_");
            SpriteName = SpriteName.Replace("-", "_");
            SpriteName = SpriteName.Replace("=", "_");
            SpriteName = SpriteName.Replace("+", "_");
            SpriteName = SpriteName.Replace(",", "_");
            SpriteName = SpriteName.Replace("~", "_");
            SpriteName = SpriteName.Replace("?", "_");
            SpriteName = SpriteName.Replace("\"", "_");
            SpriteName = SpriteName.Replace(":", "_");
            SpriteName = SpriteName.Replace(";", "_");
            SpriteName = SpriteName.Replace("'", "_");
            SpriteName = SpriteName.Replace(".", "_");
            SpriteName = SpriteName.Replace(">", "_");
            SpriteName = SpriteName.Replace("<", "_");
            SpriteName = SpriteName.Replace("`", "_");
            SpriteName = SpriteName.Replace("~", "_");

            
            if(char.IsNumber(SpriteName[0]))
            {
                SpriteName = "_" + SpriteName;
            }

            #region Binary Sprite Form

            eGFX_ImagePlane NextSprite = new eGFX_ImagePlane(NextBmp.Width, NextBmp.Height, IPT);

            #region Gather Sprite Data

            Color PixelColor = new Color();

            int Height = NextBmp.Height;
            int Width = NextBmp.Width;


            for (int y = 0; y < Height; y++)
                          {
                            for (int x = 0; x < Width; x++)
                            {
                                 PixelColor = NextBmp.GetPixel(x, y);

                                 NextSprite.PutPixel(x, y, RemapPixelColor(IPT , PixelColor));

                            }

                          }

                    #endregion

                    string RawDataArrayString = SpriteStructNamePrefix + SpriteName + "_Data";
                    string SpriteStructNameString = SpriteStructNamePrefix + SpriteName;


                StringBuilder CodeString = new StringBuilder();
                StringBuilder HeaderString = new StringBuilder();


                CodeString.Append("\r\n");
                CodeString.Append("////***************************************************************************\r\n");
                CodeString.Append("////                            " +SpriteStructNameString+ "\r\n");
                CodeString.Append("////***************************************************************************\r\n");

                HeaderString.Append("\r\n");
                HeaderString.Append("////***************************************************************************\r\n");
                HeaderString.Append("////                         " + SpriteStructNameString + "\r\n");
                HeaderString.Append("////***************************************************************************\r\n\r\n");

                HeaderString.Append("\r\n" + "extern " + SECTION_MACRO_ID + " eGFX_ImagePlane \t" + SpriteStructNameString + ";\r\n");

                CodeString.Append("\r\n" + SECTION_MACRO_ID + " uint8_t " + RawDataArrayString + "[" + NextSprite.Data.Length + "] = { "); 
                
                for(int i=0;i< NextSprite.Data.Length;i++)
                {
                    if(i%16 == 0)
                    {
                    CodeString.Append("\r\n");
                    }

                    CodeString.Append("0x");
                    CodeString.AppendFormat("{0:X2}", NextSprite.Data[i]);

                if (i == NextSprite.Data.Length - 1)
                    {
                    CodeString.Append(" }; \r\n\r\n");
                    }
                    else
                    {
                    CodeString.Append( ",");
                    }
                }

         
            CodeString.Append("\r\n" + SECTION_MACRO_ID + " eGFX_ImagePlane " + SpriteStructNameString + " =\r\n{\r\n" +
                
                ".Type =  " + IPT.ToString()  + ", \r\n" + 
                ".Data = (uint8_t *)" + RawDataArrayString + ", \r\n" +
                ".SizeX = " + NextBmp.Width   + ", \r\n" +
                ".SizeY = " + NextBmp.Height  + ", \r\n" +
               
                ".User = NULL\r\n" +
                
                "};\r\n");


            #endregion

            CodeOutput = CodeString.ToString();
            HeaderOutput = HeaderString.ToString();

          
        }

        void SelfMessagePost(String Msg)
        {
            
        }

        void PostToConsole(String Msg)
        {
            Console.WriteLine(Msg);
        }

        byte MapStringToASCII_Code(string input)
        {
            byte i = 0;

            switch (input)
            {
                case  "space": i=0x20; break;
                    case  "exclamation_point": i=0x21; break;
                    case  "double_quotes": i=0x22; break;
                    case  "pound": i=0x23; break;
                    case  "dollar_sign": i=0x24; break;
                    case  "precent_sign": i=0x25; break;
                    case  "ampersand": i=0x26; break;
                    case  "single_quote": i=0x27; break;
                    case  "left_parenthesis": i=0x28; break;
                    case  "right_parenthesis": i=0x29; break;
                    case  "star": i=0x2A; break;
                    case  "plus": i=0x2B; break;
                    case  "comma": i=0x2C; break;
                    case  "dash": i=0x2D; break;
                    case  "period": i=0x2E; break;
                    case  "forward_slash": i=0x2F; break;
                    case  "0": i=0x30; break;
                    case  "1": i=0x31; break;
                    case  "2": i=0x32; break;
                    case  "3": i=0x33; break;
                    case  "4": i=0x34; break;
                    case  "5": i=0x35; break;
                    case  "6": i=0x36; break;
                    case  "7": i=0x37; break;
                    case  "8": i=0x38; break;
                    case  "9": i=0x39; break;
                    case  "colon": i=0x3A; break;
                    case  "semicolon": i=0x3B; break;
                    case  "less_than": i=0x3C; break;
                    case  "equals": i=0x3D; break;
                    case  "great_than": i=0x3E; break;
                    case  "question_mark": i=0x3F; break;
                    case  "at": i=0x40; break;
                    case  "A": i=0x41; break;
                    case  "B": i=0x42; break;
                    case  "C": i=0x43; break;
                    case  "D": i=0x44; break;
                    case  "E": i=0x45; break;
                    case  "F": i=0x46; break;
                    case  "G": i=0x47; break;
                    case  "H": i=0x48; break;
                    case  "I": i=0x49; break;
                    case  "J": i=0x4A; break;
                    case  "K": i=0x4B; break;
                    case  "L": i=0x4C; break;
                    case  "M": i=0x4D; break;
                    case  "N": i=0x4E; break;
                    case  "O": i=0x4F; break;
                    case  "P": i=0x50; break;
                    case  "Q": i=0x51; break;
                    case  "R": i=0x52; break;
                    case  "S": i=0x53; break;
                    case  "T": i=0x54; break;
                    case  "U": i=0x55; break;
                    case  "V": i=0x56; break;
                    case  "W": i=0x57; break;
                    case  "X": i=0x58; break;
                    case  "Y": i=0x59; break;
                    case  "Z": i=0x5A; break;
                    case  "left_bracket": i=0x5B; break;
                    case  "backslash": i=0x5C; break;
                    case  "right_bracket": i=0x5D; break;
                    case  "carret": i=0x5E; break;
                    case  "underscore": i=0x5F; break;
                    case  "slanted_single_quote": i=0x60; break;
                    case  "a": i=0x61; break;
                    case  "b": i=0x62; break;
                    case  "c": i=0x63; break;
                    case  "d": i=0x64; break;
                    case  "e": i=0x65; break;
                    case  "f": i=0x66; break;
                    case  "g": i=0x67; break;
                    case  "h": i=0x68; break;
                    case  "i": i=0x69; break;
                    case  "j": i=0x6A; break;
                    case  "k": i=0x6B; break;
                    case  "l": i=0x6C; break;
                    case  "m": i=0x6D; break;
                    case  "n": i=0x6E; break;
                    case  "o": i=0x6F; break;
                    case  "p": i=0x70; break;
                    case  "q": i=0x71; break;
                    case  "r": i=0x72; break;
                    case  "s": i=0x73; break;
                    case  "t": i=0x74; break;
                    case  "u": i=0x75; break;
                    case  "v": i=0x76; break;
                    case  "w": i=0x77; break;
                    case  "x": i=0x78; break;
                    case  "y": i=0x79; break;
                    case  "z": i=0x7A; break;
                    case  "left_curly_brace": i=0x7B; break;
                    case  "vertical_bar": i=0x7C; break;
                    case  "right_curly_brace": i=0x7D; break;
                    case  "tilde": i=0x7E; break;
                    case "delete": i = 0x7F; break;

                    default: i = 0; break;


            }
            return i;
        }

        string MapASCII_CodeToString(byte input)
        {
            string i="";

            switch(input)
            {
                case 0x20: i = "space"; break;
                case 0x21: i = "exclamation_point"; break;
                case 0x22: i = "double_quotes"; break;
                case 0x23: i = "pound"; break;
                case 0x24: i = "dollar_sign"; break;
                case 0x25: i = "precent_sign"; break;
                case 0x26: i = "ampersand"; break;
                case 0x27: i = "single_quote"; break;
                case 0x28: i = "left_parenthesis"; break;
                case 0x29: i = "right_parenthesis"; break;
                case 0x2A: i = "star"; break;
                case 0x2B: i = "plus"; break;
                case 0x2C: i = "comma"; break;
                case 0x2D: i = "dash"; break;
                case 0x2E: i = "period"; break;
                case 0x2F: i = "forward_slash"; break;
                case 0x30: i = "0"; break;
                case 0x31: i = "1"; break;
                case 0x32: i = "2"; break;
                case 0x33: i = "3"; break;
                case 0x34: i = "4"; break;
                case 0x35: i = "5"; break;
                case 0x36: i = "6"; break;
                case 0x37: i = "7"; break;
                case 0x38: i = "8"; break;
                case 0x39: i = "9"; break;
                case 0x3A: i = "colon"; break;
                case 0x3B: i = "semicolon"; break;
                case 0x3C: i = "less_than"; break;
                case 0x3D: i = "equals"; break;
                case 0x3E: i = "great_than"; break;
                case 0x3F: i = "question_mark"; break;
                case 0x40: i = "at"; break;
                case 0x41: i = "uc_A"; break;
                case 0x42: i = "uc_B"; break;
                case 0x43: i = "uc_C"; break;
                case 0x44: i = "uc_D"; break;
                case 0x45: i = "uc_E"; break;
                case 0x46: i = "uc_F"; break;
                case 0x47: i = "uc_G"; break;
                case 0x48: i = "uc_H"; break;
                case 0x49: i = "uc_I"; break;
                case 0x4A: i = "uc_J"; break;
                case 0x4B: i = "uc_K"; break;
                case 0x4C: i = "uc_L"; break;
                case 0x4D: i = "uc_M"; break;
                case 0x4E: i = "uc_N"; break;
                case 0x4F: i = "uc_O"; break;
                case 0x50: i = "uc_P"; break;
                case 0x51: i = "uc_Q"; break;
                case 0x52: i = "uc_R"; break;
                case 0x53: i = "uc_S"; break;
                case 0x54: i = "uc_T"; break;
                case 0x55: i = "uc_U"; break;
                case 0x56: i = "uc_V"; break;
                case 0x57: i = "uc_W"; break;
                case 0x58: i = "uc_X"; break;
                case 0x59: i = "uc_Y"; break;
                case 0x5A: i = "uc_Z"; break;
                case 0x5B: i = "left_bracket"; break;
                case 0x5C: i = "backslash"; break;
                case 0x5D: i = "right_bracket"; break;
                case 0x5E: i = "carret"; break;
                case 0x5F: i = "underscore"; break;
                case 0x60: i = "slanted_single_quote"; break;
                case 0x61: i = "lc_a"; break;
                case 0x62: i = "lc_b"; break;
                case 0x63: i = "lc_c"; break;
                case 0x64: i = "lc_d"; break;
                case 0x65: i = "lc_e"; break;
                case 0x66: i = "lc_f"; break;
                case 0x67: i = "lc_g"; break;
                case 0x68: i = "lc_h"; break;
                case 0x69: i = "lc_i"; break;
                case 0x6A: i = "lc_j"; break;
                case 0x6B: i = "lc_k"; break;
                case 0x6C: i = "lc_l"; break;
                case 0x6D: i = "lc_m"; break;
                case 0x6E: i = "lc_n"; break;
                case 0x6F: i = "lc_o"; break;
                case 0x70: i = "lc_p"; break;
                case 0x71: i = "lc_q"; break;
                case 0x72: i = "lc_r"; break;
                case 0x73: i = "lc_s"; break;
                case 0x74: i = "lc_t"; break;
                case 0x75: i = "lc_u"; break;
                case 0x76: i = "lc_v"; break;
                case 0x77: i = "lc_w"; break;
                case 0x78: i = "lc_x"; break;
                case 0x79: i = "lc_y"; break;
                case 0x7A: i = "lc_z"; break;
                case 0x7B: i = "left_curly_brace"; break;
                case 0x7C: i = "vertical_bar"; break;
                case 0x7D: i = "right_curly_brace"; break;
                case 0x7E: i = "tilde"; break;
                case 0x7F: i = "delete"; break;

            }


            return i;


        }

    
    }

}
