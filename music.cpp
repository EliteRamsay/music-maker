#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <windows.h>
#include <ctime>
#include <cstdlib>

struct Note {
    std::string name;
    int freq;
    int duration;
};

std::vector<Note> notes = {
    {"C", 261, 300}, {"D", 294, 300}, {"E", 329, 300}, {"F", 349, 300},
    {"G", 392, 300}, {"A", 440, 300}, {"B", 493, 300}, {"C5", 523, 300}
};

std::vector<Note> song;

void showMenu() {
    std::cout << "\n==== C++ Terminal Music Composer ====\n";
    std::cout << "1. Add note to song\n";
    std::cout << "2. Play song\n";
    std::cout << "3. Save song\n";
    std::cout << "4. Load song\n";
    std::cout << "5. Generate random song\n";
    std::cout << "6. Exit\n";
    std::cout << "7. Play 'Happy Birthday'\n";
    std::cout << "-------------------------------------\n";
    std::cout << "Choice: ";
}

void listNotes() {
    std::cout << "\nAvailable notes:\n";
    for (size_t i = 0; i < notes.size(); ++i) {
        std::cout << i + 1 << ". " << notes[i].name << " (" << notes[i].freq << " Hz)\n";
    }
}

void addNote() {
    listNotes();
    std::cout << "\nChoose a note number: ";
    int choice;
    std::cin >> choice;

    if (choice < 1 || choice > (int)notes.size()) {
        std::cout << "Invalid choice.\n";
        return;
    }

    int duration;
    std::cout << "Enter duration in milliseconds (e.g., 300): ";
    std::cin >> duration;

    Note n = notes[choice - 1];
    n.duration = duration;
    song.push_back(n);
    std::cout << "Added " << n.name << " (" << n.freq << " Hz, " << n.duration << " ms)\n";
}

void playSong() {
    if (song.empty()) {
        std::cout << "No notes to play!\n";
        return;
    }

    std::cout << "\nPlaying song...\n";
    for (auto& n : song) {
        std::cout << n.name << " ";
        Beep(n.freq, n.duration);
        Sleep(50);
    }
    std::cout << "\nDone!\n";
}

void saveSong() {
    std::ofstream file("song.txt");
    if (!file) {
        std::cout << "Error saving song.\n";
        return;
    }
    for (auto& n : song) {
        file << n.name << " " << n.freq << " " << n.duration << "\n";
    }
    std::cout << "Song saved to song.txt\n";
}

void loadSong() {
    std::ifstream file("song.txt");
    if (!file) {
        std::cout << "Error loading song.\n";
        return;
    }
    song.clear();
    Note n;
    while (file >> n.name >> n.freq >> n.duration) {
        song.push_back(n);
    }
    std::cout << "Song loaded! " << song.size() << " notes.\n";
}

void generateRandomSong() {
    int count;
    std::cout << "Enter number of notes to generate (max 500): ";
    std::cin >> count;

    if (count < 1 || count > 500) {
        std::cout << "Invalid number.\n";
        return;
    }

    song.clear();
    srand((unsigned)time(nullptr));

    // Duration patterns in milliseconds — short, medium, long
    int durations[] = {200, 300, 400, 500, 600, 750};

    for (int i = 0; i < count; ++i) {
        // Occasionally add a rest (0 frequency)
        bool isRest = (rand() % 10 == 0); // 10% chance

        Note n;
        if (isRest) {
            n = {"Rest", 0, durations[rand() % 6]};
        } else {
            n = notes[rand() % notes.size()];
            n.duration = durations[rand() % 6];
        }
        song.push_back(n);
    }

    std::cout << "Random song generated with " << count << " notes!\n\n";

    std::cout << "\n";
}


void playHappyBirthday() {
    std::cout << "\n Playing 'Happy Birthday' \n";

    struct Tune { int freq; int duration; };
    Tune melody[] = {
        {264, 250}, {264, 250}, {297, 500}, {264, 500}, {352, 500}, {330, 750},
        {264, 250}, {264, 250}, {297, 500}, {264, 500}, {396, 500}, {352, 750},
        {264, 250}, {264, 250}, {528, 500}, {440, 500}, {352, 500}, {330, 500}, {297, 750},
        {466, 250}, {466, 250}, {440, 500}, {352, 500}, {396, 500}, {352, 750}
    };

    for (auto &n : melody) {
        Beep(n.freq, n.duration);
        Sleep(50);
    }

    std::cout << "Done! \n";
}

int main() {
    int choice;
    do {
        showMenu();
        std::cin >> choice;

        switch (choice) {
        case 1: addNote(); break;
        case 2: playSong(); break;
        case 3: saveSong(); break;
        case 4: loadSong(); break;
        case 5: generateRandomSong(); break;
        case 6: std::cout << "Goodbye!\n"; break;
        case 7: playHappyBirthday(); break;
        default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 6);

    return 0;
}
