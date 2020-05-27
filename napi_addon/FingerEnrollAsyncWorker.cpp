//
// Created by Vladislav on 5/20/2020.
//

#include "FingerEnrollAsyncWorker.h"

using namespace std;

FingerEnrollAsyncWorker::FingerEnrollAsyncWorker(Function &callback) :
        AsyncWorker(callback),
        stop(false) {
    this->scanner = new FingerScanner();
}

void FingerEnrollAsyncWorker::Execute() {
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
                if (scanner->enroll(acquiredTmpl, acTmplLen) == 1) {
                    this->tmpl = scanner->getLastRegistered();
                    stop = true;
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


void FingerEnrollAsyncWorker::OnOK() {
    Callback().Call({Env().Null(), String::New(Env(), tmpl)});
}
