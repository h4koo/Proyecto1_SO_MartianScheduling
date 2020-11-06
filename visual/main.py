from VisualGUI import VisualGUI
from tkinter import Tk

if __name__ == "__main__":
    rm_root = Tk()
    eds_root = Tk()

    rm_gui = VisualGUI(rm_root,"Simulacion RM","rm_report.txt")
    eds_gui = VisualGUI(eds_root,"Simulacion EDS","eds_report.txt")

    rm_root.mainloop()
    eds_root.mainloop()