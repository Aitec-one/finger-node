

#include "FingerEnrollAsyncWorker.h"
#include "FingerVerifyAsyncWorker.h"

void Enroll(const CallbackInfo &info) {
    Function cb = info[0].As<Function>();

    auto *worker = new FingerEnrollAsyncWorker(cb);
    worker->Queue();
}

void Verify(const CallbackInfo &info) {
    String tmpl = info[0].As<String>();
    Function cb = info[1].As<Function>();

    const string &templ = tmpl.Utf8Value();
    auto *worker = new FingerVerifyAsyncWorker(templ, cb);
    worker->Queue();
}

Object Init(Env env, Object exports) {
    exports["enroll"] = Function::New(env, Enroll, std::string("enroll"));
    exports["verify"] = Function::New(env, Verify, std::string("verify"));
    return exports;
}

NODE_API_MODULE(addon, Init)