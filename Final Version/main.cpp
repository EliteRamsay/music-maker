#include <iostream>
#include "music.h"

using namespace std;

int main() {
    initMIDI();
    
    cout << "=== C++ Terminal Music Composer (MIDI Enhanced) ===\n";
    cout << "Now with MIDI support, multiple instruments, and real-time playback control!\n";
    
    int choice;
    do {
        showMenu();
        cin >> choice;
        
        switch(choice) {
            case 1: addMeasure(); break;
            case 2: playSection(currentSection); break;
            case 3: playEntireSong(); break;
            case 4: saveSong(); break;
            case 5: loadSong(); break;
            case 6: generateRandomSection(); break;
            case 7: playHappyBirthday(); break;
            case 8: listAllNotes(); break;
            case 9: listCommonChords(); break;
            case 10: printMusicSheet(); break;
            case 11: addNewSection(); break;
            case 12: switchSection(); break;
            case 13: addChordByName(); break;
            case 14: changeInstrument(); break;
            case 15: pausePlayback(); break;
            case 16: resumePlayback(); break;
            case 17: stopPlaybackCommand(); break;
            case 18: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 18);
    
    closeMIDI();
    return 0;
}