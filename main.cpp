#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include <map>
#include <thread>
#include <sstream>
#include <algorithm>
#include <iomanip>

struct Note {
    std::string name;
    int freq;
    int duration;
};

struct Measure {
    std::string chord;
    std::vector<Note> notes;
    int measureNumber;
    std::string section;
    int duration;  // Custom duration for this measure
};

struct MusicSection {
    std::string name;
    std::vector<Measure> measures;
};

// Complete note frequencies from Octave 0 to Octave 8
std::map<std::string, int> noteFrequencies = {
    // Octave 0
    {"C0", 16}, {"C#0", 17}, {"D0", 18}, {"D#0", 19}, {"E0", 21}, {"F0", 22}, {"F#0", 23}, {"G0", 25}, {"G#0", 26}, {"A0", 28}, {"A#0", 29}, {"B0", 31},
    
    // Octave 1
    {"C1", 33}, {"C#1", 35}, {"D1", 37}, {"D#1", 39}, {"E1", 41}, {"F1", 44}, {"F#1", 46}, {"G1", 49}, {"G#1", 52}, {"A1", 55}, {"A#1", 58}, {"B1", 62},
    
    // Octave 2
    {"C2", 65}, {"C#2", 69}, {"D2", 73}, {"D#2", 78}, {"E2", 82}, {"F2", 87}, {"F#2", 93}, {"G2", 98}, {"G#2", 104}, {"A2", 110}, {"A#2", 117}, {"B2", 123},
    
    // Octave 3
    {"C3", 131}, {"C#3", 139}, {"D3", 147}, {"D#3", 156}, {"E3", 165}, {"F3", 175}, {"F#3", 185}, {"G3", 196}, {"G#3", 208}, {"A3", 220}, {"A#3", 233}, {"B3", 247},
    
    // Octave 4 (Middle Octave)
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

// Common chord definitions
std::map<std::string, std::vector<std::string>> chordDefinitions = {
    {"CMAJ7", {"C4", "E4", "G4", "B4"}},
    {"GMAJ7", {"G4", "B4", "D5", "F#5"}},
    {"D6", {"D4", "F#4", "A4", "B4"}},
    {"A7", {"A4", "C#5", "E5", "G5"}},
    {"DMAJ7", {"D4", "F#4", "A4", "C#5"}},
    {"CMAJ", {"C4", "E4", "G4"}},
    {"DMIN", {"D4", "F4", "A4"}},
    {"EMIN", {"E4", "G4", "B4"}},
    {"FMAJ", {"F4", "A4", "C5"}},
    {"GMAJ", {"G4", "B4", "D5"}},
    {"AMIN", {"A4", "C5", "E5"}},
    {"BMAJ", {"B4", "D#5", "F#5"}},
    {"C7", {"C4", "E4", "G4", "A#4"}},
    {"D7", {"D4", "F#4", "A4", "C5"}},
    {"E7", {"E4", "G#4", "B4", "D5"}},
    {"F7", {"F4", "A4", "C5", "D#5"}},
    {"G7", {"G4", "B4", "D5", "F5"}},
    {"A7", {"A4", "C#5", "E5", "G5"}},
    {"B7", {"B4", "D#5", "F#5", "A5"}}
};

std::vector<MusicSection> songSections;
std::string currentSection = "A";

void showMenu() {
    std::cout << "\n==== C++ Terminal Music Composer ====\n";
    std::cout << "1. Add measure to current section\n";
    std::cout << "2. Play current section\n";
    std::cout << "3. Play entire song\n";
    std::cout << "4. Save song\n";
    std::cout << "5. Load song\n";
    std::cout << "6. Generate random section\n";
    std::cout << "7. Exit\n";
    std::cout << "8. List all available notes\n";
    std::cout << "9. Show common chords\n";
    std::cout << "10. Print music sheet\n";
    std::cout << "11. Add new section\n";
    std::cout << "12. Switch section\n";
    std::cout << "13. Add chord by name\n";
    std::cout << "-------------------------------------\n";
    std::cout << "Current Section: " << currentSection << "\n";
    std::cout << "Choice: ";
}

MusicSection* getCurrentSection() {
    for (auto& section : songSections) {
        if (section.name == currentSection) {
            return &section;
        }
    }
    // Create section if it doesn't exist
    MusicSection newSection;
    newSection.name = currentSection;
    songSections.push_back(newSection);
    return &songSections.back();
}

void printMusicSheet() {
    if (songSections.empty()) {
        std::cout << "No music to display!\n";
        return;
    }
    
    std::cout << "\n";
    std::cout << "===============================================\n";
    std::cout << "               MUSIC SHEET\n";
    std::cout << "===============================================\n";
    
    for (const auto& section : songSections) {
        std::cout << "\nSection " << section.name << "\n";
        std::cout << std::string(60, '-') << "\n";
        std::cout << std::left << std::setw(8) << "Measure" 
                  << std::setw(12) << "Chord" 
                  << std::setw(10) << "Duration"
                  << "Notes\n";
        std::cout << std::string(60, '-') << "\n";
        
        for (const auto& measure : section.measures) {
            std::cout << std::setw(8) << measure.measureNumber
                      << std::setw(12) << measure.chord
                      << std::setw(10) << (std::to_string(measure.duration) + "ms");
            
            // Print notes
            for (size_t i = 0; i < measure.notes.size(); ++i) {
                std::cout << measure.notes[i].name;
                if (i < measure.notes.size() - 1) {
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }
    }
    std::cout << "===============================================\n";
}

void listCommonChords() {
    std::cout << "\nCommon Chords Available:\n";
    std::cout << "=======================\n";
    int count = 0;
    for (const auto& chord : chordDefinitions) {
        std::cout << chord.first << "\t";
        count++;
        if (count % 4 == 0) std::cout << "\n";
    }
    std::cout << "\n\nUse these chord names when adding measures.\n";
}

void addChordByName() {
    listCommonChords();
    std::cout << "\nEnter chord name (e.g., GMAJ7, AMIN, C7): ";
    std::string chordName;
    std::cin >> chordName;
    
    // Convert to uppercase
    for (char& c : chordName) {
        c = toupper(c);
    }
    
    if (chordDefinitions.find(chordName) == chordDefinitions.end()) {
        std::cout << "Chord not found! Available chords:\n";
        listCommonChords();
        return;
    }
    
    int duration;
    std::cout << "Enter duration in milliseconds: ";
    std::cin >> duration;
    
    MusicSection* current = getCurrentSection();
    Measure newMeasure;
    newMeasure.measureNumber = current->measures.size() + 1;
    newMeasure.chord = chordName;
    newMeasure.section = currentSection;
    newMeasure.duration = duration;
    
    // Add notes from chord definition
    for (const auto& noteName : chordDefinitions[chordName]) {
        Note n;
        n.name = noteName;
        n.freq = noteFrequencies[noteName];
        n.duration = duration;
        newMeasure.notes.push_back(n);
    }
    
    current->measures.push_back(newMeasure);
    std::cout << "Added " << chordName << " chord to Section " << currentSection 
              << ", Measure " << newMeasure.measureNumber << " (" << duration << "ms)\n";
}

void addMeasure() {
    MusicSection* current = getCurrentSection();
    Measure newMeasure;
    newMeasure.measureNumber = current->measures.size() + 1;
    newMeasure.section = currentSection;
    
    std::cout << "\nAdding Measure " << newMeasure.measureNumber << " to Section " << currentSection << "\n";
    
    // Get chord name
    std::cout << "Enter chord name (e.g., Gmaj7, D6): ";
    std::cin.ignore();
    std::getline(std::cin, newMeasure.chord);
    
    // Get duration
    std::cout << "Enter duration in milliseconds (e.g., 500): ";
    std::cin >> newMeasure.duration;
    std::cin.ignore(); // clear the newline
    
    // Get notes
    std::cout << "Enter notes separated by commas (e.g., G4,B4,D5,F#5): ";
    std::string input;
    std::getline(std::cin, input);
    
    // Convert to uppercase for note names
    for (size_t i = 0; i < input.length(); ++i) {
        if (isalpha(input[i]) && input[i] != '#') {
            input[i] = toupper(input[i]);
        }
    }
    
    // Parse notes
    std::stringstream ss(input);
    std::string noteName;
    while (std::getline(ss, noteName, ',')) {
        noteName.erase(std::remove_if(noteName.begin(), noteName.end(), ::isspace), noteName.end());
        
        if (noteFrequencies.find(noteName) != noteFrequencies.end()) {
            Note n;
            n.name = noteName;
            n.freq = noteFrequencies[noteName];
            n.duration = newMeasure.duration;
            newMeasure.notes.push_back(n);
        } else {
            std::cout << "Invalid note: " << noteName << " - skipping.\n";
        }
    }
    
    if (newMeasure.notes.empty()) {
        std::cout << "No valid notes entered. Measure not added.\n";
        return;
    }
    
    current->measures.push_back(newMeasure);
    std::cout << "Added Measure " << newMeasure.measureNumber << " to Section " << currentSection 
              << " (" << newMeasure.duration << "ms)\n";
}

void playNote(int frequency, int duration) {
    if (frequency > 0 && frequency >= 37 && frequency <= 32767) {
        Beep(frequency, duration);
    } else if (frequency > 0) {
        Sleep(duration);
    } else {
        Sleep(duration);
    }
}

void playSection(const std::string& sectionName) {
    MusicSection* section = nullptr;
    for (auto& s : songSections) {
        if (s.name == sectionName) {
            section = &s;
            break;
        }
    }
    
    if (!section || section->measures.empty()) {
        std::cout << "Section " << sectionName << " is empty or doesn't exist!\n";
        return;
    }
    
    std::cout << "\nPlaying Section " << sectionName << "...\n";
    std::cout << "===================\n";
    
    for (const auto& measure : section->measures) {
        std::cout << "Section " << measure.section << " - Measure " << measure.measureNumber 
                  << " - " << measure.chord << " (" << measure.duration << "ms): ";
        
        for (const auto& note : measure.notes) {
            std::cout << note.name << " ";
        }
        std::cout << "\n";
        
        // Play chord (all notes simultaneously)
        std::vector<std::thread> threads;
        for (const auto& note : measure.notes) {
            threads.push_back(std::thread(playNote, note.freq, note.duration));
        }
        
        for (auto& th : threads) {
            if (th.joinable()) {
                th.join();
            }
        }
        
        Sleep(100); // Gap between measures
    }
    std::cout << "Finished Section " << sectionName << "!\n";
}

void playEntireSong() {
    if (songSections.empty()) {
        std::cout << "No song to play!\n";
        return;
    }
    
    std::cout << "\nPlaying Entire Song...\n";
    std::cout << "=====================\n";
    
    for (const auto& section : songSections) {
        std::cout << "\n>>> SECTION " << section.name << " <<<\n";
        for (const auto& measure : section.measures) {
            std::cout << "Measure " << measure.measureNumber << " - " << measure.chord 
                      << " (" << measure.duration << "ms): ";
            
            for (const auto& note : measure.notes) {
                std::cout << note.name << " ";
            }
            std::cout << "\n";
            
            // Play chord
            std::vector<std::thread> threads;
            for (const auto& note : measure.notes) {
                threads.push_back(std::thread(playNote, note.freq, note.duration));
            }
            
            for (auto& th : threads) {
                if (th.joinable()) {
                    th.join();
                }
            }
            
            Sleep(100);
        }
    }
    std::cout << "\nSong finished!\n";
}

void addNewSection() {
    std::cout << "Enter new section name (single character, e.g., A, B, C): ";
    std::string newSection;
    std::cin >> newSection;
    
    if (newSection.length() != 1 || !isalpha(newSection[0])) {
        std::cout << "Invalid section name. Use single letters like A, B, C.\n";
        return;
    }
    
    newSection[0] = toupper(newSection[0]);
    currentSection = newSection;
    
    // Check if section already exists
    for (const auto& section : songSections) {
        if (section.name == newSection) {
            std::cout << "Switched to existing Section " << newSection << "\n";
            return;
        }
    }
    
    // Create new section
    MusicSection newSec;
    newSec.name = newSection;
    songSections.push_back(newSec);
    std::cout << "Created and switched to Section " << newSection << "\n";
}

void switchSection() {
    if (songSections.empty()) {
        std::cout << "No sections exist yet. Create one first.\n";
        return;
    }
    
    std::cout << "Available sections: ";
    for (const auto& section : songSections) {
        std::cout << section.name << " ";
    }
    std::cout << "\nEnter section to switch to: ";
    
    std::string sectionName;
    std::cin >> sectionName;
    sectionName[0] = toupper(sectionName[0]);
    
    for (const auto& section : songSections) {
        if (section.name == sectionName) {
            currentSection = sectionName;
            std::cout << "Switched to Section " << sectionName << "\n";
            return;
        }
    }
    
    std::cout << "Section " << sectionName << " not found.\n";
}

void saveSong() {
    std::ofstream file("song_sheet.txt");
    if (!file) {
        std::cout << "Error saving song.\n";
        return;
    }
    
    for (const auto& section : songSections) {
        file << "[SECTION " << section.name << "]\n";
        for (const auto& measure : section.measures) {
            file << measure.measureNumber << "|" << measure.chord << "|";
            for (size_t i = 0; i < measure.notes.size(); ++i) {
                file << measure.notes[i].name;
                if (i < measure.notes.size() - 1) file << ",";
            }
            file << "|" << measure.duration << "\n";
        }
    }
    std::cout << "Song saved to song_sheet.txt\n";
}

void loadSong() {
    std::ifstream file("song_sheet.txt");
    if (!file) {
        std::cout << "Error loading song.\n";
        return;
    }
    
    songSections.clear();
    std::string line;
    MusicSection* currentSectionPtr = nullptr;
    
    while (std::getline(file, line)) {
        if (line.find("[SECTION") == 0) {
            MusicSection newSection;
            newSection.name = line.substr(9, 1); // Get section letter
            songSections.push_back(newSection);
            currentSectionPtr = &songSections.back();
        } else if (currentSectionPtr && !line.empty()) {
            std::stringstream ss(line);
            std::string part;
            std::vector<std::string> parts;
            
            while (std::getline(ss, part, '|')) {
                parts.push_back(part);
            }
            
            if (parts.size() >= 4) {
                Measure measure;
                measure.measureNumber = std::stoi(parts[0]);
                measure.chord = parts[1];
                measure.section = currentSectionPtr->name;
                measure.duration = std::stoi(parts[3]);
                
                // Parse notes
                std::stringstream noteStream(parts[2]);
                std::string noteName;
                while (std::getline(noteStream, noteName, ',')) {
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
    
    std::cout << "Song loaded! " << songSections.size() << " sections.\n";
}

void listAllNotes() {
    std::cout << "\nAll Available Notes (Octaves 0-8, format: NoteOctave, e.g., C#4, A5):\n";
    std::cout << "======================================================================\n";
    
    std::vector<std::string> octaves = {"3", "4", "5", "6"}; // Most useful range
    std::vector<std::string> noteNames = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    
    for (const auto& octave : octaves) {
        std::cout << "\nOctave " << octave << ":\n";
        std::cout << "---------\n";
        for (const auto& noteName : noteNames) {
            std::string fullNote = noteName + octave;
            if (noteFrequencies.find(fullNote) != noteFrequencies.end()) {
                std::cout << fullNote << " (" << noteFrequencies[fullNote] << " Hz)\t";
                if (noteName == "D#" || noteName == "F#" || noteName == "A#") {
                    std::cout << "\n";
                }
            }
        }
        std::cout << "\n";
    }
}

void generateRandomSection() {
    MusicSection* current = getCurrentSection();
    
    int measureCount;
    std::cout << "How many measures to generate? (1-20): ";
    std::cin >> measureCount;
    
    if (measureCount < 1 || measureCount > 20) {
        std::cout << "Invalid number.\n";
        return;
    }
    
    srand((unsigned)time(nullptr));
    std::vector<std::string> commonNotes = {"C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5", "D5", "E5"};
    
    for (int i = 0; i < measureCount; ++i) {
        Measure newMeasure;
        newMeasure.measureNumber = current->measures.size() + 1;
        newMeasure.section = currentSection;
        newMeasure.chord = "RAND" + std::to_string(i + 1);
        newMeasure.duration = 300 + (rand() % 400); // Random duration between 300-700ms
        
        // Random chord size (1-4 notes)
        int noteCount = 1 + (rand() % 4);
        for (int j = 0; j < noteCount; ++j) {
            Note n;
            std::string noteName = commonNotes[rand() % commonNotes.size()];
            n.name = noteName;
            n.freq = noteFrequencies[noteName];
            n.duration = newMeasure.duration;
            newMeasure.notes.push_back(n);
        }
        
        current->measures.push_back(newMeasure);
    }
    
    std::cout << "Generated " << measureCount << " random measures in Section " << currentSection << "!\n";
}

int main() {
    std::cout << "C++ Music Composer with Custom Durations\n";
    std::cout << "=========================================\n\n";
    
    // Initialize with Section A
    MusicSection sectionA;
    sectionA.name = "A";
    songSections.push_back(sectionA);
    
    int choice;
    do {
        showMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1: addMeasure(); break;
            case 2: playSection(currentSection); break;
            case 3: playEntireSong(); break;
            case 4: saveSong(); break;
            case 5: loadSong(); break;
            case 6: generateRandomSection(); break;
            case 7: std::cout << "Goodbye!\n"; break;
            case 8: listAllNotes(); break;
            case 9: listCommonChords(); break;
            case 10: printMusicSheet(); break;
            case 11: addNewSection(); break;
            case 12: switchSection(); break;
            case 13: addChordByName(); break;
            default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 7);
    
    return 0;
}