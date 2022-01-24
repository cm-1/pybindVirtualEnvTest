# Using pybind11 with virtual environments

This repo is to test using pybind11 to use modules from a virtual environment using the python interpreter embedded into C++ code.

## Notes on what I've learned so far

If you build the code by referencing one version of Python (e.g., via the `PYTHON_EXECUTABLE` variable), but then run the code in an environment that uses a _different_ version of Python, things may not work as expected. For example, if you print out `sys.executable` and `sys.version`, they may seem to conflict. For example, the path in `sys.executable` may point to the python in a virtual environment you set up to use Python 3.6, but `sys.version` may say "3.9".

In such a case, from what I've been able to tell, `sys.executable` is the incorrect one.

Meanwhile, `sys.version` **is** accurate, it seems.

I tested this by using the fact that, for Python versions up to and including 3.5, the "order" of key-value pairs does not depend on how you inserted them, while in 3.6 and above, it _is_ based on the order in which they were inserted.

That is, if you have the Python code:
```python
myDictionary = {"one": 1, "two": 2, "three": 3}
print(myDictionary)
```
then in Python 3.6 and above, the output will be _exactly_ `{"one": 1, "two": 2, "three": 3}`, while in 3.5 and earlier, it could be something else, such as `{"two": 2, "one": 1, "three": 3}`. This was the easiest way I could think of to test which version of Python was really being used without spending a ton of time researching.

So, to use Python version X and environment MyEnv, right now, I need to set `PYTHON_EXECUTABLE` (in CMakeLists.txt) to point to MyEnv's "python" executable, and then run the application or the IDE from a terminal in which MyEnv has been activated. The only IDE I've tested this with so far is Qt Creator (on my Ubuntu/Mint setup, Qt Creator is located at `~/Qt/Tools/QtCreator/bin/qtcreator`).

For `PYTHON_EXECUTABLE`, what that looks like in CMakeLists.txt is the following:

```cmake
set(VENV_ROOT_PATH "/home/u1/Documents/FunCoding/learningPybind/tempVenvDir/py35venv") # my custom var for convenience
set(
    PYTHON_EXECUTABLE "${VENV_ROOT_PATH}/bin/python3"
    CACHE FILEPATH "python virtual environment executable")
message(STATUS "PYTHON_EXECUTABLE is now: ${PYTHON_EXECUTABLE}")    #DEBUG
```



Seems that, instead of running Qt Creator from the terminal, I could also just manually edit `sys.path` and stuff, similar to how activate_this.py does things? Or maybe using PYTHONPATH (as [this source](https://stackoverflow.com/questions/53480120/embedding-pybind11-with-virtual-environment) and others have done); I think I tried this in both CMakeLists.txt and Qt Creator's "Projects > Environment" section at one point and it didn't work, but it could be worth trying again in combination with some other stuff I may be doing differently now.

I could also experiment with a "shebang" at the top of a python file that I run, but from my (very minimal) initial understanding of how shebangs work, I'm guessing this wouldn't be a solution.

Shebang links:

 * [https://stackoverflow.com/questions/44076804/what-shebang-to-use-for-python-scripts-run-under-a-pyenv-virtualenv](https://stackoverflow.com/questions/44076804/what-shebang-to-use-for-python-scripts-run-under-a-pyenv-virtualenv)
 * [https://stackoverflow.com/a/23762093](https://stackoverflow.com/a/23762093)

Links on modifying `sys.path`:

 * [One about how to define the path to add in CMakeLists.txt and then insert it at the front of `sys.path`.](https://stackoverflow.com/questions/56904149/embedding-python-with-pybind11-virtual-environment-doesnt-work)
 * [This answer](https://stackoverflow.com/a/23762093) would probably yield a more "complete" `sys.path`.
 * Then there are two links for using "activate_this.py":
    * [A general overview](https://stackoverflow.com/a/23845366)
    * [How to do it with Python 3 and a non-virtualenv setup.](https://stackoverflow.com/a/33637414)
    * [An example activate_this.py from official Virtualenv GitHub.](https://github.com/pypa/virtualenv/blob/main/src/virtualenv/activation/python/activate_this.py)
