from tkinter import Tk, Canvas

class VisualGUI:
    def __init__(self,master):
        self.master = master
        master.title("Proyecto 1")
        master.geometry("800x600")

        self.canvasWidth = 800
        self.canvasHeight = 600

        self.canvas = Canvas(master, width=self.canvasWidth, height=self.canvasHeight)
        self.canvas.pack()

        self.loadData()


    def loadData(self):
        processArray = []
        frecuencyArray = []
        singleArray = []
        total = 0
        elements = 0

        scheduleFile = open("/home/beer/Desktop/REPOS/Proyecto1_SO_MartianScheduling/src/report.txt","r")
        lines = scheduleFile.readlines()

        for line in lines:
            if line.rstrip("\n") not in processArray:
                singleArray.append(line.rstrip("\n"))
                elements+=1


            if not processArray:
                frecuencyArray.append(1)
                processArray.append(line.rstrip("\n"))
                total = 1

            elif (processArray[-1] != line.rstrip("\n")):
                frecuencyArray.append(1)
                processArray.append(line.rstrip("\n"))
                total += 1

            else:
                frecuencyArray[-1] += 1
                total += 1

        self.drawData(self.canvasWidth//total,self.canvasHeight//elements, processArray,frecuencyArray,total,elements,singleArray)


    def drawData(self, boxWidth, boxHeight, processArray, frecuencyArray,total,elements,singleArray):
        for i in range(total):
            for j in range(elements):
                self.canvas.create_rectangle(i*boxWidth,j*boxHeight,i*boxWidth+boxWidth,j*boxHeight+boxHeight)

        acum = 0
        for i in range(len(processArray)):
            frec = frecuencyArray[i]
            y = singleArray.index(processArray[i])
            color = self.colorPicker(y)

            for j in range(frec):
                self.canvas.create_rectangle(
                    acum*boxWidth,
                    boxHeight*y,
                    acum*boxWidth + boxWidth,
                    y*boxHeight + boxHeight, 
                    fill = color)
                acum += 1

        acum = 0
        for i in range(len(processArray)):
            frec = frecuencyArray[i]
            y = singleArray.index(processArray[i])

            x_text = ((acum*boxWidth) + ((((acum + frec) * boxWidth) - (acum*boxWidth))/2))
            y_text = ((boxHeight* y) + ((((boxHeight* y)+boxHeight) - (boxHeight* y))/2))

            self.canvas.create_text(x_text,y_text,fill="black",font="Helvetica 16", text = processArray[i])

            acum +=frec


    def colorPicker(self, number):
        if (number == 0):
            return "red"
        elif (number == 1):
            return "green"
        elif (number == 2):
            return "blue"
        elif (number == 3):
            return "yellow"
        elif (number == 4):
            return "magenta"
        elif (number == 5):
            return "cyan"
        return "black"