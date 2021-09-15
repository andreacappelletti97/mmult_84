**README mmul_84 configuration **

---

## Vivado HLS (High Level Synthesis)

In this part we are going to work on the synthesis through Vivado HLS software.
You will need to download the mmul_84.cpp file and test.cpp file.  
Both files are located in the hls folder.

1. Open Vivado HLS
2. Create a New Project
3. Add mmul_84.cpp as C-based source file
4. Select Top Function [Browse...]Êand select mmult_84
5. Add test.cpp as C-based testbench file
6. In the *solution configuration* menu, open part [...] and select **xc7z020clg400-1** using the search bar.  

    Run a simulation, this will execute only the software part of the .cpp files,  
    skipping hardware optimization like #PRAGMA, etc.  

7. Run a simulation: Project -> Run C Simulation  

    Run a synthesis, this will take in consideration only the hardware optimization parts.  

8. Run C Synthesis set target time 10 (10ns == 100Mhz)  

    Finally run a cosimulation, both software and hardware logic are combined.   

8. Run C/RTL cosimulation  

    Export the RTL  

9. Solution -> Export RTL

---

## Vivado 

**Project set-up and import IP Core**

1. Open Vivado
2. Create Project
3. The type is RTL Project
4. In the *default part* windows search for the part **xc7z020clg400-1** and select it
5. In the *Flow Navigator* select Create Block Design, you will find it under *IP INTEGRATOR*
6. Tools -> Settings -> IP -> Repository and add Core from selecting the HLS directory created before

**Pynq**

1. Download [Zynq Presets](https://reference.digilentinc.com/reference/programmable-logic/pynq-z1/start)
2. Unzip the archive
3. In the block design add ZYNQ7 Processing System
4. Double click on it
5. Presets -> Apply Configuration -> select the .tcl extracted from the archive (pynq_revC.tcl)
6. Then in the *Page Navigator* select PS-PL Configuration
7. Under AXI Non Secure Enablement -> GP Master AXI Interface, enable GP0
8. Always under PS-PL Configuration select HP Slave AXI and enable HP0
9. Once done, close the menu and in the block diagram click Run Connection Automation -> All Automation

**AXI DMA**

1. Add AXI Direct Memory Access to the block diagram (+)
2. Double click on the AXI Direct Memory Access block
3. Disable Scatter Gather Engine
4. Set the Width of Buffer Lenght Register: 23
5. Set Address Width: 32 [bits]
6. Only under *Enable Read Channel* Set Max Buster Size: 256
7. Enable allow unaligned transfer
8. Once done, close the menu and in the block diagram click Run Connection Automation -> All Automation

**mmul_84**

1. Add mmul_84 to the block diagram (+)
2. Connect the stream interface to the DMA read and write channel (s_in_V to M_AXIS_MM2S and s_out to S_AXIS_S2MM)
3. Connect clock to the DMA clock (ap_clk to m_axi_s2mm_aclk)
4. Connect reset to the DMA reset (ap_rst_n to mm2s_prmry_reset_out_n)

**AXI Timer**

1. Add AXI Timer to the block diagram (+)
2. Double click on the AXI Timer block
3. Enable 64-bit mode
4. Close the option menu and click Run Connection Automation -> All Automation

**Validation**  

1. Click on Validate Design (F6)

    You should get the message *Validation successful*  

**Bitstream generation**  

1. Go to *Source* -> right click on your Design -> Create HDL Wrapper
2. In the *Flow Navigator* under Program and Debug select Generate Bitstream
3. File -> Export -> Hardware -> Include Bitstream

**TCL generation**  

1. From the TCL console type: **write_bd_tcl mmul_84.tcl**





