#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#define pi 3.14159265359
#define src(i, j) (src.data[(i)*src.w + (j)])
#define dst(i, j) (dst.data[(i)*dst.w + (j)])

using namespace std;

namespace ppm {

struct RGB {
    uint8_t r, g, b;
};

struct imagem {
    string type;
    int w, h, bits;
    vector<RGB> data;
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
        cout << "Could not open file: " << name << endl;
        return;
    }
    lerComentario(f);
    f >> src.type;
    lerComentario(f);
    f >> src.w;
    lerComentario(f);
    f >> src.h;
    lerComentario(f);
    int bits = 0;
    f >> bits;
    src.data.resize(src.w * src.h);
    if (src.type == "P6") {
        f.get();
        f.read(reinterpret_cast<char *>(src.data.data()), src.data.size() * 3);
    } else if (src.type == "P3") {
        for (int i = 0; i < src.data.size(); i++) {
            int v;
            f >> v;
            src.data[i].r = v;
            f >> v;
            src.data[i].g = v;
            f >> v;
            src.data[i].b = v;
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
    if (src.type == "P6") {
        f.write((char *)&src.data[0], src.data.size() * 3);
    } else if (src.type == "P3") {
        for (int i = 0; i < src.h; i++) {
            for (int j = 0; j < src.w; j++) {
                f << +src(i, j).r << ' ';
                f << +src(i, j).g << ' ';
                f << +src(i, j).b << endl;
            }
        }
        f.close();
    }
}

void printHeader(imagem src) {
    cout << src.type << endl
         << src.w << ' ' << src.h << endl
         << src.bits << endl;
}

void printData(imagem src) {
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            cout << +src(i, j).r << ' ';
            cout << +src(i, j).g << ' ';
            cout << +src(i, j).b << endl;
        }
    }
}

void setHeader(imagem &src, string type, int w, int h, int bits) {
    src.type = type;
    src.w = w;
    src.h = h;
    src.bits = bits;
    src.data.clear();
    src.data.resize(w * h);
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
            dst(i, j).r = 255 - src(i, j).r;
            dst(i, j).g = 255 - src(i, j).g;
            dst(i, j).b = 255 - src(i, j).b;
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

void histograma(imagem src, imagem &dst1, imagem &dst2, imagem &dst3) {
    vector<int> R(256, 0), G(256, 0), B(256, 0);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            R[src(i, j).r]++;
            G[src(i, j).g]++;
            B[src(i, j).b]++;
        }
    }
    int max = 0;
    for (int i = 0; i < 256; i++) {
        if (R[i] > max) max = R[i];
        if (G[i] > max) max = G[i];
        if (B[i] > max) max = B[i];
    }
    for (int i = 0; i < 256; i++) {
        R[i] = R[i] * 255 / max;
        G[i] = G[i] * 255 / max;
        B[i] = B[i] * 255 / max;
    }
    setHeader(dst1, "P6", 256, 256, 255);
    setHeader(dst2, "P6", 256, 256, 255);
    setHeader(dst3, "P6", 256, 256, 255);
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            if (i < R[j])
                dst1.data[i * dst1.w + j] = {255, 0, 0};
            else
                dst1.data[i * dst1.w + j] = {0, 0, 0};
            if (i < G[j])
                dst2.data[i * dst2.w + j] = {0, 255, 0};
            else
                dst2.data[i * dst2.w + j] = {0, 0, 0};
            if (i < B[j])
                dst3.data[i * dst3.w + j] = {0, 0, 255};
            else
                dst3.data[i * dst3.w + j] = {0, 0, 0};
        }
    }
    flipVertical(dst1, dst1);
    flipVertical(dst2, dst2);
    flipVertical(dst3, dst3);
}

void media(imagem src, imagem &dst, int range) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            int R = 0, G = 0, B = 0;
            int count = 0;
            for (int ii = i - range; ii <= i + range; ii++) {
                for (int jj = j - range; jj <= j + range; jj++) {
                    if (ii < 0 || jj < 0 || ii >= src.h || jj >= src.w)
                        continue;
                    R += src(ii, jj).r;
                    G += src(ii, jj).g;
                    B += src(ii, jj).b;
                    count++;
                }
            }
            dst(i, j).r = R * (1.0f / count);
            dst(i, j).g = G * (1.0f / count);
            dst(i, j).b = B * (1.0f / count);
        }
    }
}

void mediana(imagem src, imagem &dst, int range) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            vector<int> R, G, B;
            int count = 0;
            for (int ii = i - range; ii <= i + range; ii++) {
                for (int jj = j - range; jj <= j + range; jj++) {
                    if (ii < 0 || jj < 0 || ii >= src.h || jj >= src.w)
                        continue;
                    R.push_back(src(ii, jj).r);
                    G.push_back(src(ii, jj).g);
                    B.push_back(src(ii, jj).b);
                    count++;
                }
            }
            sort(R.begin(), R.end());
            dst(i, j).r = R.at(count / 2 + 1);
            sort(G.begin(), G.end());
            dst(i, j).g = G.at(count / 2 + 1);
            sort(B.begin(), B.end());
            dst(i, j).b = B.at(count / 2 + 1);
        }
    }
}
}  // namespace ppm
