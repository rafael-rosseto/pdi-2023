#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
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
        for (int i = 0; i < img.w; i++) {
            for (int j = 0; j < img.h; j++) {
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
        for (int i = 0; i < img.w; i++) {
            for (int j = 0; j < img.h; j++) {
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
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            cout << setw(3);
            cout << +img(i, j) << ' ';
        }
        cout << endl;
    }
}

void cloneHeader(image img, image &out) {
    out.type = img.type;
    out.bits = img.bits;
    out.w = img.w;
    out.h = img.h;
    out.data.clear();
    out.data.resize(img.w * img.h);
}

void cloneData(image img, image &out) {
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            out(i, j) = img(i, j);
        }
    }
}

void inverter(image img, image &out) {
    cloneHeader(img, out);
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            out(i, j) = 255 - img(i, j);
        }
    }
}

void rotacionar(image img, image &out) {
    cloneHeader(img, out);
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            out(i, j) = img(j, i);
        }
    }
}

void brilho(image img, image &out, float num) {
    cloneHeader(img, out);
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
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
    cloneHeader(img, out);
    int max = 0, min = 255;
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            if (img(i, j) > max) max = img(i, j);
            if (img(i, j) < min) min = img(i, j);
        }
    }
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            out(i, j) = (img(i, j) - min) * 255 / (max - min);
        }
    }
}

void tons(image img, image &out, int nivel) {
    int aux = 255 / nivel;
    cloneHeader(img, out);
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            out(i, j) = ((uint8_t)img(i, j) / aux) * aux;
        }
    }
    normalizar(out, out);
}

void media(image img, image &out, int range) {
    cloneHeader(img, out);
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
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
    cloneHeader(img, out);
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            vector<int> pixel;
            int count = 0;
            for (int ii = i - range; ii <= i + range; ii++) {
                for (int jj = j - range; jj <= j + range; jj++) {
                    if (ii < 0 || jj < 0 || ii >= img.w || jj >= img.h)
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
