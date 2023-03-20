#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#define pi 3.14159265359

using namespace std;

namespace pbm {

struct image {
    string type;
    int w, h, bits;
    vector<vector<bool>> data;
};

void eatComment(ifstream &f) {
    char linebuf[1024];
    char ppp;
    while (ppp = f.peek(), ppp == '\n' || ppp == '\r')
        f.get();
    if (ppp == '#')
        f.getline(linebuf, 1023);
}

void loadPbm(image &img, const string &name) {
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
    img.data.resize(img.w, vector<bool>(img.h));
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            bool v;
            f >> v;
            img.data[i][j] = v;
        }
    }
    f.close();
}

void cloneHeader(image &img, image &out) {
    out.type = img.type;
    out.bits = img.bits;
    out.w = img.w;
    out.h = img.h;
    out.data.clear();
    out.data.resize(img.w, vector<bool>(img.h));
}

void invert(image &img, image &out) {
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            out.data[i][j] = 1 - img.data[i][j];
        }
    }
}
}  // namespace pbm
