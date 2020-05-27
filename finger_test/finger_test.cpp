#if defined(_WIN32)
#include <windows.h>
#endif
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "FingerScanner.h"
#include "base64.h"

using namespace std;

const char* tmplFileName = R"(C:\Users\Vladislav\CLionProjects\zktecotest\templates.txt)";
bool stop = false;
int opType = 0;

string readTemplate(const char* fileName) {
    ifstream t(fileName);
    stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

void writeTemplate(const string& tmpl, const char* fileName) {
    std::ofstream out(fileName);
    out << tmpl;
    out.close();
}

void mainLoop(FingerScanner* scanner) {
    string templ = readTemplate(tmplFileName);
    vector<BYTE> storedTmpl;
    if (!templ.empty()) {
        cout << "Using template from file" << endl;
        storedTmpl = base64_decode(templ);
        opType = 2;
    }

    while (!stop) {
        unsigned char acquiredTmpl[MAX_TEMPLATE_SIZE];
        unsigned int acTmplLen = MAX_TEMPLATE_SIZE;
        int ret = scanner->acquireFingerprint(acquiredTmpl, &acTmplLen);
        if (ret == ZKFP_ERR_OK) {
            if (opType == 0) {
                if (scanner->enroll(acquiredTmpl, acTmplLen) == 1) {
                    cout << "Registered!" << endl;
                    string base64 = scanner->getLastRegistered();
                    writeTemplate(base64, tmplFileName);
                    cout << "Writen to templates.txt" << endl;
                    opType = 1;
                }

            } else if (opType == 1) {
                if (scanner->verify(acquiredTmpl, acTmplLen) == 1) {
                    cout << "Verified!" << endl;
                } else {
                    cout << "Not verified!" << endl;
                }

            } else if (opType == 2) {
                if (scanner->match(acquiredTmpl, acTmplLen, &storedTmpl[0], storedTmpl.size())) {
                    cout << "Matched!" << endl;
                } else {
                    cout << "Not matched!" << endl;
                }
            }
        }
#ifdef WIN32
        Sleep(10);
#endif
    }
}

int main() {
    cout << "Starting fingerprint demo..." << endl;

    FingerScanner scanner;

    if (scanner.connect()) {
        cout << "Starting main loop..." << endl;
        mainLoop(&scanner);
    }
    return 0;
}
