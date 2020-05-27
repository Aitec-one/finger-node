//
// Created by Vladislav on 5/20/2020.
//

#ifndef ZKTECOTEST_FINGERVERIFYASYNCWORKER_H
#define ZKTECOTEST_FINGERVERIFYASYNCWORKER_H

#include <napi.h>
#include <iostream>
#include "FingerScanner.h"
#include "base64.h"

using namespace Napi;
using namespace std;

class FingerVerifyAsyncWorker : public AsyncWorker {
public:
    FingerVerifyAsyncWorker(string tmpl,
                      Function &callback);

    void Execute();

    void OnOK();

private:
    string tmpl;
    bool stop;
    bool verified;
    FingerScanner *scanner;
};


#endif //ZKTECOTEST_FINGERVERIFYASYNCWORKER_H
