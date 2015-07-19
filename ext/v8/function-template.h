// -*- mode: c++ -*-

#ifndef RR_FUNCTION_TEMPLATE_H
#define RR_FUNCTION_TEMPLATE_H

namespace rr {


  class FunctionTemplate : public Ref<v8::FunctionTemplate> {
  public:
    FunctionTemplate(VALUE self) : Ref<v8::FunctionTemplate>(self) {}
    FunctionTemplate(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> t) :
      Ref<v8::FunctionTemplate>(isolate, t) {}
    static inline void Init() {
      ClassBuilder("FunctionTemplate", Template::Class).
        defineSingletonMethod("New", &New).
        defineMethod("GetFunction", &GetFunction).
        store(&Class);
    }

    static VALUE New(int argc, VALUE argv[], VALUE self) {
      VALUE r_isolate, r_callback, r_data, r_signature, r_length;
      rb_scan_args(argc, argv, "14", &r_isolate, &r_callback, &r_data, &r_signature, &r_length);
      Isolate isolate(r_isolate);
      Locker lock(isolate);

      FunctionCallback callback(isolate, r_callback, r_data);
      Signature signature(r_signature);
      int length(RTEST(r_length) ? NUM2INT(r_length) : 0);

      return FunctionTemplate(isolate, v8::FunctionTemplate::New(isolate, callback, callback, v8::Local<v8::Signature>(), length));
    }

    static VALUE GetFunction(VALUE self, VALUE context) {
      FunctionTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      return Function::Maybe(isolate, t->GetFunction(Context(context)));
    }
  };
}

#endif /* RR_FUNCTION_TEMPLATE_H */
