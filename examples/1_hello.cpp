#include <Eigen/Dense>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "seg/seg"

using namespace std::literals::chrono_literals;
namespace segobj = ::seg::object;

int main()
{
    seg::Options option;
    option.verbosity = seg::Verbosity::DEBUG;
    option.theme = seg::Theme::LIGHT;

    // SEG call -> this will imediately start SEG Window
    seg::initialize("Hello, SEG!", seg::WindowSize(1000, 600), option);

    // object with name & raw pointer
    seg::addObject("Sphere", new segobj::Mesh(
                                 segobj::primitives::Icosphere(0.498, 5)));

    // object without name -> SEG will generate one and return it.
    std::string name = seg::addObject(new segobj::StaticPointcloud(
        segobj::primitives::GaussianRandomVertices(3000, Eigen::Vector3f{10, 0, 0}, 1)));
    std::cout << "Name : " << name << std::endl;

    // object given through shared pointer ( recommended )
    auto line_renderer = std::make_shared<segobj::StaticLine>(
        segobj::primitives::Wireframe(segobj::primitives::Icosphere(0.5, 5)));
    seg::addObject("Sphere Frame", line_renderer);

    // object with same name -> SEG will not accept it,
    // When raw pointer given, SEG will delete it.
    seg::addObject("Sphere", new segobj::Mesh(
                                 segobj::primitives::Icosphere(0.498, 1)));

    // SEG objects can be added anytime!
    std::this_thread::sleep_for(3000ms);  // 3 sceonds sleep
    seg::addObject(new segobj::StaticPointcloud(
        segobj::primitives::GaussianRandomVertices(3000, Eigen::Vector3f{0, 10, 0}, 1)));

    // wait for user intput to terminate.
    // closing SEG window will not affect main thread.
    // However, killing main thread will cause SEG to shutdown.
    std::cin.ignore();

    return 0;
}