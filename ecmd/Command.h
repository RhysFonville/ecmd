#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <vector>
#include "util.h"

/*
* "ecmd" stands for "Epic Command". As in, "My Epic Command Prompt"
* Began as a linux like command prompt, it is now an API to make creating commands easy.
* 
* Made September 18, 2022, at 2:07 PM
*/

class CommandOutput {
public:
	friend std::ostream & operator<<(std::ostream &os, const CommandOutput &output);

	std::string out;
};

inline std::ostream & operator<<(std::ostream &os, const CommandOutput &output) {
	os << output.out;
	return os;
}

class Argument {
public:
	Argument(bool mandatory = false, std::string name = "", std::string description = "",
		bool repeating = false);

	bool mandatory = false;
	std::string name = "";
	std::string description = "";
	bool repeating = false;
};

struct Command {
	std::string name;
	std::vector<Argument> arguments;
	std::string help_message;
	std::function<void()> callback;

	Command(std::string name, std::vector<Argument> arguments,
		std::string help_message, std::function<void()> callback)
		: name(name), arguments(arguments), help_message(help_message),
		callback(callback) { }
};

class CommandHandler {
public:
	CommandHandler(const std::vector<Command> &commands);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	void string_to_args(const std::string &input);
#endif

	void process_command(bool clear_args = true);

	void help_prompt(const Command &command);

	std::string & get_arg(size_t index) { return argv[FIRST_ARG + index]; }

	std::vector<Command> commands;

	int argc;
	std::vector<std::string> argv;
	CommandOutput output;

	static void error_from_string(const std::string &str);

	static constexpr size_t FIRST_ARG = 2;
private:
	void sort_arguments();
};

class ArgumentHandler {
public:
	ArgumentHandler(const std::vector<Argument> &arguments);

	std::vector<Argument> arguments;

	int argc;
	std::vector<std::string> argv;
	CommandOutput output;

	static constexpr size_t FIRST_ARG = 1;
};
