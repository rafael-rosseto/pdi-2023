#include <thread>

#include "pgm.hpp"

using namespace std;
using namespace pgm;

int main(int argc, char *argv[]) {
    imagem input, hist, equal, histEqual;
    carregarImagem(input, "imagens\\lenna.pgm");
    histograma(input, hist);
    equalizarHistograma(input, equal);
    histograma(equal, histEqual);
    salvarImagem(hist, "temp-hist.pgm");
    salvarImagem(equal, "temp-equalizado.pgm");
    salvarImagem(histEqual, "temp-hist-equalizado.pgm");
    thread th1(system, "visualizar imagens\\lenna.pgm");
    thread th2(system, "visualizar temp-hist.pgm");
    thread th3(system, "visualizar temp-equalizado.pgm");
    thread th4(system, "visualizar temp-hist-equalizado.pgm");
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    system("del temp*");
    return 0;
}