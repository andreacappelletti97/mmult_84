#include <stdio.h>
#include <stdlib.h>
#include <hls_stream.h>

#define DIM 84
#define SIZE (DIM * DIM)

struct data_struct{
  float data;
  bool last;
};

void mmult_84(hls::stream<float> &in, hls::stream<data_struct> &out);

void mmult_sw(float a[DIM][DIM], float b[DIM][DIM], float out[DIM][DIM]) {
    // matrix multiplication of a A*B matrix
    for (int ia = 0; ia < DIM; ++ia)
        for (int ib = 0; ib < DIM; ++ib) {

            float sum = 0;

            for (int id = 0; id < DIM; ++id)

                sum += a[ia][id] * b[id][ib];

            out[ia][ib] = sum;
        }
}

int main(void)
{

    int ret_val = 0;

    int i,j, err;

    float matOp1[DIM][DIM];
    float matOp2[DIM][DIM];
    float matMult_sw[DIM][DIM];
    float matMult_hw[DIM][DIM];

    /** Matrix Initiation */
    for(i = 0; i<DIM; i++)
        for(j = 0; j<DIM; j++)
            matOp1[i][j] = (float)(i+j);

    for(i = 0; i<DIM; i++)
        for(j = 0; j<DIM; j++)
            matOp2[i][j] = (float)(i*j);
    /** End of Initiation */


    // Streams creation
    hls::stream<float> in("in");
    hls::stream<data_struct> out("out");

    for(i = 0; i<DIM; i++)
        for(j = 0; j<DIM; j++)
            in.write(matOp1[i][j]);

    for(i = 0; i<DIM; i++)
        for(j = 0; j<DIM; j++)
            in.write(matOp2[i][j]);

    /* HW Matrix Multiplication */
    mmult_84(in, out);

    // Write Output
    for(i = 0; i<DIM; i++)
        for(j = 0; j<DIM; j++)
            matMult_hw[i][j] = out.read().data;

    /* reference Matrix Multiplication */
    mmult_sw(matOp1, matOp2, matMult_sw);

    /** Matrix comparison */
    err = 0;
    for (i = 0; (i<DIM && !err); i++)
        for (j = 0; (j<DIM && !err); j++)
            if (matMult_sw[i][j] != matMult_hw[i][j])
                err++;

    if (err == 0)
        printf("Matrixes identical ... Test successful!\r\n");
    else
        printf("Test failed!\r\n");

    return err;

}
