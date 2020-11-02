from tkinter import Tk, Label, Button, Entry, OptionMenu, StringVar

class VisualGUI:
    def __init__(self,master):
        self.master = master
        master.title("Proyecto 1")
        master.geometry("800x600")

        self.loop = True