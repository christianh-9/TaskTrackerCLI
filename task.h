#ifndef TASKTRACKER_TASK_H
#define TASKTRACKER_TASK_H
#include <string>

struct task {
    int ID;
    std::string description;
    std::string status;
    std::string createdAt;
    std::string updatedAt;
};

std::string getCurrentTime();
void addTask (const std::string& desc);
void updateTask(int ID, const std::string& desc);
void deleteTask(int ID);
void markTask(int ID, const std::string& status);
void listTasks(const std::string& status = "");

#endif //TASKTRACKER_TASK_H
