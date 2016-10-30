from collections import namedtuple

from kivy.clock import Clock
from kivy.core.window import Window


KeyStroke = namedtuple('KeyStroke', ['code', 'character'])


class InputBuffer:

    def __init__(
            self,
            collection_interval,
            rest_interval=None,
            update_interval=1./60):
        self.collection_interval = collection_interval
        self.rest_interval = rest_interval
        if rest_interval is None:
            self.rest_interval = collection_interval
        self.keystrokes = []
        self._new_keystrokes = []
        self._idle = False
        self._keyboard = None
        self.update_interval = update_interval
        self._update_clock = None

    def start(self):
        self._idle = True
        self._keyboard = Window.request_keyboard(self.stop, self, 'text')
        self._keyboard.bind(on_key_down=self._on_key_down)
        self._update_clock = Clock.schedule_interval(
            self._update, self.update_interval)

    def stop(self):
        if self._update_clock is not None:
            self._update_clock.cancel()
            self._update_clock = None
        if self._keyboard is not None:
            self._keyboard.unbind(on_key_down=self._on_key_down)
            self._keyboard.release()
            self._keyboard = None
        self._idle = True

    def _on_key_down(self, keyboard, keycode, text, modifiers):
        keystroke = KeyStroke(*keycode)
        print(keystroke)

        # TODO: ignore other special keystrokes
        if keystroke.character == 'escape':
            self.stop()
            return False  # allow escape to be processed elsewhere

        self._new_keystrokes.append(keystroke)
        # accept the keystroke as handled
        return True

    def _update(self, dt):
        if self._idle and self._new_keystrokes:
            Clock.schedule_once(self._process_input, self.collection_interval)
            self._idle = False

    def _process_input(self, dt):
        self.keystrokes = self._new_keystrokes.copy()
        Clock.schedule_once(self._reset_input, self.rest_interval)

    def _reset_input(self, dt):
        self._new_keystrokes.clear()
        self._idle=True
