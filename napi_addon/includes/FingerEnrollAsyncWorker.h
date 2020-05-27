//
// Created by Vladislav on 5/20/2020.
//

#ifndef ZKTECOTEST_FINGERENROLLASYNCWORKER_H
#define ZKTECOTEST_FINGERENROLLASYNCWORKER_H

#include <napi.h>
#include <iostream>
#include "FingerScanner.h"
#include "base64.h"

using namespace Napi;
using namespace std;


class FingerEnrollAsyncWorker : public AsyncWorker {
public:
    FingerEnrollAsyncWorker(Function &callback);

    void Execute();

    void OnOK();

private:
    string tmpl;
    bool stop;
    FingerScanner *scanner;
};


#endif //ZKTECOTEST_FINGERENROLLASYNCWORKER_H
