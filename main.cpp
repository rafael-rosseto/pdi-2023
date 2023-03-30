#include <thread>

#include "pgm.hpp"

using namespace std;
using namespace pgm;

int main(int argc, char *argv[]) {
    image input, output;
    carregarPgm(input, "imagens\\lenna.pgm");
    histograma(input, output);
    salvarPgm(output, "temp.ppm");
    thread th1(system, "visualizar imagens\\lenna.pgm");
    thread th2(system, "visualizar temp.ppm");
    th1.join();
    th2.join();
    system("del temp*");
    return 0;
}