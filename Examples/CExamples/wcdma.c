// SigLib WCDMA based Spreading & Scrambling modulation test program
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                         // SigLib
#include <gnuplot_c.h>                                      // Gnuplot/C

// Define constants
#define DISPLAY_CONSTELLATION           1                   // Set to '1' to display the constellation diagram, '0' otherwise

#define ADD_WHITE_NOISE                 0                   // Set to '1' to add white noise to the output signal, '0' otherwise
#define ADD_JITTERANDGAUSSIAN_NOISE     1                   // Set to '1' to add jitter and Gaussian noise to the output signal, '0' otherwise

#define SPREADING_FACTOR                16                  // Spreading factor
#define CHANNELIZATION_CODE_INDEX       5                   // Channelization code index

#define SIGLIB_3GPPDL_SHIFT_REGISTER_MASK   0x03ffff        // The DL uses an 18 bit shift shift register
#define SIGLIB_3GPPUL_SHIFT_REGISTER_MASK   0x01ffffff      // The DL uses a 25 bit shift shift register

#define JITTER_SINE_FREQUENCY           0.11                // Jitter sine wave frequency
#define JITTER_SINE_MAGNITUDE           0.11                // Jitter sine wave magnitude
#define GAUSSIAN_NOISE_VARIANCE         0.01                // Gaussian noise variance

// Declare global variables and arrays
static SLData_t     ChannelizationCode [SPREADING_FACTOR];      // Channelization code array


int main (int argc, char *argv[])

{
#if DISPLAY_CONSTELLATION
    h_GPC_Plot          *hConstellationDiagram;
#endif

    SLArrayIndex_t  NumberOfIterations;

    SLFixData_t     DataInBits, DataOutBits;
    SLComplexRect_s DataOut[SPREADING_FACTOR];

    SLData_t        w16I = SIGLIB_ONE;                      // Weight
    SLData_t        w16Q = SIGLIB_ONE;

    SLUInt32_t      XShiftRegister, YShiftRegister;
    SLComplexRect_s ScramblingCode[SPREADING_FACTOR], DescramblingCode[SPREADING_FACTOR];

#if ADD_JITTERANDGAUSSIAN_NOISE
    SLData_t        JitterPhaseOffset;
    SLData_t        GaussianNoisePhaseOffset, GaussianNoiseCurrentValue;
#endif

    SLFixData_t     DataInByte = SIGLIB_FIX_ZERO;
    SLArrayIndex_t  LoopCount;

    SLUInt32_t      TxShiftRegister;                        // Shift registers - Must be at least 17 bits long

    SLData_t        DemodErrorArray[4];                     // 10Error, 01Error, M10Error, 0M1Error;

    SLFixData_t     BitCount = (SLFixData_t)0;
    SLFixData_t     BitErrorCount = (SLFixData_t)0;

    if (argc < 2) {
        printf("Usage   : wcdma <Number of iterations>\n");
        printf("Example : wcdma 50\n\n");
        NumberOfIterations = 50;
    }
    else {
        NumberOfIterations = (SLArrayIndex_t)atol (argv[1]);
    }


    printf("wcdma: \n");
    printf(" Number Of Iterations = %d\n", NumberOfIterations);


#if DISPLAY_CONSTELLATION
    hConstellationDiagram =                                 // Initialize plot
        gpc_init_xy ("WCDMA Constellation Diagram",         // Plot title
                     "X-Axis",                              // X-Axis label
                     "Y-Axis",                              // Y-Axis label
                     2.0,                                   // Dimension - this is square
                     GPC_KEY_ENABLE);                       // Legend / key mode
    if (NULL == hConstellationDiagram) {                    // Graph creation failed
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }
#endif


                                                            // Generate channelization code
    SDS_ChannelizationCode (ChannelizationCode,             // Channelization code array
                            SPREADING_FACTOR,               // Spreading factor
                            CHANNELIZATION_CODE_INDEX);     // Channelization code index

                                                            // Generate scrambling code
    XShiftRegister = (SLUInt32_t)1;                         // Initialize the seeds
    YShiftRegister = (SLUInt32_t)(SIGLIB_3GPPDL_SHIFT_REGISTER_MASK);
    SDS_LongCodeGenerator3GPPDL (ScramblingCode,            // Scrambling code
                                 &XShiftRegister,           // X shift register
                                 &YShiftRegister,           // Y shift register
                                 SPREADING_FACTOR);         // Output length


                                                            // Generate descrambling code
    SDA_ComplexRectInverse (ScramblingCode,                 // Scrambling code
                            DescramblingCode,               // Descrambling code
                            SPREADING_FACTOR);              // Scrambling code length

#if ADD_JITTERANDGAUSSIAN_NOISE
    JitterPhaseOffset = SIGLIB_ZERO;                        // Initialize the jitter and aditive noise
    GaussianNoisePhaseOffset = SIGLIB_ZERO;
    GaussianNoiseCurrentValue = SIGLIB_ZERO;
#endif

    TxShiftRegister = 1;                                    // Seed the shift registers

#if DEBUG
    for (i = 0; i < SPREADING_FACTOR; i++) {
        printf ("Scram   : i = %lf, q = %lf\n", ScramblingCode[i].real, ScramblingCode[i].imag);
        printf ("DeScram : i = %lf, q = %lf\n", DescramblingCode[i].imag, DescramblingCode[i].imag);
    }
#endif

    for (LoopCount = 0; LoopCount < NumberOfIterations; LoopCount++) {
        if ((LoopCount % 4) == 0) {                         // Should we generate the next PN sequence bits
            DataInByte =
                SDS_SequenceGeneratorPN9 (&TxShiftRegister);    // Shift register
            DataInBits = DataInByte & 0x3;
            DataInByte >>= 2;
            DataInBits = SDS_ReverseDiBits (DataInBits);    // Put bits in ITU order, not computer order
//printf ("DataInByte = %x\n", DataInByte);
        }

        else {
            DataInBits = DataInByte & 0x3;
            DataInByte >>= 2;
            DataInBits = SDS_ReverseDiBits (DataInBits);    // Put bits in ITU order, not computer order
        }

//printf ("DataInBits = %d\n", (int)DataInBits);

        SDA_ComplexQPSKSpread (DataInBits,                  // Tx di-bit
                               DataOut,                     // Pointer to destination array
                               ChannelizationCode,          // In-phase channelization code
                               ChannelizationCode,          // Quadrature-phase channelization code
                               w16I,                        // In-phase weighting value
                               w16Q,                        // Quadrature-phase weighting value
                               ScramblingCode,              // Complex scrambling code
                               SPREADING_FACTOR);           // Spreading factor


                    // Add another scrambling code, jitter and noise here

#if ADD_WHITE_NOISE
        SDA_SignalGeneratePolarWhiteNoise (DataOut,             // Output data
//                                         2.4,                 // Peak level - No errors
                                           5.0,                 // Peak level - Few errors
                                           SIGLIB_ADD,          // Buffer fill mode, fill up or add to
                                           SPREADING_FACTOR);   // Dataset length
#endif

#if ADD_JITTERANDGAUSSIAN_NOISE
        SDA_SignalAddPolarJitterAndGaussianNoise (DataOut,                          // Input data
                                                  DataOut,                          // Output data
                                                  JITTER_SINE_FREQUENCY,            // Jitter sine wave frequency
                                                  JITTER_SINE_MAGNITUDE,            // Jitter sine wave magnitude
                                                  &JitterPhaseOffset,               // Jitter sine wave phase offset
                                                  GAUSSIAN_NOISE_VARIANCE,          // Variance
                                                  &GaussianNoisePhaseOffset,        // Phase offset
                                                  &GaussianNoiseCurrentValue,       // Current value for next iteration
                                                  SPREADING_FACTOR);                // Sample length
#endif

#if DISPLAY_CONSTELLATION
        if (LoopCount == 0) {
            gpc_plot_xy (hConstellationDiagram,             // Graph handle
                         (ComplexRect_s *)DataOut,          // Array of complex dataset
                         (int)SPREADING_FACTOR,             // Dataset length
                         "Constellation Diagram",           // Dataset title
                         "points pt 7 ps 0.5",              // Graph type
                         "blue",                            // Colour
                         GPC_NEW);                          // New graph
        }
        else {
            gpc_plot_xy (hConstellationDiagram,             // Graph handle
                         (ComplexRect_s *)DataOut,          // Array of complex dataset
                         (int)SPREADING_FACTOR,             // Dataset length
                         "Constellation Diagram",           // Dataset title
                         "points pt 7 ps 0.5",              // Graph type
                         "blue",                            // Colour
                         GPC_ADD);                          // New graph
        }
#endif

        DataOutBits =
            SDA_ComplexQPSKDeSpread (DataOut,               // Pointer to destination array
                                     ChannelizationCode,    // In-phase channelization code
                                     ChannelizationCode,    // Quadrature-phase channelization code
                                     SIGLIB_ONE / w16I,     // In-phase weighting value
                                     SIGLIB_ONE / w16Q,     // Quadrature-phase weighting value
                                     DescramblingCode,      // Complex scrambling code
                                     DemodErrorArray,       // Demodulator error array
                                     SPREADING_FACTOR);     // Spreading factor

//printf ("DataOutBits = %d\n", (int)DataOutBits);
        DataOutBits = SDS_ReverseDiBits (DataOutBits);      // Put bits in ITU order, not computer order

        SDS_QpskBitErrorCount (DataInBits,                  // Input data bits
                               DataOutBits,                 // Output data bits
                               &BitCount,                   // Bit count
                               &BitErrorCount);             // Bit error count

        if (DataOutBits != DataInBits) {
            printf ("FAIL - LoopCount = %d, DataInBits = %d, DataOutBits = %d\n", (int)LoopCount, (int)DataInBits, (int)DataOutBits);
        }
    }

    if (BitErrorCount == (SLFixData_t)0) {
        printf ("PASS - Bit error rate = 0\n");
    }
    else {
        printf ("FAIL !\n");
        printf ("Total number of bits = %d, Number of bits in error = %d, Bit error rate = %lf\n",
                BitCount, BitErrorCount, SDS_BitErrorRate (BitCount, BitErrorCount));
    }

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (hConstellationDiagram);

    return 0;
}

