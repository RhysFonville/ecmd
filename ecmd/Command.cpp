#include "Command.h"

NCommand::CommandHandler::CommandHandler(const std::vector<NCommand::Command> &commands)
	: commands(commands), argc(0), argv(std::vector<std::string>({ })) { }

void NCommand::CommandHandler::sort_arguments() {
	// I have no idea how to do this
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
void NCommand::CommandHandler::string_to_args(const std::string &input) {
	LPWSTR *args = CommandLineToArgvW(string_to_wstring(input).c_str(), &argc);

	argc++;
	argv.push_back(std::filesystem::current_path().string());
	for (int i = 0; i < argc-1; i++) {
		argv.push_back(wstring_to_string(args[i]));
	}
}
#endif

void NCommand::CommandHandler::process_command(bool clear_args) {
	if (argv.size() > 1) {
		for (size_t i = 0; i < argv.size(); i++) {
			if (argv[i].find('>') != -1) {
				output.open_file(argv[i+1]);
			}
		}

		bool found_command = false;
		for (Command command : commands) {
			if (command.name == argv[FIRST_ARG-1]) {
				found_command = true;

				size_t number_of_mandatory_args = 0;
				for (const std::shared_ptr<Argument> &arg : command.arguments) {
					if (arg->type == Argument::Type::Argument) number_of_mandatory_args++;
				}
				size_t first_optional_arg_index = number_of_mandatory_args;
				for (std::vector<std::string>::iterator it = argv.begin(); it == argv.end(); it++) {
					if ((*it)[0] == '-') {
						first_optional_arg_index = std::distance(argv.begin(), it);
						break;
					}
				}
				if (first_optional_arg_index == number_of_mandatory_args) {
					try {
						command.callback();
						output.print();
						output.close_file();
						output.out.clear();
					} catch (std::exception &e) {
						print_error(e.what(), output);
						output.print("Command aborted.\n\n");
					}
				} else {
					error<std::invalid_argument>("A mandatory argument was not specified.", "Mandatory arguments to not need a dashed dashed modifier (ex: -x) and need to be specified at the beginning of the arguments.");
				}
				break;
			}
		}
		if (!found_command)
			print_error("Command does not exist.", output);
	}

	if (clear_args) {
		argv.clear();
		argc = 0;
	}
}

void NCommand::CommandHandler::help_prompt(const NCommand::Command &command) {
	output.out += command.help_message + "\n\n";

	output.out += to_upper(command.name);

	for (const std::shared_ptr<Argument> &argument : command.arguments) {
		output.out += (argument->type == Argument::Type::Argument ? " {" : " [");
		output.out += argument->name;
		output.out += (argument->type == Argument::Type::Argument ? "}" : "]");
	}
	output.out += "\n\n";
	for (const std::shared_ptr<Argument> &argument : command.arguments) {
		output.out += argument->name + ": " + argument->description + '\n';
	}

	output.out += '\n';
}

NCommand::Argument::Argument(std::string name, std::string description, Type type)
	: name(name), description(description), type(type) { }

NCommand::OptionalArgument::OptionalArgument(std::string name,
	std::string description, Type type)
	: Argument(OPTIONAL_ARG_NAME(name), description, type) { }

NCommand::ExpansiveOptionalArgument::ExpansiveOptionalArgument(std::string name,
	std::string expansion_name, std::string description, Type type)
	: OptionalArgument(name, description, type),
	expansion_name(expansion_name) { }

NArgument::Argument::Argument(std::string name, std::string description,
	std::function<void()> callback, Type type) : NCommand::Argument(name, description, type),
	callback(callback) { }

NArgument::OptionalArgument::OptionalArgument(std::string name,
	std::string description, std::function<void()> callback, Type type)
	: NCommand::OptionalArgument(name, description, type),
	callback(callback) {}

NArgument::ExpansiveOptionalArgument::ExpansiveOptionalArgument(std::string name,
	std::string expansion_name, std::string description, std::function<void()> callback,
	Type type)
	: NCommand::ExpansiveOptionalArgument(name, expansion_name, description, type),
	callback(callback) { }

NArgument::ArgumentHandler::ArgumentHandler(const std::vector<std::shared_ptr<Argument>> &arguments)
	: arguments(arguments), argc(0), argv(std::vector<std::string>()) { }

void NArgument::ArgumentHandler::process_arguments() {
	for (size_t i = 0; i < argv.size(); i++) {
		if (argv[i].find('>') != -1) {
			output.open_file(argv[i+1]);
		}
	}

	for (const std::string &argv : argv) {
		for (const std::shared_ptr<Argument> &arg : arguments) {
			if (arg->name == argv) {
				try {
					arg->callback();
					output.print();
					output.close_file();
					output.out.clear();
				} catch (std::exception &e) {
					print_error(e.what());
					output.print("Command aborted.\n\n");
				}
				break;
			}
		}
	}
}

void NArgument::ArgumentHandler::help_prompt(const NCommand::Argument &argument) {
	output.out += argument.name +
		(argument.type == Argument::Type::ExpansiveOptionalArgument ? argument.name : "") +
		"\n\n" + argument.description + '\n';
}
