#include <cassert>
#include <iostream>
#include <limits>

#include "kernel.h"
#include "omp.h"

using std::cout;
using std::endl;

int THD_COUNT = 1;

using std::string;

void normalize(csr_t* snaph, array2d_t<float> & matrix) {
  #pragma omp parallel{
    #pragma omp for
    for (int i = 0 ; i < matrix.row_count ; i++) {
        matrix.row_normalize(i, snaph->get_degree(i));
    }
  }
}

void multiply(csr_t* snaph, array2d_t<float> & input, array2d_t<float> & output){
    vid_t* offset = snaph->offset;
    vid_t* nebrs = snaph->nebrs;
    
  
    #pragma omp parallel {
      #pragma omp for
      for (int i = 0; i < output.row_count; i++) {
         output.row_add(input.data_ptr + i*output.col_count, i); 
        
        for (int j = offset[i] ; j < offset[i+1] ; j++) {
              output.row_add(input.data_ptr + nebrs[j]*output.col_count, nebrs[j]);
          }
      }
    }
  
}


void _gspmm(csr_t* snaph, array2d_t<float> & input, array2d_t<float> & output,
                     op_t op, bool reverse, bool norm /*= true*/)
{
    //cout << "spmm " << op << "reverse = " << reverse << endl;

    //If in backward, normalize it first, else normalize it after computation

    //The core logic goes here.
  
  
  
    if(reverse){
    normalize(snaph, input);
    multiply(snaph, input, output);
    }
    else{
      normalize(snaph, input);
      multiply(snaph, input, output);
    }

    /*int64_t columnCount = output.columnCount;
    int64_t rowCount = output.rowCount;
    vid_t* nbrs = snaph->get_nebrs();
    vid_t* offset = snaph->get_offset();
    vid_t v_count = snaph->get_vcount();

    for(vid_t i = 0; i <v_count; i++){
    vid_t deg = snaph->get_degree(node)
    if(reverse == 0){
    	for (vid_t nbrID = offset[i]; nbrID < offset[i]+ deg; nbrID++){
    		output.row_add(input.operator[](nbrs[nbrID]), i);
    	}
    	output.row_normalize(i, deg);
    }
    else {
    	input.row_normalize(i, deg);
    	for (vid_t nbrID = offset[i]; nbrID < offset[i]+ deg; nbrID++){
    		output.row_add(input.operator[](nbrs[nbrID]), i);
    	}
    } */
   

}

void invoke_gspmm(graph_t& graph, array2d_t<float> & input_array, array2d_t<float> & output_array,
                 bool reverse, bool norm /*= true*/)
{
    if (reverse) {
         return _gspmm(&graph.csr, input_array, output_array, eSUM, reverse, norm);
    } else {
         return _gspmm(&graph.csc, input_array, output_array, eSUM, reverse, norm);
    }
}
