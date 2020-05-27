//
// Created by Vladislav on 5/20/2020.
//

#include "FingerVerifyAsyncWorker.h"
using namespace std;

FingerVerifyAsyncWorker::FingerVerifyAsyncWorker(string &tmpl, Function &callback) :
        AsyncWorker(callback),
        tmpl(tmpl),
        stop(false) {
    this->scanner = new FingerScanner();
}

void FingerVerifyAsyncWorker::Execute() {
    if (scanner->connect()) {
        cout << "Starting main loop..." << endl;
        vector<BYTE> storedTmpl;
        if (!tmpl.empty()) {
            cout << "Using template from file" << endl;
            storedTmpl = base64_decode(tmpl);
        }

        while (!stop) {
            unsigned char acquiredTmpl[MAX_TEMPLATE_SIZE];
            unsigned int acTmplLen = MAX_TEMPLATE_SIZE;
            int ret = scanner->acquireFingerprint(acquiredTmpl, &acTmplLen);
            if (ret == ZKFP_ERR_OK) {
                if (scanner->match(acquiredTmpl, acTmplLen, &storedTmpl[0], storedTmpl.size())) {
                    cout << "Matched!" << endl;
                    stop = true;
                } else {
                    cout << "Not matched!" << endl;
                }

            }
            Sleep(10);
        }
    } else {
        cout << "Could not connect to finger!" << endl;
    }
}

void FingerVerifyAsyncWorker::OnOK() {
    Callback().Call({});
}
