#include <windows.h>
#include "TextEditor.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    TextEditor editor;
    editor.run();
    return 0;
}
