#include <repl/repl.h>

int main() {
    try {
        REPL::Start();
    } catch (...) {
        return -1;
    }
}
