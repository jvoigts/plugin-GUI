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

#include "JuliaProcessor.h"
#include "JuliaEditor.h"
#include <stdio.h>
#include <julia.h>

JuliaProcessor::JuliaProcessor()
    : GenericProcessor("Julia Processor")
{
	hasJuliaInstance = false;
    outputImageSizeW = 256;
    outputImageSizeH = 256;

    outputImage = new double[outputImageSizeW*outputImageSizeH]();
    

    //outputImage[5]=1;
    //outputImage = malloc(30*30*sizeof(float));

    //dataHistoryBuffer = new AudioSampleBuffer(outputImageSizeW, 60000);
    //dataHistoryBuffer->clear();


}

JuliaProcessor::~JuliaProcessor()
{
    free(outputImage);
	jl_atexit_hook(0);
	//deleteAndZero(dataHistoryBuffer);
}

AudioProcessorEditor* JuliaProcessor::createEditor()
{
    editor = new JuliaEditor(this, true);
    std::cout << "Creating Julia editor." << std::endl;
    return editor;
}


bool JuliaProcessor::enable()
{
    std::cout << "julia::enable()" << std::endl;
    JuliaEditor* editor = (JuliaEditor*) getEditor();

    editor->enable();
    return true;

}

bool JuliaProcessor::disable()
{
    std::cout << "julia disabled!" << std::endl;
    JuliaEditor* editor = (JuliaEditor*) getEditor();
    editor->disable();
    return true;
}


void JuliaProcessor::setFile(String fullpath)
{
	hasJuliaInstance = true;
	filePath = fullpath;

	FILE* fp = popen("echo $JULIA", "r");
	char input[255];
	fgets(input, sizeof(input), fp);
	
	String julia_bin_dir = input;
	julia_bin_dir = julia_bin_dir.trimEnd();
	julia_bin_dir += "/home/jvoigts/Documents/Github/julia/usr/bin/";
	String julia_sys_dir = input;
	julia_sys_dir = julia_sys_dir.trimEnd();
	julia_sys_dir += "/home/jvoigts/Documents/Github/julia/usr/lib/julia/sys.so";
	
	const char* jbin = julia_bin_dir.toRawUTF8();
	const char* jsys = julia_sys_dir.toRawUTF8();
	
	jl_init_with_image(jbin, jsys);

	String juliaString = "include(\"" + filePath + "\")";
	run_julia_string(juliaString);

    // for some reaspn, julia generates an outofmemory error if we start printing debug messages from the process function later, 
    // but then recovers after a few calls, about 100ms after the 1st call
    // - this simple print command emulates this and sidesteps the issue - this will likely be patched in some future julia version
    run_julia_string("println(\"Julia instance running\");");

}

void JuliaProcessor::reloadFile()
{
    if (hasJuliaInstance) 
    {
        //String juliaString = "reload(\"" + filePath + "\")"; // doenst seem to work? include does the job though
        String juliaString = "include(\"" + filePath + "\")";
        run_julia_string(juliaString);
    }
    else
    {
        std::cout << "No julia instance running - cant refresh" << std::endl;
    }
}


void JuliaProcessor::setParameter(int parameterIndex, float newValue)
{
    editor->updateParameterButtons(parameterIndex);
}

void JuliaProcessor::setOutputImageSize(int W, int H)
{
    
    if (W > 0)
    {
        printf("Setting output image width to %d  pixels \n", W);
        outputImageSizeW = W;
    }
    if (H > 0)
    {
        printf("Setting output image height to %d pixels \n", H);
        outputImageSizeH = H;
    }
    free(outputImage);
    outputImage = new double[outputImageSizeW * outputImageSizeH]();
    memset(outputImage, 0, outputImageSizeW * outputImageSizeH * sizeof(*outputImage));
}

void JuliaProcessor::run_julia_string(String juliaString)
{
    // need to convert from juce String to char array
    const char* jstr = juliaString.toRawUTF8();

    printf("executing julia cmd: %s\n", jstr);

    jl_eval_string(jstr);

    if (jl_exception_occurred())
    	printf("julia exception: %s \n", jl_typeof_str(jl_exception_occurred()));
}



String JuliaProcessor::getFile()
{
    return filePath;
}

void JuliaProcessor::process(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	if (hasJuliaInstance)
    {

        buffer.clear();

        /*
        // populate sampled channels with spikes from sorter
        if (midiMessages.getNumEvents() > 0)
        {

            // int m = midiMessages.getNumEvents();
            //std::cout << m << " events received by node " << getNodeId() << std::endl;

            MidiBuffer::Iterator i(midiMessages);
            MidiMessage message(0xf4);

            int samplePosition = 0;
            i.setNextSamplePosition(samplePosition);

            while (i.getNextEvent(message, samplePosition))
            {

                const uint8* dataptr = message.getRawData();

               // handleEvent(*dataptr, message, samplePosition);

                int bufferSize = message.getRawDataSize();

                if (bufferSize > 0 && dataptr[0]==SPIKE)
                {

                    SpikeObject newSpike;

                    bool isValid = unpackSpike(&newSpike, dataptr, bufferSize);

                    if (isValid)
                    {
                        int electrodeNum = newSpike.source;

                        //Electrode& e = electrodes.getReference(electrodeNum);
                        // std::cout << electrodeNum << std::endl;

                        // select if spike is of interest

                        //if (electrodeNum==1){
                        buffer.setSample(electrodeNum,samplePosition,1);
                        //}

                    }
                }
            }
        } // if (midiMessages.getNumEvents() > 0)
        */

        // send data to julia
    	jl_function_t *func = jl_get_function(jl_main_module, "oe_process!");    

    	
    	//for (int n = 0; n < getNumOutputs(); n++)
        int n=0; // only operate on 1st chyannel for now - switch to 2d array later
    	{

            /*
            {
    	    // create 1D array of float32 type
            jl_value_t *array_type = jl_apply_array_type(jl_float32_type, 1); // last arg is nDims


        	float* ptr = buffer.getWritePointer(n); // to perform in-place edits to the buffer
    		jl_array_t *x = jl_ptr_to_array_1d(array_type, ptr , buffer.getNumSamples(), 0);
    	             
            }
            */
            //jl_value_t *dims = jl_eval_string("(30,31)"); 

            // make tuple for dimensions of raw neural data
            float* ptr = buffer.getWritePointer(0); // to perform in-place edits to the buffer
            jl_value_t *array_type_samples = jl_apply_array_type(jl_float32_type, 2);
            jl_function_t *tfunc = jl_get_function(jl_base_module, "tuple");
            jl_value_t *a = jl_box_int32(getNumOutputs()); // N channels
            jl_value_t *b = jl_box_int32(buffer.getNumSamples()); // N samples
            jl_value_t *dims = jl_call2(tfunc, b, a);
            //dims = jl_eval_string("(16,1024)"); 

            // wrap array for julia
            jl_array_t *jl_neuralData =  jl_ptr_to_array(array_type_samples, ptr , dims , 0);


             // make tuple for dimensions of outputImage
            jl_value_t *array_type_im = jl_apply_array_type(jl_float64_type, 2);
            a = jl_box_int32(outputImageSizeH);
            b = jl_box_int32(outputImageSizeW);
            dims = jl_call2(tfunc, a, b);

            

            // wrap array for julia
            jl_array_t *jl_outputImage =  jl_ptr_to_array(array_type_im, outputImage , dims , 0);

            // DEBUG:  
            // Get number of dimensions
            //int ndims = jl_array_ndims(jl_neuralData);
            //size_t size0 = jl_array_dim(jl_neuralData,0);
            //size_t size1 = jl_array_dim(jl_neuralData,1);
            //printf("input size: %d %d \n", size0, size1);


            JL_GC_PUSH2(&jl_neuralData,&jl_outputImage);

    		jl_call2(func, (jl_value_t*)jl_neuralData, (jl_value_t*)jl_outputImage);

            //debug
            //size0 = jl_array_dim(jl_outputImage,0);
            //size1 = jl_array_dim(jl_outputImage,1);
            //printf("output size: %d %d \n", size0, size1);

    		outputImage = (double*)jl_array_data(jl_outputImage); // return output image
            
            JL_GC_POP(); // image, and neural data (encoded spikes)
    	}
        // std::cout << im[0] << std::endl;
    	
        
        if (jl_exception_occurred())
            printf("julia exception: %s \n", jl_typeof_str(jl_exception_occurred()));
    }

}

void JuliaProcessor::saveCustomParametersToXml(XmlElement* parentElement)
{
    XmlElement* childNode = parentElement->createNewChildElement("FILENAME");
    childNode->setAttribute("path", getFile());
}

double JuliaProcessor::getIm(int index)
{
        return outputImage[index];
}

void JuliaProcessor::loadCustomParametersFromXml()
{
    if (parametersAsXml != nullptr)
    {
        // use parametersAsXml to restore state
        forEachXmlChildElement(*parametersAsXml, xmlNode)
        {
            if (xmlNode->hasTagName("FILENAME"))
            {
                String filepath = xmlNode->getStringAttribute("path");
                JuliaEditor* fre = (JuliaEditor*) getEditor();
                fre->setFile(filepath);
            }
        }
    }
}