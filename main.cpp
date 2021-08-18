
#include <juce_audio_processors/juce_audio_processors.h>

int main (int argc, char** argv)
{
    juce::AudioPluginFormatManager formatManager;
    formatManager.addDefaultFormats();
        
    auto filenameOrID = "/Users/daniel/Library/Audio/Plug-Ins/Components/PGM-Equalizer.component";

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


    return 0;
}
