#include "Command.h"

ArgumentHandler argument_handler(std::vector<Argument>({
	Argument(
		true,
		"greeting",
		"Greeting text to be printed.",
		false
	),
	Argument(
		false,
		"-h",
		"Whether the greeting is happy or not.",
		false
	),
	Argument(
		true,
		"-a",
		"Whether the greeting is angry or not.",
		false
	)
}));
