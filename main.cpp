// main.cpp - Replace the entire file with this:

// main.cpp
// Entry point: drives the menu loop and delegates work to music.cpp interfaces.

#include <iostream>
#include "music.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Initial context and user-facing header.
    cout << "C++ Music Composer with Custom Durations\n";
    cout << string(50, '=') << "\n\n";

    // Handle drag & drop - if a file is dragged onto the executable
    if (argc > 1) {
        cout << "Drag & drop detected!\n";
        cout << "Loading: " << argv[1] << "\n\n";
        
        // Simple drag & drop loading
        ifstream file(argv[1]);
        if (file) {
            songSections.clear();
            string line;
            MusicSection* currentSectionPtr = nullptr;

            while (getline(file, line)) {
                if (line.find("[SECTION") == 0) {
                    MusicSection newSection;
                    newSection.name = line.substr(9, 1);
                    songSections.push_back(newSection);
                    currentSectionPtr = &songSections.back();
                } else if (currentSectionPtr && !line.empty()) {
                    stringstream ss(line);
                    string part;
                    vector<string> parts;
                    while (getline(ss, part, '|')) parts.push_back(part);
                    if (parts.size() >= 4) {
                        Measure measure;
                        measure.measureNumber = stoi(parts[0]);
                        measure.chord = parts[1];
                        measure.section = currentSectionPtr->name;
                        measure.duration = stoi(parts[3]);

                        stringstream noteStream(parts[2]);
                        string noteName;
                        while (getline(noteStream, noteName, ',')) {
                            if (noteFrequencies.find(noteName) != noteFrequencies.end()) {
                                Note n;
                                n.name = noteName;
                                n.freq = noteFrequencies[noteName];
                                n.duration = measure.duration;
                                measure.notes.push_back(n);
                            }
                        }
                        currentSectionPtr->measures.push_back(measure);
                    }
                }
            }
            cout << "Auto-loaded: " << argv[1] << " (" << songSections.size() << " sections)\n";
            cout << "Press Enter to continue to menu...";
            cin.ignore();
            cin.get();
        } else {
            cout << "Error: Could not open " << argv[1] << "\n";
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    // Ensure there is an initial Section A in the song state.
    MusicSection sectionA;
    sectionA.name = "A";
    songSections.push_back(sectionA);

    // Menu loop: read choice, dispatch to corresponding action.
    int choice;
    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1:  addMeasure();                 break; // manual measure entry
            case 2:  playSection(currentSection);  break; // play active section
            case 3:  playEntireSong();             break; // play all sections
            case 4:  saveSong();                   break; // write to file
            case 5:  loadSong();                   break; // load from file
            case 6:  generateRandomSection();      break; // random content in active section
            case 7:  playHappyBirthday();          break; // play Happy Birthday
            case 8:  listAllNotes();               break; // note/frequency reference
            case 9: listCommonChords();           break; // chord reference
            case 10: printMusicSheet();            break; // formatted view of song
            case 11: addNewSection();              break; // create + switch
            case 12: switchSection();              break; // switch only
            case 13: addChordByName();             break; // quick-add by chord
            case 14:  cout << "Goodbye!\n";         break; // exit loop
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 14);

    return 0;
}