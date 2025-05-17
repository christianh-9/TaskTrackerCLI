#include <iostream>
#include <string>
#include "task.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage:\n";
        cout << "  task-cli add \"description\"\n";
        cout << "  task-cli update ID \"new description\"\n";
        cout << "  task-cli delete ID\n";
        cout << "  task-cli mark-in-progress ID\n";
        cout << "  task-cli mark-done ID\n";
        cout << "  task-cli list [status]\n";
        return 1;
    }

    string command = argv[1];

    if (command == "add") {
        if (argc < 3) {
            cout << "Error: Missing task description.\n";
            return 1;
        }
        addTask(argv[2]);
    }
    else if (command == "update") {
        if (argc < 4) {
            cout << "Error: Missing ID or new description.\n";
            return 1;
        }
        int id = stoi(argv[2]);
        string newDesc;
        for (int i = 3; i < argc; ++i) {
            newDesc += argv[i];
            if (i < argc - 1) newDesc += " ";
        }
        updateTask(id, newDesc);
    }

    else if (command == "delete") {
        if (argc < 3) {
            cout << "Error: Missing ID.\n";
            return 1;
        }
        int id = stoi(argv[2]);
        deleteTask(id);
    }
    else if (command == "mark-in-progress") {
        if (argc < 3) {
            cout << "Error: Missing ID.\n";
            return 1;
        }
        int id = stoi(argv[2]);
        markTask(id, "in-progress");
    }
    else if (command == "mark-done") {
        if (argc < 3) {
            cout << "Error: Missing ID.\n";
            return 1;
        }
        int id = stoi(argv[2]);
        markTask(id, "done");
    }
    else if (command == "list") {
        if (argc == 2) {
            listTasks(""); // list all
        } else {
            string status = argv[2];
            listTasks(status);
        }
    }
    else {
        cout << "Error: Unknown command '" << command << "'.\n";
    }

    return 0;
}