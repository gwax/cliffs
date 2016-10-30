from kivy.clock import Clock
from kivy.core.window import Window
from kivy.properties import StringProperty
from kivy.uix.widget import Widget

from cliffs.input_buffer import InputBuffer

REFRESH_INTERVAL = 0.1


class GameWindow(Widget):
    keychars = StringProperty('No input yet')

    def __init__(self, ledge_file, turn_length, **kwargs):
        super().__init__(**kwargs)
        self._ledge_file = ledge_file
        self.turn_length = turn_length
        self._input_buffer = None

    @property
    def input_buffer(self):
        if self._input_buffer is None:
            self._input_buffer = InputBuffer(self.turn_length)
        return self._input_buffer

    def start(self, *args, **kwargs):
        self.input_buffer.start()
        self._update_clock = Clock.schedule_interval(
            self.update, REFRESH_INTERVAL)

    def stop(self, *args, **kwargs):
        self._update_clock.cancel()
        self.input_buffer.stop()

    def update(self, dt):
        if self.input_buffer.keystrokes:
            self.keychars = ','.join(
                ks.character for ks in self.input_buffer.keystrokes)
