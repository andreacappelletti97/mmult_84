#include <hls_stream.h>

#define DIM 84

struct data_struct{
  float data;
  bool last;
};

void mmult_84(hls::stream<float> &s_in, hls::stream<data_struct> &s_out) {
    
//This PRAGRAMA get the interface post from the RTL
    
#pragma HLS INTERFACE axis port=s_in
#pragma HLS INTERFACE axis port=s_out
#pragma HLS INTERFACE ap_ctrl_none port=return


    float a[DIM][DIM];
    float b[DIM][DIM];
    float c[DIM][DIM];

    int const FACTOR = DIM/4;

    //The FACTOR specify in how many copies reduce the loop, in this case DIM/4 --> 21
    //It splits the array in Factor blocks
    //New array will have a dimension of 4 a = [21][2] and 4 b = [21][1]
    
    #pragma HLS array_partition variable=a block factor=FACTOR dim=2
    #pragma HLS array_partition variable=b block factor=FACTOR dim=1

    // stream in first matrix
    for (int i = 0; i < DIM; i++)
        for (int j = 0; j < DIM; j++) {
//Process a new input every clock cycle
            
#pragma HLS PIPELINE II=1
            a[i][j] = s_in.read();

        }

    // stream in second matrix
    for (int i = 0; i < DIM; i++)
        for (int j = 0; j < DIM; j++) {
#pragma HLS PIPELINE II=1
            b[i][j] = s_in.read();
        }

    // matrix multiplication of a A*B matrix
    L1:for (int ia = 0; ia < DIM; ++ia)
        L2:for (int ib = 0; ib < DIM; ++ib)
        {
            #pragma HLS PIPELINE II=1
            float sum = 0;
            L3:for (int id = 0; id < DIM; ++id)
                sum += a[ia][id] * b[id][ib];
            c[ia][ib] = sum;
        }

    // stream out result matrix
    data_struct out_data;
    for (int i = 0; i < DIM; i++)
        for (int j = 0; j < DIM; j++) {
#pragma HLS PIPELINE II=1
            out_data.data = c[i][j];
            out_data.last = (i == (DIM - 1) && j == (DIM - 1)) ? 1 : 0;

            s_out.write(out_data);
        }

        return;
}
