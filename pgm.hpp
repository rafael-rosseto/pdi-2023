#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#define pi 3.14159265359
#define src(i, j) (src.data[(i)*src.w + (j)])
#define dst(i, j) (dst.data[(i)*dst.w + (j)])

using namespace std;

namespace pgm {

struct imagem {
    string type;
    int w, h, bits;
    vector<uint8_t> data;
};

void lerComentario(ifstream &f) {
    char linebuf[1024];
    char ppp;
    while (ppp = f.peek(), ppp == '\n' || ppp == '\r')
        f.get();
    if (ppp == '#')
        f.getline(linebuf, 1023);
}

void carregarImagem(imagem &src, const string &name) {
    ifstream f(name.c_str(), ios::binary);
    if (f.fail()) {
        cout << "Erro ao abrir o arquivo " << name << endl;
        return;
    }
    lerComentario(f);
    f >> src.type;
    lerComentario(f);
    f >> src.w;
    lerComentario(f);
    f >> src.h;
    lerComentario(f);
    f >> src.bits;
    src.data.resize(src.w * src.h);
    if (src.type == "P5") {
        f.get();
        f.read(reinterpret_cast<char *>(src.data.data()), src.data.size());
    } else if (src.type == "P2") {
        for (int i = 0; i < src.h; i++) {
            for (int j = 0; j < src.w; j++) {
                int v;
                f >> v;
                src(i, j) = v;
            }
        }
    }
    f.close();
}

void salvarImagem(imagem &src, const string &name) {
    ofstream f(name.c_str(), ios::binary);
    if (f.fail()) {
        cout << "Erro ao abrir o arquivo " << name << endl;
        return;
    }
    f << src.type << endl
      << src.w << ' ' << src.h << endl
      << src.bits << endl;
    if (src.type == "P5") {
        f.write((char *)&src.data[0], src.data.size());
    } else if (src.type == "P2") {
        for (int i = 0; i < src.h; i++) {
            for (int j = 0; j < src.w; j++) {
                f << +src(i, j) << endl;
            }
        }
    }
    f.close();
}

void printHeader(imagem src) {
    cout << src.type << endl
         << src.w << ' ' << src.h << endl
         << src.bits << endl;
}

void printData(imagem src) {
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            cout << +src(i, j) << endl;
        }
    }
}

void clonarCabecalho(imagem src, imagem &dst) {
    dst.type = src.type;
    dst.bits = src.bits;
    dst.w = src.w;
    dst.h = src.h;
    dst.data.clear();
    dst.data.resize(src.w * src.h);
}

void clonarDados(imagem src, imagem &dst) {
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(i, j) = src(i, j);
        }
    }
}

void inverter(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(i, j) = 255 - src(i, j);
        }
    }
}

void transposta(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(i, j) = src(j, i);
        }
    }
}

void flipHorizontal(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(i, j) = src(i, (src.h - j - 1));
        }
    }
}

void flipVertical(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(i, j) = src((src.w - i - 1), j);
        }
    }
}

void girar90(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(i, j) = src((src.h - j - 1), i);
        }
    }
}

void girarM90(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(i, j) = src(j, (src.w - i - 1));
        }
    }
}

void girar180(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(i, j) = src((src.w - i - 1), (src.h - j - 1));
        }
    }
}

void binarizarArea(imagem src, imagem &dst, int min, int max) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(i, j) = (src(i, j) > min && src(i, j) < max) ? 255 : 0;
        }
    }
}

void destacarArea(imagem src, imagem &dst, int min, int max, int cor) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(i, j) = (src(i, j) > min && src(i, j) < max) ? cor : src(i, j);
        }
    }
}

void histograma(imagem src, imagem &dst) {
    vector<int> aux(256, 0);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            aux[src(i, j)]++;
        }
    }
    int max = 0, min = src.w * src.h;
    for (int i = 0; i < 256; i++) {
        if (aux[i] > max) max = aux[i];
        if (aux[i] < min) min = aux[i];
    }
    for (int i = 0; i < 256; i++) {
        aux[i] = (aux[i] - min) * 255 / (max - min);
    }
    dst.type = "P5";
    dst.w = 256;
    dst.h = 256;
    dst.bits = 255;
    dst.data.clear();
    dst.data.resize(256 * 256);
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            dst(i, j) = (i < aux[j]) ? 255 : 0;
        }
    }
    flipVertical(dst, dst);
}

void brilho(imagem src, imagem &dst, float num) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            int aux = src(i, j) * num;
            if (aux > 255)
                dst(i, j) = 255;
            else if (aux < 0)
                dst(i, j) = 0;
            else
                dst(i, j) = aux;
        }
    }
}

void normalizar(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    int max = 0, min = 255;
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            if (src(i, j) > max) max = src(i, j);
            if (src(i, j) < min) min = src(i, j);
        }
    }
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(i, j) = (src(i, j) - min) * 255 / (max - min);
        }
    }
}

void tons(imagem src, imagem &dst, int nivel) {
    clonarCabecalho(src, dst);
    int aux = 256 / nivel;
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(i, j) = (src(i, j) / aux) * aux;
        }
    }
    normalizar(dst, dst);
}

void media(imagem src, imagem &dst, int range) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            int pixel = 0;
            int count = 0;
            for (int ii = i - range; ii <= i + range; ii++) {
                for (int jj = j - range; jj <= j + range; jj++) {
                    if (ii < 0 || jj < 0 || ii >= src.h || jj >= src.w)
                        continue;
                    pixel += src(ii, jj);
                    count++;
                }
            }
            dst(i, j) = pixel * (1.0f / count);
        }
    }
}

void mediana(imagem src, imagem &dst, int range) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            vector<int> pixel;
            int count = 0;
            for (int ii = i - range; ii <= i + range; ii++) {
                for (int jj = j - range; jj <= j + range; jj++) {
                    if (ii < 0 || jj < 0 || ii >= src.h || jj >= src.w)
                        continue;
                    pixel.push_back(src(ii, jj));
                    count++;
                }
            }
            sort(pixel.begin(), pixel.end());
            dst(i, j) = pixel.at(count / 2 + 1);
        }
    }
}

}  // namespace pgm
