function bayesrun()		

npix = [30; 30];

fprev = zeros(2001,1);
xprev = ones(2,2001);

global fprev;
global xprev;

include("stimBayes.jl");
include("makeSobol.jl");
include("OptProbBayes.jl");
include("EIDMat.jl");
include("kernelD.jl");
include("neuron.jl");

x1 = broadcast(+, zeros(npix[1], npix[2]), collect(1:npix[1]));
x2 = broadcast(+, zeros(npix[1], npix[2]), collect(1:npix[2])');
xt          = zeros(2,length(x1));
xt[1,:]     = x1;
xt[2,:]     = x2;
bp          = [5;25];
r         = sum(broadcast(-,xt,bp).^2,1);
sig1     = 3;
Im1      = exp(-r/(2*sig1^2))/sig1;
Im       = (Im1/maximum(Im1) + 1)/2 * 10;
Im       = reshape(Im,npix[1],npix[2]);
G        = Im;

nt = 200;
d = zeros(nt,1);
for iter = 1:nt
	nSpikes = neuron(xprev[:,iter],G);
	Im = stimBayes(nSpikes, iter);
	
	d[iter] = sqrt(sum((bp - xprev[:,iter+1]).^2));
	
end
println(sum(broadcast(>,d,4)))
xps = xprev[1,:]*npix[2]+xprev[2,:];
println(length(unique(xps)))

#plot(x=xprev[1,1:nt],y=xprev[2,1:nt])


end
