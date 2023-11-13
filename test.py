import sys
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QSlider, QLabel
from PyQt5.QtCore import Qt

class SliderApp(QWidget):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        layout = QVBoxLayout()

        self.labels = []
        self.sliders = []

        for i in range(5):
            label = QLabel('Slider {}: 0'.format(i + 1))
            slider = QSlider(Qt.Horizontal)
            slider.setMaximum(180)
            slider.setMinimum(0)
            slider.setValue(0)
            slider.valueChanged.connect(self.sliderChanged)

            self.labels.append(label)
            self.sliders.append(slider)

            layout.addWidget(label)
            layout.addWidget(slider)

        self.setLayout(layout)
        self.setGeometry(100, 100, 400, 300)
        self.setWindowTitle('Slider App')
        self.show()

    def sliderChanged(self):
        for i, slider in enumerate(self.sliders):
            value = slider.value()
            self.labels[i].setText('Slider {}: {}'.format(i + 1, value))

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = SliderApp()
    sys.exit(app.exec_())
