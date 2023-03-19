#include "pgm.hpp"

using namespace std;
using namespace pgm;

int main(int argc, char *argv[]) {
    image input, output;
    loadPgm(input, "lena256.pgm");
    cloneHeader(input, output);
    normalizar(input, output);
    savePgm(output, "teste.pgm");
    system("visualizar teste.pgm");
    return 0;
}