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
                if (scanner->enroll(acquiredTmpl, acTmplLen) == 1) {
                    cout << "Registered!" << endl;
                    this->tmpl = scanner->getLastRegistered();
                    stop = true;
                }
            }
            Sleep(10);
        }
    } else {
        cout << "Could not connect to finger!" << endl;
    }
}


void FingerEnrollAsyncWorker::OnOK() {
    Callback().Call({Env().Null(), String::New(Env(), tmpl)});
}
