#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <functional>

/* This can obviously be made alot better but this is just a base i made cause im bored
   Contact Discord: perle.btr
*/

class Ecstasy {
public:
    Ecstasy(const std::string& title) : title(title) {}

    void addItem(const std::string& label, std::function<void()> action) {
        labels.push_back(label);
        actions.push_back(action);
    }

    void addItemWithToggle(const std::string& label, bool& toggle) {
        labels.push_back(label);
        toggleStates.push_back(&toggle);
        actions.push_back(ToggleAction(toggle));
    }

    void run() {
        int selectedItem = 0;

        while (true) {
            drawMenu(selectedItem);

            int key = _getch();
            if (key == 224) {
                key = _getch();

                if (key == 72) {
                    selectedItem = (selectedItem - 1 + labels.size()) % labels.size();
                }
                else if (key == 80) {
                    selectedItem = (selectedItem + 1) % labels.size();
                }
            }
            else if (key == 13) {
                if (selectedItem == labels.size() - 1) {
                    break;
                }
                else {
                    actions[selectedItem]();
                }
            }
        }
    }

private:
    void drawMenu(int selectedItem) {
        system("cls");

        std::cout << title << std::endl;
        for (size_t i = 0; i < labels.size(); ++i) {
            myGotoxy(10, 5 + i);
            if (i == selectedItem) {
                std::cout << ">> " << labels[i] << " << ";
            }
            else {
                std::cout << "   " << labels[i] << "   ";
            }
            std::cout << std::endl;
        }

        for (size_t i = 0; i < toggleStates.size(); ++i) {
            int onOffLength = (*toggleStates[i] ? 2 : 3);
            int toggleXOffset = 25 + onOffLength;

            myGotoxy(toggleXOffset, 7 + i);
            std::cout << "[" << (*toggleStates[i] ? "\x1B[32mON\x1B[0m" : "\x1B[31mOFF\x1B[0m") << "]";
            std::cout << std::endl;
        }
    }

    void myGotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

private:
    std::string title;
    std::vector<std::string> labels;
    std::vector<bool*> toggleStates;
    std::vector<std::function<void()>> actions;
};

void option1Function() {
    std::cout << "You chose Option 1." << std::endl;
}

void option2Function() {
    std::cout << "You chose Option 2." << std::endl;
}

void subOption1Function() {
    std::cout << "You chose Sub Option 1." << std::endl;
}

void subOption2Function() {
    std::cout << "You chose Sub Option 2." << std::endl;
}

void toggleBoolean(bool& boolean) {
    boolean = !boolean;
}

class ToggleAction {
public:
    ToggleAction(bool& toggle) : toggle(toggle) {}

    void operator()() {
        toggleBoolean(toggle);
    }

private:
    bool& toggle;
};

void exitFunction() {
    exit(0);
}

int main() {
    bool toggle1 = false;
    bool toggle2 = false;

    Ecstasy ecstasy("Ecstasy");
    ecstasy.addItem("Option 1", option1Function);
    ecstasy.addItem("Option 2", option2Function);
    ecstasy.addItemWithToggle("DoCleaning", toggle1);
    ecstasy.addItem("Sub Menu", [&]() {
        Ecstasy subEcstasy("Sub Menu");
        subEcstasy.addItem("Sub Option 1", subOption1Function);
        subEcstasy.addItem("Sub Option 2", subOption2Function);
        subEcstasy.run();
        });
    ecstasy.addItem("Exit", exitFunction);

    ecstasy.run();

    return 0;
}
