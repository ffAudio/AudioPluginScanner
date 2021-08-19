
#include <juce_audio_processors/juce_audio_processors.h>

#include "scanFile.h"



/*!
 * Extra process to scan an audio plugin and register it with the database
 */
int main (int argc, char** argv)
{
    juce::ScopedJuceInitialiser_GUI juceStuff;

    juce::ConsoleApplication app;

    app.addHelpCommand ("--help|-h", "Scans an audio plugin file and registers it with the database", true);
    app.addVersionCommand ("--version|-v", "AudioPluginScanner version 0.0.1");

    app.addCommand ({"--file|-f", "--file filename", "scans the given file", "",
        [](const juce::ArgumentList& args) { scan (args); }});

    return app.findAndRunCommand (argc, argv);
}
