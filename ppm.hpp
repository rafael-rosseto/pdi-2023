#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#define pi 3.14159265359

using namespace std;

namespace ppm {

struct RGB {
    uint8_t r, g, b;
};

struct image {
    string type;
    int w, h, bits;
    vector<vector<RGB>> data;
};

void eatComment(ifstream &f) {
    char linebuf[1024];
    char ppp;
    while (ppp = f.peek(), ppp == '\n' || ppp == '\r')
        f.get();
    if (ppp == '#')
        f.getline(linebuf, 1023);
}

void loadPpm(image &img, const string &name) {
    ifstream f(name.c_str(), ios::binary);
    if (f.fail()) {
        cout << "Could not open file: " << name << endl;
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
    img.data.resize(img.w, vector<RGB>(img.h));
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            int v;
            f >> v;
            img.data[i][j].r = v;
            f >> v;
            img.data[i][j].g = v;
            f >> v;
            img.data[i][j].b = v;
        }
    }
    f.close();
}

void savePpm(image &img, const string &name) {
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
            f << +img.data[i][j].r << ' ';
            f << +img.data[i][j].g << ' ';
            f << +img.data[i][j].b << endl;
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
            cout << +img.data[i][j].r << ' ';
            cout << +img.data[i][j].g << ' ';
            cout << +img.data[i][j].b << endl;
        }
    }
}

void cloneHeader(image img, image &out) {
    out.type = img.type;
    out.bits = img.bits;
    out.w = img.w;
    out.h = img.h;
    out.data.clear();
    out.data.resize(img.w, vector<RGB>(img.h));
}

void cloneData(image img, image &out) {
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            out.data[i][j].r = img.data[i][j].r;
            out.data[i][j].g = img.data[i][j].g;
            out.data[i][j].b = img.data[i][j].b;
        }
    }
}

void invert(image img, image &out) {
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            out.data[i][j].r = 255 - img.data[i][j].r;
            out.data[i][j].g = 255 - img.data[i][j].g;
            out.data[i][j].b = 255 - img.data[i][j].b;
        }
    }
}

void media(image img, image &out, int range) {
    cloneHeader(img, out);
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            int R = 0, G = 0, B = 0;
            int count = 0;
            for (int ii = i - range; ii <= i + range; ii++) {
                for (int jj = j - range; jj <= j + range; jj++) {
                    if (ii < 0 || jj < 0 || ii >= img.h || jj >= img.w)
                        continue;
                    R += img.data[ii][jj].r;
                    G += img.data[ii][jj].g;
                    B += img.data[ii][jj].b;
                    count++;
                }
            }
            out.data[i][j].r = R * (1.0f / count);
            out.data[i][j].g = G * (1.0f / count);
            out.data[i][j].b = B * (1.0f / count);
        }
    }
}

void mediana(image img, image &out, int range) {
    cloneHeader(img, out);
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            vector<int> R, G, B;
            int count = 0;
            for (int ii = i - range; ii <= i + range; ii++) {
                for (int jj = j - range; jj <= j + range; jj++) {
                    if (ii < 0 || jj < 0 || ii >= img.w || jj >= img.h)
                        continue;
                    R.push_back(img.data[ii][jj].r);
                    G.push_back(img.data[ii][jj].g);
                    B.push_back(img.data[ii][jj].b);
                    count++;
                }
            }
            sort(R.begin(), R.end());
            out.data[i][j].r = R.at(count / 2 + 1);
            sort(G.begin(), G.end());
            out.data[i][j].g = G.at(count / 2 + 1);
            sort(B.begin(), B.end());
            out.data[i][j].b = B.at(count / 2 + 1);
        }
    }
}
}  // namespace ppm
