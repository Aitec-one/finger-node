//
// Created by Vladislav on 5/20/2020.
//

#ifndef ZKTECOTEST_BASE64_H
#define ZKTECOTEST_BASE64_H


#include <vector>
#include <string>
typedef unsigned char BYTE;

std::string base64_encode(BYTE const* buf, unsigned int bufLen);
std::vector<BYTE> base64_decode(std::string const&);

#endif //ZKTECOTEST_BASE64_H
