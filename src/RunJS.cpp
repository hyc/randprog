#include <ostream>
#include <string>
#include <v8.h>

static std::ostream *my_out;

// Extracts a C string from a V8 Utf8Value.
static const char* ToCString(const v8::String::Utf8Value& value) {
	return *value ? *value : "<string conversion failed>";
}

// The callback that is invoked by v8 whenever the JavaScript 'print'
// function is called.  Prints its arguments on stdout separated by
// spaces and ending with a newline.
static v8::Handle<v8::Value> Print(const v8::Arguments& args) {
	bool first = true;
	for (int i = 0; i < args.Length(); i++) {
		v8::HandleScope handle_scope;
		if (first) {
			first = false;
		} else {
			*my_out << " ";
		}
		v8::String::Utf8Value str(args[i]);
		const char* cstr = ToCString(str);
		*my_out << cstr;
	}
	*my_out << std::endl;
}

void RunJS(std::ostream &out, std::string src) {
	my_out = &out;

	{
		v8::HandleScope handle_scope;
		// Create a template for the global object.
		v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
		// Bind the global 'print' function to the C++ Print callback.
		global->Set(v8::String::New("print"), v8::FunctionTemplate::New(Print));
		// Create a new context.
		v8::Persistent<v8::Context> context = v8::Context::New(NULL, global);
		// Enter the context for compiling and running the script.
		context->Enter();

		// Create a string containing the JavaScript source code.
		v8::Handle<v8::String> source = v8::String::New(src.c_str(), src.size());

		// Compile the source code.
		v8::Handle<v8::Script> script = v8::Script::Compile(source);
		// Run the script to get the result.
		v8::Handle<v8::Value> result = script->Run();
		// Convert the result to an UTF8 string and print it.
		v8::String::Utf8Value str(result);
		context->Exit();
		context.Dispose();
	}
	v8::V8::Dispose();
}
