from kivy.app import App
from kivy.clock import Clock

from cliffs.window import CliffsWindow


class CliffsApp(App):
    def build(self):
        window = CliffsWindow()
        Clock.schedule_interval(window.update, 1. / 60)
        return window
