#!/usr/bin/env python

import wx


class ButtonXY(wx.Frame):

    def __init__(self, parent, title):
        super(ButtonXY, self).__init__(parent, title=title,
            size=(350, 300))

        self.InitUI()
        self.Centre()

        self.bind_events()

    def bind_events(self):
        self.Bind(wx.EVT_LEFT_DOWN, self.OnLeftDown)

    def InitUI(self):

        self.panel = wx.Panel(self)
        self.panel.SetBackgroundColour("gray")

        self.LoadImages()


    def LoadImages(self):

        self.controlerXY = wx.StaticBitmap(self.panel, wx.ID_ANY,
            wx.Bitmap("control_xy.png", wx.BITMAP_TYPE_ANY))

    def OnLeftDown(self, event):
        print("On left!")

        mpos = event.GetPosition()

        print(mpos)
        """
        if not self.enabled:
            return

        # Take focus when clicked so that arrow keys can control movement
        self.SetFocus()

        mpos = event.GetPosition()

        idx = self.mouseOverKeypad(mpos)
        if idx is None:
            self.quadrant, self.concentric = self.getQuadrantConcentricFromPosition(mpos)
            if self.concentric is not None:
                if self.concentric < len(self.concentric_circle_radii):
                    if self.concentric == 0:
                        self.lastCorner = -1
                        self.lastMove = None
                        self.cornerCallback(self.corner_to_axis[-1])
                    elif self.quadrant is not None:
                        x, y, z = self.getMovement(event)
                        if self.moveCallback:
                            self.lastMove = (x, y)
                            self.lastCorner = None
                            self.moveCallback(x, y)
                elif self.corner is not None:
                    if self.cornerCallback:
                        self.lastCorner = self.corner
                        self.lastMove = None
                        self.cornerCallback(self.corner_to_axis[self.corner])
        else:
            self.setKeypadIndex(-1 if self.keypad_idx == idx else idx)

        """

def main():

    app = wx.App()
    ex = ButtonXY(None, title='Absolute positioning')
    ex.Show()
    app.MainLoop()


if __name__ == '__main__':
    main()
