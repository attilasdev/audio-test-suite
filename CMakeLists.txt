cmake_minimum_required(VERSION 3.15)
project(AudioTestSuite VERSION 1.0.0)

# Add JUCE as a subdirectory
add_subdirectory(JUCE)

# Enable JUCE modules we need
juce_add_plugin(AudioTestSuite
    PLUGIN_MANUFACTURER_CODE Ytst
    PLUGIN_CODE Atst
    FORMATS AU VST3
    PRODUCT_NAME "Audio Test Suite"
    COMPANY_NAME "YourCompany"
)

# Add all source files
target_sources(AudioTestSuite
    PRIVATE
        # Main Plugin Files
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        
        # Components
        Source/Components/AnalyzerComponent.cpp
        Source/Components/SignalGeneratorComponent.cpp
        
        # Test Suite
        Source/TestSuite/SignalGenerator.cpp
        Source/TestSuite/AudioAnalyzer.cpp
        Source/TestSuite/TestRunner.cpp
        
        # Utils
        Source/Utils/FFTWrapper.cpp
        Source/Utils/ReportGenerator.cpp
)

target_compile_options(AudioTestSuite PRIVATE
    $<$<CONFIG:Release>:-O3 -ffast-math -march=native>
    $<$<PLATFORM_ID:Darwin>:-mcpu=apple-m1>  # Optimize for Apple Silicon
)

# Enable multi-threading
find_package(Threads REQUIRED)
target_link_libraries(AudioTestSuite PRIVATE Threads::Threads)

# Enable link-time optimization
set_target_properties(AudioTestSuite PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION TRUE
)

# Add JUCE modules
target_compile_features(AudioTestSuite PRIVATE cxx_std_17)

target_link_libraries(AudioTestSuite
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Generate JuceHeader.h
juce_generate_juce_header(AudioTestSuite)

# Optional: Add testing support
# include(CTest)
# add_subdirectory(Tests)