import imgui
import glfw
import OpenGL.GL as gl
from imgui.integrations.glfw import GlfwRenderer
from PIL import Image
import serial
import threading
import queue
import numpy as np

print("Available window flags:")
for attr in dir(imgui):
    if attr.startswith('WINDOW_'):
        print(attr)
        
class SerialMonitor:
    def __init__(self, port='COM5', baudrate=115200):
        self.serial = serial.Serial(port=port, baudrate=baudrate)
        self.queue = queue.Queue()
        self.running = True
        self.buffer = ""
        self.thread = threading.Thread(target=self._read_serial)
        self.thread.daemon = True
        self.thread.start()

    def _read_serial(self):
        while self.running:
            if self.serial.in_waiting:
                try:
                    data = self.serial.readline().decode('utf-8')
                    self.queue.put(data)
                except:
                    pass

    def get_data(self):
        while not self.queue.empty():
            self.buffer += self.queue.get_nowait()
            if len(self.buffer.split('\n')) > 100:
                self.buffer = '\n'.join(self.buffer.split('\n')[-100:])
        return self.buffer

    def close(self):
        self.running = False
        self.serial.close()

def load_image(image_path):
    image = Image.open(image_path)
    image = image.convert('RGBA')
    image_data = np.array(image)
    
    texture_id = gl.glGenTextures(1)
    gl.glBindTexture(gl.GL_TEXTURE_2D, texture_id)
    gl.glTexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_MIN_FILTER, gl.GL_LINEAR)
    gl.glTexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_MAG_FILTER, gl.GL_LINEAR)
    gl.glTexImage2D(gl.GL_TEXTURE_2D, 0, gl.GL_RGBA, image.width, image.height,
                    0, gl.GL_RGBA, gl.GL_UNSIGNED_BYTE, image_data)
    
    return texture_id, image.width, image.height

def main():
    if not glfw.init():
        return

    window = glfw.create_window(1280, 720, "Gibbon-T eMMC Test", None, None)
    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)

    # Initialize Dear ImGui
    imgui.create_context()
    impl = GlfwRenderer(window)
    
    # Enable blending for transparency
    gl.glEnable(gl.GL_BLEND)
    gl.glBlendFunc(gl.GL_SRC_ALPHA, gl.GL_ONE_MINUS_SRC_ALPHA)

    # Load background image
    try:
        texture_id, img_width, img_height = load_image('to.png')  # Replace with your image path
    except Exception as e:
        print(f"Error loading image: {e}")
        texture_id, img_width, img_height = None, 0, 0

    # Initialize serial monitor
    try:
        serial_monitor = SerialMonitor('COM5')  # Replace with your port
    except Exception as e:
        print(f"Error opening serial port: {e}")
        serial_monitor = None

    # Set style for translucent serial monitor window
    style = imgui.get_style()
    style.colors[imgui.COLOR_WINDOW_BACKGROUND] = (0.0, 0.0, 0.0, 0.85)
    style.colors[imgui.COLOR_FRAME_BACKGROUND] = (0.0, 0.0, 0.0, 0.6)
    style.colors[imgui.COLOR_FRAME_BACKGROUND_HOVERED] = (0.1, 0.1, 0.1, 0.7)
    style.colors[imgui.COLOR_TITLE_BACKGROUND_ACTIVE] = (0.2, 0.2, 0.2, 0.8)
    
    while not glfw.window_should_close(window):
        glfw.poll_events()
        impl.process_inputs()

        win_width, win_height = glfw.get_window_size(window)

        imgui.new_frame()

        if texture_id is not None:
            imgui.set_next_window_position(0, 0)
            imgui.set_next_window_size(win_width, win_height)
            imgui.begin(
                "Background",
                False,
                imgui.WINDOW_NO_TITLE_BAR |
                imgui.WINDOW_NO_RESIZE |
                imgui.WINDOW_NO_MOVE |
                imgui.WINDOW_NO_SCROLLBAR |
                imgui.WINDOW_NO_INPUTS |
                imgui.WINDOW_NO_BRING_TO_FRONT_ON_FOCUS |
                imgui.WINDOW_NO_FOCUS_ON_APPEARING |
                imgui.WINDOW_NO_NAV |
                imgui.WINDOW_NO_BACKGROUND
            )
            imgui.image(texture_id, win_width, win_height)
            imgui.end()

        # Serial monitor window (translucent)
        imgui.set_next_window_size(800, 500, imgui.FIRST_USE_EVER)
        imgui.begin("Terminal", True)
        
        if serial_monitor is not None:
            serial_data = serial_monitor.get_data()
            imgui.begin_child("scrolling", width=0, height=0, border=True)
            imgui.push_style_color(imgui.COLOR_TEXT, 0, 1.0, 0, 0.95)
            imgui.text_unformatted(serial_data)
            imgui.pop_style_color()
            
            if imgui.get_scroll_y() >= imgui.get_scroll_max_y():
                imgui.set_scroll_here_y(1.0)
            imgui.end_child()
        
        imgui.end()

        # Rendering
        imgui.render()
        gl.glClearColor(0.0, 0.0, 0.0, 0.0)  # Transparent background
        gl.glClear(gl.GL_COLOR_BUFFER_BIT)
        impl.render(imgui.get_draw_data())
        glfw.swap_buffers(window)

    # Cleanup
    if serial_monitor:
        serial_monitor.close()
    impl.shutdown()
    glfw.terminate()

if __name__ == "__main__":
    main()