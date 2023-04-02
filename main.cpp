#include <thread>

#include "ppm.hpp"

using namespace std;
using namespace ppm;

int main(int argc, char *argv[]) {
    imagem input, histR, histG, histB;
    carregarImagem(input, "imagens\\lenna.ppm");
    histograma(input, histR, histG, histB);
    salvarImagem(histR, "temp-histR.ppm");
    salvarImagem(histG, "temp-histG.ppm");
    salvarImagem(histB, "temp-histB.ppm");
    thread th1(system, "visualizar imagens\\lenna.ppm");
    thread th2(system, "visualizar temp-histR.ppm");
    thread th3(system, "visualizar temp-histG.ppm");
    thread th4(system, "visualizar temp-histB.ppm");
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    system("del temp*");
    return 0;
}