#include <thread>

#include "pgm.hpp"

using namespace std;
using namespace pgm;

int main(int argc, char *argv[]) {
    image input, output;
    loadPgm(input, "lena256.pgm");
    normalizar(input, output);
    savePgm(output, "out.pgm");
    thread th1(system, "visualizar lena256.pgm");
    thread th2(system, "visualizar out.pgm");
    th1.join();
    th2.join();
    return 0;
}