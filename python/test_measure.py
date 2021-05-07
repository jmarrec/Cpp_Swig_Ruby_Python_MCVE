
import sys

sys.path.append("/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/build-modif/Products/python/")

import mylib

class PythonTestMeasure(mylib.Measure):
    def __init__(self):
        mylib.Measure.__init__(self)
        print("Created Object")

    def run(self, r: mylib.Runner):
        r.get_current_model().pushOp("Op from Python")
        return True
    def name(self):
        return "Python Test Measure"


def make_measure():
    return PythonTestMeasure()

