#include <ncurses.h>
#include <string>
#include <vector>

// Function prototypes
void show_menu(const std::vector<std::string> &options);
void show_submenu();

int main() {
    initscr();
    start_color();  // Start color functionality
    init_pair(1, COLOR_CYAN, COLOR_BLACK);  // Initialize color pair
    
    raw();
    keypad(stdscr, TRUE);
    noecho();
    
    // Menu options
    std::vector<std::string> options = {
        "Option 1",
        "Option 2",
        "Option 3",
        "Quit"
    };
    
    int choice;
    while (true) {
        show_menu(options);
        choice = getch();
        
        switch (choice) {
            case '1':
                show_submenu();
                break;
            case '2':
                attron(COLOR_PAIR(1));  // Set text color
                mvprintw(6, 0, "You selected Option 2");
                attroff(COLOR_PAIR(1)); // Reset text color
                getch();
                break;
            case '3':
                mvprintw(6, 0, "You selected Option 3");
                getch();
                break;
            case '4':
                mvprintw(6, 0, "Exiting...");
                getch();
                endwin();
                return 0;
        }
    }
}

void show_menu(const std::vector<std::string> &options) {
    clear();
    mvprintw(0, 0, "Main Menu:");
    
    for (size_t i = 0; i < options.size(); ++i) {
        mvprintw(2 + i, 1, ("[" + std::to_string(i + 1) + "] " + options[i]).c_str());
    }
    
    mvprintw(6, 0, "Enter your choice: ");
}

void show_submenu() {
    clear();
    mvprintw(0, 0, "Sub Menu:");
    mvprintw(2, 1, "[1] Sub Option 1");
    mvprintw(3, 1, "[2] Sub Option 2");
    mvprintw(4, 1, "[3] Go back");
    
    mvprintw(6, 0, "Enter your choice: ");
    
    int choice;
    choice = getch();
    
    switch (choice) {
        case '1':
            mvprintw(8, 0, "You selected Sub Option 1");
            getch();
            break;
        case '2':
            mvprintw(8, 0, "You selected Sub Option 2");
            getch();
            break;
        case '3':
            // Going back to main menu
            break;
    }
}
