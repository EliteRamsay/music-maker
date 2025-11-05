// main.cpp
// Entry point: drives the menu loop and delegates work to music.cpp interfaces.

#include <iostream>
#include "music.h"

using namespace std;

int main() {
    // Initial context and user-facing header.
    cout << "C++ Music Composer with Custom Durations\n";
    cout << "=========================================\n\n";

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
            case 7:  cout << "Goodbye!\n";         break; // exit loop
            case 8:  listAllNotes();               break; // note/frequency reference
            case 9:  listCommonChords();           break; // chord reference
            case 10: printMusicSheet();            break; // formatted view of song
            case 11: addNewSection();              break; // create + switch
            case 12: switchSection();              break; // switch only
            case 13: addChordByName();             break; // quick-add by chord
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 7);

    return 0;
}
