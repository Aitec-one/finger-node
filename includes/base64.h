//
// Created by Vladislav on 5/20/2020.
//

#ifndef ZKTECOTEST_BASE64_H
#define ZKTECOTEST_BASE64_H

#include <vector>
#include <string>

using namespace std;
typedef unsigned char BYTE;

string base64_encode(BYTE const* buf, unsigned int bufLen);
vector<BYTE> base64_decode(string const&);

#endif //ZKTECOTEST_BASE64_H
