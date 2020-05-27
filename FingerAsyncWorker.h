//
// Created by Vladislav on 5/20/2020.
//

#ifndef ZKTECOTEST_DATAPROCESSINGASYNCWORKER_H
#define ZKTECOTEST_DATAPROCESSINGASYNCWORKER_H

#include <napi.h>
#include <iostream>
#include "FingerScanner.h"
#include "base64.h"

using namespace Napi;
using namespace std;

class FingerAsyncWorker : public AsyncWorker {
public:
    FingerAsyncWorker(string &tmpl,
                      Function &callback);

    void Execute();

    void OnOK();

private:
    string tmpl;
    bool stop;
    int opType;
    bool success;
    FingerScanner *scanner;
};


#endif //ZKTECOTEST_DATAPROCESSINGASYNCWORKER_H
