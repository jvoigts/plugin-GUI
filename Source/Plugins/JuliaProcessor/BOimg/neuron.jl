function neuron(xt,G)


xt = round(Int, xt);
lambda = G[xt[1],xt[2]];
f      = lambda + sqrt(lambda)*randn();

return f;

end