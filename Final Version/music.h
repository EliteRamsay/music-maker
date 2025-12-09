#pragma once
#ifndef MUSIC_H
#define MUSIC_H

// Public interface for the terminal music composer.
// Provides MIDI-based playback with multiple instruments and real-time control.

#include <string>
#include <vector>
#include <map>

using namespace std;

// ===== Enums =====
enum MIDIInstruments {
    INSTRUMENT_PIANO = 0,
    INSTRUMENT_ORGAN = 16,
    INSTRUMENT_GUITAR = 24,
    INSTRUMENT_BASS = 32,
    INSTRUMENT_STRINGS = 48,
    INSTRUMENT_TRUMPET = 56,
    INSTRUMENT_SAXOPHONE = 65,
    INSTRUMENT_FLUTE = 73,
    INSTRUMENT_SYNTH = 80
};

enum PlaybackState {
    STATE_STOPPED,
    STATE_PLAYING,
    STATE_PAUSED
};

// ===== Data Structures =====
// Represents a single musical note (e.g., "A4", "C#5")
struct Note {
    string name;      // Note name (e.g., "A4")
    int midiNote;     // MIDI note number (0-127)
    int duration;     // Duration in milliseconds
};

// Represents a musical measure containing one or more notes
struct Measure {
    string chord;           // Chord label (e.g., "CMAJ7", "AMIN")
    vector<Note> notes;     // Notes played in this measure
    int measureNumber;      // Position within the section
    string section;         // Parent section name
    int duration;           // Default duration for notes (ms)
};

// A labeled section of a song (e.g., "A", "B") containing ordered measures
struct MusicSection {
    string name;              // Section label (e.g., "A", "B")
    vector<Measure> measures; // Ordered list of measures
};

// ===== Global State (defined in music.cpp) =====
extern map<string, int> noteToMidi;                          // Maps note names to MIDI numbers
extern map<string, vector<string>> chordDefinitions;         // Predefined chord spellings
extern vector<MusicSection> songSections;                    // All sections in the song
extern string currentSection;                                // Active section name
extern PlaybackState playbackState;                          // Current playback state
extern int currentInstrument;                                // Current MIDI instrument
extern bool stopPlayback;                                    // Flag to stop playback

// ===== MIDI Functions =====
void initMIDI();                                                      // Initialize MIDI system
void closeMIDI();                                                     // Close MIDI system
void setInstrument(int instrument);                                   // Set MIDI instrument
void playMIDINote(int note, int velocity = 127, int channel = 0);    // Play a single MIDI note
void stopMIDINote(int note, int channel = 0);                         // Stop a single MIDI note
void playMIDIChord(const vector<int>& notes, int velocity = 127, int channel = 0);  // Play multiple notes simultaneously
void stopMIDIChord(const vector<int>& notes, int channel = 0);        // Stop multiple notes simultaneously

// ===== Composition Functions =====
void addMeasure();                  // Add a measure by manually entering notes
void addChordByName();              // Add a measure by selecting a predefined chord
void addNewSection();               // Create a new section and switch to it
void switchSection();               // Switch to an existing section
void generateRandomSection();       // Generate random measures in current section

// ===== Playback Functions =====
void playSection(const string& sectionName);  // Play all measures in a specific section
void playEntireSong();                        // Play all sections in order
void playHappyBirthday();                     // Play the Happy Birthday melody
void pausePlayback();                         // Pause current playback
void resumePlayback();                        // Resume paused playback
void stopPlaybackCommand();                   // Stop current playback
void checkPlaybackControl();                  // Check for keyboard input during playback

// ===== UI Functions =====
void showMenu();                    // Display the main menu
void printMusicSheet();             // Print all sections and measures
void listCommonChords();            // List available predefined chords
void listAllNotes();                // List all available notes (octaves 0-8)
void changeInstrument();            // Change the current MIDI instrument
void showInstruments();             // Display available instruments

// ===== File I/O Functions =====
void saveSong();                    // Save song to a text file
void loadSong();                    // Load song from a text file

// ===== Helper Functions =====
MusicSection* getCurrentSection();  // Get pointer to active section (creates if missing)

#endif // MUSIC_H