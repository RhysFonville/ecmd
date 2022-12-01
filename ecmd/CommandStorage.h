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

CommandHandler command_handler(std::vector<Command>({
	Command(
		"man",
		std::vector<Argument>({
			Argument({ false, "-c command", "Command to show the manual on." })
		}),
		"Prints a command's manual or gives a brief description about all command_handler.commands.",
		std::function<void()>([]() {
			man();
		})
	),
	Command(
		"colortest",
		std::vector<Argument>({
			Argument({ false, "-s string", "String to test the colors with." }),
			Argument({ false, "-c colors", "Colors to test.", true })
		}),
		"Tests the console text colors.",
		std::function<void()>([]() {
			colortest();
		})
	),
	Command(
		"cd",
		std::vector<Argument>({
			Argument({ true, "path", "Path to switch the current directory to." })
		}),
		"Changes the current directory.",
		std::function<void()>([]() {
			cd();
		})
	),
	Command(
		"exit",
		std::vector<Argument>({
			Argument({ false, "-ms seconds", "How long to wait before exiting in milliseconds." }),
			Argument({ false, "-s seconds", "How long to wait before exiting in seconds." }),
			Argument({ false, "-m seconds", "How long to wait before exiting in minutes." }),
			Argument({ false, "-h seconds", "How long to wait before exiting in hours." })
		}),
		"Exits the program.",
		std::function<void()>([]() {
			exit();
		})
	),
	Command(
		"ls",
		std::vector<Argument>({
			Argument({ false, "-p path", "Path to search." }),
			Argument({ false, "-l", "More detailed list." })
		}),
		"Lists the files or subdirectories in a directory.",
		std::function<void()>([]() {
			ls();
		})
	),
	Command(
		"exists",
		std::vector<Argument>({
			Argument({ true, "path", "Path to use." })
		}),
		"Prints \"true\" if specified file exists, prints \"false\" if it doesn't.",
		std::function<void()>([]() {
			exists();
		})
	),
	Command(
		"touch",
		std::vector<Argument>({
			Argument({ true, "path", "Path to use." })
		}),
		"Changes file timestamps.",
		std::function<void()>([]() {
			touch();
		})
	),
	Command(
		"date",
		std::vector<Argument>({
			Argument({ true, "path", "Path to use." })
		}),
		"Shows the date of the last modification.",
		std::function<void()>([]() {
			date();
		})
	),
	Command(
		"open",
		std::vector<Argument>({
			Argument({ true, "path", "Path to use." })
		}),
		"Opens/runs the specified file.",
		std::function<void()>([]() {
			open();
		})
	),
	Command(
		"mkdir",
		std::vector<Argument>({
			Argument({ true, "path", "Path/directory to add." }),
			Argument({ false, "-m", "Move the working directory to the newly added directory." }),
			Argument({ false, "-o", "If a directory with that name already exists, override it." })
		}),
		"Adds a directory.",
		std::function<void()>([]() {
			mkdir();
		})
	),
	Command(
		"rm",
		std::vector<Argument>({
			Argument({ true, "path", "File/Directory to remove." }),
			Argument({ false, "-r", "Force remove." })
		}),
		"Removes a file/directory.",
		std::function<void()>([]() {
			rm();
		})
	),
	Command(
		"copy",
		std::vector<Argument>({
			Argument({ true, "source_path", "Path of the source." }),
			Argument({ true, "destination_path", "Path of the destination." })
		}),
		"Copies a file/directory to a specified location.",
		std::function<void()>([]() {
			copy();
		})
	),
	Command(
		"echo",
		std::vector<Argument>({
			Argument({ false, "string", "String to echo" }),
			Argument({ true, "-o output_file", "File to echo string to." }),
			Argument({ true, "-t", "Truncate file." })
		}),
		"Copies a file/directory to a specified location.",
		std::function<void()>([]() {
			echo();
		})
	),
	Command(
		"pwd",
		std::vector<Argument>({ }),
		"Prints the working directory.",
		std::function<void()>([]() {
			pwd();
		})
	),
	Command(
		"cat",
		std::vector<Argument>({
			Argument({ true, "file", "File to catenate." })
		}),
		"Catenates the specified file. Prints its contents.",
		std::function<void()>([]() {
			cat();
		})
	)
}));

static void man() {
	command_handler.output.out += '\n';

	size_t arg = find_arg(command_handler.argv, "-c");
	if (arg == -1) {
		for (Command command : command_handler.commands) {
			command_handler.output.out += command.name + ": " + command.help_message + '\n';
		}
		command_handler.output.out += '\n';
	} else {
		for (Command command : command_handler.commands) {
			if (command.name == command_handler.argv[arg+1]) {
				command_handler.output.out += command.help_message + "\n\n";

				command_handler.output.out += to_upper(command.name);

				for (Argument argument : command.arguments) {
					command_handler.output.out += (argument.mandatory ? " {" : " [");
					command_handler.output.out += argument.name;
					command_handler.output.out += (argument.mandatory ? "}" : "]");

					if (argument.repeating) {
						command_handler.output.out += "...";
					}
				}
				command_handler.output.out += "\n\n";
				for (Argument argument : command.arguments) {
					command_handler.output.out += argument.name + ": " + argument.description + '\n';
				}

				command_handler.output.out += '\n';

				return;
			}
		}
		error<std::invalid_argument>("Command does not exist.");
	}
}

static void colortest() {
	if (find_arg(command_handler.argv, "-c") == -1) {
		for (int i = 0; i < 256; i++) {
			SetConsoleTextAttribute(console, i);
			std::cout << std::to_string(i) << ' ' << (find_arg(command_handler.argv, "-s") == -1 ? "Hello World!" : command_handler.argv[find_arg(command_handler.argv, "-s") + 1]) << std::endl;
		}
	} else {
		for (std::string color : std::vector<std::string>(command_handler.argv.begin() + 5, command_handler.argv.end())) {
			SetConsoleTextAttribute(console, std::stoi(color));
			std::cout << color << ' ' << (find_arg(command_handler.argv, "-s") == -1 ? "Hello World!" : command_handler.argv[find_arg(command_handler.argv, "-s") + 1]) << std::endl;
		}
	}
	SetConsoleTextAttribute(console, DEFAULT_TEXT_COLOR);
	command_handler.output.out += '\n';
}

static void cd() {
	if (fs::exists(command_handler.argv[FIRST_ARG])) {
		fs::current_path(command_handler.argv[FIRST_ARG]);
	} else {
		error<std::invalid_argument>(PATH_DOES_NOT_EXIST);
	}
}

static void exit() {
	int milliseconds	= find_arg(command_handler.argv, "-ms") != -1	? std::stoi(command_handler.argv[find_arg(command_handler.argv, "-ms")+1]) : 0;
	int seconds			= find_arg(command_handler.argv, "-s")	 != -1	? std::stoi(command_handler.argv[find_arg(command_handler.argv, "-s")+1])	 : 0;
	int minutes			= find_arg(command_handler.argv, "-m")	 != -1	? std::stoi(command_handler.argv[find_arg(command_handler.argv, "-m")+1])	 : 0;
	int hours			= find_arg(command_handler.argv, "-h")	 != -1	? std::stoi(command_handler.argv[find_arg(command_handler.argv, "-h")+1])	 : 0;

	std::cout << "Exiting in:\n\t"
		+ std::to_string(hours)			+ " hours\n\t"
		+ std::to_string(minutes)		+ " minutes\n\t"
		+ std::to_string(seconds)		+ " seconds\n\t"
		+ std::to_string(milliseconds)	+ " milliseconds";

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
	command_handler.output.out += std::string(fs::exists(command_handler.argv[FIRST_ARG]) ? "true" : "false") + '\n';
}

static void touch() {
	std::ofstream file(command_handler.argv[FIRST_ARG]);
	if (file.is_open()) {
		file << "";
		file.close();
	} else {
		error<std::invalid_argument>("Failed to open file.");
	}
}

static void date() {
	command_handler.output.out = std::format("{}", fs::last_write_time(command_handler.argv[FIRST_ARG])) + '\n';
}

static void open() {
	if (!fs::exists(command_handler.argv[FIRST_ARG])) {
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
		&words_to_string(command_handler.argv)[0],		    // Command line
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
		if (fs::exists(command_handler.argv[FIRST_ARG])) {
			fs::remove_all(command_handler.argv[FIRST_ARG]);
		}
	}
	fs::create_directory(command_handler.argv[FIRST_ARG]);

	if (find_arg(command_handler.argv, "-m") != -1) {
		fs::current_path(command_handler.argv[FIRST_ARG]);
	}
}

static void rm() {
	if (fs::exists(command_handler.argv[FIRST_ARG])) {
		if (!fs::equivalent(fs::current_path(), command_handler.argv[FIRST_ARG])) {
			if (find_arg(command_handler.argv, "-r") != -1) {
				fs::remove_all(command_handler.argv[FIRST_ARG]);
			} else {
				if (fs::is_directory(command_handler.argv[FIRST_ARG])) {
					if (fs::is_empty(command_handler.argv[FIRST_ARG])) {
						fs::remove(command_handler.argv[FIRST_ARG]);
					} else {
						error<std::invalid_argument>("Directory is not empty.", "Either add the -r modifer to force remove that directory or empty the directory.");
					}
				} else {
					fs::remove(command_handler.argv[FIRST_ARG]);
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
	if (fs::exists(command_handler.argv[FIRST_ARG])) {
		fs::copy(command_handler.argv[FIRST_ARG], command_handler.argv[ARG(1)]);
	} else {
		error<std::invalid_argument>(PATH_DOES_NOT_EXIST);
	}
}

static void echo() {
	if (int i = find_arg(command_handler.argv, "-o"); i != -1) {
		std::ofstream out;
		if (find_arg(command_handler.argv, "-t") != -1) {
			out.open(command_handler.argv[i+1], std::ios::trunc);
		} else {
			out.open(command_handler.argv[i+1], std::ios::app);
		}
		out << command_handler.argv[FIRST_ARG];
	} else {
		command_handler.output.out += command_handler.argv[FIRST_ARG] + '\n';
	}
}

static void pwd() {
	command_handler.output.out += fs::current_path().string() + '\n';
}

static void cat() {
	if (fs::exists(command_handler.argv[FIRST_ARG])) {
		std::ifstream in(command_handler.argv[FIRST_ARG]);
		std::string str;
		while (std::getline(in, str)) {
			command_handler.output.out += str + '\n';
		}
	}
}

