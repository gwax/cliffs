from collections import namedtuple
import os

from kivy.clock import Clock
from kivy.core.window import Window
from kivy.lang import Builder
from kivy.properties import StringProperty
from kivy.uix.widget import Widget

from cliffs.input_buffer import InputBuffer

KV_PATH = os.path.join(os.path.dirname(__file__), 'kivy')
Builder.load_file(os.path.join(KV_PATH, 'game.kv'))


class GameWindow(Widget):
    keychars = StringProperty('')

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.input_buffer = InputBuffer(0.5)

    def enter(self, *args, **kwargs):
        self.input_buffer.start()
        self._update_clock = Clock.schedule_interval(self.update, 0.1)

    def leave(self, *args, **kwargs):
        self._update_clock.cancel()
        self.input_buffer.stop()

    def update(self, dt):
        self.keychars = ','.join(
            ks.character for ks in self.input_buffer.keystrokes)
