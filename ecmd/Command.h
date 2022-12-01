#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <vector>
#include "util.h"

#define FIRST_ARG 2
#define ARG(index) (FIRST_ARG+index) 

class CommandOutput {
public:
	friend std::ostream & operator<<(std::ostream &os, const CommandOutput &output);

	std::string out;
};

static std::ostream & operator<<(std::ostream &os, const CommandOutput &output) {
	os << output.out;
	return os;
}

struct Argument {
	bool mandatory = false;
	std::string name = "";
	std::string description = "";
	bool repeating = false;
};

struct Command {
	std::string name;
	std::vector<Argument> arguments;
	std::string help_message;
	std::function<void()> function;

	Command(std::string name, std::vector<Argument> arguments,
		std::string help_message, std::function<void()> function)
		: name(name), arguments(arguments), help_message(help_message),
		function(function) { }
};

class CommandHandler {
public:
	CommandHandler(const std::vector<Command> &commands);

	void process_command(const std::string &input, bool clear_args = true);

	std::vector<Command> commands;

	int argc;
	std::vector<std::string> argv;
	CommandOutput output;

	static void error_from_string(const std::string &str);
private:
	void sort_arguments();
};


