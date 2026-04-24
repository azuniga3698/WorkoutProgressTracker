#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct WorkoutEntry {
    string date;
    string exercise;
    int sets;
    int reps;
    double weight;
};

string toLowerCase(string text) {
    for (char& c : text) {
        c = static_cast<char>(tolower(c));
    }
    return text;
}

string trim(const string& text) {
    size_t start = text.find_first_not_of(" \t\n\r");
    size_t end = text.find_last_not_of(" \t\n\r");

    if (start == string::npos || end == string::npos) {
        return "";
    }

    return text.substr(start, end - start + 1);
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getIntInput(const string& prompt, int minimumValue) {
    int value;

    while (true) {
        cout << prompt;
        cin >> value;

        if (!cin.fail() && value >= minimumValue) {
            clearInput();
            return value;
        }

        cout << "Invalid input. Please enter a number greater than or equal to " << minimumValue << ".\n";
        clearInput();
    }
}

double getDoubleInput(const string& prompt, double minimumValue) {
    double value;

    while (true) {
        cout << prompt;
        cin >> value;

        if (!cin.fail() && value >= minimumValue) {
            clearInput();
            return value;
        }

        cout << "Invalid input. Please enter a number greater than or equal to " << minimumValue << ".\n";
        clearInput();
    }
}

string getLineInput(const string& prompt) {
    string value;

    while (true) {
        cout << prompt;
        getline(cin, value);
        value = trim(value);

        if (!value.empty()) {
            return value;
        }

        cout << "Input cannot be empty. Please try again.\n";
    }
}

double calculateVolume(const WorkoutEntry& entry) {
    return entry.sets * entry.reps * entry.weight;
}

void printHeader() {
    cout << left
         << setw(14) << "Date"
         << setw(25) << "Exercise"
         << setw(8) << "Sets"
         << setw(8) << "Reps"
         << setw(10) << "Weight"
         << setw(12) << "Volume"
         << "\n";

    cout << string(77, '-') << "\n";
}

void printWorkout(const WorkoutEntry& entry) {
    cout << left
         << setw(14) << entry.date
         << setw(25) << entry.exercise
         << setw(8) << entry.sets
         << setw(8) << entry.reps
         << setw(10) << fixed << setprecision(1) << entry.weight
         << setw(12) << fixed << setprecision(1) << calculateVolume(entry)
         << "\n";
}

void addWorkout(vector<WorkoutEntry>& workouts) {
    WorkoutEntry entry;

    cout << "\n--- Add Workout ---\n";
    entry.date = getLineInput("Enter date, example 2026-04-24: ");
    entry.exercise = getLineInput("Enter exercise name: ");
    entry.sets = getIntInput("Enter number of sets: ", 1);
    entry.reps = getIntInput("Enter number of reps: ", 1);
    entry.weight = getDoubleInput("Enter weight used: ", 0.0);

    workouts.push_back(entry);

    cout << "Workout added successfully.\n";
}

void viewAllWorkouts(const vector<WorkoutEntry>& workouts) {
    cout << "\n--- All Workouts ---\n";

    if (workouts.empty()) {
        cout << "No workouts have been added yet.\n";
        return;
    }

    printHeader();

    for (const WorkoutEntry& entry : workouts) {
        printWorkout(entry);
    }
}

void searchByExercise(const vector<WorkoutEntry>& workouts) {
    cout << "\n--- Search By Exercise ---\n";

    if (workouts.empty()) {
        cout << "No workouts available to search.\n";
        return;
    }

    string searchTerm = getLineInput("Enter exercise name to search for: ");
    string lowerSearchTerm = toLowerCase(searchTerm);
    bool found = false;

    printHeader();

    for (const WorkoutEntry& entry : workouts) {
        string lowerExercise = toLowerCase(entry.exercise);

        if (lowerExercise.find(lowerSearchTerm) != string::npos) {
            printWorkout(entry);
            found = true;
        }
    }

    if (!found) {
        cout << "No matching exercises found.\n";
    }
}

void showHighestWeight(const vector<WorkoutEntry>& workouts) {
    cout << "\n--- Highest Weight Lifted ---\n";

    if (workouts.empty()) {
        cout << "No workouts available.\n";
        return;
    }

    WorkoutEntry best = workouts[0];

    for (const WorkoutEntry& entry : workouts) {
        if (entry.weight > best.weight) {
            best = entry;
        }
    }

    printHeader();
    printWorkout(best);
}

void showTotalVolume(const vector<WorkoutEntry>& workouts) {
    cout << "\n--- Total Training Volume ---\n";

    if (workouts.empty()) {
        cout << "No workouts available.\n";
        return;
    }

    double totalVolume = 0.0;

    for (const WorkoutEntry& entry : workouts) {
        totalVolume += calculateVolume(entry);
    }

    cout << "Total volume for all workouts: " << fixed << setprecision(1) << totalVolume << " pounds\n";
}

void sortByExercise(vector<WorkoutEntry>& workouts) {
    cout << "\n--- Sort By Exercise ---\n";

    if (workouts.empty()) {
        cout << "No workouts available to sort.\n";
        return;
    }

    sort(workouts.begin(), workouts.end(), [](const WorkoutEntry& a, const WorkoutEntry& b) {
        return toLowerCase(a.exercise) < toLowerCase(b.exercise);
    });

    cout << "Workouts sorted by exercise name.\n";
    viewAllWorkouts(workouts);
}

void sortByVolume(vector<WorkoutEntry>& workouts) {
    cout << "\n--- Sort By Volume ---\n";

    if (workouts.empty()) {
        cout << "No workouts available to sort.\n";
        return;
    }

    sort(workouts.begin(), workouts.end(), [](const WorkoutEntry& a, const WorkoutEntry& b) {
        return calculateVolume(a) > calculateVolume(b);
    });

    cout << "Workouts sorted from highest volume to lowest volume.\n";
    viewAllWorkouts(workouts);
}

void deleteWorkout(vector<WorkoutEntry>& workouts) {
    cout << "\n--- Delete Workout ---\n";

    if (workouts.empty()) {
        cout << "No workouts available to delete.\n";
        return;
    }

    for (size_t i = 0; i < workouts.size(); i++) {
        cout << i + 1 << ". " << workouts[i].date << " - " << workouts[i].exercise << "\n";
    }

    int choice = getIntInput("Enter workout number to delete: ", 1);

    if (choice > static_cast<int>(workouts.size())) {
        cout << "That workout number does not exist.\n";
        return;
    }

    workouts.erase(workouts.begin() + choice - 1);
    cout << "Workout deleted successfully.\n";
}

void saveToFile(const vector<WorkoutEntry>& workouts, const string& filename) {
    ofstream outFile(filename);

    if (!outFile) {
        cout << "Error: could not open file for saving.\n";
        return;
    }

    for (const WorkoutEntry& entry : workouts) {
        outFile << entry.date << "|"
                << entry.exercise << "|"
                << entry.sets << "|"
                << entry.reps << "|"
                << entry.weight << "\n";
    }

    outFile.close();
    cout << "Workout data saved to " << filename << ".\n";
}

vector<string> splitLine(const string& line, char delimiter) {
    vector<string> parts;
    stringstream ss(line);
    string item;

    while (getline(ss, item, delimiter)) {
        parts.push_back(item);
    }

    return parts;
}

void loadFromFile(vector<WorkoutEntry>& workouts, const string& filename) {
    ifstream inFile(filename);

    if (!inFile) {
        cout << "No saved file found yet. Starting with empty workout list.\n";
        return;
    }

    workouts.clear();
    string line;

    while (getline(inFile, line)) {
        vector<string> parts = splitLine(line, '|');

        if (parts.size() == 5) {
            WorkoutEntry entry;
            entry.date = parts[0];
            entry.exercise = parts[1];
            entry.sets = stoi(parts[2]);
            entry.reps = stoi(parts[3]);
            entry.weight = stod(parts[4]);
            workouts.push_back(entry);
        }
    }

    inFile.close();
    cout << "Workout data loaded from " << filename << ".\n";
}

void addSampleData(vector<WorkoutEntry>& workouts) {
    workouts.push_back({"2026-04-01", "Bench Press", 3, 8, 225});
    workouts.push_back({"2026-04-02", "Lat Pulldown", 4, 10, 180});
    workouts.push_back({"2026-04-03", "Leg Press", 4, 12, 540});
    workouts.push_back({"2026-04-04", "Dumbbell Shoulder Press", 3, 10, 65});
    workouts.push_back({"2026-04-05", "Barbell Row", 4, 8, 225});
    workouts.push_back({"2026-04-06", "Hack Squat", 3, 10, 180});
    workouts.push_back({"2026-04-07", "Cable Tricep Pushdown", 3, 12, 85});
    workouts.push_back({"2026-04-08", "Dumbbell Curl", 3, 10, 50});
    workouts.push_back({"2026-04-09", "RDL", 3, 8, 185});
    workouts.push_back({"2026-04-10", "Seated Row", 4, 10, 220});
}

void showMenu() {
    cout << "\n====================================\n";
    cout << "     Workout Progress Tracker\n";
    cout << "====================================\n";
    cout << "1. Add workout\n";
    cout << "2. View all workouts\n";
    cout << "3. Search by exercise\n";
    cout << "4. Show highest weight lifted\n";
    cout << "5. Show total training volume\n";
    cout << "6. Sort workouts by exercise\n";
    cout << "7. Sort workouts by volume\n";
    cout << "8. Delete workout\n";
    cout << "9. Save workouts to file\n";
    cout << "10. Load workouts from file\n";
    cout << "11. Add sample data\n";
    cout << "12. Exit\n";
    cout << "====================================\n";
}

int main() {
    vector<WorkoutEntry> workouts;
    const string filename = "workouts.txt";
    bool running = true;

    cout << "Welcome to the Workout Progress Tracker.\n";

    while (running) {
        showMenu();
        int choice = getIntInput("Choose an option: ", 1);

        switch (choice) {
            case 1:
                addWorkout(workouts);
                break;
            case 2:
                viewAllWorkouts(workouts);
                break;
            case 3:
                searchByExercise(workouts);
                break;
            case 4:
                showHighestWeight(workouts);
                break;
            case 5:
                showTotalVolume(workouts);
                break;
            case 6:
                sortByExercise(workouts);
                break;
            case 7:
                sortByVolume(workouts);
                break;
            case 8:
                deleteWorkout(workouts);
                break;
            case 9:
                saveToFile(workouts, filename);
                break;
            case 10:
                loadFromFile(workouts, filename);
                break;
            case 11:
                addSampleData(workouts);
                cout << "Sample data added.\n";
                break;
            case 12:
                running = false;
                cout << "Goodbye. Keep training hard.\n";
                break;
            default:
                cout << "Invalid option. Please choose between 1 and 12.\n";
                break;
        }
    }

    return 0;
}
