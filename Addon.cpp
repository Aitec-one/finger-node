#include <napi.h>

#include "FingerAsyncWorker.h"

using namespace Napi;

void Start(const CallbackInfo &info) {
    String tmpl = info[0].As<String>();
    Function cb = info[1].As<Function>();

    FingerAsyncWorker *worker = new FingerAsyncWorker(tmpl.Utf8Value(), cb);
    worker->Queue();
}

Object Init(Env env, Object exports) {
    exports.Set(String::New(env, "start"), Function::New(env, Start));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)