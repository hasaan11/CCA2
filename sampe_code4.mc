func int main:
begin
	n1 int;
	n2 int;
	hcf int;
	#variable declarations
	println "Enter two numbers:"; #print
	in n1; #input n1
	in n2;
	if n2 > n1: #swap if n2 is greater
	begin
		temp<-n2 int;
		n2 <- n1;
		n1 <- temp;
	end
	i int; # declaration
	for i<-1, i<=n2, i<-i+1: # for loop
	begin
		if n1 % i = 0 and n2 % i = 0:# a number whose modulo is 0 for both numbers
		begin
			hcf<-i;#that number is hcf
		end
	end
	print "hcf: ";#print
	println hcf;
	return 0;#halts the program
end