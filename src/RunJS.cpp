#include <ostream>
#include <string>
#include <v8.h>
#include <libplatform/libplatform.h>

#include "RunJS.h"

// Extracts a C string from a V8 Utf8Value.
static const char* ToCString(const v8::String::Utf8Value& value) {
	return *value ? *value : "<string conversion failed>";
}

// The callback that is invoked by v8 whenever the JavaScript 'print'
// function is called.  Prints its arguments on stdout separated by
// spaces and ending with a newline.
static void Print(const v8::FunctionCallbackInfo<v8::Value>&args) {
	v8::Local<v8::External> ext = v8::Local<v8::External>::Cast(args.Data());
	std::ostream *my_out = (std::ostream *)ext->Value();
	bool first = true;
	for (int i = 0; i < args.Length(); i++) {
		v8::HandleScope handle_scope(args.GetIsolate());
		if (first) {
			first = false;
		} else {
			*my_out << " ";
		}
		v8::String::Utf8Value str(args.GetIsolate(), args[i]);
		const char* cstr = ToCString(str);
		*my_out << cstr;
	}
	*my_out << std::endl;
}

static v8::Isolate::CreateParams create_params;
static std::unique_ptr<v8::Platform> platform;

void InitJS(int argc, char*argv[]){
	v8::V8::InitializeICUDefaultLocation(argv[0]);
	v8::V8::InitializeExternalStartupData(argv[0]);
	platform = v8::platform::NewDefaultPlatform();
	v8::V8::InitializePlatform(platform.get());
	v8::V8::Initialize();
	v8::V8::SetFlagsFromCommandLine(&argc,argv,true);
	create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
}

static thread_local v8::Isolate *isolate;
void InitJSthread() {
	isolate = v8::Isolate::New(create_params);
}

void FiniJSthread() {
	isolate->Dispose();
}

void RunJS(std::ostream *out, std::string src) {
	{
		v8::Isolate::Scope isolate_scope(isolate);
		v8::HandleScope handle_scope(isolate);
		// Create a template for the global object.
		v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
		// Bind the global 'print' function to the C++ Print callback.
		global->Set(v8::String::NewFromUtf8(isolate, "print", v8::NewStringType::kNormal).ToLocalChecked(), v8::FunctionTemplate::New(isolate, Print, v8::External::New(isolate, out)));
		// Create a new context.
		v8::Local<v8::Context> context = v8::Context::New(isolate, NULL, global);
		// Enter the context for compiling and running the script.
		v8::Context::Scope context_scope(context);

		// Create a string containing the JavaScript source code.
		v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, src.c_str(), v8::NewStringType::kNormal).ToLocalChecked();

		// Compile the source code.
		v8::Local<v8::Script> script;
		if (v8::Script::Compile(context, source).ToLocal(&script)) {
			// Run the script to get the result.
			v8::Handle<v8::Value> result;
			if (script->Run(context).ToLocal(&result)) {
				// Convert the result to an UTF8 string and print it.
				v8::String::Utf8Value str(result);
			}
		}
	}
}

void FiniJS() {
	v8::V8::Dispose();
	v8::V8::ShutdownPlatform();
	delete create_params.array_buffer_allocator;
	platform.reset();
}
