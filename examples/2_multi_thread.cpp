#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "seg/seg"

using namespace std::literals::chrono_literals;
namespace segobj = ::seg::object;

// FIXME: 
// For Extreme cases,
// SEG produces double free / segfault

const int iteration = 8;
const int batch = 1000;

void producer()
{
    std::cout<<"Producer"<<std::endl;
    for (int i = 0; i < iteration; i++) {
        for (int j = 0; j < batch; j++) {
            seg::addObject(
                "icosahedron" + std::to_string(i * batch + j),
                new segobj::Mesh(segobj::primitives::Icosahedron()));
        }
    }
}

void consumer()
{
    std::cout<<"Consumer"<<std::endl;
    for (int i = 0; i < iteration; i++) {
        for (int j = 1; j < batch; j++) {
            seg::deleteObject("icosahedron"+std::to_string(i * batch + j));
        }
        std::this_thread::sleep_for(20ms); // throtle. Producer is slower.
    }
}

int main()
{
    seg::Options option;
    option.verbosity = seg::Verbosity::QUIET;
    seg::initialize("Multithread test.", seg::WindowSize(1000, 600),option);

    std::thread t1(producer);
    std::this_thread::sleep_for(200ms);
    std::thread t2(consumer);

    std::cin.ignore();

    t1.join();
    t2.join();

    return 0;
}