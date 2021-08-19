
#pragma once

/*!
 * Add a juce::PluginDescrition to the XML file. If an entry with the uniqueId exists, it will be replaced.
 */
void addToFile (juce::XmlElement& pluginDesc, juce::File& pluginDBfile)
{
    static juce::String uniqueId {"uniqueId"};

    juce::InterProcessLock lockPluginDB (pluginDBfile.getFileName());
    
    auto pluginDBxml = juce::XmlDocument::parse (pluginDBfile);
    if (!pluginDBxml)
        pluginDBxml = std::make_unique<juce::XmlElement>("KnownPlugins");

    auto oldXml = pluginDBxml->getChildByAttribute (uniqueId, pluginDesc.getStringAttribute ("uniqueId"));

    if (oldXml)
        pluginDBxml->replaceChildElement (oldXml, new juce::XmlElement (pluginDesc));
    else
        pluginDBxml->addChildElement (new juce::XmlElement (pluginDesc));

    pluginDBxml->writeTo (pluginDBfile);
}

/*!
 *  Scans a file and adds found plugins to the XML file
 */
void scanFile (juce::AudioPluginFormatManager& formatManager, juce::File& pluginFile, juce::File& pluginDBfile)
{
    DBG ("scanning file: " << pluginFile.getFullPathName());
    static juce::StringArray extensions = {".component", ".vst3", ".dll", ".aaxplugin"};

    if (pluginFile.isDirectory() && !extensions.contains (pluginFile.getFileExtension()))
    {
        auto files = pluginFile.findChildFiles (juce::File::findFilesAndDirectories | juce::File::ignoreHiddenFiles, false);
        for (auto& f : files)
            scanFile (formatManager, f, pluginDBfile);

        return;
    }

    auto filenameOrID = pluginFile.getFullPathName();

    for (auto format : formatManager.getFormats())
    {
        if (format->fileMightContainThisPluginType (filenameOrID))
        {
            juce::OwnedArray<juce::PluginDescription> found;
            format->findAllTypesForFile (found, filenameOrID);

            for (auto* f : found)
            {
                auto xmlDescription = f->createXml();
                addToFile (*xmlDescription, pluginDBfile);
            }
        }
    }
}

/*!
 *  Initiate scan from the CLI arguments
 */
void scan (const juce::ArgumentList& args)
{
    static juce::String pluginDBarg { "--plugin|-p" };
    auto pluginDatabase = args.containsOption (pluginDBarg) ? args.getFileForOption (pluginDBarg) : juce::File();

    juce::AudioPluginFormatManager formatManager;
    formatManager.addDefaultFormats();

    auto pluginFile = args.getFileForOption ("--file|-f");
    
    scanFile (formatManager, pluginFile, pluginDatabase);
}
