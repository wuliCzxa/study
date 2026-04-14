import sys
from PySide6.QtWidgets import QApplication
from gui import GUI
import os
os.chdir(os.path.dirname(os.path.abspath(__file__)))


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = GUI()
    window.show()
    sys.exit(app.exec())
