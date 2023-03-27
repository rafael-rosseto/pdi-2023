#include <thread>

#include "pgm.hpp"

using namespace std;
using namespace pgm;

int main(int argc, char *argv[]) {
    image input, output;
    carregarPgm(input, "imagens\\lena.pgm");
    mediana(input, output, 2);
    salvarPgm(output, "temp.pgm");
    thread th1(system, "visualizar imagens\\lena.pgm");
    thread th2(system, "visualizar temp.pgm");
    th1.join();
    th2.join();
    system("del temp*");
    return 0;
}