#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#define pi 3.14159265359

using namespace std;

namespace pgm {

struct image {
    string type;
    int w, h, bits;
    vector<vector<uint8_t>> data;
};

vector<vector<float>> dctdata;
int dct_width, dct_height;

void eatComment(ifstream &f) {
    char linebuf[1024];
    char ppp;
    while (ppp = f.peek(), ppp == '\n' || ppp == '\r')
        f.get();
    if (ppp == '#')
        f.getline(linebuf, 1023);
}

void loadPgm(image &img, const string &name) {
    ifstream f(name.c_str(), ios::in);
    if (f.fail()) {
        cout << "Erro ao abrir o arquivo " << name << endl;
        return;
    }
    eatComment(f);
    f >> img.type;
    eatComment(f);
    f >> img.w;
    eatComment(f);
    f >> img.h;
    eatComment(f);
    f >> img.bits;
    img.data.resize(img.w, vector<uint8_t>(img.h));
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            int v;
            f >> v;
            img.data[i][j] = v;
        }
    }
    f.close();
}

void savePgm(image &img, const string &name) {
    ofstream f(name.c_str(), ios::out);
    if (f.fail()) {
        cout << "Erro ao abrir o arquivo " << name << endl;
        return;
    }
    f << img.type << endl
      << img.w << ' ' << img.h << endl
      << img.bits << endl;
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            f << +img.data[i][j] << endl;
        }
    }
    f.close();
}

void printHeader(image img) {
    cout << "PGM type:   " << img.type << endl;
    cout << "PGM width:  " << img.w << endl;
    cout << "PGM height: " << img.h << endl;
    cout << "PGM bits:   " << img.bits << endl;
}

void printData(image img) {
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            cout << setw(3);
            cout << +img.data[i][j] << ' ';
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
    out.data.resize(img.w, vector<uint8_t>(img.h));
}

void cloneData(image img, image &out) {
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            out.data[i][j] = img.data[i][j];
        }
    }
}

void inverter(image img, image &out) {
    cloneHeader(img, out);
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            out.data[i][j] = 255 - img.data[i][j];
        }
    }
}

void rotacionar(image img, image &out) {
    cloneHeader(img, out);
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            out.data[i][j] = img.data[j][i];
        }
    }
}

void brilho(image img, image &out, float num) {
    cloneHeader(img, out);
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            int aux = img.data[i][j] * num;
            if (aux > 255)
                out.data[i][j] = 255;
            else if (aux < 0)
                out.data[i][j] = 0;
            else
                out.data[i][j] = aux;
        }
    }
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
                    pixel += img.data[ii][jj];
                    count++;
                }
            }
            out.data[i][j] = pixel * (1.0f / count);
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
                    pixel.push_back(img.data[ii][jj]);
                    count++;
                }
            }
            sort(pixel.begin(), pixel.end());
            out.data[i][j] = pixel.at(count / 2 + 1);
        }
    }
}

void normalizar(image img, image &out) {
    cloneHeader(img, out);
    int max = 0, min = 255;
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            if (img.data[i][j] > max) max = img.data[i][j];
            if (img.data[i][j] < min) min = img.data[i][j];
        }
    }
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            out.data[i][j] = (img.data[i][j] - min) * 255 / (max - min);
        }
    }
}

void dctTransform(image img) {
    dct_width = img.w;
    dct_height = img.h;
    dctdata.resize(dct_width, vector<float>(dct_height));
    float ci, cj, dct1, sum;
    for (int i = 0; i < dct_width; i++) {
        for (int j = 0; j < dct_height; j++) {
            if (i == 0)
                ci = 1 / sqrt(dct_width);
            else
                ci = sqrt(2) / sqrt(dct_width);
            if (j == 0)
                cj = 1 / sqrt(dct_height);
            else
                cj = sqrt(2) / sqrt(dct_height);
            sum = 0;
            for (int k = 0; k < dct_width; k++) {
                for (int l = 0; l < dct_height; l++) {
                    dct1 = img.data[k][l] *
                           cos((2 * k + 1) * i * pi / (2 * dct_width)) *
                           cos((2 * l + 1) * j * pi / (2 * dct_height));
                    sum = sum + dct1;
                }
            }
            dctdata[i][j] = ci * cj * sum;
        }
    }
}

void idctTransform(image &out) {
    float ci, cj, dct1, sum;
    for (int i = 0; i < dct_width; i++) {
        for (int j = 0; j < dct_height; j++) {
            if (i == 0)
                ci = 1 / sqrt(dct_width);
            else
                ci = sqrt(2) / sqrt(dct_width);
            if (j == 0)
                cj = 1 / sqrt(dct_height);
            else
                cj = sqrt(2) / sqrt(dct_height);
            sum = 0;
            for (int k = 0; k < dct_width; k++) {
                for (int l = 0; l < dct_height; l++) {
                    dct1 = dctdata[k][l] * ci * cj *
                           cos((2 * k + 1) * i * pi / (2 * dct_width)) *
                           cos((2 * l + 1) * j * pi / (2 * dct_height));
                    sum = sum + dct1;
                }
            }
            out.data[i][j] = sum;
        }
    }
}

void normalizarDct(image &out) {
    float max = numeric_limits<float>::min();
    float min = numeric_limits<float>::max();
    for (int i = 0; i < dct_width; i++) {
        for (int j = 0; j < dct_height; j++) {
            if (dctdata[i][j] > max) max = dctdata[i][j];
            if (dctdata[i][j] < min) min = dctdata[i][j];
        }
    }
    for (int i = 0; i < dct_width; i++) {
        for (int j = 0; j < dct_height; j++) {
            out.data[i][j] = (dctdata[i][j] - min) * 255 / (max - min);
        }
    }
}

void laplaciano(image img, image &out) {
    cloneHeader(img, out);
    int masc[9] = {1, 1, 1,
                   1, -8, 1,
                   1, 1, 1};
    int pos_masc = 0;
    int cont_masc = 0;
    for (int i = 1; i < img.w - 1; i++) {
        for (int j = 1; j < img.h - 1; j++) {
            cont_masc = 0;
            for (int ii = -1; ii <= 1; ii++) {
                for (int jj = -1; jj <= 1; jj++, cont_masc++) {
                    pos_masc += img.data[i + ii][j + jj] * masc[cont_masc];
                }
            }
            pos_masc /= 9;
            out.data[i][j] = pos_masc;
        }
    }
}

void laplacGaussiano(image img, image &out) {
    cloneHeader(img, out);
    int masc[25] = {0, 0, 1, 0, 0,
                    0, 1, 2, 1, 0,
                    1, 2, -16, 2, 1,
                    0, 1, 2, 1, 0,
                    0, 0, 1, 0, 0};
    int pos_masc = 0, cont_masc = 0;
    for (int i = 2; i < img.w - 2; i++) {
        for (int j = 2; j < img.h - 2; j++) {
            cont_masc = 0;
            for (int ii = -2; ii <= 2; ii++) {
                for (int jj = -2; jj <= 2; jj++, cont_masc++) {
                    pos_masc += img.data[i + ii][j + jj] * masc[cont_masc];
                }
            }
            pos_masc /= 25;
            out.data[i][j] = pos_masc;
        }
    }
}

}  // namespace pgm
