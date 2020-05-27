#include <napi.h>

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

    FingerVerifyAsyncWorker *worker = new FingerVerifyAsyncWorker(tmpl.Utf8Value(), cb);
    worker->Queue();
}

Object Init(Env env, Object exports) {
    exports.Set(String::New(env, "enroll"), Function::New(env, Enroll));
    exports.Set(String::New(env, "verify"), Function::New(env, Verify));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)