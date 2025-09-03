###  Toolchain

&nbsp;	Xtensa Xplorer for Windows V10.1.11 

&nbsp;	DSP Configuration for Windows (newlib) V10.1.11

&nbsp;	Note: The generated license key is for this version, no for the last version available





Using dsp\_hello\_world\_usart project

&nbsp;	For the ARM side, I used the dsp\_hello\_world\_usart\_cm33 project from the mcuxpresso SDK examples.

&nbsp;	For the DSP side, I did unzip the downloaded SDK for the RT685 and opened this path in Xtensa Xplore:
		\\boards\\mimxrt685audevk\\dsp\_examples\\hello\_world\_usart\\hifi4\\xtensa

	I did it this way because it was the sweetiest way I found to load the projects to their corresponding IDE.

&nbsp;	

&nbsp;	



Once I was able to build both projects, I copied and pasted the output binary files of the Xtensa project (hello\_world\_usart\\hifi4\\xtensa\\binary) to the path where the mcuxpreso project expects for this binary files (dsp\_binary folder)

Build the MCUXPresso project and load the app :)



