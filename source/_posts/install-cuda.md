---
title: Installing CUDA Toolkit for GPGPU-sim on WSL
date: 2020-03-28 11:32:23
tags: CUDA, WSL
---

[GPGPU-sim](http://www.gpgpu-sim.org/) is a cycle-level simulator modeling contemporary graphics processing units (GPUs) and running GPU computing workloads written in CUDA or OpenCL. To look insight of its source code, I try to build it on the WSL (just for fun). For now WSL has not provided the support for GPU device yet. Fortunately compiling of GPGPU-sim has no dependency on NVIDIA driver. All that it needs is CUDA toolkit.

# Where are CUDA Toolkit installed 
```
Default Install Location of CUDA Toolkit                                    
                                                                               
   Windows platform:                                                           
                                                                               
   %ProgramFiles%\NVIDIA GPU Computing Toolkit\CUDA\v#.#                       
                                                                               
   Linux platform:                                                             
                                                                               
   /usr/local/cuda-#.#                                                         
                                                                               
   Mac platform:                                                               
                                                                               
   /Developer/NVIDIA/CUDA-#.#

Default Install Location of CUDA Samples                                    
                                                                               
   Windows platform:                                                           
                                                                               
   %ProgramData%\NVIDIA Corporation\CUDA Samples\v#.#                          
                                                                               
   Linux platform:                                                             
                                                                               
   /usr/local/cuda-#.#/samples                                                 
                                                                               
   and                                                                         
                                                                               
   $HOME/NVIDIA_CUDA-#.#_Samples                                               
                                                                               
   Mac platform:                                                               
                                                                               
   /Developer/NVIDIA/CUDA-#.#/samples                                          
                                                                               
                                                                               
   NVIDIA Nsight Visual Studio Edition (Windows only)  
```

# What are there in CUDA Toolkit
```
 CUDA Installer                                                               
  - [X] Driver                                                                 
       [X] 440.33.01                                                           
  - [X] CUDA Toolkit 10.2                                                      
     - [X] CUDA Tools 10.2                                                     
        - [X] CUDA Command Line Tools 10.2                                     
             [X] cuda-gdb                                                      
             [ ] cuda-gdb-src                                                  
             [X] cuda-nvprof                                                   
             [X] cuda-memcheck                                                 
             [X] cuda-nvdisasm                                                 
             [X] cuda-cupti                                                    
             [X] cuda-sanitizer-api                                            
             [X] cuda-nvtx                                                     
        - [X] CUDA Visual Tools 10.2                                           
             [X] cuda-nsight                                                   
           - [X] cuda-nsight-compute                                           
                [X] cuda-nsight-compute-integration                            
           - [X] cuda-nsight-systems                                           
                [X] cuda-nsight-systems-integration                            
             [X] cuda-nvvp                                                     
     - [X] CUDA Libraries 10.2                                                 
        - [X] CUDA Runtime 10.2                                                
             [X] cuda-cusolver                                                 
             [X] libcublas10                                                   
             [X] cuda-cufft                                                    
             [X] cuda-curand                                                   
             [X] cuda-cusparse                                                 
             [X] cuda-nvgraph                                                  
             [X] cuda-npp                                                      
             [X] cuda-cudart                                                   
             [X] cuda-nvrtc                                                    
             [X] cuda-nvjpeg                                                   
        - [X] CUDA Development 10.2                                            
             [X] cuda-cusolver-dev                                             
             [X] libcublas-dev                                                 
             [X] cuda-cufft-dev                                                
             [X] cuda-curand-dev                                               
             [X] cuda-cusparse-dev                                             
             [X] cuda-driver-dev                                               
             [X] cuda-nvgraph-dev                                              
             [X] cuda-npp-dev                                                  
             [X] cuda-cudart-dev                                               
             [X] cuda-nvrtc-dev                                                
             [X] cuda-nvml-dev                                                 
             [X] cuda-nvjpeg-dev                                               
     - [X] CUDA Compiler 10.2                                                  
          [X] cuda-nvcc                                                        
          [X] cuda-cuobjdump                                                   
          [X] cuda-nvprune                                                     
       [X] CUDA Misc Headers 10.2                                              
    [X] CUDA Samples 10.2                                                      
    [X] CUDA Demo Suite 10.2                                                   
    [X] CUDA Documentation 10.2                                                
```

# Post-Installation
```
Driver:   Not Selected
Toolkit:  Installed in /usr/local/cuda-10.2/
Samples:  Installed in /home/luc/, but missing recommended libraries

Please make sure that
 -   PATH includes /usr/local/cuda-10.2/bin
 -   LD_LIBRARY_PATH includes /usr/local/cuda-10.2/lib64, or, add /usr/local/cuda-10.2/lib64 to /etc/ld.so.conf and run ldconfig as root

  To uninstall the CUDA Toolkit, run cuda-uninstaller in /usr/local/cuda-10.2/bin

  Please see CUDA_Installation_Guide_Linux.pdf in /usr/local/cuda-10.2/doc/pdf for detailed information on setting up CUDA.
  ***WARNING: Incomplete installation! This installation did not install the CUDA Driver. A driver of version at least 440.00 is required for CUDA 10.2 functionality to work.
  To install the driver using this installer, run the following command, replacing <CudaInstaller> with the name of this
  run file:
      sudo <CudaInstaller>.run --silent --driver

      Logfile is /var/log/cuda-installer.log
```

```
* Please make sure your PATH includes /usr/local/cuda/bin
* * Please make sure your LD_LIBRARY_PATH
* *   for 32-bit Linux distributions includes /usr/local/cuda/lib
* *   for 64-bit Linux distributions includes /usr/local/cuda/lib64:/usr/local/cuda/lib
* * OR
* *   for 32-bit Linux distributions add /usr/local/cuda/lib
* *   for 64-bit Linux distributions add /usr/local/cuda/lib64 and /usr/local/cuda/lib
* * to /etc/ld.so.conf and run ldconfig as root
*
* * Please read the release notes in /usr/local/cuda/doc/
*
* * To uninstall CUDA, remove the CUDA files in /usr/local/cuda
* * Installation Complete
```

NOTES:

GPGPU-sim has a restricted version of CUDA that ranging from 2.3 to 4.2. Neither the higher nor the lower does get it done.
