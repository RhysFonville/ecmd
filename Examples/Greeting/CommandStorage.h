#include "Command.h"

static void base_callback() { }
static void greeting(NArgument::ArgvLocation location);
static void optional_h(NArgument::ArgvLocation location);
static void optional_a(NArgument::ArgvLocation location);

NArgument::ArgumentHandler argument_handler(base_callback,
std::vector<std::shared_ptr<NArgument::Argument>>({
	MAKE_ARG(NArgument, Argument(
		"greeting",
		"Greeting text to be printed.",
		greeting
	)),
	MAKE_ARG(NArgument, OptionalArgument(
		"h",
		"Whether the greeting is happy or not.",
		optional_h
	)),
	MAKE_ARG(NArgument, OptionalArgument(
		"a",
		"Whether the greeting is angry or not.",
		optional_a
	))
}));

static void greeting(NArgument::ArgvLocation location) {
	argument_handler.output.out += *location;
}

static void optional_h(NArgument::ArgvLocation location) {
	argument_handler.output.out += " :D";
}

static void optional_a(NArgument::ArgvLocation location) {
	argument_handler.output.out += " >:(";
}
