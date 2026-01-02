#include "lib.hpp"
#include <array>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using std::array;

Command::Command(const string &cmd) {
    this->command = cmd;
}

Command &Command::arg(const string arg) {
    this->arguments.push_back(arg);
    return *this;
}

Command &Command::args(const vector<string> args) {
    this->arguments.insert(this->arguments.end(), args.begin(), args.end());
    return *this;
}

Command &Command::run() {
    array<int, 2> stdout_pipe{}; // [0] = read, [1] = pipe
    array<int, 2> stderr_pipe{};

    if (pipe(stdout_pipe.data()) < 0 || pipe(stderr_pipe.data()) < 0) {
        perror("pipe");
        this->status = 303;
        return *this;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        this->status = 303;
        return *this;
    }

    if (pid == 0) {
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);

        dup2(stdout_pipe[1], STDOUT_FILENO);
        close(stdout_pipe[1]);

        dup2(stderr_pipe[1], STDERR_FILENO);
        close(stderr_pipe[1]);

        const char *name = this->command.c_str();
        vector<string> args = this->arguments;

        std::vector<char *> exec_args;
        exec_args.push_back(const_cast<char *>(name));
        for (auto &arg : args) {
            exec_args.push_back(&arg[0]);
        }
        exec_args.push_back(nullptr);

        execvp(exec_args[0], exec_args.data());

        this->status = 303;
    } else {
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        constexpr size_t buffsize = 4096;
        array<char, buffsize> buffer;
        size_t bytes;

        while ((bytes = read(
                    stdout_pipe[0], buffer.data(), buffer.size() - 1)) > 0) {
            this->output.append(buffer.data(), bytes);
        }
        close(stdout_pipe[0]);

        while ((bytes = read(
                    stderr_pipe[0], buffer.data(), buffer.size() - 1)) > 0) {
            this->error.append(buffer.data(), bytes);
        }
        close(stderr_pipe[0]);

        wait(nullptr);
    }
    return *this;
}

string Command::stdout() {
    return this->output;
}

string Command::stderr() {
    return this->error;
}
