#include <cstdint>
#include <string>
#include <vector>

using std::string, std::vector;

class Command {
private:
    string command;
    vector<string> arguments;
    int16_t status = 0;
    string input;  // stdin
    string output; // stdout
    string error;  // stderr

public:
    Command(const string &);
    Command &arg(const string);
    Command &args(const vector<string>);
    Command &stdin(const string);
    Command &run();
    string stdout();
    string stderr();
};
