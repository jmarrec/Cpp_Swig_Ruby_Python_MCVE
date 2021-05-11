# import sys
# sys.path.append("/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/build-modif/Products/python/")

from time import time, ctime
print('Today is',ctime(time()))

import mylib

class PythonTestMeasure(mylib.PythonMeasure):
    def __init__(self):
        mylib.PythonMeasure.__init__(self)
        print("Created Object (Python __init__)")

    def run_impl(self, r: mylib.Runner):
        m = r.get_current_model()
        print(f"Python Model named: {m.getName()}")
        m.pushOp("Op from Python")
        m.setName("Python Model")
        print(f"Python: {m.numObjects()} objects")
        m.pushObject("Python Space")

        for i in range(m.numObjects()):
            print(f" * {i} = {m.getObject(i).getName()}")

        m.getObject(1).setName("MODIFIED FROM PYTHON")

        return True
    def name(self):
        return "Python Test Measure"


def make_measure():
    return PythonTestMeasure()

