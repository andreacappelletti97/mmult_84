import pynq
import pynq.lib.dma
from pynq import Overlay
import numpy as np
from pynq import Xlnk
import random
import sys
np.set_printoptions(threshold=sys.maxsize)

#Inser the path of the overlay to download
ol = Overlay("/home/xilinx/pynq/overlays/mmul_84/mmul84.bit")
#Download the overlay
ol.download()

xlnk = Xlnk()

#DMA inizialization
dma = ol.axi_dma_0

#Define a standard dimension, in this case 84
DIM = 84

#Utility function to get matrix dimension [rows]
def getMatrixRows(matrix):
    return matrix.shape[0]

#Utility function to get matrix dimension [cols]
def getMatrixColumns(matrix):
    return matrix.shape[1]

#Resize the dimension of the matrix
def resizeMatrix(matrix):
    if getMatrixRows(matrix) < DIM or getMatrixColumns(matrix) < DIM:
        delta_r = DIM - getMatrixRows(matrix)
        delta_c = DIM - getMatrixColumns(matrix)
        matrix = np.lib.pad(matrix, ((0,delta_r),(0,delta_c)), mode = 'constant')

def _mmult84(a,b):
    #Check the size of the two matrix
    resizeMatrix(a)
    resizeMatrix(b)
    #Define the output stream matrix
    output_stream = xlnk.cma_array(shape=(7056,), dtype=np.float32)
    #Output before because result can be ready before all the input has been transferred
    dma.recvchannel.transfer(output_stream)
    #set input stream dma.sendchannel.transfer

    dma.sendchannel.wait()
    dma.recvchannel.wait()
    return output_stream

    
    

    