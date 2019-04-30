#include "sim_utilities.hpp"

namespace SimUtilities {
    void drawMap(WINDOW *window, const int mapOffsetY, const int mapOffsetX, bool has_border) {
        // Clear the window
        wclear(window);
        // Redraw the border if present
        if (has_border) {
            wattron(window, COLOR_PAIR(1));
            box(window, 0, 0);
            wattroff(window, COLOR_PAIR(1));
        }

        // Cursor location entered in the form row, column (y, x)
        // Draw terrain
        for (auto &pointCharPair: MapManager::terrain) {
            switch (pointCharPair.second) {
                case '~' :
                    // Water
                    wattron(window, COLOR_PAIR(2));
                    mvwaddch(window, pointCharPair.first.second + mapOffsetY, pointCharPair.first.first + mapOffsetX,
                             '~');
                    wattroff(window, COLOR_PAIR(2));
                    break;
                case '#':
                    // Obstacle
                    wattron(window, COLOR_PAIR(3));
                    mvwaddch(window, pointCharPair.first.second + mapOffsetY, pointCharPair.first.first + mapOffsetX,
                             '#');
                    wattroff(window, COLOR_PAIR(3));
                    break;
                default:
                    break;
            }
        }

        // Draw plants and animals
        for (auto &element: MapManager::floraFauna) {
            wattron(window, COLOR_PAIR(element.second->getColorPair()));
            mvwaddch(window, element.first.second + mapOffsetY, element.first.first + mapOffsetX,
                     element.second->getCharID());
            wattroff(window, COLOR_PAIR(element.second->getColorPair()));
        }

        wmove(window, 1, 1);
        wrefresh(window);
    }

    WINDOW *createWindow(int height, int width, int startY, int startX, bool addBorders) {
        auto local_win = newwin(height, width, startY, startX);
        // Add border around the window
        if (addBorders) {
            wattron(local_win, COLOR_PAIR(1));
            box(local_win, 0, 0);
            wattroff(local_win, COLOR_PAIR(1));
        }
        // Show the border
        wrefresh(local_win);
        return local_win;
    }

    void destroyWindow(WINDOW *local_win) {
        // Erase the window borders
        wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        wrefresh(local_win);
        // Remove the window
        delwin(local_win);
    }

    void printStringToWindow(WINDOW *window, const char *printString, bool has_border) {
        int x, y;
        getyx(window, y, x);

        if (y == 0) {
            ++y;
        }

        int maxY = getmaxy(window);

        if (y >= maxY - 1) {
            wclear(window);
            wmove(window, 1, 1);
            getyx(window, y, x);

            if (has_border) {
                wattron(window, COLOR_PAIR(1));
                box(window, 0, 0);
                wattroff(window, COLOR_PAIR(1));
            }
        }

        mvwprintw(window, y, 1, "%s", printString);
        wrefresh(window);
    }

    int windowPromptInt(WINDOW *window, const char *promptString, int bufferSize) {
        int userInputNum = 0;
        bool shouldRetry;
        char input_str[bufferSize];
        do {
            printStringToWindow(window, promptString, true);
            wgetstr(window, input_str);
            try {
                userInputNum = stoi(input_str);
                shouldRetry = false;
            } catch (invalid_argument &invArg) {
                shouldRetry = true;
                printStringToWindow(window, "Please enter a valid numeric value\n", true);
            }
        } while (shouldRetry);

        return userInputNum;
    }

    string windowPromptStr(WINDOW *window, const char *promptString, vector<string> &allowedValues, int bufferSize) {
        string userInputStr;
        bool shouldRetry;
        char input_str[bufferSize];
        do {
            printStringToWindow(window, promptString, true);
            wgetstr(window, input_str);
            userInputStr = input_str;
            transform(userInputStr.begin(), userInputStr.end(), userInputStr.begin(), ::tolower);
            if (userInputStr.empty() ||
                find(allowedValues.begin(), allowedValues.end(), userInputStr) == allowedValues.end()) {
                shouldRetry = true;
                printStringToWindow(window, "Please enter a valid option\n", true);
            } else {
                shouldRetry = false;
            }
        } while (shouldRetry);

        return userInputStr;
    }

    std::vector<Point> random_select(const std::vector<Point> &locations, size_t count) {
        vector<Point> selection;
        std::sample(locations.begin(), locations.end(), std::back_inserter(selection),
                    count, std::mt19937{std::random_device{}()});
        return selection;
    }
}
