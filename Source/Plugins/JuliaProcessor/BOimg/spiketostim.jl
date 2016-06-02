function spiketostim()

fprev = zeros(2001,1);
xprev = zeros(2,2001);

global fprev;
global xprev;


include("stimBayes.jl");
include("makeSobol.jl");
include("OptProbBayes.jl");
include("EIDMat.jl");
include("kernelD.jl");

for iter = 1:22
	nSpikes = rand(100,1);
	Im = stimBayes(nSpikes, iter);
end

return Im;
end