// music.cpp
// Implementation for the terminal music composer: data, helpers, UI actions, and playback.

#include "music.h"

using namespace std;

// ===== Global state =====
vector<MusicSection> songSections;
string currentSection = "A";
PlaybackState playbackState = STATE_STOPPED;
int currentInstrument = INSTRUMENT_PIANO;
bool stopPlayback = false;

// MIDI handle
HMIDIOUT hMidiOut = NULL;

// ===== MIDI Implementation =====
void initMIDI() {
    if (hMidiOut == NULL) {
        midiOutOpen(&hMidiOut, 0, 0, 0, CALLBACK_NULL);
        setInstrument(currentInstrument);
    }
}

void closeMIDI() {
    if (hMidiOut != NULL) {
        midiOutClose(hMidiOut);
        hMidiOut = NULL;
    }
}

void setInstrument(int instrument) {
    if (hMidiOut != NULL) {
        midiOutShortMsg(hMidiOut, 0xC0 | (instrument << 8));
    }
}

void playMIDINote(int note, int velocity, int channel) {
    if (hMidiOut != NULL) {
        midiOutShortMsg(hMidiOut, 0x90 | channel | (note << 8) | (velocity << 16));
    }
}

void stopMIDINote(int note, int channel) {
    if (hMidiOut != NULL) {
        midiOutShortMsg(hMidiOut, 0x80 | channel | (note << 8));
    }
}

void playMIDIChord(const vector<int>& notes, int velocity, int channel) {
    for (int note : notes) {
        playMIDINote(note, velocity, channel);
    }
}

void stopMIDIChord(const vector<int>& notes, int channel) {
    for (int note : notes) {
        stopMIDINote(note, channel);
    }
}

// ===== Notation data (notes and chords) =====
// Mapping of note names to MIDI note numbers (C4 = 60)
map<string, int> noteToMidi = {
    // Octave 0
    {"C0", 12}, {"C#0", 13}, {"D0", 14}, {"D#0", 15}, {"E0", 16}, {"F0", 17}, {"F#0", 18}, 
    {"G0", 19}, {"G#0", 20}, {"A0", 21}, {"A#0", 22}, {"B0", 23},
    // Octave 1
    {"C1", 24}, {"C#1", 25}, {"D1", 26}, {"D#1", 27}, {"E1", 28}, {"F1", 29}, {"F#1", 30}, 
    {"G1", 31}, {"G#1", 32}, {"A1", 33}, {"A#1", 34}, {"B1", 35},
    // Octave 2
    {"C2", 36}, {"C#2", 37}, {"D2", 38}, {"D#2", 39}, {"E2", 40}, {"F2", 41}, {"F#2", 42}, 
    {"G2", 43}, {"G#2", 44}, {"A2", 45}, {"A#2", 46}, {"B2", 47},
    // Octave 3
    {"C3", 48}, {"C#3", 49}, {"D3", 50}, {"D#3", 51}, {"E3", 52}, {"F3", 53}, {"F#3", 54}, 
    {"G3", 55}, {"G#3", 56}, {"A3", 57}, {"A#3", 58}, {"B3", 59},
    // Octave 4
    {"C4", 60}, {"C#4", 61}, {"D4", 62}, {"D#4", 63}, {"E4", 64}, {"F4", 65}, {"F#4", 66}, 
    {"G4", 67}, {"G#4", 68}, {"A4", 69}, {"A#4", 70}, {"B4", 71},
    // Octave 5
    {"C5", 72}, {"C#5", 73}, {"D5", 74}, {"D#5", 75}, {"E5", 76}, {"F5", 77}, {"F#5", 78}, 
    {"G5", 79}, {"G#5", 80}, {"A5", 81}, {"A#5", 82}, {"B5", 83},
    // Octave 6
    {"C6", 84}, {"C#6", 85}, {"D6", 86}, {"D#6", 87}, {"E6", 88}, {"F6", 89}, {"F#6", 90}, 
    {"G6", 91}, {"G#6", 92}, {"A6", 93}, {"A#6", 94}, {"B6", 95},
    // Octave 7
    {"C7", 96}, {"C#7", 97}, {"D7", 98}, {"D#7", 99}, {"E7", 100}, {"F7", 101}, {"F#7", 102}, 
    {"G7", 103}, {"G#7", 104}, {"A7", 105}, {"A#7", 106}, {"B7", 107},
    // Octave 8
    {"C8", 108}, {"C#8", 109}, {"D8", 110}, {"D#8", 111}, {"E8", 112}, {"F8", 113}, {"F#8", 114}, 
    {"G8", 115}, {"G#8", 116}, {"A8", 117}, {"A#8", 118}, {"B8", 119}
};

// Keep frequency mapping for compatibility
map<string, int> noteFrequencies = {
    {"C4", 262}, {"C#4", 277}, {"D4", 294}, {"D#4", 311}, {"E4", 330}, {"F4", 349}, 
    {"F#4", 370}, {"G4", 392}, {"G#4", 415}, {"A4", 440}, {"A#4", 466}, {"B4", 494},
    {"C5", 523}, {"C#5", 554}, {"D5", 587}, {"D#5", 622}, {"E5", 659}, {"F5", 698}, 
    {"F#5", 740}, {"G5", 784}, {"G#5", 831}, {"A5", 880}, {"A#5", 932}, {"B5", 988}
};

// Named chord spellings used to quickly build a measure.
map<string, vector<string>> chordDefinitions = {
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
    {"B7", {"B4", "D#5", "F#5", "A5"}}};

// ===== UI / Menu =====
void showMenu()
{
    // Displays available operations and current context.
    cout << "\n==== C++ Terminal Music Composer (MIDI Enhanced) ====\n";
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
    cout << "14. Change Instrument\n";
    cout << "15. Pause Playback\n";
    cout << "16. Resume Playback\n";
    cout << "17. Stop Playback\n";
    cout << "18. Exit\n";
    cout << string(35, '-') << "\n";
    cout << "Current Section: " << currentSection << "\n";
    cout << "Current Instrument: ";
    switch(currentInstrument) {
        case INSTRUMENT_PIANO: cout << "Piano"; break;
        case INSTRUMENT_SYNTH: cout << "Synth"; break;
        case INSTRUMENT_ORGAN: cout << "Organ"; break;
        case INSTRUMENT_GUITAR: cout << "Guitar"; break;
        case INSTRUMENT_BASS: cout << "Bass"; break;
        case INSTRUMENT_STRINGS: cout << "Strings"; break;
        case INSTRUMENT_TRUMPET: cout << "Trumpet"; break;
        case INSTRUMENT_SAXOPHONE: cout << "Saxophone"; break;
        case INSTRUMENT_FLUTE: cout << "Flute"; break;
        default: cout << "Unknown";
    }
    cout << "\n";
    cout << "Choice: ";
}

// Returns the active section; creates a new empty one if not present.
MusicSection *getCurrentSection()
{
    for (auto &section : songSections)
    {
        if (section.name == currentSection)
            return &section;
    }
    MusicSection newSection;
    newSection.name = currentSection;
    songSections.push_back(newSection);
    return &songSections.back();
}

// ===== Presentation helpers =====
void printMusicSheet()
{
    // Outputs a tabular view of all sections/measures/notes.
    if (songSections.empty())
    {
        cout << "No music to display!\n";
        return;
    }

    cout << "\n";
    cout << string(47, '=') << "\n";
    cout << "               MUSIC SHEET\n";
    cout << string(47, '=') << "\n";
    
    for (const auto &section : songSections)
    {
        cout << "\nSection " << section.name << "\n";
        cout << string(60, '-') << "\n";
        cout << left << setw(8) << "Measure" << setw(12) << "Chord" << setw(10) << "Duration" << "Notes\n";
        cout << string(60, '-') << "\n";

        for (const auto &measure : section.measures)
        {
            cout << setw(8) << measure.measureNumber
                 << setw(12) << measure.chord
                 << setw(10) << (to_string(measure.duration) + "ms");
            for (size_t i = 0; i < measure.notes.size(); ++i)
            {
                cout << measure.notes[i].name << (i + 1 < measure.notes.size() ? " " : "");
            }
            cout << "\n";
        }
    }
    cout << string(47, '=') << "\n";
}

// Lists supported chord names for reference.
void listCommonChords()
{
    cout << "\nCommon Chords Available:\n";
    cout << string(25,'=') << "\n";
    int count = 0;
    for (const auto &chord : chordDefinitions)
    {
        cout << chord.first << "\t";
        if (++count % 4 == 0)
            cout << "\n";
    }
    cout << "\n\nUse these chord names when adding measures.\n";
}

// Adds a measure by selecting a chord name from chordDefinitions.
void addChordByName()
{
    listCommonChords();
    cout << "\nEnter chord name (e.g., GMAJ7, AMIN, C7): ";
    string chordName;
    cin >> chordName;

    for (char &c : chordName)
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));

    if (chordDefinitions.find(chordName) == chordDefinitions.end())
    {
        cout << "Chord not found! Available chords:\n";
        listCommonChords();
        return;
    }

    int duration;
    cout << "Enter duration in milliseconds: ";
    cin >> duration;

    MusicSection *current = getCurrentSection();
    Measure newMeasure;
    newMeasure.measureNumber = static_cast<int>(current->measures.size()) + 1;
    newMeasure.chord = chordName;
    newMeasure.section = currentSection;
    newMeasure.duration = duration;

    for (const auto &noteName : chordDefinitions[chordName])
    {
        Note n;
        n.name = noteName;
        n.freq = noteFrequencies[noteName];
        n.midiNote = noteToMidi[noteName];
        n.duration = duration;
        newMeasure.notes.push_back(n);
    }

    current->measures.push_back(newMeasure);
    cout << "Added " << chordName << " chord to Section " << currentSection
         << ", Measure " << newMeasure.measureNumber << " (" << duration << "ms)\n";
}

// Adds a measure by manual entry (free choice of chord label and note list).
void addMeasure()
{
    MusicSection *current = getCurrentSection();
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
    for (size_t i = 0; i < input.length(); ++i)
    {
        if (isalpha(static_cast<unsigned char>(input[i])) && input[i] != '#')
        {
            input[i] = static_cast<char>(toupper(static_cast<unsigned char>(input[i])));
        }
    }

    // Split and validate note tokens.
    stringstream ss(input);
    string noteName;
    while (getline(ss, noteName, ','))
    {
        noteName.erase(remove_if(noteName.begin(), noteName.end(), ::isspace), noteName.end());
        if (noteToMidi.find(noteName) != noteToMidi.end())
        {
            Note n;
            n.name = noteName;
            n.freq = noteFrequencies[noteName];
            n.midiNote = noteToMidi[noteName];
            n.duration = newMeasure.duration;
            newMeasure.notes.push_back(n);
        }
        else
        {
            cout << "Invalid note: " << noteName << " - skipping.\n";
        }
    }

    if (newMeasure.notes.empty())
    {
        cout << "No valid notes entered. Measure not added.\n";
        return;
    }

    current->measures.push_back(newMeasure);
    cout << "Added Measure " << newMeasure.measureNumber << " to Section " << currentSection
         << " (" << newMeasure.duration << "ms)\n";
}

// Plays a single tone using MIDI; otherwise sleeps for the duration.
void playNote(int frequency, int duration)
{
    // For compatibility, we'll use the MIDI system
    // This function is kept for backward compatibility
    Sleep(static_cast<DWORD>(duration));
}

// Iterates measures in a named section and plays notes (chord tones in parallel).
void playSection(const string &sectionName)
{
    MusicSection *section = nullptr;
    for (auto &s : songSections)
    {
        if (s.name == sectionName)
        {
            section = &s;
            break;
        }
    }

    if (!section || section->measures.empty())
    {
        cout << "Section " << sectionName << " is empty or doesn't exist!\n";
        return;
    }

    cout << "\nPlaying Section " << sectionName << "...\n";
    cout << "Press 'p' to pause, 'r' to resume, 's' to stop\n";
    cout << string(25, '=') << "\n";

    stopPlayback = false;
    playbackState = STATE_PLAYING;

    for (const auto &measure : section->measures)
    {
        if (stopPlayback) break;
        
        while (playbackState == STATE_PAUSED) {
            Sleep(100);
            if (stopPlayback) break;
        }
        if (stopPlayback) break;

        cout << "Section " << measure.section << " - Measure " << measure.measureNumber
             << " - " << measure.chord << " (" << measure.duration << "ms): ";
        for (const auto &note : measure.notes)
            cout << note.name << " ";
        cout << "\n";

        // Extract MIDI notes from the measure
        vector<int> midiNotes;
        for (const auto &note : measure.notes) {
            midiNotes.push_back(note.midiNote);
        }

        // Play chord using MIDI
        playMIDIChord(midiNotes);

        // Wait for the duration while checking for user input
        DWORD startTime = GetTickCount();
        while ((GetTickCount() - startTime) < static_cast<DWORD>(measure.duration)) {
            checkPlaybackControl();
            if (stopPlayback || playbackState == STATE_PAUSED) break;
            Sleep(50);
        }

        // Stop the chord
        stopMIDIChord(midiNotes);

        // Small gap between measures.
        Sleep(50);
    }
    
    playbackState = STATE_STOPPED;
    if (!stopPlayback) {
        cout << "Finished Section " << sectionName << "!\n";
    } else {
        cout << "Playback stopped!\n";
    }
}

// Plays all sections in stored order.
void playEntireSong()
{
    if (songSections.empty())
    {
        cout << "No song to play!\n";
        return;
    }

    cout << "\nPlaying Entire Song...\n";
    cout << "Press 'p' to pause, 'r' to resume, 's' to stop\n";
    cout << string(25, '=') << "\n";

    stopPlayback = false;
    playbackState = STATE_PLAYING;

    for (const auto &section : songSections)
    {
        if (stopPlayback) break;
        
        cout << "\n>>> SECTION " << section.name << " <<<\n";
        for (const auto &measure : section.measures)
        {
            if (stopPlayback) break;
            
            while (playbackState == STATE_PAUSED) {
                Sleep(100);
                if (stopPlayback) break;
            }
            if (stopPlayback) break;

            cout << "Measure " << measure.measureNumber << " - " << measure.chord
                 << " (" << measure.duration << "ms): ";
            for (const auto &note : measure.notes)
                cout << note.name << " ";
            cout << "\n";

            // Extract MIDI notes from the measure
            vector<int> midiNotes;
            for (const auto &note : measure.notes) {
                midiNotes.push_back(note.midiNote);
            }

            // Play chord using MIDI
            playMIDIChord(midiNotes);

            // Wait for the duration while checking for user input
            DWORD startTime = GetTickCount();
            while ((GetTickCount() - startTime) < static_cast<DWORD>(measure.duration)) {
                checkPlaybackControl();
                if (stopPlayback || playbackState == STATE_PAUSED) break;
                Sleep(50);
            }

            // Stop the chord
            stopMIDIChord(midiNotes);

            // Small gap between measures.
            Sleep(50);
        }
    }
    
    playbackState = STATE_STOPPED;
    if (!stopPlayback) {
        cout << "\nSong finished!\n";
    } else {
        cout << "\nPlayback stopped!\n";
    }
}

// Creates a new labeled section and switches context to it.
void addNewSection()
{
    cout << "Enter new section name (single character, e.g., A, B, C): ";
    string newSection;
    cin >> newSection;

    if (newSection.length() != 1 || !isalpha(static_cast<unsigned char>(newSection[0])))
    {
        cout << "Invalid section name. Use single letters like A, B, C.\n";
        return;
    }

    newSection[0] = static_cast<char>(toupper(static_cast<unsigned char>(newSection[0])));
    currentSection = newSection;

    for (const auto &section : songSections)
    {
        if (section.name == newSection)
        {
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
void switchSection()
{
    if (songSections.empty())
    {
        cout << "No sections exist yet. Create one first.\n";
        return;
    }

    cout << "Available sections: ";
    for (const auto &section : songSections)
        cout << section.name << " ";
    cout << "\nEnter section to switch to: ";

    string sectionName;
    cin >> sectionName;
    sectionName[0] = static_cast<char>(toupper(static_cast<unsigned char>(sectionName[0])));

    for (const auto &section : songSections)
    {
        if (section.name == sectionName)
        {
            currentSection = sectionName;
            cout << "Switched to Section " << sectionName << "\n";
            return;
        }
    }
    cout << "Section " << sectionName << " not found.\n";
}

// Serializes all sections/measures to a simple line format.
void saveSong()
{
    string filename;
    cout << "Enter filename (or press Enter for song_sheet.txt): ";
    cin.ignore();
    getline(cin, filename);

    if (filename.empty())
    {
        filename = "song_sheet.txt";
    }

    ofstream file(filename);
    if (!file)
    {
        cout << "Error saving song to " << filename << "\n";
        return;
    }

    for (const auto &section : songSections)
    {
        file << "[SECTION " << section.name << "]\n";
        for (const auto &measure : section.measures)
        {
            file << measure.measureNumber << "|" << measure.chord << "|";
            for (size_t i = 0; i < measure.notes.size(); ++i)
            {
                file << measure.notes[i].name;
                if (i + 1 < measure.notes.size())
                    file << ",";
            }
            file << "|" << measure.duration << "\n";
        }
    }
    cout << "Song saved to " << filename << "\n";
}

void loadSong()
{
    string filename;
    cout << "Enter filename (or press Enter for song_sheet.txt): ";
    cin.ignore();
    getline(cin, filename);

    if (filename.empty())
    {
        filename = "song_sheet.txt";
    }

    ifstream file(filename);
    if (!file)
    {
        cout << "Error loading " << filename << "\n";
        return;
    }

    songSections.clear();
    string line;
    MusicSection *currentSectionPtr = nullptr;

    while (getline(file, line))
    {
        if (line.find("[SECTION") == 0)
        {
            MusicSection newSection;
            newSection.name = line.substr(9, 1);
            songSections.push_back(newSection);
            currentSectionPtr = &songSections.back();
        }
        else if (currentSectionPtr && !line.empty())
        {
            stringstream ss(line);
            string part;
            vector<string> parts;
            while (getline(ss, part, '|'))
                parts.push_back(part);
            if (parts.size() >= 4)
            {
                Measure measure;
                measure.measureNumber = stoi(parts[0]);
                measure.chord = parts[1];
                measure.section = currentSectionPtr->name;
                measure.duration = stoi(parts[3]);

                stringstream noteStream(parts[2]);
                string noteName;
                while (getline(noteStream, noteName, ','))
                {
                    if (noteToMidi.find(noteName) != noteToMidi.end())
                    {
                        Note n;
                        n.name = noteName;
                        n.freq = noteFrequencies[noteName];
                        n.midiNote = noteToMidi[noteName];
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
void listAllNotes()
{
    cout << "\nAll Available Notes (Octaves 0-8, format: NoteOctave, e.g., C#4, A5):\n";
    cout << string(70, '=') << "\n";
    vector<string> octaves = {"0", "1", "2", "3", "4", "5", "6", "7", "8"};
    vector<string> noteNames = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    for (const auto &octave : octaves)
    {
        cout << "\nOctave " << octave << ":\n";
        cout << "---------\n";
        for (const auto &noteName : noteNames)
        {
            string fullNote = noteName + octave;
            if (noteToMidi.find(fullNote) != noteToMidi.end())
            {
                cout << fullNote << " (MIDI: " << noteToMidi[fullNote] << ")\t";
                if (noteName == "D#" || noteName == "F#" || noteName == "A#")
                    cout << "\n";
            }
        }
        cout << "\n";
    }
}

// Creates a batch of measures with randomized content in the active section.
void generateRandomSection()
{
    MusicSection *current = getCurrentSection();

    int measureCount;
    cout << "How many measures to generate? (1-20): ";
    cin >> measureCount;

    if (measureCount < 1 || measureCount > 20)
    {
        cout << "Invalid number.\n";
        return;
    }

    srand(static_cast<unsigned>(time(nullptr)));
    vector<string> commonNotes = {"C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5", "D5", "E5"};

    for (int i = 0; i < measureCount; ++i)
    {
        Measure newMeasure;
        newMeasure.measureNumber = static_cast<int>(current->measures.size()) + 1;
        newMeasure.section = currentSection;
        newMeasure.chord = "RAND" + to_string(i + 1);
        newMeasure.duration = 300 + (rand() % 400);

        int noteCount = 1 + (rand() % 4);
        for (int j = 0; j < noteCount; ++j)
        {
            Note n;
            string noteName = commonNotes[rand() % commonNotes.size()];
            n.name = noteName;
            n.freq = noteFrequencies[noteName];
            n.midiNote = noteToMidi[noteName];
            n.duration = newMeasure.duration;
            newMeasure.notes.push_back(n);
        }
        current->measures.push_back(newMeasure);
    }
    cout << "Generated " << measureCount << " random measures in Section " << currentSection << "!\n";
}

// Plays the "Happy Birthday" melody using MIDI
void playHappyBirthday()
{
    cout << "\nPlaying Happy Birthday...\n";
    cout << "Press 'p' to pause, 'r' to resume, 's' to stop\n";
    cout << string(25, '=') << "\n";

    // Happy Birthday melody notes with MIDI note numbers
    vector<pair<int, int>> melody = {
        {67, 250}, {67, 250}, {69, 500}, {67, 500}, {72, 500}, {71, 1000}, 
        {67, 250}, {67, 250}, {69, 500}, {67, 500}, {74, 500}, {72, 1000}, 
        {67, 250}, {67, 250}, {79, 500}, {76, 500}, {72, 500}, {71, 500}, {69, 1000}, 
        {77, 250}, {77, 250}, {76, 500}, {72, 500}, {74, 500}, {72, 500}
    };

    stopPlayback = false;
    playbackState = STATE_PLAYING;

    for (const auto &note : melody)
    {
        if (stopPlayback) break;
        
        while (playbackState == STATE_PAUSED) {
            Sleep(100);
            if (stopPlayback) break;
        }
        if (stopPlayback) break;

        int midiNote = note.first;
        int duration = note.second;

        cout << "Playing: MIDI Note " << midiNote << " (" << duration << "ms)\n";
        
        // Play the note
        playMIDINote(midiNote);
        
        // Wait for the duration while checking for user input
        DWORD startTime = GetTickCount();
        while ((GetTickCount() - startTime) < static_cast<DWORD>(duration)) {
            checkPlaybackControl();
            if (stopPlayback || playbackState == STATE_PAUSED) break;
            Sleep(50);
        }
        
        // Stop the note
        stopMIDINote(midiNote);
        
        Sleep(1); // brief pause between notes
    }
    
    playbackState = STATE_STOPPED;
    if (!stopPlayback) {
        cout << "Finished playing Happy Birthday!\n";
    } else {
        cout << "Playback stopped!\n";
    }
}

// New functions for enhanced features
void showInstruments() {
    cout << "\nAvailable Instruments:\n";
    cout << string(25, '=') << "\n";
    cout << "0. Piano\n";
    cout << "16. Organ\n";
    cout << "24. Guitar\n";
    cout << "32. Bass\n";
    cout << "48. Strings\n";
    cout << "56. Trumpet\n";
    cout << "65. Saxophone\n";
    cout << "73. Flute\n";
    cout << "80. Synth\n";
    cout << string(25, '=') << "\n";
}

void changeInstrument() {
    showInstruments();
    cout << "Enter instrument number: ";
    int instrument;
    cin >> instrument;
    
    if (instrument >= 0 && instrument <= 127) {
        currentInstrument = instrument;
        setInstrument(instrument);
        cout << "Instrument changed to ";
        switch(instrument) {
            case INSTRUMENT_PIANO: cout << "Piano"; break;
            case INSTRUMENT_SYNTH: cout << "Synth"; break;
            case INSTRUMENT_ORGAN: cout << "Organ"; break;
            case INSTRUMENT_GUITAR: cout << "Guitar"; break;
            case INSTRUMENT_BASS: cout << "Bass"; break;
            case INSTRUMENT_STRINGS: cout << "Strings"; break;
            case INSTRUMENT_TRUMPET: cout << "Trumpet"; break;
            case INSTRUMENT_SAXOPHONE: cout << "Saxophone"; break;
            case INSTRUMENT_FLUTE: cout << "Flute"; break;
            default: cout << "Custom (" << instrument << ")";
        }
        cout << "\n";
    } else {
        cout << "Invalid instrument number (0-127)\n";
    }
}

void pausePlayback() {
    if (playbackState == STATE_PLAYING) {
        playbackState = STATE_PAUSED;
        cout << "Playback paused\n";
    } else {
        cout << "No playback to pause\n";
    }
}

void resumePlayback() {
    if (playbackState == STATE_PAUSED) {
        playbackState = STATE_PLAYING;
        cout << "Playback resumed\n";
    } else {
        cout << "No playback to resume\n";
    }
}

void stopPlaybackCommand() {
    stopPlayback = true;
    playbackState = STATE_STOPPED;
    cout << "Playback stopping...\n";
}

void checkPlaybackControl() {
    if (_kbhit()) {
        char ch = _getch();
        switch(tolower(ch)) {
            case 'p': 
                pausePlayback();
                break;
            case 'r':
                resumePlayback();
                break;
            case 's':
                stopPlaybackCommand();
                break;
        }
    }
}