#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <thread>
#include <mutex>
#include <sstream>
#include <iomanip>

std::mutex print_mutex;

std::string operation(const std::string& arithmetic, int idx, bool use_mutex) {
    float a[2];
    char op;
    int index = 0;
    float result = 0;
    int number = 0;
    std::stringstream ss;

    for (int i = 0; i < arithmetic.size(); i++) {
        if (isdigit(arithmetic[i])) {
            number = number * 10 + (arithmetic[i] - '0');
        } else {
            op = arithmetic[i];
            a[index] = number;
            number = 0;
            index++;
        }
    }

    a[index] = number;

    ss << "Task " << idx + 1 << "\n";

    if (op == '*') {
        for (int i = 0; i < a[1]; i++) {
            result += a[0];
            ss << "Step " << i + 1 << ": " << result << "\n";
        }
        ss << "Final Result: " << a[0] << " * " << a[1] << " = " << result << "\n";
    } else if (op == '+') {
        result = a[0] + a[1];
        ss << a[0] << " + " << a[1] << " = " << result << "\n";
    } else if (op == '/') {
        if (a[1] != 0) {
            for (int i = 0; i < a[0]; i++) {
                result += 1 / a[1];
                ss << "Step " << i + 1 << ": " << result << "\n";
            }
            ss << "Final Result: " << a[0] << " / " << a[1] << " = " << result << "\n";
        } else {
            ss << "Error: Division by zero\n";
        }
    } else if (op == '-') {
        result = a[0] - a[1];
        ss << a[0] << " - " << a[1] << " = " << result << "\n";
    }

    return ss.str();
}

void concurrency(const std::vector<std::string>& arithmetic, int num_arth) {
    std::cout << "\n--- Concurrent Execution Output ---\n";
    for (int i = 0; i < num_arth; i++) {
        int pid = fork();
        if (pid == -1) {
            std::cout << "Error Forking\n";
            exit(-1);
        } else if (pid == 0) {
            std::string result = operation(arithmetic[i], i, false);
            std::cout << result;
            exit(0);
        }
    }

    for (int i = 0; i < num_arth; i++) {
        wait(NULL);
    }
}

void parallelism(const std::vector<std::string>& arithmetic, int num_arth) {
    std::vector<std::thread> threads;
    std::vector<std::string> outputs(num_arth);

    for (int i = 0; i < num_arth; i++) {
        threads.emplace_back([i, &arithmetic, &outputs]() {
            std::string result = operation(arithmetic[i], i, true);
            std::lock_guard<std::mutex> lock(print_mutex);
            outputs[i] = result;
        });
    }

    for (auto& t : threads) t.join();

    std::cout << "\n--- Parallel Execution Output ---\n";

    for (int i = 0; i < num_arth; i++) {
        std::cout << "Task " << i + 1 << "\t\t";
    }
    std::cout << "\n------------------------------------------------------------\n";

    std::vector<std::vector<std::string>> lines(num_arth);
    for (int i = 0; i < num_arth; i++) {
        std::istringstream iss(outputs[i]);
        std::string line;
        while (std::getline(iss, line)) {
            lines[i].push_back(line);
        }
    }

    int max_lines = 0;
    for (auto& l : lines) {
        if (l.size() > max_lines) max_lines = l.size();
    }

    for (int row = 0; row < max_lines; row++) {
        for (int col = 0; col < num_arth; col++) {
            if (row < lines[col].size()) {
                std::cout << std::left << std::setw(24) << lines[col][row];
            } else {
                std::cout << std::setw(24) << " ";
            }
        }
        std::cout << "\n";
    }
}

int main() {
    int num_arth;
    std::cout << "Enter number of expressions: ";
    std::cin >> num_arth;

    std::vector<std::string> array_of_arithmetic(num_arth);
    for (int i = 0; i < num_arth; i++) {
        std::cout << "Enter expression #" << i + 1 << " (ex, 3*5): ";
        std::cin >> array_of_arithmetic[i];
    }

    concurrency(array_of_arithmetic, num_arth);
    parallelism(array_of_arithmetic, num_arth);

    return 0;
}



