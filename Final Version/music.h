#pragma once
#ifndef MUSIC_H
#define MUSIC_H

// Public interface and core data types for the terminal music composer.
// Split into: (1) data structures, (2) globals (declared here, defined in music.cpp),
// and (3) function declarations used by main.cpp and other translation units.

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
#include <mmsystem.h>
#include <conio.h>

#pragma comment(lib, "winmm.lib")

using namespace std;

// MIDI Instrument Constants
enum MIDIInstruments {
    INSTRUMENT_PIANO = 0,
    INSTRUMENT_SYNTH = 80,
    INSTRUMENT_ORGAN = 16,
    INSTRUMENT_GUITAR = 24,
    INSTRUMENT_BASS = 32,
    INSTRUMENT_STRINGS = 48,
    INSTRUMENT_TRUMPET = 56,
    INSTRUMENT_SAXOPHONE = 65,
    INSTRUMENT_FLUTE = 73,
<<<<<<< Updated upstream
    INSTRUMENT_DRUM_KIT = 0  // Channel 10 for drums
=======
    INSTRUMENT_DRUM_KIT = 0,  // Channel 10 for drums
    INSTRUMENT_CHOIR = 52,
    INSTRUMENT_FRENCH_HORN = 60,
    INSTRUMENT_ORCHESTRA_HIT = 55,
    INSTRUMENT_ELECTRIC_BASS = 33,
    INSTRUMENT_CELLO = 42
>>>>>>> Stashed changes
};

// Playback control states
enum PlaybackState {
    STATE_STOPPED,
    STATE_PLAYING,
    STATE_PAUSED
};

<<<<<<< Updated upstream
// Represents a single tone.
// name: symbolic note (e.g., "A4"); freq: frequency in Hz; duration: milliseconds.
=======
// Represents a single tone with channel assignment
>>>>>>> Stashed changes
struct Note {
    string name;
    int freq;
    int duration;
<<<<<<< Updated upstream
    int midiNote;  // MIDI note number (0-127)
};

// Represents a musical measure.
// chord: label for the harmony; notes: tones played in the measure;
// measureNumber: order within the section; section: owning section label;
// duration: default duration for notes in this measure (ms).
=======
    int midiNote;
    int channel;      // MIDI channel (0-15)
    int instrument;   // Instrument for this channel
    int velocity;     // Note volume (0-127)
};

// Represents a musical measure.
>>>>>>> Stashed changes
struct Measure {
    string chord;
    vector<Note> notes;
    int measureNumber;
    string section;
    int duration;
};

<<<<<<< Updated upstream
// A labeled section of a song (e.g., "A", "B") containing ordered measures.
=======
// A labeled section of a song
>>>>>>> Stashed changes
struct MusicSection {
    string name;
    vector<Measure> measures;
};

// ===== Global state (defined in music.cpp) =====
<<<<<<< Updated upstream
// Mapping from note names (e.g., "C#4") to frequencies in Hz.
extern map<string, int> noteFrequencies;
// Mapping from note names to MIDI note numbers
extern map<string, int> noteToMidi;
// Named chord → list of note names that form the chord.
extern map<string, vector<string>> chordDefinitions;
// Ordered collection of all sections in the song.
extern vector<MusicSection> songSections;
// Name of the active section (e.g., "A").
extern string currentSection;
// Current playback state
extern PlaybackState playbackState;
// Current instrument
extern int currentInstrument;
// Flag to stop playback
extern bool stopPlayback;

// ===== MIDI Functions =====
// Initialize MIDI
void initMIDI();
// Close MIDI
void closeMIDI();
// Set MIDI instrument
void setInstrument(int instrument);
// Play a MIDI note
void playMIDINote(int note, int velocity = 127, int channel = 0);
// Stop a MIDI note
void stopMIDINote(int note, int channel = 0);
// Play multiple MIDI notes simultaneously
void playMIDIChord(const vector<int>& notes, int velocity = 127, int channel = 0);
// Stop multiple MIDI notes simultaneously
void stopMIDIChord(const vector<int>& notes, int channel = 0);

// ===== UI / Control =====
// Prints the text menu and shows current section.
void showMenu();

// Returns pointer to the active section; creates it if missing.
MusicSection* getCurrentSection();

// Pretty-prints all sections and measures to stdout.
void printMusicSheet();

// Lists supported chord names available for quick entry.
void listCommonChords();

// Adds a full measure by selecting a known chord name.
void addChordByName();

// Adds a measure by manually entering chord, duration, and notes.
void addMeasure();

// Plays a single note or rests for duration if frequency is out of range.
void playNote(int frequency, int duration);

// Plays all measures in the requested section by name.
void playSection(const string& sectionName);

// Plays all sections in order.
void playEntireSong();

// Creates a new section (e.g., "B") and switches to it.
void addNewSection();

// Switches currentSection to an existing section.
void switchSection();

// Saves all sections/measures to a simple text file.
void saveSong();

// Loads sections/measures from a previously saved text file.
void loadSong();

// Lists a curated range of notes with their frequencies.
void listAllNotes();

// Generates a number of random measures in the current section.
void generateRandomSection();

void playHappyBirthday();

=======
extern map<string, int> noteFrequencies;
extern map<string, int> noteToMidi;
extern map<string, vector<string>> chordDefinitions;
extern vector<MusicSection> songSections;
extern string currentSection;
extern PlaybackState playbackState;
extern int currentInstrument;
extern bool stopPlayback;
extern map<int, int> channelInstruments;  // channel -> instrument

// ===== MIDI Functions =====
void initMIDI();
void closeMIDI();
void setInstrument(int instrument);
void setInstrumentOnChannel(int instrument, int channel);
void playMIDINote(int note, int velocity = 127, int channel = 0);
void stopMIDINote(int note, int channel = 0);
void playMIDIChord(const vector<int>& notes, int velocity = 127, int channel = 0);
void stopMIDIChord(const vector<int>& notes, int channel = 0);

// ===== UI / Control =====
void showMenu();
MusicSection* getCurrentSection();
void printMusicSheet();
void listCommonChords();
void addChordByName();
void addMeasure();
void playNote(int frequency, int duration);
void playSection(const string& sectionName);
void playEntireSong();
void addNewSection();
void switchSection();
void saveSong();
void loadSong();
void listAllNotes();
void generateRandomSection();
void playHappyBirthday();

>>>>>>> Stashed changes
// New functions for enhanced features
void changeInstrument();
void showInstruments();
void pausePlayback();
void resumePlayback();
void stopPlaybackCommand();
void checkPlaybackControl();
<<<<<<< Updated upstream
=======
void setupChannelInstruments();
void showChannelStatus();
void addMultiInstrumentMeasure();
>>>>>>> Stashed changes

#endif // MUSIC_H