# Audio Test Suite

A audio testing plugin built with JUCE framework, designed for audio signal analysis and automated testing.

## Overview
Audio Test Suite is a versatile audio plugin that provides essential tools for audio testing and analysis. This project serves as a demonstration of audio plugin development skills using modern C++ and the JUCE framework.

## Features
- **Signal Generator**
  - Sine wave generation
  - Square wave generation
  - White/Pink noise generation
- **Analysis Tools**
  - Real-time frequency analysis
  - Level metering
  - Phase correlation
- **Test Automation**
  - Automated test sequences
  - Customizable test parameters
- **Reporting**
  - Detailed test results
  - Export capabilities

## Technical Stack
- JUCE Framework
- C++17
- CMake build system
- VST3/AU plugin formats

## Building from Source

### Prerequisites
- CMake (3.15 or higher)
- C++17 compatible compiler
- Git

### Build Instructions
1. Clone the repository with submodules:
   ```bash
   git clone --recursive https://github.com/attilasdev/audio-test-suite.git
   cd audio-test-suite
   ```

2. Generate build files:
   ```bash
   cmake -B build -S .
   ```

3. Build the plugin:
   ```bash
   cmake --build build
   ```

### Plugin Locations
After building, you can find the plugins in:
- VST3: `build/AudioTestSuite_artefacts/VST3/`
- AU: `build/AudioTestSuite_artefacts/AU/` (macOS only)