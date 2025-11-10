// music.cpp
// Implementation for the terminal music composer: data, helpers, UI actions, and playback.

#include "music.h"

using namespace std;

// ===== Notation data (notes and chords) =====
// Full mapping of note names to approximate equal-temperament frequencies.
map<string, int> noteFrequencies = {
    // Octave 0
    {"C0", 16}, {"C#0", 17}, {"D0", 18}, {"D#0", 19}, {"E0", 21}, {"F0", 22}, {"F#0", 23}, {"G0", 25}, {"G#0", 26}, {"A0", 28}, {"A#0", 29}, {"B0", 31},
    // Octave 1
    {"C1", 33}, {"C#1", 35}, {"D1", 37}, {"D#1", 39}, {"E1", 41}, {"F1", 44}, {"F#1", 46}, {"G1", 49}, {"G#1", 52}, {"A1", 55}, {"A#1", 58}, {"B1", 62},
    // Octave 2
    {"C2", 65}, {"C#2", 69}, {"D2", 73}, {"D#2", 78}, {"E2", 82}, {"F2", 87}, {"F#2", 93}, {"G2", 98}, {"G#2", 104}, {"A2", 110}, {"A#2", 117}, {"B2", 123},
    // Octave 3
    {"C3", 131}, {"C#3", 139}, {"D3", 147}, {"D#3", 156}, {"E3", 165}, {"F3", 175}, {"F#3", 185}, {"G3", 196}, {"G#3", 208}, {"A3", 220}, {"A#3", 233}, {"B3", 247},
    // Octave 4
    {"C4", 262}, {"C#4", 277}, {"D4", 294}, {"D#4", 311}, {"E4", 330}, {"F4", 349}, {"F#4", 370}, {"G4", 392}, {"G#4", 415}, {"A4", 440}, {"A#4", 466}, {"B4", 494},
    // Octave 5
    {"C5", 523}, {"C#5", 554}, {"D5", 587}, {"D#5", 622}, {"E5", 659}, {"F5", 698}, {"F#5", 740}, {"G5", 784}, {"G#5", 831}, {"A5", 880}, {"A#5", 932}, {"B5", 988},
    // Octave 6
    {"C6", 1047}, {"C#6", 1109}, {"D6", 1175}, {"D#6", 1245}, {"E6", 1319}, {"F6", 1397}, {"F#6", 1480}, {"G6", 1568}, {"G#6", 1661}, {"A6", 1760}, {"A#6", 1865}, {"B6", 1976},
    // Octave 7
    {"C7", 2093}, {"C#7", 2217}, {"D7", 2349}, {"D#7", 2489}, {"E7", 2637}, {"F7", 2794}, {"F#7", 2960}, {"G7", 3136}, {"G#7", 3322}, {"A7", 3520}, {"A#7", 3729}, {"B7", 3951},
    // Octave 8
    {"C8", 4186}, {"C#8", 4435}, {"D8", 4699}, {"D#8", 4978}, {"E8", 5274}, {"F8", 5588}, {"F#8", 5920}, {"G8", 6272}, {"G#8", 6645}, {"A8", 7040}, {"A#8", 7459}, {"B8", 7902}
};

// Named chord spellings used to quickly build a measure.
map<string, vector<string>> chordDefinitions = {
    {"CMAJ7", {"C4", "E4", "G4", "B4"}},
    {"GMAJ7", {"G4", "B4", "D5", "F#5"}},
    {"D6",    {"D4", "F#4", "A4", "B4"}},
    {"A7",    {"A4", "C#5", "E5", "G5"}},
    {"DMAJ7", {"D4", "F#4", "A4", "C#5"}},
    {"CMAJ",  {"C4", "E4", "G4"}},
    {"DMIN",  {"D4", "F4", "A4"}},
    {"EMIN",  {"E4", "G4", "B4"}},
    {"FMAJ",  {"F4", "A4", "C5"}},
    {"GMAJ",  {"G4", "B4", "D5"}},
    {"AMIN",  {"A4", "C5", "E5"}},
    {"BMAJ",  {"B4", "D#5", "F#5"}},
    {"C7",    {"C4", "E4", "G4", "A#4"}},
    {"D7",    {"D4", "F#4", "A4", "C5"}},
    {"E7",    {"E4", "G#4", "B4", "D5"}},
    {"F7",    {"F4", "A4", "C5", "D#5"}},
    {"G7",    {"G4", "B4", "D5", "F5"}},
    {"B7",    {"B4", "D#5", "F#5", "A5"}}
};

// ===== Global song state =====
vector<MusicSection> songSections;
string currentSection = "A";

// ===== UI / Menu =====
void showMenu() {
    // Displays available operations and current context.
    cout << "\n==== C++ Terminal Music Composer ====\n";
    cout << "1. Add measure to current section\n";
    cout << "2. Play current section\n";
    cout << "3. Play entire song\n";
    cout << "4. Save song\n";
    cout << "5. Load song\n";
    cout << "6. Generate random section\n";
    cout << "7. Play Happy Birthday \n";
    cout << "8. List all available notes\n";
    cout << "9. Show common chords\n";
    cout << "10. Print music sheet\n";
    cout << "11. Add new section\n";
    cout << "12. Switch section\n";
    cout << "13. Add chord by name\n";
    cout << "14. Exit\n";
    cout << "-------------------------------------\n";
    cout << "Current Section: " << currentSection << "\n";
    cout << "Choice: ";
}

// Returns the active section; creates a new empty one if not present.
MusicSection* getCurrentSection() {
    for (auto& section : songSections) {
        if (section.name == currentSection) return &section;
    }
    MusicSection newSection;
    newSection.name = currentSection;
    songSections.push_back(newSection);
    return &songSections.back();
}

// ===== Presentation helpers =====
void printMusicSheet() {
    // Outputs a tabular view of all sections/measures/notes.
    if (songSections.empty()) { cout << "No music to display!\n"; return; }

    cout << "\n";
    cout << "===============================================\n";
    cout << "               MUSIC SHEET\n";
    cout << "===============================================\n";

    for (const auto& section : songSections) {
        cout << "\nSection " << section.name << "\n";
        cout << string(60, '-') << "\n";
        cout << left << setw(8) << "Measure" << setw(12) << "Chord" << setw(10) << "Duration" << "Notes\n";
        cout << string(60, '-') << "\n";

        for (const auto& measure : section.measures) {
            cout << setw(8) << measure.measureNumber
                 << setw(12) << measure.chord
                 << setw(10) << (to_string(measure.duration) + "ms");
            for (size_t i = 0; i < measure.notes.size(); ++i) {
                cout << measure.notes[i].name << (i + 1 < measure.notes.size() ? " " : "");
            }
            cout << "\n";
        }
    }
    cout << "===============================================\n";
}

// Lists supported chord names for reference.
void listCommonChords() {
    cout << "\nCommon Chords Available:\n";
    cout << "=======================\n";
    int count = 0;
    for (const auto& chord : chordDefinitions) {
        cout << chord.first << "\t";
        if (++count % 4 == 0) cout << "\n";
    }
    cout << "\n\nUse these chord names when adding measures.\n";
}

// Adds a measure by selecting a chord name from chordDefinitions.
void addChordByName() {
    listCommonChords();
    cout << "\nEnter chord name (e.g., GMAJ7, AMIN, C7): ";
    string chordName;
    cin >> chordName;

    for (char& c : chordName) c = static_cast<char>(toupper(static_cast<unsigned char>(c)));

    if (chordDefinitions.find(chordName) == chordDefinitions.end()) {
        cout << "Chord not found! Available chords:\n";
        listCommonChords();
        return;
    }

    int duration;
    cout << "Enter duration in milliseconds: ";
    cin >> duration;

    MusicSection* current = getCurrentSection();
    Measure newMeasure;
    newMeasure.measureNumber = static_cast<int>(current->measures.size()) + 1;
    newMeasure.chord = chordName;
    newMeasure.section = currentSection;
    newMeasure.duration = duration;

    for (const auto& noteName : chordDefinitions[chordName]) {
        Note n;
        n.name = noteName;
        n.freq = noteFrequencies[noteName];
        n.duration = duration;
        newMeasure.notes.push_back(n);
    }

    current->measures.push_back(newMeasure);
    cout << "Added " << chordName << " chord to Section " << currentSection
         << ", Measure " << newMeasure.measureNumber << " (" << duration << "ms)\n";
}

// Adds a measure by manual entry (free choice of chord label and note list).
void addMeasure() {
    MusicSection* current = getCurrentSection();
    Measure newMeasure;
    newMeasure.measureNumber = static_cast<int>(current->measures.size()) + 1;
    newMeasure.section = currentSection;

    cout << "\nAdding Measure " << newMeasure.measureNumber << " to Section " << currentSection << "\n";

    cout << "Enter chord name (e.g., Gmaj7, D6): ";
    cin.ignore();
    getline(cin, newMeasure.chord);

    cout << "Enter duration in milliseconds (e.g., 500): ";
    cin >> newMeasure.duration;
    cin.ignore();

    cout << "Enter notes separated by commas (e.g., G4,B4,D5,F#5): ";
    string input;
    getline(cin, input);

    // Normalize letter case (keep sharps).
    for (size_t i = 0; i < input.length(); ++i) {
        if (isalpha(static_cast<unsigned char>(input[i])) && input[i] != '#') {
            input[i] = static_cast<char>(toupper(static_cast<unsigned char>(input[i])));
        }
    }

    // Split and validate note tokens.
    stringstream ss(input);
    string noteName;
    while (getline(ss, noteName, ',')) {
        noteName.erase(remove_if(noteName.begin(), noteName.end(), ::isspace), noteName.end());
        if (noteFrequencies.find(noteName) != noteFrequencies.end()) {
            Note n;
            n.name = noteName;
            n.freq = noteFrequencies[noteName];
            n.duration = newMeasure.duration;
            newMeasure.notes.push_back(n);
        } else {
            cout << "Invalid note: " << noteName << " - skipping.\n";
        }
    }

    if (newMeasure.notes.empty()) {
        cout << "No valid notes entered. Measure not added.\n";
        return;
    }

    current->measures.push_back(newMeasure);
    cout << "Added Measure " << newMeasure.measureNumber << " to Section " << currentSection
         << " (" << newMeasure.duration << "ms)\n";
}

// Plays a single tone using Windows Beep; otherwise sleeps for the duration.
void playNote(int frequency, int duration) {
    if (frequency > 0 && frequency >= 37 && frequency <= 32767) {
        Beep(static_cast<DWORD>(frequency), static_cast<DWORD>(duration));
    } else {
        Sleep(static_cast<DWORD>(duration));
    }
}

// Iterates measures in a named section and plays notes (chord tones in parallel).
void playSection(const string& sectionName) {
    MusicSection* section = nullptr;
    for (auto& s : songSections) {
        if (s.name == sectionName) { section = &s; break; }
    }

    if (!section || section->measures.empty()) {
        cout << "Section " << sectionName << " is empty or doesn't exist!\n";
        return;
    }

    cout << "\nPlaying Section " << sectionName << "...\n";
    cout << "===================\n";

    for (const auto& measure : section->measures) {
        cout << "Section " << measure.section << " - Measure " << measure.measureNumber
             << " - " << measure.chord << " (" << measure.duration << "ms): ";
        for (const auto& note : measure.notes) cout << note.name << " ";
        cout << "\n";

        // Play notes in a measure concurrently to emulate a chord.
        vector<thread> threads;
        for (const auto& note : measure.notes) {
            threads.emplace_back(playNote, note.freq, note.duration);
        }
        for (auto& th : threads) if (th.joinable()) th.join();

        // Small gap between measures.
        Sleep(10);
    }
    cout << "Finished Section " << sectionName << "!\n";
}

// Plays all sections in stored order.
void playEntireSong() {
    if (songSections.empty()) { cout << "No song to play!\n"; return; }

    cout << "\nPlaying Entire Song...\n";
    cout << "=====================\n";

    for (const auto& section : songSections) {
        cout << "\n>>> SECTION " << section.name << " <<<\n";
        for (const auto& measure : section.measures) {
            cout << "Measure " << measure.measureNumber << " - " << measure.chord
                 << " (" << measure.duration << "ms): ";
            for (const auto& note : measure.notes) cout << note.name << " ";
            cout << "\n";

            vector<thread> threads;
            for (const auto& note : measure.notes) {
                threads.emplace_back(playNote, note.freq, note.duration);
            }
            for (auto& th : threads) if (th.joinable()) th.join();
            Sleep(5);
        }
    }
    cout << "\nSong finished!\n";
}

// Creates a new labeled section and switches context to it.
void addNewSection() {
    cout << "Enter new section name (single character, e.g., A, B, C): ";
    string newSection;
    cin >> newSection;

    if (newSection.length() != 1 || !isalpha(static_cast<unsigned char>(newSection[0]))) {
        cout << "Invalid section name. Use single letters like A, B, C.\n";
        return;
    }

    newSection[0] = static_cast<char>(toupper(static_cast<unsigned char>(newSection[0])));
    currentSection = newSection;

    for (const auto& section : songSections) {
        if (section.name == newSection) {
            cout << "Switched to existing Section " << newSection << "\n";
            return;
        }
    }

    MusicSection newSec;
    newSec.name = newSection;
    songSections.push_back(newSec);
    cout << "Created and switched to Section " << newSection << "\n";
}

// Switches currentSection to a chosen existing section.
void switchSection() {
    if (songSections.empty()) {
        cout << "No sections exist yet. Create one first.\n";
        return;
    }

    cout << "Available sections: ";
    for (const auto& section : songSections) cout << section.name << " ";
    cout << "\nEnter section to switch to: ";

    string sectionName;
    cin >> sectionName;
    sectionName[0] = static_cast<char>(toupper(static_cast<unsigned char>(sectionName[0])));

    for (const auto& section : songSections) {
        if (section.name == sectionName) {
            currentSection = sectionName;
            cout << "Switched to Section " << sectionName << "\n";
            return;
        }
    }
    cout << "Section " << sectionName << " not found.\n";
}

// Serializes all sections/measures to a simple line format.
void saveSong() {
    string filename;
    cout << "Enter filename (or press Enter for song_sheet.txt): ";
    cin.ignore();
    getline(cin, filename);
    
    if (filename.empty()) {
        filename = "song_sheet.txt";
    }
    
    ofstream file(filename);
    if (!file) { cout << "Error saving song to " << filename << "\n"; return; }

    for (const auto& section : songSections) {
        file << "[SECTION " << section.name << "]\n";
        for (const auto& measure : section.measures) {
            file << measure.measureNumber << "|" << measure.chord << "|";
            for (size_t i = 0; i < measure.notes.size(); ++i) {
                file << measure.notes[i].name;
                if (i + 1 < measure.notes.size()) file << ",";
            }
            file << "|" << measure.duration << "\n";
        }
    }
    cout << "Song saved to " << filename << "\n";
}

void loadSong() {
    string filename;
    cout << "Enter filename (or press Enter for song_sheet.txt): ";
    cin.ignore();
    getline(cin, filename);
    
    if (filename.empty()) {
        filename = "song_sheet.txt";
    }
    
    ifstream file(filename);
    if (!file) { cout << "Error loading " << filename << "\n"; return; }

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
    cout << "Song loaded from " << filename << "! " << songSections.size() << " sections.\n";
}

// Prints selected octaves/notes with frequency values for reference.
void listAllNotes() {
    cout << "\nAll Available Notes (Octaves 0-8, format: NoteOctave, e.g., C#4, A5):\n";
    cout << "======================================================================\n";

    vector<string> octaves = {"0","1","2","3", "4", "5", "6","7","8"};
    vector<string> noteNames = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    for (const auto& octave : octaves) {
        cout << "\nOctave " << octave << ":\n";
        cout << "---------\n";
        for (const auto& noteName : noteNames) {
            string fullNote = noteName + octave;
            if (noteFrequencies.find(fullNote) != noteFrequencies.end()) {
                cout << fullNote << " (" << noteFrequencies[fullNote] << " Hz)\t";
                if (noteName == "D#" || noteName == "F#" || noteName == "A#") cout << "\n";
            }
        }
        cout << "\n";
    }
}

// Creates a batch of measures with randomized content in the active section.
void generateRandomSection() {
    MusicSection* current = getCurrentSection();

    int measureCount;
    cout << "How many measures to generate? (1-20): ";
    cin >> measureCount;

    if (measureCount < 1 || measureCount > 20) { cout << "Invalid number.\n"; return; }

    srand(static_cast<unsigned>(time(nullptr)));
    vector<string> commonNotes = {"C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5", "D5", "E5"};

    for (int i = 0; i < measureCount; ++i) {
        Measure newMeasure;
        newMeasure.measureNumber = static_cast<int>(current->measures.size()) + 1;
        newMeasure.section = currentSection;
        newMeasure.chord = "RAND" + to_string(i + 1);
        newMeasure.duration = 300 + (rand() % 400);

        int noteCount = 1 + (rand() % 4);
        for (int j = 0; j < noteCount; ++j) {
            Note n;
            string noteName = commonNotes[rand() % commonNotes.size()];
            n.name = noteName;
            n.freq = noteFrequencies[noteName];
            n.duration = newMeasure.duration;
            newMeasure.notes.push_back(n);
        }
        current->measures.push_back(newMeasure);
    }
    cout << "Generated " << measureCount << " random measures in Section " << currentSection << "!\n";
}

// Plays the "Happy Birthday" melody
void playHappyBirthday() {
    cout << "\nPlaying Happy Birthday...\n";
    cout << "========================\n";
    
    // Happy Birthday melody notes (simple version)
    vector<pair<string, int>> melody = {
        {"G4", 250}, {"G4", 250}, {"A4", 500}, {"G4", 500}, {"C5", 500}, {"B4", 1000},
        {"G4", 250}, {"G4", 250}, {"A4", 500}, {"G4", 500}, {"D5", 500}, {"C5", 1000},
        {"G4", 250}, {"G4", 250}, {"G5", 500}, {"E5", 500}, {"C5", 500}, {"B4", 500}, {"A4", 1000},
        {"F5", 250}, {"F5", 250}, {"E5", 500}, {"C5", 500}, {"D5", 500}, {"C5", 500}
    };
    
    for (const auto& note : melody) {
        string noteName = note.first;
        int duration = note.second;
        
        if (noteFrequencies.find(noteName) != noteFrequencies.end()) {
            int freq = noteFrequencies[noteName];
            cout << "Playing: " << noteName << " (" << duration << "ms)\n";
            playNote(freq, duration);

            Sleep(5); // brief pause between notes
        } else {
            cout << "Note not found: " << noteName << "\n";
        }
    }
    
    cout << "Finished playing Happy Birthday!\n";
}