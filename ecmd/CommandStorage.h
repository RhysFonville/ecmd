#pragma once

#include <filesystem>
#include <thread>
#include <chrono>
#include <format>
#include "Command.h"

static void man();
static void colortest();
static void cd();
static void exit();
static void ls();
static void exists();
static void touch();
static void date();
static void open();
static void mkdir();
static void rm();
static void copy();
static void echo();
static void pwd();
static void cat();

NCommand::CommandHandler command_handler(std::vector<NCommand::Command>({
	NCommand::Command(
		"man",
		std::vector<std::shared_ptr<NCommand::Argument>>({
			MAKE_ARG(NCommand, ExpansiveOptionalArgument("c", "command", "Command to show the manual on."))
		}),
		"Prints a command's manual or gives a brief description about all command_handler.commands.",
		man
	),
	NCommand::Command(
		"colortest",
		std::vector<std::shared_ptr<NCommand::Argument>>({
			MAKE_ARG(NCommand, ExpansiveOptionalArgument("s", "string", "String to test the colors with.")),
			MAKE_ARG(NCommand, ExpansiveOptionalArgument("c", "color", "Color to test."))
		}),
		"Tests the console text colors.",
		colortest
	),
	NCommand::Command(
		"cd",
		std::vector<std::shared_ptr<NCommand::Argument>>({
			MAKE_ARG(NCommand, Argument("path", "Path to switch the current directory to."))
		}),
		"Changes the current directory.",
		cd
	),
	NCommand::Command(
		"exit",
		std::vector<std::shared_ptr<NCommand::Argument>>({
			MAKE_ARG(NCommand, ExpansiveOptionalArgument("ms", "seconds", "How long to wait before exiting in milliseconds.")),
			MAKE_ARG(NCommand, ExpansiveOptionalArgument("s", "seconds", "How long to wait before exiting in seconds.")),
			MAKE_ARG(NCommand, ExpansiveOptionalArgument("m", "seconds", "How long to wait before exiting in minutes.")),
			MAKE_ARG(NCommand, ExpansiveOptionalArgument("h", "seconds", "How long to wait before exiting in hours."))
		}),
		"Exits the program.",
		std::function<void()>([]() { exit(); }) // Compiler didn't like the normal way here for some reason
	),
	NCommand::Command(
		"ls",
		std::vector<std::shared_ptr<NCommand::Argument>>({
			MAKE_ARG(NCommand, ExpansiveOptionalArgument("p", "path", "Path to search.")),
			MAKE_ARG(NCommand, OptionalArgument("l", "More detailed list."))
		}),
		"Lists the files or subdirectories in a directory.",
		ls
	),
	NCommand::Command(
		"exists",
		std::vector<std::shared_ptr<NCommand::Argument>>({
			MAKE_ARG(NCommand, Argument("path", "Path to use."))
		}),
		"Prints \"true\" if specified file exists, prints \"false\" if it doesn't.",
		exists
	),
	NCommand::Command(
		"touch",
		std::vector<std::shared_ptr<NCommand::Argument>>({
			MAKE_ARG(NCommand, Argument("path", "Path to use."))
		}),
		"Changes file timestamps.",
		touch
	),
	NCommand::Command(
		"date",
		std::vector<std::shared_ptr<NCommand::Argument>>({
			MAKE_ARG(NCommand, Argument("path", "Path to use."))
		}),
		"Shows the date of the last modification.",
		date
	),
	NCommand::Command(
		"open",
		std::vector<std::shared_ptr<NCommand::Argument>>({
			MAKE_ARG(NCommand, Argument("path", "Path to use."))
		}),
		"Opens/runs the specified file.",
		open
	),
	NCommand::Command(
		"mkdir",
		std::vector<std::shared_ptr<NCommand::Argument>>({
			MAKE_ARG(NCommand, Argument("path", "directory to add.")),
			MAKE_ARG(NCommand, OptionalArgument("m", "Move the working directory to the newly added directory.")),
			MAKE_ARG(NCommand, OptionalArgument("o", "If a directory with that name already exists, override it."))
		}),
		"Adds a directory.",
		mkdir
	),
	NCommand::Command(
		"rm",
		std::vector<std::shared_ptr<NCommand::Argument>>({
			MAKE_ARG(NCommand, Argument("path", "File/Directory to remove.")),
			MAKE_ARG(NCommand, OptionalArgument("r", "Force remove."))
		}),
		"Removes a file/directory.",
		rm
	),
	NCommand::Command(
		"copy",
		std::vector<std::shared_ptr<NCommand::Argument>>({
			MAKE_ARG(NCommand, Argument("source_path", "Path of the source.")),
			MAKE_ARG(NCommand, Argument("destination_path", "Path of the destination."))
		}),
		"Copies a file/directory to a specified location.",
		copy
	),
	NCommand::Command(
		"echo",
		std::vector<std::shared_ptr<NCommand::Argument>>({
			MAKE_ARG(NCommand, Argument("string", "String to echo")),
		}),
		"Echos string to output destination.",
		echo
	),
	NCommand::Command(
		"pwd",
		std::vector<std::shared_ptr<NCommand::Argument>>({ }),
		"Prints the working directory.",
		pwd
	),
	NCommand::Command(
		"cat",
		std::vector<std::shared_ptr<NCommand::Argument>>({
			MAKE_ARG(NCommand, Argument("file", "File to catenate."))
		}),
		"Catenates the specified file. Prints its contents.",
		cat
	)
}));

static void man() {
	command_handler.output.out += '\n';

	size_t arg = find_arg(command_handler.argv, "-c");
	if (arg == -1) {
		for (NCommand::Command command : command_handler.commands) {
			command_handler.output.out += command.name + ": " + command.help_message + '\n';
		}
		command_handler.output.out += '\n';
	} else {
		for (NCommand::Command command : command_handler.commands) {
			if (command.name == command_handler.argv[arg+1]) {
				command_handler.help_prompt(command);
				return;
			}
		}
		error<std::invalid_argument>("Command::Command does not exist.");
	}
}

static void colortest() {
	if (size_t i = find_arg(command_handler.argv, "-c"); i == -1) {
		for (int i = 0; i < 256; i++) {
			SetConsoleTextAttribute(console, i);
			command_handler.output.print(std::to_string(i) + ' ' + (find_arg(command_handler.argv, "-s") == -1 ? "Hello World!" : command_handler.argv[find_arg(command_handler.argv, "-s") + 1]) + '\n');
		}
	} else {
		SetConsoleTextAttribute(console, std::stoi(command_handler.argv[i+1]));
		command_handler.output.print(command_handler.argv[i+1] + ' ' + (find_arg(command_handler.argv, "-s") == -1 ? "Hello World!" : command_handler.argv[find_arg(command_handler.argv, "-s") + 1]) + '\n');
	}
	SetConsoleTextAttribute(console, DEFAULT_TEXT_COLOR);
	command_handler.output.out += '\n';
}

static void cd() {
	if (fs::exists(command_handler.argv[NCommand::CommandHandler::FIRST_ARG])) {
		fs::current_path(command_handler.argv[NCommand::CommandHandler::FIRST_ARG]);
	} else {
		error<std::invalid_argument>(PATH_DOES_NOT_EXIST);
	}
}

static void exit() {
	int milliseconds	= find_arg(command_handler.argv, "-ms") != -1	? std::stoi(command_handler.argv[find_arg(command_handler.argv, "-ms")+1]) : 0;
	int seconds			= find_arg(command_handler.argv, "-s")	 != -1	? std::stoi(command_handler.argv[find_arg(command_handler.argv, "-s")+1])	 : 0;
	int minutes			= find_arg(command_handler.argv, "-m")	 != -1	? std::stoi(command_handler.argv[find_arg(command_handler.argv, "-m")+1])	 : 0;
	int hours			= find_arg(command_handler.argv, "-h")	 != -1	? std::stoi(command_handler.argv[find_arg(command_handler.argv, "-h")+1])	 : 0;

	command_handler.output.print("Exiting in:\n\t"
		+ std::to_string(hours)			+ " hours\n\t"
		+ std::to_string(minutes)		+ " minutes\n\t"
		+ std::to_string(seconds)		+ " seconds\n\t"
		+ std::to_string(milliseconds)	+ " milliseconds");

	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	std::this_thread::sleep_for(std::chrono::seconds(seconds));
	std::this_thread::sleep_for(std::chrono::minutes(minutes));
	std::this_thread::sleep_for(std::chrono::hours(hours));

	exit(0);
}

static void ls() {
	std::string path = (find_arg(command_handler.argv, "-p") != -1 ? command_handler.argv[find_arg(command_handler.argv, "-p")+1] : fs::current_path().string());

	if (!fs::exists(path)) {
		error<std::invalid_argument>(PATH_DOES_NOT_EXIST);
	}

	if (find_arg(command_handler.argv, "-l") != -1) {
		command_handler.output.out += "DATE MODIFIED, TYPE, SIZE, PATH\n";
	}

	for (const auto &entry : fs::directory_iterator(path)) {
		if (find_arg(command_handler.argv, "-l") != -1) {
			command_handler.output.out += std::format("{}", fs::last_write_time(path)) + '\t';

			command_handler.output.out += file_type_to_string(fs::get_file_type(path)) + '\t' + std::to_string(entry.file_size());

			command_handler.output.out += "\t\t";
		}

		command_handler.output.out += entry.path().filename().string();

		command_handler.output.out += '\n';
	}

	// GetFileAttributesA(path.c_str()) & FILE_ATTRIBUTE_NORMAL
}

static void exists() {
	command_handler.output.out += std::string(fs::exists(command_handler.argv[NCommand::CommandHandler::FIRST_ARG]) ? "true" : "false") + '\n';
}

static void touch() {
	std::ofstream file(command_handler.argv[NCommand::CommandHandler::FIRST_ARG]);
	if (file.is_open()) {
		file << "";
		file.close();
	} else {
		error<std::invalid_argument>("Failed to open file.");
	}
}

static void date() {
	command_handler.output.out = std::format("{}", fs::last_write_time(command_handler.argv[NCommand::CommandHandler::FIRST_ARG])) + '\n';
}

static void open() {
	if (!fs::exists(command_handler.argv[NCommand::CommandHandler::FIRST_ARG])) {
		error<std::invalid_argument>(PATH_DOES_NOT_EXIST);
	}

	// additional information
	STARTUPINFOA si = { };
	PROCESS_INFORMATION pi = { };

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	CreateProcessA(command_handler.argv[2].c_str(),   // the path
		&words_to_string(command_handler.argv)[0],		    // Command::Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);

	WaitForSingleObject( pi.hProcess, INFINITE );

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

static void mkdir() {
	if (find_arg(command_handler.argv, "-o") != -1) {
		if (fs::exists(command_handler.argv[NCommand::CommandHandler::FIRST_ARG])) {
			fs::remove_all(command_handler.argv[NCommand::CommandHandler::FIRST_ARG]);
		}
	}
	fs::create_directory(command_handler.argv[NCommand::CommandHandler::FIRST_ARG]);

	if (find_arg(command_handler.argv, "-m") != -1) {
		fs::current_path(command_handler.argv[NCommand::CommandHandler::FIRST_ARG]);
	}
}

static void rm() {
	if (fs::exists(command_handler.argv[NCommand::CommandHandler::FIRST_ARG])) {
		if (!fs::equivalent(fs::current_path(), command_handler.argv[NCommand::CommandHandler::FIRST_ARG])) {
			if (find_arg(command_handler.argv, "-r") != -1) {
				fs::remove_all(command_handler.argv[NCommand::CommandHandler::FIRST_ARG]);
			} else {
				if (fs::is_directory(command_handler.argv[NCommand::CommandHandler::FIRST_ARG])) {
					if (fs::is_empty(command_handler.argv[NCommand::CommandHandler::FIRST_ARG])) {
						fs::remove(command_handler.argv[NCommand::CommandHandler::FIRST_ARG]);
					} else {
						error<std::invalid_argument>("Directory is not empty.", "Either add the -r modifer to force remove that directory or empty the directory.");
					}
				} else {
					fs::remove(command_handler.argv[NCommand::CommandHandler::FIRST_ARG]);
				}
			}
		} else {
			error<std::invalid_argument>("You cannot remove a directory that the working directory is already in or a child of.", "Exit the directory before you try remove it.");
		}
	} else {
		error<std::invalid_argument>(PATH_DOES_NOT_EXIST);
	}
}

static void copy() {
	if (fs::exists(command_handler.argv[NCommand::CommandHandler::FIRST_ARG])) {
		fs::copy(command_handler.argv[NCommand::CommandHandler::FIRST_ARG], command_handler.get_arg(1));
	} else {
		error<std::invalid_argument>(PATH_DOES_NOT_EXIST);
	}
}

static void echo() {
	command_handler.output.out += command_handler.argv[NCommand::CommandHandler::FIRST_ARG] + '\n';
}

static void pwd() {
	command_handler.output.out += fs::current_path().string() + '\n';
}

static void cat() {
	if (fs::exists(command_handler.argv[NCommand::CommandHandler::FIRST_ARG])) {
		std::ifstream in(command_handler.argv[NCommand::CommandHandler::FIRST_ARG]);
		std::string str;
		while (std::getline(in, str)) {
			command_handler.output.out += str + '\n';
		}
	}
}

