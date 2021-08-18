
#include <juce_audio_processors/juce_audio_processors.h>


void scanFile (const juce::ArgumentList& args)
{
    juce::AudioPluginFormatManager formatManager;
    formatManager.addDefaultFormats();

    auto pluginFile = args.getFileForOption ("--file|-f");
    auto filenameOrID = pluginFile.getFullPathName();

    for (auto format : formatManager.getFormats())
    {
        if (format->fileMightContainThisPluginType (filenameOrID))
        {
            juce::OwnedArray<juce::PluginDescription> found;
            format->findAllTypesForFile (found, filenameOrID);

            for (auto* f : found)
                DBG (f->createXml()->toString());
        }
    }
}


int main (int argc, char** argv)
{
    juce::ConsoleApplication app;

    app.addHelpCommand ("--help|-h", "Help me!", true);
    app.addVersionCommand ("--version|-v", "AudioPluginScanner version 0.0.1");

    app.addCommand ({"--file", "--file filename", "scans the given file", "",
        [](const juce::ArgumentList& args) { scanFile(args); }});
    
    return app.findAndRunCommand (argc, argv);
}
