#function spiketostim()

fprev = zeros(5001,1);
xprev = zeros(2,5001);

global fprev;
global xprev;
global flag_accum = 1;
global buffer_count =0;	
global nSpikes=0;
global iter=0;

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

	global fprev;
	global xprev;
	global flag_accum;
	global buffer_count;	
	global nSpikes;
	global iter;


	println("computed ", buffer_count );
	buffer_count = buffer_count+1;
	

	if (flag_accum==1 & buffer_count > 2)

		nSpikes=nSpikes+sum(data);
		println("buffer spikes: ", nSpikes, "data: ",data[1] );
	elseif (flag_accum==1 & buffer_count >10)
		
		iter	   = iter+1;
		flag_accum = 0;
		
		#image=image*0;		

		image[1:30*30] = stimBayes(nSpikes, iter);
		#image[1:220]=[1:220]/220;
		
		buffer_count = 0;
		flag_accum   = 1;
		
		data[1] = nSpikes;
		data[2] = xprev[1,iter];
		data[3] = xprev[2,iter];

		#println("computed spikes: ",nSpikes, " image: ",image[1]);
		nSpikes=0;
		#@printf "nSpikes: %d bufferCount: %f image %f" nSpikes buffer_count image[1];

	end;
	
	

	return data, image
end;

