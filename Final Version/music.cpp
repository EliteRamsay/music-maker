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

<<<<<<< Updated upstream
=======
// Multi-instrument support
map<int, int> channelInstruments;  // channel -> instrument
map<string, int> instrumentNames = {
    {"PIANO", 0}, {"ORGAN", 16}, {"GUITAR", 24}, {"BASS", 32},
    {"STRINGS", 48}, {"TRUMPET", 56}, {"SAX", 65}, {"FLUTE", 73},
    {"CHOIR", 52}, {"FRENCHHORN", 60}, {"ORCHESTRAHIT", 55}, {"SYNTH", 80},
    {"ELECTRICBASS", 33}, {"CELLO", 42}, {"DRUMS", 0}
};

>>>>>>> Stashed changes
// ===== MIDI Implementation =====
void initMIDI() {
    if (hMidiOut == NULL) {
        midiOutOpen(&hMidiOut, 0, 0, 0, CALLBACK_NULL);
        setupChannelInstruments();
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

<<<<<<< Updated upstream
=======
void setInstrumentOnChannel(int instrument, int channel) {
    if (hMidiOut != NULL) {
        midiOutShortMsg(hMidiOut, 0xC0 | channel | (instrument << 8));
    }
}

void setupChannelInstruments() {
    // Default setup for common channels
    channelInstruments[0] = INSTRUMENT_PIANO;    // Main melody
    channelInstruments[1] = INSTRUMENT_STRINGS;  // Harmony
    channelInstruments[2] = INSTRUMENT_ELECTRIC_BASS; // Bass line
    channelInstruments[3] = INSTRUMENT_TRUMPET;  // Brass
    channelInstruments[4] = INSTRUMENT_SAXOPHONE; // Woodwinds
    channelInstruments[9] = INSTRUMENT_DRUM_KIT; // Drums (channel 10)
    
    // Initialize all channels
    for (auto& pair : channelInstruments) {
        setInstrumentOnChannel(pair.second, pair.first);
    }
}

>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
// Named chord spellings used to quickly build a measure.
=======
// Named chord spellings
>>>>>>> Stashed changes
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
    {"B7", {"B4", "D#5", "F#5", "A5"}}
};

// ===== UI / Menu =====
void showMenu()
{
<<<<<<< Updated upstream
    // Displays available operations and current context.
    cout << "\n==== C++ Terminal Music Composer (MIDI Enhanced) ====\n";
=======
    cout << "\n==== C++ Terminal Music Composer (Multi-Instrument) ====\n";
>>>>>>> Stashed changes
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
    cout << "18. Add multi-instrument measure\n";
    cout << "19. Show channel status\n";
    cout << "20. Setup channel instruments\n";
    cout << "21. Exit\n";
    cout << string(35, '-') << "\n";
    cout << "Current Section: " << currentSection << "\n";
    cout << "Active channels: ";
    for (const auto& pair : channelInstruments) {
        cout << pair.first << " ";
    }
    cout << "\nChoice: ";
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
    cout << string(60, '=') << "\n";
    cout << "               MULTI-INSTRUMENT MUSIC SHEET\n";
    cout << string(60, '=') << "\n";
    
    for (const auto &section : songSections)
    {
        cout << "\nSection " << section.name << "\n";
        cout << string(80, '-') << "\n";
        cout << left << setw(8) << "Measure" << setw(12) << "Chord" << setw(10) << "Duration" << "Notes (Channel:Instrument)\n";
        cout << string(80, '-') << "\n";

        for (const auto &measure : section.measures)
        {
            cout << setw(8) << measure.measureNumber
                 << setw(12) << measure.chord
                 << setw(10) << (to_string(measure.duration) + "ms");
            
            // Group notes by channel for display
            map<int, vector<string>> notesByChannel;
            map<int, int> instrumentsByChannel;
            
            for (const auto &note : measure.notes) {
                notesByChannel[note.channel].push_back(note.name);
                instrumentsByChannel[note.channel] = note.instrument;
            }
            
            bool firstChannel = true;
            for (const auto& channelEntry : notesByChannel) {
                if (!firstChannel) cout << " | ";
                cout << "Ch" << channelEntry.first << "(";
                
                // Show instrument name
                int instr = instrumentsByChannel[channelEntry.first];
                switch(instr) {
                    case INSTRUMENT_PIANO: cout << "Piano"; break;
                    case INSTRUMENT_STRINGS: cout << "Strings"; break;
                    case INSTRUMENT_BASS: cout << "Bass"; break;
                    case INSTRUMENT_TRUMPET: cout << "Trumpet"; break;
                    default: cout << "Instr" << instr;
                }
                cout << "): ";
                
                for (size_t i = 0; i < channelEntry.second.size(); ++i) {
                    cout << channelEntry.second[i];
                    if (i + 1 < channelEntry.second.size()) cout << ",";
                }
                firstChannel = false;
            }
            cout << "\n";
        }
    }
    cout << string(60, '=') << "\n";
}

<<<<<<< Updated upstream
=======
void showChannelStatus() {
    cout << "\n=== Channel Status ===\n";
    cout << string(25, '=') << "\n";
    for (const auto& pair : channelInstruments) {
        cout << "Channel " << pair.first << ": ";
        switch(pair.second) {
            case INSTRUMENT_PIANO: cout << "Piano"; break;
            case INSTRUMENT_SYNTH: cout << "Synth"; break;
            case INSTRUMENT_ORGAN: cout << "Organ"; break;
            case INSTRUMENT_GUITAR: cout << "Guitar"; break;
            case INSTRUMENT_BASS: cout << "Bass"; break;
            case INSTRUMENT_STRINGS: cout << "Strings"; break;
            case INSTRUMENT_TRUMPET: cout << "Trumpet"; break;
            case INSTRUMENT_SAXOPHONE: cout << "Saxophone"; break;
            case INSTRUMENT_FLUTE: cout << "Flute"; break;
            case INSTRUMENT_CHOIR: cout << "Choir"; break;
            case INSTRUMENT_FRENCH_HORN: cout << "French Horn"; break;
            case INSTRUMENT_ORCHESTRA_HIT: cout << "Orchestra Hit"; break;
            case INSTRUMENT_ELECTRIC_BASS: cout << "Electric Bass"; break;
            case INSTRUMENT_CELLO: cout << "Cello"; break;
            default: cout << "Instrument " << pair.second;
        }
        cout << "\n";
    }
}

>>>>>>> Stashed changes
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
        n.channel = 0;  // Default to channel 0
        n.instrument = channelInstruments[0];
        n.velocity = 100;
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

    // Normalize letter case
    for (size_t i = 0; i < input.length(); ++i)
    {
        if (isalpha(static_cast<unsigned char>(input[i])) && input[i] != '#')
        {
            input[i] = static_cast<char>(toupper(static_cast<unsigned char>(input[i])));
        }
    }

    // Split and validate note tokens
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
            n.channel = 0;  // Default channel
            n.instrument = channelInstruments[0];
            n.velocity = 100;
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

<<<<<<< Updated upstream
// Plays a single tone using MIDI; otherwise sleeps for the duration.
void playNote(int frequency, int duration)
{
    // For compatibility, we'll use the MIDI system
    // This function is kept for backward compatibility
    Sleep(static_cast<DWORD>(duration));
}

// Iterates measures in a named section and plays notes (chord tones in parallel).
=======
// ===== Multi-instrument measure creation =====
void addMultiInstrumentMeasure()
{
    MusicSection* current = getCurrentSection();
    Measure newMeasure;
    newMeasure.measureNumber = static_cast<int>(current->measures.size()) + 1;
    newMeasure.section = currentSection;

    cout << "\nAdding Multi-Instrument Measure " << newMeasure.measureNumber << "\n";
    
    cout << "Enter chord name (e.g., ORCHESTRAL_CHORD): ";
    cin.ignore();
    getline(cin, newMeasure.chord);
    
    cout << "Enter duration in milliseconds: ";
    cin >> newMeasure.duration;
    cin.ignore();
    
    cout << "How many instrument parts? (1-8): ";
    int parts;
    cin >> parts;
    cin.ignore();
    
    for (int p = 0; p < parts; p++) {
        cout << "\n--- Part " << (p + 1) << " ---\n";
        
        // Show current channel assignments
        cout << "Current channel assignments:\n";
        showChannelStatus();
        
        cout << "Enter channel for this part (0-15, 9=drums, or -1 for new channel): ";
        int channel;
        cin >> channel;
        cin.ignore();
        
        if (channel == -1) {
            // Find next available channel
            for (int ch = 0; ch < 16; ch++) {
                if (ch != 9 && channelInstruments.find(ch) == channelInstruments.end()) {
                    channel = ch;
                    break;
                }
            }
            if (channel == -1) {
                cout << "No available channels!\n";
                continue;
            }
            
            cout << "Using new channel " << channel << "\n";
            cout << "Select instrument number (0-127): ";
            showInstruments();
            int instrument;
            cin >> instrument;
            cin.ignore();
            
            channelInstruments[channel] = instrument;
            setInstrumentOnChannel(instrument, channel);
        }
        
        // Get instrument if channel exists
        int instrument = channelInstruments[channel];
        
        cout << "Enter notes for this part (comma-separated, e.g., D4,F#4,A4): ";
        string input;
        getline(cin, input);
        
        // Process notes for this channel
        stringstream ss(input);
        string noteName;
        while (getline(ss, noteName, ',')) {
            noteName.erase(remove_if(noteName.begin(), noteName.end(), ::isspace), noteName.end());
            if (noteToMidi.find(noteName) != noteToMidi.end()) {
                Note n;
                n.name = noteName;
                n.freq = noteFrequencies[noteName];
                n.midiNote = noteToMidi[noteName];
                n.duration = newMeasure.duration;
                n.channel = channel;
                n.instrument = instrument;
                n.velocity = 100 + (rand() % 27); // Some dynamics (100-127)
                newMeasure.notes.push_back(n);
            } else {
                cout << "Invalid note: " << noteName << " - skipping.\n";
            }
        }
    }
    
    if (newMeasure.notes.empty()) {
        cout << "No valid notes entered. Measure not added.\n";
        return;
    }
    
    current->measures.push_back(newMeasure);
    cout << "Added multi-instrument measure with " << newMeasure.notes.size() << " notes!\n";
}

// ===== PLAYBACK FUNCTIONS (Updated for multi-instrument) =====
void playNote(int frequency, int duration)
{
    // For compatibility
    Sleep(static_cast<DWORD>(duration));
}

// COMPLETE playSection() with multi-instrument support
>>>>>>> Stashed changes
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
    cout << "Active channels: ";
    for (const auto& pair : channelInstruments) {
        cout << pair.first << " ";
    }
    cout << "\n" << string(40, '=') << "\n";

    stopPlayback = false;
    playbackState = STATE_PLAYING;

    // Ensure all channels have their instruments set
    for (auto& pair : channelInstruments) {
        setInstrumentOnChannel(pair.second, pair.first);
    }

    for (const auto &measure : section->measures)
    {
        if (stopPlayback) break;
        
        // Handle pause state
        while (playbackState == STATE_PAUSED) {
            Sleep(50);
            checkPlaybackControl();
            if (stopPlayback) break;
        }
        if (stopPlayback) break;

        // Display measure info
        cout << "\nMeasure " << measure.measureNumber << " - " << measure.chord 
             << " (" << measure.duration << "ms)\n";
        
        // Group notes by channel for better display
        map<int, vector<string>> notesByChannel;
        for (const auto &note : measure.notes) {
            notesByChannel[note.channel].push_back(note.name);
        }
        
        // Display notes by channel
        for (const auto& channelEntry : notesByChannel) {
            int channel = channelEntry.first;
            cout << "  Ch" << channel << ": ";
            for (size_t i = 0; i < channelEntry.second.size(); ++i) {
                cout << channelEntry.second[i];
                if (i + 1 < channelEntry.second.size()) cout << ",";
            }
            cout << "\n";
        }

        // Play all notes in the measure (each on its own channel)
        for (const auto &note : measure.notes) {
            playMIDINote(note.midiNote, note.velocity, note.channel);
        }

        // Wait for the duration while checking for user input
        DWORD startTime = GetTickCount();
        while ((GetTickCount() - startTime) < static_cast<DWORD>(measure.duration)) {
            checkPlaybackControl();
            if (stopPlayback || playbackState == STATE_PAUSED) break;
            Sleep(50);
        }

        // Stop all notes in the measure
        for (const auto &note : measure.notes) {
            stopMIDINote(note.midiNote, note.channel);
        }

        // Small gap between measures
        Sleep(50);
    }
    
    // Turn off any lingering notes
    for (const auto &measure : section->measures) {
        for (const auto &note : measure.notes) {
            stopMIDINote(note.midiNote, note.channel);
        }
    }
    
    playbackState = STATE_STOPPED;
    if (!stopPlayback) {
        cout << "\n" << string(25, '=') << "\n";
        cout << "Finished Section " << sectionName << "!\n";
    } else {
        cout << "\nPlayback stopped!\n";
    }
}

<<<<<<< Updated upstream
// Plays all sections in stored order.
=======
// COMPLETE playEntireSong() with multi-instrument support
>>>>>>> Stashed changes
void playEntireSong()
{
    if (songSections.empty())
    {
        cout << "No song to play!\n";
        return;
    }

    cout << "\nPlaying Entire Song...\n";
    cout << "Press 'p' to pause, 'r' to resume, 's' to stop\n";
    cout << "Active channels: ";
    for (const auto& pair : channelInstruments) {
        cout << pair.first << " ";
    }
    cout << "\n" << string(40, '=') << "\n";

    stopPlayback = false;
    playbackState = STATE_PLAYING;

    // Ensure all channels have their instruments set
    for (auto& pair : channelInstruments) {
        setInstrumentOnChannel(pair.second, pair.first);
    }

    for (const auto &section : songSections)
    {
        if (stopPlayback) break;
        
        cout << "\n>>> SECTION " << section.name << " <<<\n";
        
        for (const auto &measure : section.measures)
        {
            if (stopPlayback) break;
            
            while (playbackState == STATE_PAUSED) {
                Sleep(50);
                checkPlaybackControl();
                if (stopPlayback) break;
            }
            if (stopPlayback) break;

            // Display measure info
            cout << "\nMeasure " << measure.measureNumber << " - " << measure.chord 
                 << " (" << measure.duration << "ms)\n";
            
            // Group notes by channel for better display
            map<int, vector<string>> notesByChannel;
            for (const auto &note : measure.notes) {
                notesByChannel[note.channel].push_back(note.name);
            }
            
            // Display notes by channel
            for (const auto& channelEntry : notesByChannel) {
                int channel = channelEntry.first;
                cout << "  Ch" << channel << ": ";
                for (size_t i = 0; i < channelEntry.second.size(); ++i) {
                    cout << channelEntry.second[i];
                    if (i + 1 < channelEntry.second.size()) cout << ",";
                }
                cout << "\n";
            }

            // Play all notes in the measure (each on its own channel)
            for (const auto &note : measure.notes) {
                playMIDINote(note.midiNote, note.velocity, note.channel);
            }

            // Wait for the duration while checking for user input
            DWORD startTime = GetTickCount();
            while ((GetTickCount() - startTime) < static_cast<DWORD>(measure.duration)) {
                checkPlaybackControl();
                if (stopPlayback || playbackState == STATE_PAUSED) break;
                Sleep(50);
            }

            // Stop all notes in the measure
            for (const auto &note : measure.notes) {
                stopMIDINote(note.midiNote, note.channel);
            }

            // Small gap between measures
            Sleep(50);
        }
    }
    
    // Turn off any lingering notes
    for (const auto &section : songSections) {
        for (const auto &measure : section.measures) {
            for (const auto &note : measure.notes) {
                stopMIDINote(note.midiNote, note.channel);
            }
        }
    }
    
    playbackState = STATE_STOPPED;
    if (!stopPlayback) {
        cout << "\n" << string(25, '=') << "\n";
        cout << "Song finished!\n";
    } else {
        cout << "\nPlayback stopped!\n";
    }
}

// Creates a new labeled section and switches context to it.
void addNewSection()
{
    cout << "Enter new section name (e.g., INTRO, VERSE, CHORUS): ";
    string newSection;
    cin >> newSection;

    if (newSection.empty())
    {
        cout << "Invalid section name.\n";
        return;
    }

    // Convert to uppercase
    for (char &c : newSection) {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }
    
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
    
    // Convert to uppercase
    for (char &c : sectionName) {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }

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

<<<<<<< Updated upstream
// Serializes all sections/measures to a simple line format.
=======
// Serializes all sections/measures to multi-instrument format
>>>>>>> Stashed changes
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
            
            // Group notes by channel and instrument
            map<pair<int, int>, vector<string>> channelNotes;
            for (const auto &note : measure.notes) {
                pair<int, int> key = {note.channel, note.instrument};
                channelNotes[key].push_back(note.name);
            }
            
            // Write each channel's notes
            bool firstChannel = true;
            for (const auto &entry : channelNotes) {
                if (!firstChannel) file << ";";
                file << entry.first.first << ":" << entry.first.second << ":";
                for (size_t i = 0; i < entry.second.size(); ++i) {
                    file << entry.second[i];
                    if (i + 1 < entry.second.size())
                        file << ",";
                }
                firstChannel = false;
            }
            
            file << "|" << measure.duration << "\n";
        }
    }
    cout << "Song saved to " << filename << " (multi-instrument format)\n";
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
    channelInstruments.clear(); // Clear old channel assignments
    string line;
    MusicSection *currentSectionPtr = nullptr;

    while (getline(file, line))
    {
        if (line.find("[SECTION") == 0)
        {
            // Extract section name (supports multi-character names)
            size_t start = line.find(' ') + 1;
            size_t end = line.find(']');
            string sectionName = line.substr(start, end - start);
            
            MusicSection newSection;
            newSection.name = sectionName;
            songSections.push_back(newSection);
            currentSectionPtr = &songSections.back();
        }
        else if (currentSectionPtr && !line.empty())
        {
            stringstream ss(line);
            string part;
            vector<string> parts;
            
            // Split by '|'
            while (getline(ss, part, '|')) {
                parts.push_back(part);
            }
            
            if (parts.size() >= 4)
            {
                Measure measure;
                measure.measureNumber = stoi(parts[0]);
                measure.chord = parts[1];
                measure.section = currentSectionPtr->name;
                measure.duration = stoi(parts[3]);
                
                // Parse multi-channel instrument data
                stringstream channelStream(parts[2]);
                string channelPart;
                
                while (getline(channelStream, channelPart, ';'))
                {
                    // Each channelPart format: channel:instrument:notes
                    stringstream channelDetail(channelPart);
                    string channelStr, instrumentStr, notesStr;
                    
                    getline(channelDetail, channelStr, ':');
                    getline(channelDetail, instrumentStr, ':');
                    getline(channelDetail, notesStr, ':');
                    
                    int channel = stoi(channelStr);
                    int instrument = stoi(instrumentStr);
                    
                    // Store instrument for this channel
                    channelInstruments[channel] = instrument;
                    
                    // Parse notes for this channel
                    stringstream noteStream(notesStr);
                    string noteName;
                    
                    while (getline(noteStream, noteName, ','))
                    {
<<<<<<< Updated upstream
                        Note n;
                        n.name = noteName;
                        n.freq = noteFrequencies[noteName];
                        n.midiNote = noteToMidi[noteName];
                        n.duration = measure.duration;
                        measure.notes.push_back(n);
=======
                        if (noteToMidi.find(noteName) != noteToMidi.end())
                        {
                            Note n;
                            n.name = noteName;
                            n.freq = noteFrequencies[noteName];
                            n.midiNote = noteToMidi[noteName];
                            n.duration = measure.duration;
                            n.channel = channel;
                            n.instrument = instrument;
                            n.velocity = 100; // Default velocity
                            measure.notes.push_back(n);
                        }
>>>>>>> Stashed changes
                    }
                }
                
                currentSectionPtr->measures.push_back(measure);
            }
        }
    }
    
    // Set instruments on all loaded channels
    for (auto& pair : channelInstruments) {
        setInstrumentOnChannel(pair.second, pair.first);
    }
    
    cout << "Song loaded from " << filename << "! " << songSections.size() << " sections.\n";
    cout << "Instruments loaded on " << channelInstruments.size() << " channels.\n";
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

        // Randomly assign 1-3 channels
        int channelCount = 1 + (rand() % 3);
        for (int ch = 0; ch < channelCount; ++ch)
        {
<<<<<<< Updated upstream
            Note n;
            string noteName = commonNotes[rand() % commonNotes.size()];
            n.name = noteName;
            n.freq = noteFrequencies[noteName];
            n.midiNote = noteToMidi[noteName];
            n.duration = newMeasure.duration;
            newMeasure.notes.push_back(n);
=======
            int channel = ch;
            if (channel == 9) channel = 10; // Skip drum channel
            
            int noteCount = 1 + (rand() % 3);
            for (int j = 0; j < noteCount; ++j)
            {
                Note n;
                string noteName = commonNotes[rand() % commonNotes.size()];
                n.name = noteName;
                n.freq = noteFrequencies[noteName];
                n.midiNote = noteToMidi[noteName];
                n.duration = newMeasure.duration;
                n.channel = channel;
                n.instrument = channelInstruments[channel];
                n.velocity = 80 + (rand() % 47);
                newMeasure.notes.push_back(n);
            }
>>>>>>> Stashed changes
        }
        current->measures.push_back(newMeasure);
    }
    cout << "Generated " << measureCount << " random multi-instrument measures in Section " << currentSection << "!\n";
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
            Sleep(50);
            checkPlaybackControl();
            if (stopPlayback) break;
        }
        if (stopPlayback) break;

        int midiNote = note.first;
        int duration = note.second;

        cout << "Playing: MIDI Note " << midiNote << " (" << duration << "ms)\n";
        
        // Play the note on channel 0 (piano)
        playMIDINote(midiNote, 100, 0);
        
        // Wait for the duration while checking for user input
        DWORD startTime = GetTickCount();
        while ((GetTickCount() - startTime) < static_cast<DWORD>(duration)) {
            checkPlaybackControl();
            if (stopPlayback || playbackState == STATE_PAUSED) break;
            Sleep(50);
        }
        
        // Stop the note
        stopMIDINote(midiNote, 0);
        
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
    cout << "42. Cello\n";
    cout << "48. Strings\n";
    cout << "52. Choir\n";
    cout << "55. Orchestra Hit\n";
    cout << "56. Trumpet\n";
    cout << "60. French Horn\n";
    cout << "65. Saxophone\n";
    cout << "73. Flute\n";
    cout << "80. Synth\n";
    cout << string(25, '=') << "\n";
    cout << "Note: Channel 9 is reserved for Drums\n";
}

void changeInstrument() {
    cout << "\nChange instrument on which channel? ";
    showChannelStatus();
    cout << "Enter channel number: ";
    int channel;
    cin >> channel;
    
    if (channelInstruments.find(channel) == channelInstruments.end()) {
        cout << "Channel " << channel << " not in use. Add it first with multi-instrument measure.\n";
        return;
    }
    
    showInstruments();
    cout << "Enter new instrument number (0-127): ";
    int instrument;
    cin >> instrument;
    
    if (instrument >= 0 && instrument <= 127) {
        channelInstruments[channel] = instrument;
        setInstrumentOnChannel(instrument, channel);
        cout << "Channel " << channel << " changed to instrument " << instrument << "\n";
    } else {
        cout << "Invalid instrument number (0-127)\n";
    }
}

void pausePlayback() {
    if (playbackState == STATE_PLAYING) {
        playbackState = STATE_PAUSED;
        cout << "Playback paused\n";
        
        // Stop all notes when pausing
        for (const auto& section : songSections) {
            for (const auto& measure : section.measures) {
                for (const auto& note : measure.notes) {
                    stopMIDINote(note.midiNote, note.channel);
                }
            }
        }
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
    
    // Stop all notes immediately
    for (const auto& section : songSections) {
        for (const auto& measure : section.measures) {
            for (const auto& note : measure.notes) {
                stopMIDINote(note.midiNote, note.channel);
            }
        }
    }
    
    cout << "Playback stopped\n";
}

void checkPlaybackControl() {
    if (_kbhit()) {
        char ch = _getch();
        switch(ch) {
            case 'p': 
            case 'P':
                pausePlayback();
                break;
            case 'r':
            case 'R':
                resumePlayback();
                break;
            case 's':
            case 'S':
                stopPlaybackCommand();
                break;
        }
    }
}