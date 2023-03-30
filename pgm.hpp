#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#define pi 3.14159265359
#define img(i, j) (img.data[(i)*img.w + (j)])
#define out(i, j) (out.data[(i)*out.w + (j)])

using namespace std;

namespace pgm {

struct image {
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

void carregarPgm(image &img, const string &name) {
    ifstream f(name.c_str(), ios::binary);
    if (f.fail()) {
        cout << "Erro ao abrir o arquivo " << name << endl;
        return;
    }
    lerComentario(f);
    f >> img.type;
    lerComentario(f);
    f >> img.w;
    lerComentario(f);
    f >> img.h;
    lerComentario(f);
    f >> img.bits;
    img.data.resize(img.w * img.h);
    if (img.type == "P5") {
        f.get();
        f.read(reinterpret_cast<char *>(img.data.data()), img.data.size());
    } else if (img.type == "P2") {
        for (int i = 0; i < img.h; i++) {
            for (int j = 0; j < img.w; j++) {
                int v;
                f >> v;
                img(i, j) = v;
            }
        }
    }
    f.close();
}

void salvarPgm(image &img, const string &name) {
    ofstream f(name.c_str(), ios::binary);
    if (f.fail()) {
        cout << "Erro ao abrir o arquivo " << name << endl;
        return;
    }
    f << img.type << endl
      << img.w << ' ' << img.h << endl
      << img.bits << endl;
    if (img.type == "P5") {
        f.write((char *)&img.data[0], img.data.size());
    } else if (img.type == "P2") {
        for (int i = 0; i < img.h; i++) {
            for (int j = 0; j < img.w; j++) {
                f << +img(i, j) << endl;
            }
        }
    }
    f.close();
}

void printHeader(image img) {
    cout << img.type << endl
         << img.w << ' ' << img.h << endl
         << img.bits << endl;
}

void printData(image img) {
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            cout << +img(i, j) << endl;
        }
    }
}

void clonarCabecalho(image img, image &out) {
    out.type = img.type;
    out.bits = img.bits;
    out.w = img.w;
    out.h = img.h;
    out.data.clear();
    out.data.resize(img.w * img.h);
}

void clonarDados(image img, image &out) {
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            out(i, j) = img(i, j);
        }
    }
}

void inverter(image img, image &out) {
    clonarCabecalho(img, out);
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            out(i, j) = 255 - img(i, j);
        }
    }
}

void transposta(image img, image &out) {
    clonarCabecalho(img, out);
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            out(i, j) = img(j, i);
        }
    }
}

void flipHorizontal(image img, image &out) {
    clonarCabecalho(img, out);
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            out(i, j) = img(i, (img.h - j - 1));
        }
    }
}

void flipVertical(image img, image &out) {
    clonarCabecalho(img, out);
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            out(i, j) = img((img.w - i - 1), j);
        }
    }
}

void girar90(image img, image &out) {
    clonarCabecalho(img, out);
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            out(i, j) = img((img.h - j - 1), i);
        }
    }
}

void girarM90(image img, image &out) {
    clonarCabecalho(img, out);
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            out(i, j) = img(j, (img.w - i - 1));
        }
    }
}

void girar180(image img, image &out) {
    clonarCabecalho(img, out);
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            out(i, j) = img((img.w - i - 1), (img.h - j - 1));
        }
    }
}

void binarizarArea(image img, image &out, int min, int max) {
    clonarCabecalho(img, out);
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            out(i, j) = (img(i, j) > min && img(i, j) < max) ? 255 : 0;
        }
    }
}

void destacarArea(image img, image &out, int min, int max, int cor) {
    clonarCabecalho(img, out);
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            out(i, j) = (img(i, j) > min && img(i, j) < max) ? cor : img(i, j);
        }
    }
}

void histograma(image img, image &out) {
    vector<int> aux(256, 0);
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            aux[img(i, j)]++;
        }
    }
    int max = 0, min = img.w * img.h;
    for (int i = 0; i < 256; i++) {
        if (aux[i] > max) max = aux[i];
        if (aux[i] < min) min = aux[i];
    }
    for (int i = 0; i < 256; i++) {
        aux[i] = (aux[i] - min) * 255 / (max - min);
    }
    out.type = "P5";
    out.w = 256;
    out.h = 256;
    out.bits = 255;
    out.data.clear();
    out.data.resize(256 * 256);
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            out(i, j) = (i < aux[j]) ? 255 : 0;
        }
    }
    flipVertical(out, out);
}

void brilho(image img, image &out, float num) {
    clonarCabecalho(img, out);
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            int aux = img(i, j) * num;
            if (aux > 255)
                out(i, j) = 255;
            else if (aux < 0)
                out(i, j) = 0;
            else
                out(i, j) = aux;
        }
    }
}

void normalizar(image img, image &out) {
    clonarCabecalho(img, out);
    int max = 0, min = 255;
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            if (img(i, j) > max) max = img(i, j);
            if (img(i, j) < min) min = img(i, j);
        }
    }
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            out(i, j) = (img(i, j) - min) * 255 / (max - min);
        }
    }
}

void tons(image img, image &out, int nivel) {
    int aux = 256 / nivel;
    clonarCabecalho(img, out);
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            out(i, j) = ((uint8_t)img(i, j) / aux) * aux;
        }
    }
    normalizar(out, out);
}

void media(image img, image &out, int range) {
    clonarCabecalho(img, out);
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            int pixel = 0;
            int count = 0;
            for (int ii = i - range; ii <= i + range; ii++) {
                for (int jj = j - range; jj <= j + range; jj++) {
                    if (ii < 0 || jj < 0 || ii >= img.h || jj >= img.w)
                        continue;
                    pixel += img(ii, jj);
                    count++;
                }
            }
            out(i, j) = pixel * (1.0f / count);
        }
    }
}

void mediana(image img, image &out, int range) {
    clonarCabecalho(img, out);
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            vector<int> pixel;
            int count = 0;
            for (int ii = i - range; ii <= i + range; ii++) {
                for (int jj = j - range; jj <= j + range; jj++) {
                    if (ii < 0 || jj < 0 || ii >= img.h || jj >= img.w)
                        continue;
                    pixel.push_back(img(ii, jj));
                    count++;
                }
            }
            sort(pixel.begin(), pixel.end());
            out(i, j) = pixel.at(count / 2 + 1);
        }
    }
}

}  // namespace pgm
