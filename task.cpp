#include "task.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

vector<task> tasks;

string getCurrentTime() {
    time_t now = time(0);
    tm* timeInfo = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    return string(buffer);
}

string escapeString(const string& input) {
    string result;
    for (char c : input) {
        if (c == '"') result += "\\\"";
        else result += c;
    }
    return result;
}

void loadTasks() {
    tasks.clear();
    ifstream file("tasks.json");
    if (!file.is_open()) {
        cout << "No task file found. Starting with an empty task list.\n";
        return;
    }

    string line, jsonBlock;
    while (getline(file, line)) {
        jsonBlock += line;
    }

    size_t pos = 0;
    while ((pos = jsonBlock.find('{', pos)) != string::npos) {
        size_t end = jsonBlock.find('}', pos);
        if (end == string::npos) break;

        string obj = jsonBlock.substr(pos + 1, end - pos - 1);
        task t;

        size_t idPos = obj.find("\"id\":");
        size_t descPos = obj.find("\"description\":");
        size_t statusPos = obj.find("\"status\":");
        size_t createdPos = obj.find("\"createdAt\":");
        size_t updatedPos = obj.find("\"updatedAt\":");

        if (idPos != string::npos)
            t.ID = stoi(obj.substr(idPos + 5, obj.find(',', idPos) - idPos - 5));

        if (descPos != string::npos) {
            size_t start = obj.find('"', descPos + 14) + 1;
            size_t end = obj.find('"', start);
            t.description = obj.substr(start, end - start);
        }

        if (statusPos != string::npos) {
            size_t start = obj.find('"', statusPos + 9) + 1;
            size_t end = obj.find('"', start);
            t.status = obj.substr(start, end - start);
        }

        if (createdPos != string::npos) {
            size_t start = obj.find('"', createdPos + 12) + 1;
            size_t end = obj.find('"', start);
            t.createdAt = obj.substr(start, end - start);
        }

        if (updatedPos != string::npos) {
            size_t start = obj.find('"', updatedPos + 12) + 1;
            size_t end = obj.find('"', start);
            t.updatedAt = obj.substr(start, end - start);
        }

        tasks.push_back(t);
        pos = end + 1;
    }

    file.close();
}

void saveTasks() {
    ofstream file("tasks.json");
    if (!file.is_open()) {
        cout << "Unable to open file for writing." << endl;
        return;
    }

    file << "[\n";
    for (size_t i = 0; i < tasks.size(); ++i) {
        const auto& t = tasks[i];
        file << "  {\n";
        file << "    \"id\": " << t.ID << ",\n";
        file << "    \"description\": \"" << escapeString(t.description) << "\",\n";
        file << "    \"status\": \"" << t.status << "\",\n";
        file << "    \"createdAt\": \"" << t.createdAt << "\",\n";
        file << "    \"updatedAt\": \"" << t.updatedAt << "\"\n";
        file << "  }";
        if (i < tasks.size() - 1) file << ",";
        file << "\n";
    }
    file << "]\n";

    file.close();
}

void addTask(const string& desc) {
    loadTasks();
    int newId = tasks.empty() ? 1 : tasks.back().ID + 1;
    string now = getCurrentTime();

    task newTask = { newId, desc, "todo", now, now };
    tasks.push_back(newTask);
    saveTasks();

    cout << "Successfully added task (ID: " << newId << ")\n";
}

void updateTask(int ID, const string& desc) {
    loadTasks();
    for (auto& t : tasks) {
        if (t.ID == ID) {
            t.description = desc;
            t.updatedAt = getCurrentTime();
            saveTasks();
            cout << "Task updated.\n";
            return;
        }
    }
    cout << "Task not found.\n";
}

void deleteTask(int ID) {
    loadTasks();
    auto it = remove_if(tasks.begin(), tasks.end(), [ID](const task& t) {
        return t.ID == ID;
    });

    if (it != tasks.end()) {
        tasks.erase(it, tasks.end());
        saveTasks();
        cout << "Task deleted.\n";
    } else {
        cout << "Task not found.\n";
    }
}

void markTask(int ID, const string& status) {
    loadTasks();
    for (auto& t : tasks) {
        if (t.ID == ID) {
            t.status = status;
            t.updatedAt = getCurrentTime();
            saveTasks();
            cout << "Task marked as " << status << ".\n";
            return;
        }
    }
    cout << "Task not found.\n";
}

void listTasks(const string& status) {
    loadTasks();
    bool found = false;
    for (const auto& t : tasks) {
        if (status.empty() || t.status == status) {
            cout << "ID: " << t.ID << '\n'
                 << "Description: " << t.description << '\n'
                 << "Status: " << t.status << '\n'
                 << "Created at: " << t.createdAt << '\n'
                 << "Updated at: " << t.updatedAt << '\n'
                 << "------------------------------\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No tasks found";
        if (!status.empty()) cout << " with status '" << status << "'";
        cout << ".\n";
    }
}
