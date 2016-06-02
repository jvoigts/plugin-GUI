#function spiketostim()

fprev = zeros(5001,1);
xprev = zeros(2,5001);

global fprev;
global xprev;
global buffer_count 	=0;	
global nSpikes		=0;
global iter		=0;
global flag_accum 	=1;

include("stimBayes.jl");
include("makeSobol.jl");
include("OptProbBayes.jl");
include("EIDMat.jl");
include("kernelD.jl");

#for iter = 1:22
#	nSpikes = rand(100,1);
#	Im = stimBayes(nSpikes, iter);
#end


function oe_process!(data,image)

#image[1:end]=0;
@printf "nSpikes: %d bufferCount: %f image %f (%d x %d)\n" nSpikes buffer_count image[1] size(image,1) size(image,2);

	global fprev;
	global xprev;
	global buffer_count;	
	global nSpikes;
	global iter;
	global flag_accum;

	if (flag_accum==0)
		# do nothing
	else
		buffer_count = buffer_count+1;
	
		if (buffer_count >2 && buffer_count<=30)			
			if (buffer_count > 10)
				image[1:30*30] = 1 * ones(1,900);
			else
				nSpikes = nSpikes+sum(data);
			end		
		elseif (buffer_count>30)
			iter	   = iter+1;
			flag_accum = 0;
			#image=image*0;		

			# make stim
			image[1:end] = stimBayes(nSpikes, iter);
			
			#image[1:end]=image.*0;

			buffer_count = 0;
			
			data[1] = nSpikes;
			data[2] = xprev[1,iter];
			data[3] = xprev[2,iter];
			
			
			nSpikes=0;
			@printf "nSpikes: %d bufferCount: %f image %f (%d x %d)\n" nSpikes buffer_count image[1] size(image,1) size(image,2);
			flag_accum   = 1;
		end;
	end	

#image[10,10]=1;
#image[10,11]=0;
	

	return data, image;
end;

