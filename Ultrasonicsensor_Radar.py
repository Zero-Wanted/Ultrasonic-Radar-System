import sys
import math
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout
from PyQt5.QtGui import QPainter, QColor, QPen
from PyQt5.QtCore import Qt, QTimer
import serial

# Serial connection setup (make sure to update to your COM port)
ser = serial.Serial('COM8', 9600)

class RadarWidget(QWidget):
    def __init__(self):
        super().__init__()
        self.angle = 0
        self.distance = 0
        self.initUI()

    def initUI(self):
        self.setGeometry(100, 100, 800, 600)
        self.setWindowTitle('Ultrasonic Radar System')
        self.show()

        # Timer for refreshing the radar display
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.updateRadar)
        self.timer.start(50)  # Refresh every 50 ms

    def updateRadar(self):
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()
            if ',' in data:
                angle_str, distance_str = data.split(',')
                self.angle = int(angle_str)
                self.distance = int(distance_str)
        self.update()

    def paintEvent(self, event):
        painter = QPainter(self)
        self.drawRadar(painter)

    def drawRadar(self, painter):
        # Set radar colors and style
        painter.setPen(QPen(QColor(0, 255, 0), 2, Qt.SolidLine))
        painter.setRenderHint(QPainter.Antialiasing)

        # Radar parameters
        center_x, center_y = 400, 500
        radius = 300

        # radar circle
        painter.drawEllipse(center_x - radius, center_y - radius, 2 * radius, 2 * radius)

        # Calculate radar point position
        radar_x = center_x + self.distance * math.cos(math.radians(self.angle))
        radar_y = center_y - self.distance * math.sin(math.radians(self.angle))

        # radar sweeping line
        painter.drawLine(center_x, center_y, radar_x, radar_y)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    radar = RadarWidget()
    sys.exit(app.exec_())
