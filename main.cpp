#include <iostream>
#include <windows.h>
#include "screen.h"
#include "ui.h"

void main_ui(Screen* screen, const std::string& file_path) {
    UI ui(screen);
    std::string command;
    std::ifstream f(file_path);

    if (!f.is_open()) {
        ui.render_log(ColorText("! Unable to load project file; quitting", "\x1b[31;40m").output());
        ui.update();
        exit(1);
    } // check file existence

    std::string raw = std::string(std::istreambuf_iterator<char>(f),
                                      std::istreambuf_iterator<char>());
    f.close();
    Project p;
    try {
        p = parse(ini_decode(raw));
    }
    catch(const std::exception& e) {
        ui.render_log(ColorText("! Error while parsing: " + std::string(e.what()), "\x1b[31;40m").output());
        ui.update(); 
        exit(2);
    } // try parsing project
    // init variables

    ui.render_log(ColorText("Press any key to enter command...", "\x1b[33;40m").output());
    ui.update(); // to beautify

    // command system
    int g;
    while ((g = _getch())) {
        if (g == '\b') {
            if (command.size() != 0) {
                command = command.substr(0, command.size() - 1);
            }
        } else if (g == '\x1b') {
            command = "";
        } else if (g == '\t') {
            // do nothing
        } else if (g == '\r') {
            // parsing
        } else {
            if (command.size() <= screen->size().x - 2)
                command += g;
        }
        ui.render_log(ColorText("> " + command, "").output());
        ui.update();
    }
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hStd, &dwMode);
    dwMode |= 0x4;
    SetConsoleMode(hStd, dwMode);
#endif // enable virtual terminal

    if (argc == 1) {
        std::cout << "UCl4 is designed to work with UTAU. Please run from the host software." << std::endl;
        std::getchar();
        exit(1);
    } else if (argc != 2) {
        std::cout << "Incorrect number of parameters; quitting" << std::endl;
        exit(1);
    } // param check

    const std::string build_string( "~ UCl4 ver1 build " + std::string(__DATE__) + std::string(__TIME__) + " ~");
    system(std::string("title " + build_string).c_str());
    // set build string

    Screen screen(getsize());
    // init screen for main_ui()

    main_ui(&screen, std::string(argv[1]));
    std::getchar();

    return 0;
}