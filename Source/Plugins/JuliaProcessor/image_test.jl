
function oe_process!(data,image)

@printf "  image (%d x %d)\n"  size(image,1) size(image,2);

for i=1:size(image,1)
	for j=1:size(image,2)

		image[i,j]=sin( sqrt( (i-20)^2+(j-40)^2 )*0.1 ) ; 

	end;
end;
	#image=image+1;
	image       = image/maximum(image);

	return data, image;
end;

