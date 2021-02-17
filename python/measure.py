import mylib

class PythonMeasureName:
    def run(self, p: mylib.Person, name: str):
        print(f"Python script.py: {p.getName()}")
        print(f"Changing name to {name}")
        p.setName(name)
        print(f"Python script.py: {p.getName()}")
        return True
