#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#define src(x, y) (src.data[(x) + src.w * (y)])
#define dst(x, y) (dst.data[(x) + dst.w * (y)])

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
        for (int y = 0; y < src.h; y++) {
            for (int x = 0; x < src.w; x++) {
                int v;
                f >> v;
                src(x, y) = v;
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
        for (int y = 0; y < src.h; y++) {
            for (int x = 0; x < src.w; x++) {
                f << +src(x, y) << endl;
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
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            cout << +src(x, y) << endl;
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
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = src(x, y);
        }
    }
}

void inverter(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = 255 - src(x, y);
        }
    }
}

void transposta(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = src(y, x);
        }
    }
}

void flipVertical(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = src(x, (src.h - y - 1));
        }
    }
}

void flipHorizontal(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = src((src.w - x - 1), y);
        }
    }
}

void girar90(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = src(y, (src.w - x - 1));
        }
    }
}

void girar180(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = src((src.w - x - 1), (src.h - y - 1));
        }
    }
}

void girar270(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = src((src.h - y - 1), x);
        }
    }
}

void binarizarArea(imagem src, imagem &dst, int min, int max) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = (src(x, y) > min && src(x, y) < max) ? 255 : 0;
        }
    }
}

void destacarArea(imagem src, imagem &dst, int min, int max, int cor) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = (src(x, y) > min && src(x, y) < max) ? cor : src(x, y);
        }
    }
}

void brilho(imagem src, imagem &dst, float num) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            int aux = src(x, y) * num;
            if (aux < 0)
                dst(x, y) = 0;
            else if (aux > 255)
                dst(x, y) = 255;
            else
                dst(x, y) = aux;
        }
    }
}

void correcaoGama(imagem src, imagem &dst, float gama) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            float aux = float(src(x, y)) / float(255);
            aux = pow(aux, gama) * 255;
            if (aux < 0)
                dst(x, y) = 0;
            else if (aux > 255)
                dst(x, y) = 255;
            else
                dst(x, y) = aux;
        }
    }
}

void ampliar(imagem src, imagem &dst) {
    setHeader(dst, src.type, src.w * 2, src.h * 2, 255);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(2 * x, 2 * y) = src(x, y);
            dst(2 * x + 1, 2 * y) = src(x, y);
            dst(2 * x, 2 * y + 1) = src(x, y);
            dst(2 * x + 1, 2 * y + 1) = src(x, y);
        }
    }
}

void reduzir(imagem src, imagem &dst) {
    setHeader(dst, src.type, src.w / 2, src.h / 2, 255);
    for (int y = 0; y < src.h / 2; y++) {
        for (int x = 0; x < src.w / 2; x++) {
            int aux = src(2 * x, 2 * y) +
                      src(2 * x + 1, 2 * y) +
                      src(2 * x, 2 * y + 1) +
                      src(2 * x + 1, 2 * y + 1);
            dst(x, y) = aux / 4;
        }
    }
}

void normalizar(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    int max = 0, min = 255;
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            if (src(x, y) > max) max = src(x, y);
            if (src(x, y) < min) min = src(x, y);
        }
    }
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = (src(x, y) - min) * 255 / (max - min);
        }
    }
}

void tons(imagem src, imagem &dst, int nivel) {
    clonarCabecalho(src, dst);
    int aux = 256 / nivel;
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = (src(x, y) / aux) * aux;
        }
    }
    normalizar(dst, dst);
}

void plotHistograma(imagem &dst, vector<int> hist) {
    setHeader(dst, "P5", 512, 256, 255);
    int max = 0;
    for (auto &i : hist)
        if (i > max) max = i;
    for (auto &i : hist)
        i = i * 255 / max;
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            dst(2 * x, 255 - y) = (y < hist[x]) ? 0 : 255;
            dst(2 * x + 1, 255 - y) = (y < hist[x]) ? 0 : 255;
        }
    }
}

void histograma(imagem src, imagem &dst) {
    vector<int> hist(256, 0);
    ofstream f("temp\\histograma.txt", ios::out);
    for (auto &i : src.data)
        hist[i]++;
    for (int i = 0; i < 256; i++)
        f << i << " " << hist[i] << endl;
    f.close();
    plotHistograma(dst, hist);
}

void equalizarHistograma(imagem src, imagem &dst) {
    clonarCabecalho(src, dst);
    vector<int> hist(256, 0);
    vector<float> fda(256, 0.0), sk(256, 0.0);
    for (auto &i : src.data) {
        hist[i]++;
    }
    for (int i = 0; i < 256; i++) {
        fda[i] = float(hist[i]) / float(src.w * src.h);
    }
    float soma = 0;
    for (int i = 0; i < 256; i++) {
        soma += fda[i];
        sk[i] = soma * 255;
    }
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = sk[src(x, y)];
        }
    }
}

void media(imagem src, imagem &dst, int range) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            int pixel = 0;
            int count = 0;
            for (int yy = y - range; yy <= y + range; yy++) {
                for (int xx = x - range; xx <= x + range; xx++) {
                    if (yy < 0 || xx < 0 || yy >= src.h || xx >= src.w)
                        continue;
                    pixel += src(xx, yy);
                    count++;
                }
            }
            dst(x, y) = pixel * (1.0f / count);
        }
    }
}

void mediana(imagem src, imagem &dst, int range) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            vector<int> pixel;
            int count = 0;
            for (int yy = y - range; yy <= y + range; yy++) {
                for (int xx = x - range; xx <= x + range; xx++) {
                    if (yy < 0 || xx < 0 || yy >= src.h || xx >= src.w)
                        continue;
                    pixel.push_back(src(xx, yy));
                    count++;
                }
            }
            sort(pixel.begin(), pixel.end());
            dst(x, y) = pixel.at(count / 2 + 1);
        }
    }
}

}  // namespace pgm
