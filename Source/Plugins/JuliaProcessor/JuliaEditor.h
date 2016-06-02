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

#ifndef JULIAEDITOR_H_INCLUDED
#define JULIAEDITOR_H_INCLUDED

#include <VisualizerEditorHeaders.h>
#include <VisualizerWindowHeaders.h>

#include <EditorHeaders.h>

//class ImageIcon;
class JuliaProcessor;

/**

  User interface for the Julia processor.

  @see JuliaProcessor

*/

class JuliaEditor : public VisualizerEditor, public Label::Listener

{
public:
    JuliaEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors);
    virtual ~JuliaEditor();
    void buttonCallback(Button* button);
    void labelTextChanged(Label* te);
    void setFile(String file);
    void saveEditorParameters(XmlElement*);
    void loadEditorParameters(XmlElement*);
    ImageIcon* icon;

    Visualizer* createNewCanvas();

private:
    ScopedPointer<UtilityButton> fileButton;
    ScopedPointer<UtilityButton> reloadFileButton;
    ScopedPointer<Label> fileNameLabel;
    ScopedPointer<Label> outputImageSizeSelection;
    ScopedPointer<Label> outputImageSizeSelectionLabel;
    JuliaProcessor* juliaProcessor;
    File lastFilePath;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JuliaEditor);
};

class JuliaEditorCanvas : public Visualizer
{
public:
    JuliaEditorCanvas(JuliaProcessor* n);
    ~JuliaEditorCanvas();
    
    void beginAnimation();
    void endAnimation();
    
    void refreshState();
    void update();
    
    void setParameter(int, float);
    void setParameter(int, int, int, float) {}
    
    void paint(Graphics& g);

    void refresh();
    
    void resized();

    float im[30*30];

    
private:
    JuliaProcessor* processor;

    Random random;

};

#endif  // JULIAEDITOR_H_INCLUDED