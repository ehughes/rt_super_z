using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using lib_eGFX_Tools;

using System.ComponentModel;
using System.Data;
using System.Threading;
using System.Runtime.Serialization.Formatters.Binary; //
using System.Runtime.Serialization;
using System.IO;
using System.IO.Ports;

namespace lib_eGFX_Tools
{
    class Program
    {

        public static void PostMessages(string output)
        {
            Console.WriteLine(output);
        }


        public static void ShowHelp()
        {
            Console.WriteLine("");
            Console.WriteLine("************************************************************************");
            Console.WriteLine("eGFX_Tools Usage:                                \t\t\t\t\t\t   ");
            Console.WriteLine("eGFX_Tools  Command  Argument1 Argument2 .....       \t\t\t\t\t   ");
            Console.WriteLine("                                                         \t\t\t   ");
            Console.WriteLine("Command List:                                            \t\t\t   ");
            Console.WriteLine("------------------------------------------------------------------------");
            Console.WriteLine("GenerateSprites -- Generates .c and .h files from a folder of .png files");
            Console.WriteLine("                                                                        ");
            Console.WriteLine("Argument1:   Path to folder that has .png images / sprites              ");
            Console.WriteLine("Argument2:   Target eGFX_ImagePlane Type                                ");
            Console.WriteLine("             Valid types are                                            ");

            foreach (string IPT in Enum.GetNames(typeof(eGFX_ImagePlaneType)))
            {
                Console.WriteLine("             " + IPT);
            }


            Console.WriteLine("Argument3:   Path to output folder for generated C file                ");
            Console.WriteLine("Argument4:   Optional section placement decorator                      ");


            Console.WriteLine("             i.e. __attribute__ ((section (\"ExtFlashSection\"))) __attribute__ ((aligned(4))) ");

            Console.WriteLine("                                                          \t\t\t   ");
            Console.WriteLine("FontBuilder -- Generates .c and .h files from SrcImages  \t\t\t   ");
            Console.WriteLine("                                                                        ");
            Console.WriteLine("Argument 1: Path to folder that has the font data                       ");
            Console.WriteLine("            There should be FontConfiguration.jsx in the folder.        ");
            Console.WriteLine("            If you give it a folder without a .jsx file, an example     ");
            Console.WriteLine("            will be created for you. The Font Folder should have a      ");
            Console.WriteLine("            subfolder with .png images of the font sprites.If none      ");
            Console.WriteLine("            exist, a 5x8 template will be generated. Each ASCII         ");
            Console.WriteLine("            character will have its own .png file  (see examples)       ");
            Console.WriteLine("            Look at an example .jsx for configuration                   ");
            Console.WriteLine("            .h files will have the same base file name as the input     ");
            Console.WriteLine("            folder. The .c and .h files will have the same base         ");
            Console.WriteLine("            file name as the input folder.                              ");
            
            Console.WriteLine("Argument2:   (optional)  Section Placement  ");
            Console.WriteLine("             This string is placed in front of the C structure          ");
            Console.WriteLine("             declaration to control section placement                   ");
            Console.WriteLine("                                                                        ");
            Console.WriteLine("             i.e. __attribute__ ((section (\"ExtFlashSection\"))) __attribute__ ((aligned(4))) ");



            Console.WriteLine("                                                                        ");
            Console.WriteLine("InitFont  --Generates an Example Template\t\t\t\t        \t   ");
            Console.WriteLine("                                                                        ");
            Console.WriteLine("Argument1:   Path to folder that the font tempalte                      ");
            Console.WriteLine("Argument2:   Width of the font characters                               ");
            Console.WriteLine("Argument3:   Height of the font characters                              ");
            Console.WriteLine("                                                                         ");
            Console.WriteLine("                                                                        ");
            Console.WriteLine("SliceDice  --Slices and dices an image into smaller pieces       \t   ");
            Console.WriteLine("                                                                        ");
            Console.WriteLine("Argument1:   Path to configuration file                                   ");
            Console.WriteLine("************************************************************************");
            Console.WriteLine("");
        }
        	
        		 			
        static void Main(string[] args)
        {

            eGFX_Tools MyGFX_Tools = new eGFX_Tools(PostMessages);

             
            try
            {
                if (args.Length == 0)
                {

                    ShowHelp();

                }
                else
                {
                    switch (args[0])
                    {
                        case "r":

                            MyGFX_Tools.Remap(args[1]);
                            break;

                            case "FontBuilder":

                            if (args.Length <= 2)
                            {
                                ShowHelp();
                            }
                            else
                            {
                                string SectionDecorator = "";

                                if (args.Length > 3)
                                {
                                    for (int i = 3; i < args.Length; i++)
                                    {
                                        SectionDecorator += args[i] + " ";
                                    }
                                }

                                MyGFX_Tools.FontBuilder(args[1], args[2] ,eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_1BPP, "FONT_SECTION", SectionDecorator);
                            }
                            break;

                        case "InitFont":

                            if (args.Length != 4)
                            {
                                ShowHelp();
                            }
                            else
                            {
                                MyGFX_Tools.InitFont(args[1], Convert.ToInt32(args[2]), Convert.ToInt32(args[3]));
                            }

                            break;

                        case "GenerateSprites":

                            if (args.Length < 4)
                            {
                                ShowHelp();
                            }
                            else
                            {

                                string SectionDecorator = "";

                                if(args.Length>4)
                                {
                                    for(int i=4;i<args.Length;i++)
                                    {
                                        SectionDecorator += args[i] + " ";
                                    }
                                }

                                eGFX_ImagePlaneType IPT = eGFX_ImagePlaneType.eGFX_IMAGE_PLANE_1BPP;

                                if (args[2] == "all")
                                {
                                    foreach( string IPT_String in Enum.GetNames(typeof(eGFX_ImagePlaneType)))
                                    {
                                        if(Enum.TryParse(IPT_String, out IPT) == true)
                                        {
                                            PostMessages("Generating assets for image plane type : " + IPT.ToString());
                                            MyGFX_Tools.GenerateSprites(args[1], IPT, "SPRITE_SECTION",SectionDecorator, args[3]);
                                        }
                                    }
                                }
                                else
                                {
                                    if(Enum.TryParse(args[2],out IPT) == false)
                                    {
                                        PostMessages(args[2] + " is not a  valid plane type");
                                        Console.WriteLine("Valid types are:");

                                        foreach (string S in Enum.GetNames(typeof(eGFX_ImagePlaneType)))
                                        {
                                            Console.WriteLine(S);
                                        }
                                    }
                                    else
                                    {
                                        MyGFX_Tools.GenerateSprites(args[1], IPT, "SPRITE_SECTION", SectionDecorator, args[3]);
                                    }
                                }
                             

                                
                            } 
                            
                            break;

                        case "SliceDice":

                            if (args.Length == 1)
                            {
                                ShowHelp();
                            }
                            else
                            {
                                MyGFX_Tools.SliceDice(args[1]);
                            } 
                            

                            break;



                        default:

                            ShowHelp();
                            break;
                    }

                }
            }
            catch(Exception Ex)
            {
                Console.WriteLine(Ex.Message);
            }

    
         }
    }
}
