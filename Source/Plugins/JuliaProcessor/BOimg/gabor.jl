function gabor(x,y, lx, ly, scale)

xs = repmat(collect(1:ly)', lx, 1);
ys = repmat(collect(1:lx), 1, ly);

xs = xs - x;
ys = ys - y;

theta 	= 0;
psi 	= 0;
lambda 	= scale; 
gamma 	= 1;
sig 	= scale/2;

xs 		= xs * cos(theta) + ys * sin(theta);
ys 		= -xs * sin(theta) + ys * cos(theta);

Im  	= exp(- (xs.^2 + gamma * ys.^2)/(2*sig^2)) .* cos(2*pi*xs/lambda + psi);
	
return Im

end