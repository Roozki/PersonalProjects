#include <ros/ros.h>
#include <std_msgs/String.h>
#include <ncurses.h>
#include <string>
#include <vector>

// Function prototypes
void show_menu(const std::vector<std::string> &options);

int main(int argc, char **argv) {
    ros::init(argc, argv, "ros_ncurses_example");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::String>("menu_selection", 1000);

    initscr();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    
    mousemask(ALL_MOUSE_EVENTS, NULL);  // Enable mouse events
    
    raw();
    keypad(stdscr, TRUE);
    noecho();
    
    std::vector<std::string> options = {
        "Publish Hello",
        "Publish World",
        "Quit"
    };
    
    MEVENT event;
    int ch;
    
    while (ros::ok()) {
        show_menu(options);
        ch = getch();
        
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON1_CLICKED) {
                    // Process mouse click at event.y, event.x
                    if (event.y == 2) ch = '1';
                    else if (event.y == 3) ch = '2';
                    else if (event.y == 4) ch = '3';
                }
            }
        }

        std_msgs::String msg;
        
        switch (ch) {
            case '1':
                msg.data = "Hello";
                pub.publish(msg);
                break;
            case '2':
                msg.data = "World";
                pub.publish(msg);
                break;
            case '3':
                endwin();
                return 0;
        }

        ros::spinOnce();
    }
    
    endwin();
    return 0;
}

void show_menu(const std::vector<std::string> &options) {
    clear();
    mvprintw(0, 0, "ROS Menu:");
    
    for (size_t i = 0; i < options.size(); ++i) {
        mvprintw(2 + i, 1, options[i].c_str());
    }
    
    mvprintw(6, 0, "Click an option or press 1, 2, or 3: ");
}
