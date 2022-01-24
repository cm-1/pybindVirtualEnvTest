#include <iostream>
#include <string>
#include <pybind11/embed.h>

namespace py = pybind11;
using namespace pybind11::literals; // to bring in the `_a` literal

int main() {
    std::cout << "Before pybind11." << std::endl;
    py::scoped_interpreter guard{};
    auto sys = py::module::import("sys");
    auto os = py::module::import("os");
    py::print("Hello, World from Python!");
    py::print(sys.attr("executable"));
    py::print(sys.attr("version"), "flush"_a = true);
    //sys.attr("path").attr("insert")(1, VENV_SITE_PACKAGES_PATH);
    py::print("Env Path: ", os.attr("environ").attr("get")("PATH"));
    py::print(sys.attr("path"), "flush"_a=true);



    std::cout << "Before import test." << std::endl;
    py::object scope = py::module_::import("__main__").attr("__dict__");
    py::exec(
        "d = {'zero': 0, 'two': 2, 'three': 3}\n"
        "print(d)",
        scope);
    py::print("Now testing pandas...", "flush"_a=true);
    auto pd = py::module::import("pandas");
    py::print(pd.attr("__version__"), "flush"_a = true);

    py::print("Now testing my own local .py file...", "flush"_a=true);
    // Performs "from example import myPyClass
    py::object myPyClass = py::module_::import("customPython.example").attr("myPyClass");
    py::object mpc = myPyClass(1, 1);
    py::object len1 = mpc.attr("euclength")();
    float floatLen1 = len1.cast<float>();
    std::cout << "length of origin to mpc before translation: " << floatLen1 << std::endl;
    mpc.attr("translate")(2, 3);
    py::object len2 = mpc.attr("euclength")();
    float floatLen2 = len2.cast<float>();
    std::cout << "length of origin to mpc after translation: " << floatLen2 << std::endl;

    std::cout << "End of tests" << std::endl;
    return EXIT_SUCCESS;
}
