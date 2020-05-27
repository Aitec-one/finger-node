//
// Created by Vladislav on 5/20/2020.
//

#include "FingerAsyncWorker.h"
using namespace std;

FingerAsyncWorker::FingerAsyncWorker(string &tmpl, Function &callback) :
        AsyncWorker(callback),
        tmpl(tmpl),
        stop(false),
        opType(0),
        success(false) {
    this->scanner = new FingerScanner();
}

void FingerAsyncWorker::Execute() {
    if (scanner->connect()) {
        cout << "Starting main loop..." << endl;
//    string templ = readTemplate(tmplFileName);
        vector<BYTE> storedTmpl;
        if (!tmpl.empty()) {
            cout << "Using template from file" << endl;
            storedTmpl = base64_decode(tmpl);
            opType = 2;
        }

        while (!stop) {
            unsigned char acquiredTmpl[MAX_TEMPLATE_SIZE];
            unsigned int acTmplLen = MAX_TEMPLATE_SIZE;
            int ret = scanner->acquireFingerprint(acquiredTmpl, &acTmplLen);
            if (ret == ZKFP_ERR_OK) {
                cout << "Got finger event" << endl;
                if (scanner->match(acquiredTmpl, acTmplLen, &storedTmpl[0], storedTmpl.size())) {
                    cout << "Matched!" << endl;
                    success = true;
                } else {
                    cout << "Not matched!" << endl;
                    success = false;
                }
                stop = true;
            }
            Sleep(10);
        }
    } else {
        cout << "Could not connect to finger!" << endl;
    }
}

void FingerAsyncWorker::OnOK() {
    Callback().Call({});
}
