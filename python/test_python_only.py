import sys
from pathlib import Path
#import importlib.util

python_lib_path = Path('../build/Products/python/mylib.py').resolve()

if not python_lib_path.exists():
    print("Error, this assumes you already built the python bindings project in <root>/build")
    raise IOError(f"Could Not find {python_lib_path}")

# spec = importlib.util.spec_from_file_location("person",
                                              # str(python_lib_path))
# person = importlib.util.module_from_spec(spec)
# spec.loader.exec_module(person)
sys.path.insert(0, str(python_lib_path.parent))
import mylib

def get_a_python_person():
    p = mylib.Person("John")
    return p

def print_a_person(p: mylib.Person):
    return mylib.personName(p)


if __name__ == '__main__':
    p = mylib.Person("John")
    print(p)
    print(f"{mylib.personName(p)=}")
