#include "UserInterface.h"

int main(int argc, char** argv) {
    UserInterface UI(argv[1]);
    UI.start();
    UI.interact();

    return 0;
}