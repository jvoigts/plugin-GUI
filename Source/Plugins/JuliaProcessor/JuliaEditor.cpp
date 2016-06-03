/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2016 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


// start with  sudo LD_LIBRARY_PATH=/home/oe/Documents/julia/lib/julia/ ./build/open-ephys 

#include "JuliaEditor.h"
#include "JuliaProcessor.h"
#include <stdio.h>
 
JuliaEditor::JuliaEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors=true)
    : VisualizerEditor(parentNode, useDefaultParameterEditors)
{
    juliaProcessor = (JuliaProcessor*) parentNode;

    tabText = "Julia plugin image display";

    lastFilePath = File::getCurrentWorkingDirectory();

    fileButton = new UtilityButton("Select file",Font("Small Text", 13, Font::plain));
    fileButton->addListener(this);
    fileButton->setBounds(10,85,100,25);
    addAndMakeVisible(fileButton);

    reloadFileButton = new UtilityButton("refresh",Font("Small Text", 13, Font::plain));
    reloadFileButton->addListener(this);
    reloadFileButton->setBounds(100+10,85,60,25);
    addAndMakeVisible(reloadFileButton);

    fileNameLabel = new Label("FileNameLabel", "No file selected.");
    fileNameLabel->setBounds(10,85+20,140,25);
    addAndMakeVisible(fileNameLabel);

    outputImageSizeSelectionW = new Label("Out Im. Size","256"); 
    outputImageSizeSelectionW->setEditable(true,false,false);
    outputImageSizeSelectionW->addListener(this);
    outputImageSizeSelectionW->setBounds(90,60,40,20);
    outputImageSizeSelectionW->setColour(Label::textColourId, Colours::black);
    addAndMakeVisible(outputImageSizeSelectionW);

    outputImageSizeSelectionWLabel = new Label("","OutImSize:");
    outputImageSizeSelectionWLabel->attachToComponent (outputImageSizeSelectionW,true);
    addAndMakeVisible(outputImageSizeSelectionWLabel);

    outputImageSizeSelectionH = new Label("Out Im. Size","256"); 
    outputImageSizeSelectionH->setEditable(true,false,false);
    outputImageSizeSelectionH->addListener(this);
    outputImageSizeSelectionH->setBounds(140,60,40,20);
    outputImageSizeSelectionH->setColour(Label::textColourId, Colours::black);
    addAndMakeVisible(outputImageSizeSelectionH);

    outputImageSizeSelectionHLabel = new Label("","X");
    outputImageSizeSelectionHLabel->attachToComponent (outputImageSizeSelectionH,true);
    addAndMakeVisible(outputImageSizeSelectionHLabel);


    // Image im;
    // im = ImageCache::getFromMemory(BinaryData::JuliaIconActive_png,
    //                                BinaryData::JuliaIconActive_pngSize);

    // icon = new ImageIcon(im);
    // addAndMakeVisible(icon);
    // icon->setBounds(15,25,61,54);
    // icon->setOpacity(0.3f);

    desiredWidth = 200;

    setEnabledState(false);
}

JuliaEditor::~JuliaEditor()
{

}


Visualizer* JuliaEditor::createNewCanvas()
{
    
    JuliaProcessor* processor = (JuliaProcessor*) getProcessor();
    canvas = new JuliaEditorCanvas(processor);
    return canvas;
    
}


void JuliaEditor::setFile(String file)
{
    File fileToRead(file);
    lastFilePath = fileToRead.getParentDirectory();
    juliaProcessor->setFile(fileToRead.getFullPathName());
    fileNameLabel->setText(fileToRead.getFileName(), dontSendNotification);

    // setEnabledState(true);
    // icon->setOpacity(1.0f); // tie this to hasJuliaInstance instead of just setting it!
    // repaint();
}

void JuliaEditor::buttonCallback(Button* button)
{
    if (!acquisitionIsActive)
    {
        if (button == fileButton)
        {
            //std::cout << "Button clicked." << std::endl;
            FileChooser chooseJuliaProcessorFile("Please select the file you want to load...",  lastFilePath, "*");

                // file dialogs are screwed up in current xubuntu, so we'll do this for now.
                //setFile("/home/oe/Documents/plugin-GUI/Source/Plugins/JuliaProcessor/BOimg/spiketostim_oe.jl");

            if (chooseJuliaProcessorFile.browseForFileToOpen())
            {
                // Use the selected file
                setFile(chooseJuliaProcessorFile.getResult().getFullPathName());                
               
            }
        }

        if (button == reloadFileButton)
        {
            juliaProcessor->reloadFile();
        }
    }
}

void JuliaEditor::labelTextChanged(Label* label)
{
    if (!acquisitionIsActive)
    {
        if (label == outputImageSizeSelectionW)
        {
            Value val = label->getTextValue();
            juliaProcessor->setOutputImageSize( int(val.getValue()), 0 );
        }
        if (label == outputImageSizeSelectionH)
        {
            Value val = label->getTextValue();
            juliaProcessor->setOutputImageSize( 0, int(val.getValue()) );
        }

            resized();
            repaint();
    }
}

void JuliaEditor::saveEditorParameters(XmlElement* xml)
{
    // XmlElement* fileName = xml->createNewChildElement("FILENAME");
    //    {
    // fileName->addTextElement(lastFilePath.getFullPathName());
}

void JuliaEditor::loadEditorParameters(XmlElement* xml)
{
    // forEachXmlChildElement(*xml, xmlNode)
    //    {
    //       if (xmlNode->hasTagName("FILENAME"))
    //       {
    //           lastFilePath = File(xmlNode->getText());
    //           thread->setFile(lastFilePath.getFullPathName());
    //           fileNameLabel->setText(lastFilePath.getFullPathName(),false);
    //       }
    //   }
}


JuliaEditorCanvas::JuliaEditorCanvas(JuliaProcessor* sr) : processor(sr)
{

    //rfMaps.clear();
}


JuliaEditorCanvas::~JuliaEditorCanvas(){
    
}
    
void JuliaEditorCanvas::beginAnimation()
{
    startCallbacks();
    std::cout << "Julia editor canvas beginning animation." << std::endl;

}

void JuliaEditorCanvas::endAnimation()
{
    stopCallbacks();
}
    
void JuliaEditorCanvas::refreshState()
{
}

void JuliaEditorCanvas::update()
{
   // int nMaps = processor->getNumElectrodes();

   // clearRfMaps();

   // for (int i = 0; i < nMaps; i++)
   // {
   //     RFMap* rf = addRFMap(i);
   //     processor->addRFMapForElectrode(rf, i);
   // }

    //addAndMakeVisible(rfMaps[currentMap]);

    resized();
    repaint();

}
    
void JuliaEditorCanvas::setParameter(int, float) {}

void JuliaEditorCanvas::paint(Graphics& g)
{
    g.fillAll(Colours::grey);

    //std::cout << "got " << numXPixels << " x " << numYPixels << " size" <<std::endl;

    float xHeight = getWidth()/processor->outputImageSizeW;
    float yHeight = getHeight()/processor->outputImageSizeH;

        for (int n = 0; n < processor->outputImageSizeW; n++)
        {
            for (int m = 0; m < processor->outputImageSizeH; m++)
            {
                //float c = random.nextFloat()*255;
                float c = processor->getIm(m+(n*processor->outputImageSizeH))*255;
                
                g.setColour(Colour(c,c,c));
                g.fillRect(n*xHeight, m*yHeight, xHeight, yHeight);

            }
        }
}
    
void JuliaEditorCanvas::refresh()
{
    //processor->setParameter(2, 0.0f); // request redraw
    
    repaint();

    //::cout << "Refresh" << std::endl;
}
    
void JuliaEditorCanvas::resized()
{
   // if (rfMaps.size() > 0)
   //     rfMaps[currentMap]->setBounds(0, 0, getWidth(), getHeight());

    repaint();
}