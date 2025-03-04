import cv2
import numpy as np

link = cv2.imread('BOTW.png');


def LCD_color(b,g,r):

  packed = int(0)

  r = r>>3
  g = g>>2
  b = b>>3


  return r<<11 | g<<5 | b


def start_c_png(name,c_file_name,h_file_name, data):
    
    global var_string

    filename = c_file_name + '.c'
    headername = h_file_name + '.h'
    height, width = data.shape[:2]

    with open(filename, 'w') as f:
        
        f.write('#include <stdint.h>\n');
        f.write('#include \"' + str(headername) + '\"\r\n');

        var_string = 'const uint16_t ' + name + '[]'

        f.write(var_string + ' = {\n')

        f.write('\n')        

def stop_c_png(name,c_file_name,h_file_name, data):
    filename = c_file_name + '.c'
    headername = h_file_name + '.h'
    height, width = data.shape[:2]

    with open(filename, 'a') as f:
        
        f.write('\n};')

    with open(headername, 'w') as f:
        
      f.write('#include <stdint.h>\n');

      f.write('\n');
    
      f.write('#ifndef __' + name.upper() + '_H\n');
      f.write('#define __' + name.upper() + '_H\n\r');


      f.write('#define ' + name.upper() + '_FRAME_COUNT ' + str(FrameCount) + '\n');
      f.write('#define ' + name.upper() + '_WIDTH ' + str(width) + '\n');
      f.write('#define ' + name.upper() + '_HEIGHT '+ str(height) + '\n\n');
      f.write('extern ' + var_string + ';\n');
      
      f.write('\n');

      f.write('#endif\n');

FrameCount = 0

def write_c_png(name,c_file_name,h_file_name, data):
    
    global FrameCount
    filename = c_file_name + '.c'
    headername = h_file_name + '.h'
    height, width = data.shape[:2]
    print(height)
    print(width)
  
    with open(filename, 'a') as f:
        
        for x in range(width):
          for y in range(height):
            p = data[y][x]
            f.write(str(p[0]) + "," + str(p[1]) + "," + str(p[2]) + ',');
          f.write('\n')
       


start_c_png('link','link','link', link)
write_c_png('link','link','link', link)
stop_c_png('link', 'link','link',link)

