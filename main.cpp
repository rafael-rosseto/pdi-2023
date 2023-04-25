#include "pgm.hpp"

using namespace std;
using namespace pgm;

int main(int argc, char *argv[]) {
    imagem input, hist, equal, histEqual;
    carregarImagem(input, "imagens\\lenna.pgm");
    histograma(input, hist);
    equalizarHistograma(input, equal);
    histograma(equal, histEqual);
    salvarImagem(hist, "temp\\hist.pgm");
    salvarImagem(equal, "temp\\equalizado.pgm");
    salvarImagem(histEqual, "temp\\hist-equalizado.pgm");
    system("start visualizar imagens\\lenna.pgm");
    system("start visualizar temp\\hist.pgm");
    system("start visualizar temp\\equalizado.pgm");
    system("start visualizar temp\\hist-equalizado.pgm");
    return 0;
}