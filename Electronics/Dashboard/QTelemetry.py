import sys
from PyQt4 import QtGui, QtCore
import serial.tools.list_ports
import serial 
from threading import*
import threading
import time
from numpy import *
import pyqtgraph as pg
import openpyxl as xls
import array
import re

class mainWindow(QtGui.QMainWindow):
    def __init__(self):
        super(mainWindow,self).__init__()
        self.setGeometry(0,0,750,900)
        self.setWindowTitle("Telemetry Monitor")
        self.init_components()
        
    def init_components(self):
        
        self.baudRate = 250000
        self.serialFlag = 0
        self.graphFlag = 0
        self.motorCurr1 = 0
        self.motorCurr2 = 0
        self.motorTemp1 = 0
        self.motorTemp2 = 0
        self.batteryStat = 0
        self.batteryTemp = 0
        self.speed = 0
        self.motorCurr1_Array = []
        self.motorCurr2_Array = []
        self.motorTemp1_Array = []
        self.motorTemp2_Array = []
        self.batteryStat_Array = []
        self.batteryTemp_Array = []
        self.speed_Array= []
        
        self.graphButton = QtGui.QPushButton("Show Graph",self)
        self.graphButton.resize(150,50)
        self.graphButton.move(600,0)
        self.graphButton.clicked.connect(self.openGraphWindow)
        
        self.searchButton = QtGui.QPushButton("Search",self)
        self.searchButton.clicked.connect(self.searchPort)
        self.searchButton.resize(100,50)

        self.startButton = QtGui.QPushButton("Start",self)
        self.startButton.clicked.connect(self.startSerial)
        self.startButton.resize(100,25)
        self.startButton.move(350,0)

        self.stopButton = QtGui.QPushButton("Stop",self)
        #self.stopButton.clicked.connect(self.stopSerial)
        self.stopButton.resize(100,25)
        self.stopButton.move(350,25)
                
        self.portList = QtGui.QListWidget(self)
        self.portList.resize(250,50)
        self.portList.move(100,0)

        self.statusLabel = QtGui.QPushButton("Status:",self)
        self.statusLabel.resize(150,50)
        self.statusLabel.move(450,0)
		
        self.dataTable = QtGui.QTableWidget(self)
        self.dataTable.setColumnCount(7)
        self.dataTable.setHorizontalHeaderLabels(("Motor Current 1;Motor Current 2;Motor Temperature 1;Motor Temperature 2;Speed;Battery Temperature;Battery Status").split(";"))
        self.dataTable.resize(750,600)
        self.dataTable.move(0,50)
        self.dataTable.	setColumnWidth(1, 107)
        self.dataTable.	setColumnWidth(2, 107)
        self.dataTable.	setColumnWidth(3, 107)
        self.dataTable.	setColumnWidth(4, 107)
        self.dataTable.	setColumnWidth(5, 107)
        self.dataTable.	setColumnWidth(6, 107)
        self.dataTable.	setColumnWidth(7, 107)
        self.dataTable.show()
        
        self.fileNameTextEdit = QtGui.QTextEdit(self) 
        self.fileNameTextEdit.resize(300,20)
        self.fileNameTextEdit.move(0,650)
        
        self.openFileButton = QtGui.QPushButton(self,text = "Create")
        self.openFileButton.resize(100,20)
        self.openFileButton.move(300,650)
        self.openFileButton.clicked.connect(self.openFile)
        
        #self.saveFileButton = QtGui.QPushButton(self,text = "Save")
        #self.saveFileButton.resize(100,20)
        #self.saveFileButton.move(400,650)
        #self.saveFileButton.clicked.connect(self.saveFile)
        
        self.fileNameTextEdit.setText(time.strftime("%d-%m-%Y %H:%M"))
        self.serialThread = Thread(target = self.readSerial)
        self.graphThread = Thread(target = self.updateGraphWindow)
        self.excelThread = Thread(target = self.writeFile)
    
    
        self.show()
        
    def closeEvent(self,evnt):
		print("pikacu")
		self.saveFile()    
        
    def searchPort(self):
		self.portList.clear()
		self.activePorts = list(serial.tools.list_ports.comports())
		for port in self.activePorts:	
			self.portList.addItem(str(port))		

    def startSerial(self):
		if(self.portList.currentItem()):
			self.comPort = self.portList.currentItem().text().split(" -")[0]
			self.sensorKitSerial = serial.Serial(port = self.comPort, baudrate = self.baudRate, timeout = 0, writeTimeout = 0)
			if(self.sensorKitSerial.isOpen()):
				self.statusLabel.setText("Connected")
				self.serialFlag = 1
				print(self.serialFlag)
				self.serialThread.start()

    def readSerial(self):
		while(self.serialFlag):
			self.inData = self.sensorKitSerial.readline()
			self.inDataArray = self.inData.split("/")
			time.sleep(0.1)
			res = re.match('\*+[0-9+]+\/+[0-9+]+\/+[0-9+]+\/+[0-9+]+\/+[0-9+]+\/+[0-9+]+\/+[0-9+]+\?',str(self.inData))
			print(res);
			if not (res is None):
				print("Alaeylaele")
				if(len(self.inDataArray[0].split("*"))>1):
					self.motorCurr1 = self.inDataArray[0].split("*")[1]
				self.motorCurr2 = self.inDataArray[1]
				self.speed = self.inDataArray[2]
				self.motorTemp1 = self.inDataArray[3]
				self.motorTemp2 = self.inDataArray[4]
				self.batteryTemp = self.inDataArray[5]
				self.batteryStat = self.inDataArray[6].split("?")[0]
				
				self.rowPosition = self.dataTable.rowCount()
				self.dataTable.insertRow(self.rowPosition)
				self.dataTable.setItem(self.rowPosition, 0, QtGui.QTableWidgetItem(self.motorCurr1))
				self.dataTable.setItem(self.rowPosition, 1, QtGui.QTableWidgetItem(self.motorCurr2))
				self.dataTable.setItem(self.rowPosition, 2, QtGui.QTableWidgetItem(self.motorTemp1))
				self.dataTable.setItem(self.rowPosition, 3, QtGui.QTableWidgetItem(self.motorTemp2))
				self.dataTable.setItem(self.rowPosition, 4, QtGui.QTableWidgetItem(self.speed))
				self.dataTable.setItem(self.rowPosition, 5, QtGui.QTableWidgetItem(self.batteryTemp))
				self.dataTable.setItem(self.rowPosition, 6, QtGui.QTableWidgetItem(self.batteryStat))
				 
				self.motorCurr1_Array.append(int(self.motorCurr1))
				self.motorCurr2_Array.append(int(self.motorCurr2))
				self.speed_Array.append(int(self.speed))
				self.motorTemp1_Array.append(int(self.motorTemp1))
				self.motorTemp2_Array.append(int(self.motorTemp2))
				self.batteryStat_Array.append(int(self.batteryStat))
				self.batteryTemp_Array.append(int(self.batteryTemp))
				#self.dataTable.moveCursor(QtGui.QTextCursor.End)
				#print(len(self.batteryStat_Array))
    
    def openGraphWindow(self):
        self.graphWindow = pg.GraphicsWindow(title = "Graphs")
        self.graphWindow.resize(950,300)
        self.graphPlot_Current = self.graphWindow.addPlot(title = "Current Graph")
        self.graphPlot_Temperature = self.graphWindow.addPlot(title = "Temperature Graph")
        self.graphPlot_Speed = self.graphWindow.addPlot(title = "Speed Graph")
        self.graphPlot_Battery =  self.graphWindow.addPlot(title = "Battery status Graph")
        self.graphFlag = 1
        self.graphPlot_Current.plot(self.motorCurr1_Array,pen = 'r',name='Motor current 1')
        self.graphPlot_Current.plot(self.motorCurr2_Array, pen = 'g',name='Motor current 2')
        self.graphPlot_Temperature.plot(self.motorTemp1_Array,pen = 'r',name='Motor temp 1')
        self.graphPlot_Temperature.plot(self.motorTemp2_Array,pen = 'g',name='Motor temp 2')
        self.graphPlot_Temperature.plot(self.batteryTemp_Array,pen = 'y',name='Battery temp')
        self.graphPlot_Speed.plot(self.speed_Array,pen = '#29BCCC', name='Speed')
        self.graphPlot_Battery.plot(self.batteryStat_Array,pen = 'b',name='Battery Staus')
        #self.graphThread.start()

    def updateGraphWindow(self):
		print("cukubik")
		if(self.graphFlag):
			self.graphPlot_Current.plot(self.motorCurr1_Array,pen = 'r',name='Motor current 1')
			self.graphPlot_Current.plot(self.motorCurr2_Array, pen = 'g',name='Motor current 2')
			self.graphPlot_Temperature.plot(self.motorTemp1_Array,pen = 'r',name='Motor temp 1')
			self.graphPlot_Temperature.plot(self.motorTemp2_Array,pen = 'g',name='Motor temp 2')
			self.graphPlot_Temperature.plot(self.batteryTemp_Array,pen = 'y',name='Battery temp')
			self.graphPlot_Speed.plot(self.speed_Array,pen = '#29BCCC', name='Speed')
			self.graphPlot_Battery.plot(self.batteryStat_Array,pen = 'b',name='Battery Staus')
			
    
    def openFile(self):
		self.fileName = self.fileNameTextEdit.toPlainText()
		self.workbook = xls.Workbook()
		self.sheet = self.workbook.active
		self.sheet.append(["Motor Current 1","Motor Current 2","Motor Temperature 1","Motor Temperature 2","Speed","Battery Temperature","Battery Status"])
		self.excelThread.start()
		
    def writeFile(self):
		while(1):
		    self.sheet.append([self.motorCurr1,self.motorCurr2,self.motorTemp1,self.motorTemp2,self.speed,self.batteryTemp,self.batteryStat])
		    time.sleep(0.2)
    def saveFile(self):
		self.workbook.save("/home/nurbasekin/Datalogger/"+str(self.fileNameTextEdit.toPlainText())+".xlsx")
		
app = QtGui.QApplication(sys.argv)
GUI = mainWindow()
sys.exit(app.exec_())

