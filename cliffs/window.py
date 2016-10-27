from kivy.clock import Clock
from kivy.properties import StringProperty
from kivy.uix.widget import Widget

from cliffs.input import InputHandler

INPUT_CLOCK_DELAY = 0.5


class CliffsWindow(Widget):

    keychars = StringProperty('Hello World')

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.input_handler = InputHandler()
        self._handling = False

    def update(self, dt):
        if not self._handling and self.input_handler.new_keystrokes:
            print('Recording keystrokes')
            self._handling = True
            Clock.schedule_once(self.process_input, INPUT_CLOCK_DELAY)

    def process_input(self, dt):
        keystrokes = self.input_handler.new_keystrokes
        self.keychars = ','.join(ks.character for ks in keystrokes)
        Clock.schedule_once(self.reset_handling, INPUT_CLOCK_DELAY)

    def reset_handling(self, dt):
        print('Clearing keystrokes')
        self.input_handler.new_keystrokes.clear()
        self._handling = False
