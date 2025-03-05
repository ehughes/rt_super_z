using System;
using System.Collections.Generic;
using System.Collections;
using System.IO;

using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using lib_eGFX_Tools;
using System.CodeDom.Compiler;
using System.Threading;
using System.Drawing.Text;

namespace eGFX_FontGen
{
    public partial class MainForm : Form
    {

        public const int MessageQueueSize = 64;

        public Queue<String> OutputMessageQueue = new Queue<string>(MessageQueueSize);

        eGFX_Tools MyGFX_Tools;

        public Font SelectedFont;
        CodeDomProvider MyCodeDomProvider = CodeDomProvider.CreateProvider("c#");

        bool UpdateOutputWindow = false;


        public void PostMessage(string output)
        {
            OutputMessageQueue.Enqueue(output);

            UpdateOutputWindow = true;
        
        }

        private void FormUpdateTimer_Tick(object sender, EventArgs e)
        {
            if (UpdateOutputWindow == true)
            {
                UpdateOutputWindow = false;

                while (OutputMessageQueue.Count > 64)
                {
                    string Junk = OutputMessageQueue.Dequeue();
                }

                OutputTextBox.Text = "";

                string[] Lines = OutputMessageQueue.ToArray();

                string Out = "";

                foreach (string S in Lines)
                {
                    Out = S + "\r\n" + Out;
                }

                OutputTextBox.Text = Out;

            }
            if (Directory.Exists(OutputFolder.Text) == false)
            {
                TargetFolderLabel.Text = "Output Folder [Does Not Exist... Will Create when font is generated";
                TargetFolderLabel.ForeColor = Color.Red;
            }
            else
            {
                TargetFolderLabel.Text = "Output Folder";
                TargetFolderLabel.ForeColor = Color.Black;
            }


            if (MyCodeDomProvider.IsValidIdentifier(FontBaseName.Text) == false)
            {
                FontNameLabel.Text = "Illegal Font.   Needs to be a string that could be a valid C/C++ variable name";
                TargetFolderLabel.ForeColor = Color.Red;
            
            }
            else
            {
                FontNameLabel.Text = "Font Base Name";
                FontNameLabel.ForeColor = Color.Black;
            }
        
        }

        public MainForm()
        {
            InitializeComponent();

            MyGFX_Tools = new eGFX_Tools(PostMessage);


            foreach (FontFamily fontFamily in FontFamily.Families)
            {
                if (fontFamily.IsStyleAvailable(FontStyle.Regular))
                {
                    FontComboBox.Items.Add(fontFamily.Name);
                }
            }
                      

            this.Text = "eGFX Font Builder : eGFX_API_LEVEL " + eGFX_Tools.eGFX_API_LEVEL + "    eGFX_REVISION : " + eGFX_Tools.eGFX_REVISION;
         
        }



        int GetMaxHeight()
        {

            return 0;
        }


        private void SelectFontButton_Click(object sender, EventArgs e)
        {
         

            
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            TrueTypeRenderingHint.Items.AddRange((string[])Enum.GetNames(typeof(TextRenderingHint)));
            TrueTypeRenderingHint.SelectedIndex = 0;
            FontStyleCB.Items.AddRange((string[])Enum.GetNames(typeof(FontStyle)));
            FontStyleCB.SelectedIndex = 0;

            ImagePlaneTypeCB.Items.AddRange((string[])Enum.GetNames(typeof(eGFX_ImagePlaneType)));
      

            ImagePlaneTypeCB.SelectedIndex = 0;

            FontComboBox.SelectedIndex = 0;

            WorkFlowCB.SelectedIndex = 1;

        }


        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void InitFontButton_Click(object sender, EventArgs e)
        {

            string OutputFontFolder = Path.Combine(OutputFolder.Text,FontBaseName.Text);

            MyGFX_Tools.InitFont(Generate_FontName(OutputFontFolder), (int)InitFontWidth.Value, (int)InitFontHeight.Value);

            PostMessage("Font Initialized");

        }

        private void BuildFontButton_Click(object sender, EventArgs e)
        {
            string OutputFontFolder = Path.GetFullPath(Path.Combine(OutputFolder.Text, FontBaseName.Text));

            eGFX_ImagePlaneType IPT = (eGFX_ImagePlaneType)Enum.Parse(typeof(eGFX_ImagePlaneType), (string)ImagePlaneTypeCB.SelectedItem);

            MyGFX_Tools.FontBuilder(Generate_FontName(OutputFontFolder), OutputFontFolder, IPT,  "FONT_SECTION" , SectionPlacementDecoratorTextBox.Text);

            PostMessage("Font Build Process Finished.");
        }

        public string Generate_FontName(string BaseName)
        {
            string SanitizedName;

            SanitizedName = BaseName;
            string IPT_Name = (string)ImagePlaneTypeCB.SelectedItem;
            SanitizedName += eGFX_ImagePlane.GetImagePlaneTypePrefix((eGFX_ImagePlaneType)Enum.Parse(typeof(eGFX_ImagePlaneType), IPT_Name));
            return SanitizedName;
        }

        public string GenerateTT_FontName()
        {
            SelectedFont = new Font((String)FontComboBox.SelectedItem, (int)FontSize.Value, GraphicsUnit.Pixel);

            string SanitizedName = (String)FontComboBox.SelectedItem;

            SanitizedName = SanitizedName.Trim();
            SanitizedName = SanitizedName.Replace(" ", "_");
            SanitizedName += "__" + FontSize.Value + "px";

            SanitizedName += "__" + (string)FontStyleCB.SelectedItem;
          
            string TRH_String = (string)TrueTypeRenderingHint.SelectedItem;
            SanitizedName += "__" + TRH_String;

            string IPT_Name = (string)ImagePlaneTypeCB.SelectedItem;

            SanitizedName += eGFX_ImagePlane.GetImagePlaneTypePrefix((eGFX_ImagePlaneType)Enum.Parse(typeof(eGFX_ImagePlaneType), IPT_Name));

            return SanitizedName;

        }
        private void GenerateTT_FontData_Click(object sender, EventArgs e)
        {
           
           SelectedFont = new Font((String)FontComboBox.SelectedItem, (int)FontSize.Value, GraphicsUnit.Pixel);

           FontBaseName.Text = GenerateTT_FontName();

           string TRH_String = (string)TrueTypeRenderingHint.SelectedItem;

           TextRenderingHint TRH = (TextRenderingHint)Enum.Parse(typeof(TextRenderingHint), TRH_String);

            string OutputFontFolder;

               OutputFontFolder = Path.GetFullPath(Path.Combine(OutputFolder.Text, FontBaseName.Text));
         
            MyGFX_Tools.InitFontFromTrueType(OutputFontFolder, SelectedFont, TRH);

           eGFX_ImagePlaneType IPT = (eGFX_ImagePlaneType)Enum.Parse(typeof(eGFX_ImagePlaneType), (string)ImagePlaneTypeCB.SelectedItem);
           
           MyGFX_Tools.FontBuilder(OutputFontFolder, OutputFontFolder, IPT, "FONT_SECTION", SectionPlacementDecoratorTextBox.Text);

           PostMessage("Font Process Generation Complete");
        }


      //  string Folder;
        //string FontName;
        
        /*
        private void button1_Click(object sender, EventArgs e)
        {
            Thread F = new Thread(GenerateFontRange);

            Folder = OutputFolder.Text;
            FontName = (String)FontComboBox.SelectedItem;
      
            //F.Start();
        }
        */

            /*
        void CopyOutFiles()
        {
            string[] h_files = Directory.GetFiles(Folder, "*.h", SearchOption.AllDirectories);
            string[] c_files = Directory.GetFiles(Folder, "*.c", SearchOption.AllDirectories);

            
            Directory.CreateDirectory(Path.Combine(Folder, "out"));

            foreach (string f in h_files)
            {
                try
                {
                    File.Copy(f,
                                Path.Combine(Folder, "out", Path.GetFileName(f)), true);
                }
                catch (Exception Ex)
                {
                    PostMessage(Ex.Message);
                }
            }
            foreach (string f in c_files)
            {
                try
                {
                File.Copy(f,
                                Path.Combine(Folder, "out", Path.GetFileName(f)), true);
                 }
                catch (Exception Ex)
                {
                    PostMessage(Ex.Message);
                }
            }


        }
        */
        /*
        void GenerateFontRange()
        {
            
            if (StartingPx.Value > EndingPx.Value)
            {
                decimal x, y;

                x = StartingPx.Value;
                y = EndingPx.Value;

                StartingPx.Value = y;
                EndingPx.Value = x;

            }


            for (int i = (int)StartingPx.Value; i <= (int)EndingPx.Value; i++)
            {
          
               
                FontStyle MyFontStyle;

                MyFontStyle = (FontStyle)Enum.Parse(typeof(FontStyle),FontStyleCB.SelectedText);
              
                Font F = new Font((String)FontName, i, MyFontStyle,GraphicsUnit.Pixel);

                string SanitizedName = (String)FontName;

                SanitizedName = SanitizedName.Trim();
                SanitizedName = SanitizedName.Replace(" ", "_");
                SanitizedName += "__" + i + "px";

                if (MyFontStyle == FontStyle.Bold)
                {
                    SanitizedName += "__Bold";
                }
                

                PostMessage("Building Font " + SanitizedName);

                string OutputFontFolder = Path.GetFullPath(Path.Combine(Folder, SanitizedName));

                
            }

        }
        */

        private void PixelDepthComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void OutputTextBox_TextChanged(object sender, EventArgs e)
        {

        }

        private void WorkFlowCB_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (WorkFlowCB.SelectedIndex == 0)
            {
                OutputFolder.Enabled = true;
                FontBaseName.Enabled = true;
                ImagePlaneTypeCB.Enabled = true;
                FontComboBox.Enabled = false;
                FontStyleCB.Enabled = false;
                TrueTypeRenderingHint.Enabled = false;
                FontSize.Enabled = false;
                GenerateTT_FontData.Enabled = false;
           
                InitFontButton.Enabled = true;
                BuildFontButton.Enabled = true;
                InitFontHeight.Enabled = true;
                InitFontWidth.Enabled = true;

            }
            else
            {
                OutputFolder.Enabled = true;
                FontBaseName.Enabled = false;
                ImagePlaneTypeCB.Enabled = true;
                FontComboBox.Enabled = true;
                FontStyleCB.Enabled = true;
                TrueTypeRenderingHint.Enabled = true;
                FontSize.Enabled = true;
                GenerateTT_FontData.Enabled = true;

                InitFontButton.Enabled = false;
                BuildFontButton.Enabled = false;
                InitFontHeight.Enabled = false;
                InitFontWidth.Enabled = false;
            }

        }

        private void Instructions_TextChanged(object sender, EventArgs e)
        {

        }

        private void label8_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
