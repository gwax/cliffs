import os

from kivy.app import App
from kivy.lang import Builder

from cliffs.screens import ReversableScreenManager

KV_PATH = os.path.join(os.path.dirname(__file__), 'kivy')
Builder.load_file(os.path.join(KV_PATH, 'app.kv'))


class CliffsScreenManager(ReversableScreenManager):
    pass


class CliffsApp(App):
    def build(self):
        return CliffsScreenManager()
