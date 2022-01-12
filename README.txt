
To complile my code, use gcc AES.c
You will have to change the #define'd constant NK prior to compilation 
You will also have to decide whether you want to encrypt to decrypt by changes lines 117-118 to call the appropriate function
You will also have to set both the 'message' and 'keyStr' variables to be the message and key respectively.

To run the code, simply use ./a.out (or the name of the executable if you compiled with -o )

I only used the resources presented in the lab spec in implementing my solution.  I did not use any reference implementations or other disallowed methods in creating my code
I'm sure if you look at my beautiful and liberal raw pointer usage, you'll be able to tell this is not code somebody would willingly put on the internet.


I pass all 3 test cases in appendix c, as you can verify by changing NK to be 4,6,8.  The provided test case will run automatically when NK is at the correct value


