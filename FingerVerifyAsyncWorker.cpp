//
// Created by Vladislav on 5/20/2020.
//

#include "FingerVerifyAsyncWorker.h"

#include <utility>
using namespace std;

FingerVerifyAsyncWorker::FingerVerifyAsyncWorker(string tmpl, Function &callback) :
        AsyncWorker(callback),
        tmpl(std::move(tmpl)),
        stop(false),
        verified(false) {
    this->scanner = new FingerScanner();
}

void FingerVerifyAsyncWorker::Execute() {
    if (scanner->connect()) {
        vector<BYTE> storedTmpl;
        if (!tmpl.empty()) {
            storedTmpl = base64_decode(tmpl);
        }

        while (!stop) {
            unsigned char acquiredTmpl[MAX_TEMPLATE_SIZE];
            unsigned int acTmplLen = MAX_TEMPLATE_SIZE;
            int ret = scanner->acquireFingerprint(acquiredTmpl, &acTmplLen);
            if (ret == ZKFP_ERR_OK) {
                if (scanner->match(acquiredTmpl, acTmplLen, &storedTmpl[0], storedTmpl.size())) {
                    verified = true;
                    stop = true;
                } else {
                    cout << "Not matched!" << endl;
                }
            }
#if defined(_WIN32)
            Sleep(10);
#endif
        }
    } else {
        SetError("Could not connect to finger!");
    }
}

void FingerVerifyAsyncWorker::OnOK() {
    string result = verified ? "verified" : "";
    Callback().Call({Env().Null(), String::New(Env(), result)});
}
