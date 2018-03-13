"""Object models for game objects."""

from kivy.graphics import Line


class Ledge:
    def __init__(self, height, left, right):
        self.height = height
        self.left = left
        self.right = right

    def __str__(self):
        return '<{t}: h={s.height}, l={s.left}, r={s.right}>'.format(
            t=type(self).name, s=self)

    @property
    def points(self):
        return [self.left, self.height, self.right, self.height]
