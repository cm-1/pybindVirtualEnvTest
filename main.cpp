#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <pybind11/embed.h>
#include <pybind11/stl.h> // For "stl" conversions (list, set, dict, etc.)
#include <pybind11/numpy.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

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

    py::object scope = py::module_::import("__main__").attr("__dict__");
    py::exec(
        "d = {'zero': 0, 'two': 2, 'three': 3}\n"
        "print(d)",
        scope);

    cv::VideoCapture cap(0);
    // Check if camera opened successfully
    if (!cap.isOpened()) {
            std::cout << "Error starting the webcam capture!" << std::endl;
            return -1;
    }

    py::print("Now testing my own local .py file...", "flush"_a=true);
    // Since I'm working with a different folder, my "hack" solution is to
    // just add said folder to sys.path
    sys.attr("path").attr("insert")(1, "/home/u1/Documents/FunCoding/learningPybind");
    // Performs "from example import myPyClass
    py::object getRunner = py::module_::import("HandMesh.runnerCreator").attr("getRunner");
    py::object testNumpyInput = py::module_::import("HandMesh.runnerCreator").attr("testNumpyInput");
    float focalLength = 634.f;
    py::object runner = getRunner(focalLength, 640);

    py::print("kMatrix:", runner.attr("kMatrix"), "flush"_a=true);

    cv::Matx33f someK(focalLength, 0, 320,
                    0, focalLength, 240,
                    0, 0, 1);


    cv::Mat frame;
    while (true) {
        cap >> frame;

        cv::Mat frameBGR;
        cv::cvtColor(frame, frameBGR, cv::COLOR_BGR2RGB);
        py::array myArray(frameBGR.total() * frameBGR.elemSize(), frameBGR.data);
        py::array reshaped = myArray.reshape({480, 640, 3});
        py::object result = runner.attr("processFrame")(reshaped);

        py::array points2D = result.attr("points2D");
        py::array points3D = result.attr("points3D");

        py::buffer_info bufferPoints2D = points2D.request();
        py::buffer_info bufferPoints3D = points3D.request();

        double *ptrPoints2D = (double *) bufferPoints2D.ptr;
        double *ptrPoints3D = (double *) bufferPoints3D.ptr;


        std::vector<cv::Scalar> colourList{cv::Scalar(0, 0, 255), cv::Scalar(0, 255, 0)};
        // Iterate over all hands detected.
        long numHands = bufferPoints2D.shape[0];
        long numLandmarks = bufferPoints2D.shape[1];
        long numDimsPerPt = bufferPoints2D.shape[2];


        for (long handIndex = 0; handIndex < numHands; handIndex++) {
            // Iterate over all points detected for said hand.
            for (long landmarkIndex = 0; landmarkIndex < numLandmarks; landmarkIndex++) {
                long xIndex = handIndex * numLandmarks * numDimsPerPt + landmarkIndex * numDimsPerPt;
                long yIndex = handIndex * numLandmarks * numDimsPerPt + landmarkIndex * numDimsPerPt + 1;
                int x = (int)(ptrPoints2D[xIndex]);
                int y = (int)(ptrPoints2D[yIndex]);

                cv::Point pt(x, y);
                int circleRadius = 7;
                cv::Scalar colour = colourList[handIndex];
                int thickness = 3;
                cv::circle(frame, pt, circleRadius, colour, thickness);
            }
        }

        long numPoints3D = bufferPoints3D.shape[0];

        // Iterate over all outputed 3D points:
        for (long pointIndex = 0; pointIndex < numPoints3D; pointIndex++) {
            long baseIndex = 3 * pointIndex;

            float x = ptrPoints3D[baseIndex];
            float y = ptrPoints3D[baseIndex + 1];
            float z = ptrPoints3D[baseIndex + 2];

            cv::Vec3f point3D(x, y, z);




            cv::Vec3f projected = someK * point3D;
            cv::Vec2f pt2D(projected[0]/projected[2], projected[1]/projected[2]);
            cv::Point pt2D_int((int)pt2D[0], (int)pt2D[1]);

            int circleRadius = 7;
            cv::Scalar colour = cv::Scalar(255, 255, 0);
            int thickness = 3;
            cv::circle(frame, pt2D_int, circleRadius, colour, thickness);
        }

        cv::imshow("Qt Result", frame);

        int key = cv::waitKey(1);
        if(key == (int)'q')
            break;
    }






//    py::object len1 = mpc.attr("euclength")();
//    float floatLen1 = len1.cast<float>();
//    std::cout << "length of origin to mpc before translation: " << floatLen1 << std::endl;
//    mpc.attr("translate")(2, 3);
//    py::object len2 = mpc.attr("euclength")();
//    float floatLen2 = len2.cast<float>();
//    std::cout << "length of origin to mpc after translation: " << floatLen2 << std::endl;
//    mpc.attr("translate")(0.1, 0.2);
//    py::list tripleList = mpc.attr("arrayOfTriples")();
//    std::vector<py::object> tripleVector = tripleList.cast<std::vector<py::object>>();
//    py::tuple thirdTriplePy = tripleList[3];
//    auto element0 = thirdTriplePy[0];
//    py::print("[3][0]", element0, "flush"_a = true);
//    float finalTupleThing = element0.cast<float>();

//    float sqr = finalTupleThing * finalTupleThing;
//    std::cout << "sqr: " << sqr << std::endl;

    std::cout << "End of tests" << std::endl;
    return EXIT_SUCCESS;
}
