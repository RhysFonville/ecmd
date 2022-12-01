#include <shlobj_core.h>
#include "CommandStorage.h"

#define INFO_BUFFER_SIZE 16

/*
* "ecmd" stands for "Epic Command". As in, "My Epic Command Prompt"
* Began as a linux like command prompt, it is now an API to make creating commands easy.
* 
* Made September 18, 2022, at 2:07 PM
*/

int main(int argc, char *argv[]) {
	TCHAR computer_name[INFO_BUFFER_SIZE+1] = {};
	TCHAR user_name[INFO_BUFFER_SIZE+1] = {};
	DWORD info_buffer_size = INFO_BUFFER_SIZE;

	PWSTR user_folder_path_wide;
	SHGetKnownFolderPath(FOLDERID_Profile, 0, NULL, &user_folder_path_wide);
	user_folder_path = wstring_to_string(user_folder_path_wide);

	fs::current_path(user_folder_path);

	if (!GetComputerNameW(computer_name, &info_buffer_size)) {
		CommandHandler::error_from_string("Failed to get the computer's name.\n");
		return 1;
	}

	info_buffer_size = INFO_BUFFER_SIZE;

	if (!GetUserNameW(user_name, &info_buffer_size)) {
		CommandHandler::error_from_string("Failed to get the user's name.\n");
		return 1;
	}

	std::cout.flush();
	std::wcout.flush();

	std::cout << "To get a command's manual, use the \"man\" command with the dashed modifier -c followed by the command's name." << std::endl;
	std::cout << "Ex: man -c ls\n\n";

	while (true) {
		std::string in;

		SetConsoleTextAttribute(console, 3);
		std::wcout << user_name << "@" << computer_name;
		SetConsoleTextAttribute(console, 2);

		if (std::string path = fs::current_path().string(); path != user_folder_path) {
			std::cout << " " << fs::current_path().string();
		} else {
			std::cout << " ~";
		}
		SetConsoleTextAttribute(console, 7);
		std::cout << " $ ";
		SetConsoleTextAttribute(console, DEFAULT_TEXT_COLOR);
		std::getline(std::cin, in);

		command_handler.process_command(in);
	}

	return 0;
}