from collections import namedtuple

from kivy.core.window import Window


KeyStroke = namedtuple('KeyStroke', ['code', 'character'])


class InputHandler:

    def __init__(self):
        self._keyboard = Window.request_keyboard(
            self._keyboard_closed, self, 'text')
        self._keyboard.bind(on_key_down=self._on_key_down)
        self.new_keystrokes = []

    def _keyboard_closed(self):
        print('Keyboard has been closed.')
        self._keyboard.unbind(on_key_down=self._on_key_down)
        self._keyboard = None

    def _on_key_down(self, keyboard, keycode, text, modifiers):
        print('Keycode', keycode, 'pressed')
        print('- Text', text)
        print('- Modifiers', modifiers)
        keystroke = KeyStroke(*keycode)

        if keystroke.character == 'escape':
            keyboard.release()
            return False  # allow system to process escape as a quit

        self.new_keystrokes.append(keystroke)
        # accept the keystroke as handled
        return True

    def get_keystrokes(self):
        old_keystrokes, self.new_keystrokes = self.new_keystrokes, []
        return old_keystrokes
