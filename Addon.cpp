//#include <napi.h>

#include "FingerVerifyAsyncWorker.h"
#include "FingerEnrollAsyncWorker.h"

using namespace Napi;

void Enroll(const CallbackInfo &info) {
    Function cb = info[0].As<Function>();

    FingerEnrollAsyncWorker *worker = new FingerEnrollAsyncWorker(cb);
    worker->Queue();
}

void Verify(const CallbackInfo &info) {
    String tmpl = info[0].As<String>();
    Function cb = info[1].As<Function>();

    const string &templ = tmpl.Utf8Value();
    FingerVerifyAsyncWorker *worker = new FingerVerifyAsyncWorker(templ, cb);
    worker->Queue();
}

Object Init(Env env, Object exports) {
    exports["enroll"] = Function::New(env, Enroll, std::string("enroll"));
    exports["verify"] = Function::New(env, Verify, std::string("verify"));
//    exports.Set(String::New(env, "enroll"), Function::New(env, Enroll));
//    exports.Set(String::New(env, "verify"), Function::New(env, verify));
    return exports;
}

NODE_API_MODULE(addon, Init)