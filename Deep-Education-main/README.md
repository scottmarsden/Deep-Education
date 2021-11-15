# Deep-Education


To test go into the project and then to kernel and run:

'python3 GCN_pubmed.py'

To run single thread change the value in the kernel.cpp in the gspmm() on lines 73-80 to:


'if(reverse){
    normalize(snaph, input, false);
    multiply(snaph, input, output, false);
    }
    else{
      normalize(snaph, input, false);
      multiply(snaph, input, output, false);
    }
    
 }'
 
 Changing the values at the end to false will run single thread and true will run multithread
