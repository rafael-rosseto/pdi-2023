#pragma once

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

namespace pbm {

struct image {
    string type;
    int w, h, bits;
    vector<vector<bool>> data;
};

void eat_comment(ifstream &f) {
    char linebuf[1024];
    char ppp;
    while (ppp = f.peek(), ppp == '\n' || ppp == '\r')
        f.get();
    if (ppp == '#')
        f.getline(linebuf, 1023);
}

void load_pbm(image &img, const string &name) {
    ifstream f(name.c_str(), ios::binary);
    if (f.fail()) {
        cout << "Could not open file: " << name << endl;
        return;
    }
    eat_comment(f);
    f >> img.type;
    eat_comment(f);
    f >> img.w;
    eat_comment(f);
    f >> img.h;
    eat_comment(f);
    f >> img.bits;
    if (img.w < 1) {
        cout << "Unsupported width: " << img.w << endl;
        f.close();
        return;
    }
    if (img.h < 1) {
        cout << "Unsupported height: " << img.h << endl;
        f.close();
        return;
    }
    if (img.bits < 1) {
        cout << "Unsupported number of bits: " << img.bits << endl;
        f.close();
        return;
    }
    img.data.resize(img.w, vector<bool>(img.h));
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            int v;
            f >> v;
            img.data[i][j] = v;
        }
    }
    f.close();
}

void cloneImg(image &img1, image &img2) {
    img2.type = img1.type;
    img2.bits = img1.bits;
    img2.w = img1.w;
    img2.h = img1.h;
    img2.data.clear();
    img2.data.resize(img1.w, vector<bool>(img1.h));
}

void invert(image &img, image &img2) {
    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h; j++) {
            img2.data[i][j] = 1 - img.data[i][j];
        }
    }
}
}  // namespace pbm
