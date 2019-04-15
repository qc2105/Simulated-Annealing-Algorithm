project("sa for sales man")

add_executable(sa SimulatedAnnealing.cpp)
target_link_libraries(sa opencv_core opencv_imgcodecs opencv_imgproc opencv_highgui)

