"""Application screens and general layout."""
import os

from kivy.core.window import Keyboard, Window
from kivy.lang import Builder
from kivy.properties import ListProperty, StringProperty, AliasProperty
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.uix.scrollview import ScrollView


DOCS_PATH = os.path.join(os.path.dirname(__file__), 'docs')
KV_PATH = os.path.join(os.path.dirname(__file__), 'kivy')

Builder.load_file(os.path.join(KV_PATH, 'screens.kv'))

REVERSE_DIRECTIONS = {
    'up': 'down',
    'down': 'up',
    'left': 'right',
    'right': 'left',
}


class DocView(ScrollView):
    filename = StringProperty('')

    def _get_text(self):
        if not self.filename:
            return ''
        filepath = os.path.join(DOCS_PATH, self.filename)
        if not os.path.exists(filepath):
            return '<File not found>'
        with open(filepath, 'rt') as textfile:
            return textfile.read()
    text = AliasProperty(_get_text, None, bind=['filename'])


class ReversableScreenManager(ScreenManager):

    previous = ListProperty()  # (direction, screen)

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        Window.bind(on_keyboard=self.on_keyboard)
        self.next_previous = self.current
        self.bind(current=self.curr_change)

    @staticmethod
    def curr_change(instance, value):
        if hasattr(instance.transition, 'direction'):
            direction = REVERSE_DIRECTIONS[instance.transition.direction]
        else:
            direction = None
        instance.previous.append((direction, instance.next_previous))
        instance.next_previous = value

    def on_keyboard(self, window, keycode, scancode, codepoint, modifiers):
        if keycode == 27 and self.previous:  # escape
            self.go_back()
            return True
        return False

    def go_back(self):
        direction, screen = self.previous.pop()
        if direction is not None:
            self.transition.direction = direction
        self.unbind(current=self.curr_change)
        try:
            self.current = screen
            self.next_previous = self.current
        finally:
            self.bind(current=self.curr_change)
