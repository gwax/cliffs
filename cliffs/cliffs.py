"""Entrypoint to cliffs game."""

import argparse

from kivy.app import App

import cliffs
from cliffs import game


def get_args(args=None):
    parser = argparse.ArgumentParser(
        description='Attack the Cliffs')
    parser.add_argument(
        '--versions', action='version', version=cliffs.__version__)

    parser.add_argument(
        '--turn-length', default='0.5', type=float,
        help='Length of time to capture keystrokes for a given turn.')

    parser.add_argument(
        'ledge', type=argparse.FileType('rt'),
        help='Ledge file to climb.')
    return parser.parse_args(args=args)


class CliffsApp(App):

    def __init__(self, ledge, turn_length, **kwargs):
        super().__init__(**kwargs)
        self.ledge = ledge
        self.turn_length = turn_length
        self._game = None

    @property
    def game(self):
        if self._game is None:
            self._game = game.GameWindow(self.ledge, self.turn_length)
        return self._game

    def build(self):
        return self.game

    def on_start(self):
        self.game.start()

    def on_stop(self):
        self.game.stop()



def main():
    args = get_args()
    app = CliffsApp(ledge=args.ledge, turn_length=args.turn_length)
    app.run()
