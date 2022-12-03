#include "Command.h"

CommandHandler::CommandHandler(const std::vector<Command> &commands) : commands(commands),
	argc(0), argv(std::vector<std::string>()) { }

void CommandHandler::sort_arguments() {
	// I have no idea how to do this
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
void CommandHandler::string_to_args(const std::string &input) {
	LPWSTR *args = CommandLineToArgvW(string_to_wstring(input).c_str(), &argc);

	argc++;
	argv.push_back(std::filesystem::current_path().string());
	for (int i = 0; i < argc-1; i++) {
		argv.push_back(wstring_to_string(args[i]));
	}
}
#endif

void CommandHandler::process_command(bool clear_args) {
	if (argv.size() > 1) {
		bool found_command = false;
		for (Command command : commands) {
			if (command.name == argv[1]) {
				found_command = true;

				size_t number_of_mandatory_args = 0;
				for (const Argument &arg : command.arguments) {
					if (arg.mandatory) number_of_mandatory_args++;
				}
				size_t first_optional_arg_index = 0;
				for (std::vector<std::string>::iterator it = argv.begin(); it == argv.end(); it++) {
					if ((*it)[0] == '-') {
						first_optional_arg_index = std::distance(argv.begin(), it);
						break;
					}
				}
				if (first_optional_arg_index == number_of_mandatory_args) {
					try {
						command.callback();
						std::cout << output;
						output.out.clear();
					} catch (std::exception &e) {
						error_from_string(e.what());
						std::cerr << "Command aborted.\n\n";
					}
				} else {
					error<std::invalid_argument>("A mandatory argument was not specified.", "Mandatory arguments to not need a dashed dashed modifier (ex: -x) and need to be specified at the beginning of the arguments.");
				}
				break;
			}
		}
		if (!found_command)
			error_from_string("Command does not exist.");
	}

	if (clear_args) {
		argv.clear();
		argc = 0;
	}
}

void CommandHandler::help_prompt(const Command &command) {
	output.out += command.help_message + "\n\n";

	output.out += to_upper(command.name);

	for (Argument argument : command.arguments) {
		output.out += (argument.mandatory ? " {" : " [");
		output.out += argument.name;
		output.out += (argument.mandatory ? "}" : "]");

		if (argument.repeating) {
			output.out += "...";
		}
	}
	output.out += "\n\n";
	for (Argument argument : command.arguments) {
		output.out += argument.name + ": " + argument.description + '\n';
	}

	output.out += '\n';
}

void CommandHandler::error_from_string(const std::string &str) {
	std::vector<std::string> message = split(str, "====SUGGESTIONS====");

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	SetConsoleTextAttribute(console, ERROR_TEXT_COLOR);
#endif

	std::cerr << message[0];

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	SetConsoleTextAttribute(console, ERROR_SUGGESTION_TEXT_COLOR);
#endif

	for (int i = 1; i < message.size(); i++) {
		std::cerr << message[i];
	}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	SetConsoleTextAttribute(console, DEFAULT_TEXT_COLOR);
#endif

	std::cout << std::endl;
}

ArgumentHandler::ArgumentHandler(const std::vector<Argument> &arguments)
	: arguments(arguments), argc(0), argv(std::vector<std::string>()) { }

Argument::Argument(bool mandatory, std::string name,
	std::string description, bool repeating) : mandatory(mandatory), name(name),
	description(description), repeating(repeating) {  }
