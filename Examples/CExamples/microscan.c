// SigLib Microscan Spectrum Analyzer
// The position of the decoded signal pulse relative to the time
// reference signal indicates the frequency of the signal being
// analysed. It is important that the chirp and the FM modulation
// rate are synchronised.
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                 // SigLib DSP library
#include <gnuplot_c.h>                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH           512
#define FFT_LENGTH              512
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)   // Log2 FFT length,
#define WINDOW_LENGTH           FFT_LENGTH

#define BAUD_RATE               0.02
#define CARRIER_FREQ            0.12
#define MOD_INDEX               0.06
#define FIR_FILTER_LENGTH       127
#define FIR_FILTER_GROUP_DELAY  (FIR_FILTER_LENGTH >> 1)    // Group delay through BP filter - Note : C array indexing

#define C_START_FREQ            SIGLIB_QUARTER
#define C_END_FREQ              SIGLIB_ZERO
#define C_RATE                  -0.005

#define IIR_FILTER_STAGES       3       // Number of 2nd-order filter stages

#define SINE_TABLE_SIZE         1024    // Look up table for fast sine calculation

// Declare global variables and arrays

            // IIR LPF, Fc = Fs * 0.1
                // IIR filter coeffs, b(0), b(1), b(2), a(1), a(2)
static const SLData_t   pIIRFilterTaps[IIR_FILTER_STAGES * SIGLIB_IIR_COEFFS_PER_BIQUAD] = {
     9.42380353047E-0002,  1.08526026540E-0001,  9.42380353047E-0002, -1.73648791886E+0000,  7.81845268784E-0001,
     9.42380353047E-0002, -9.71701377691E-0002,  9.42380353047E-0002, -1.64442139918E+0000,  8.62782495425E-0001,
     9.42380353047E-0002, -5.53439298689E-0002,  9.42380353047E-0002, -1.59154638820E+0000,  9.56063978767E-0001
    };

static SLData_t         pIIRFilterState[IIR_FILTER_STAGES * SIGLIB_IIR_DELAY_SIZE];


            // IIR BPF, Fcl = Fs * 0.2, Fch = Fs * 0.3
                                    // FIR filter coefficients
static const SLData_t   pFIRFilterTaps[] = {
    -1.38075490316E-0005,  2.41051088864E-0004,  4.73956931276E-0005,
    -5.85739974917E-0004, -9.00228508147E-0005,  9.56269073398E-0004,
     1.21236356302E-0004, -9.65373462443E-0004, -9.99681590042E-0005,
     4.89918366903E-0005, -1.80352187298E-0005,  2.35297169523E-0003,
     2.58084751155E-0004, -6.47520118929E-0003, -6.02311946364E-0004,
     1.18966532325E-0002,  9.77734150179E-0004, -1.73860220463E-0002,
    -1.26768523506E-0003,  2.10914594051E-0002,  1.34949934888E-0003,
    -2.11239291853E-0002, -1.14695326530E-0003,  1.63911242484E-0002,
     6.75610981552E-0004, -7.35467104645E-0003, -5.68304796037E-0005,
    -3.68978632936E-0003, -5.13643123625E-0004,  1.30960475368E-0002,
     8.36499450705E-0004, -1.71078708991E-0002, -7.92859821064E-0004,
     1.34771825066E-0002,  4.03561290231E-0004, -2.79832260629E-0003,
     1.67104251482E-0004, -1.11329941666E-0002, -6.72056901192E-0004,
     2.23307751294E-0002,  8.84894665148E-0004, -2.48468761517E-0002,
    -7.01920038599E-0004,  1.54736860245E-0002,  1.94656609430E-0004,
     4.18063903013E-0003,  4.15258753630E-0004, -2.72700605026E-0002,
    -8.52016468815E-0004,  4.34191068477E-0002,  9.11038668954E-0004,
    -4.21979381594E-0002, -5.56075416387E-0004,  1.72007250340E-0002,
    -5.90563217469E-0005,  3.08357346502E-0002,  6.57218611592E-0004,
    -9.32198991442E-0002, -9.64218200809E-0004,  1.55348973428E-0001,
     8.36321045157E-0004, -2.01069394393E-0001, -3.28653631367E-0004,
     2.17875172784E-0001, -3.28653631367E-0004, -2.01069394393E-0001,
     8.36321045157E-0004,  1.55348973428E-0001, -9.64218200809E-0004,
    -9.32198991442E-0002,  6.57218611592E-0004,  3.08357346502E-0002,
    -5.90563217469E-0005,  1.72007250340E-0002, -5.56075416387E-0004,
    -4.21979381594E-0002,  9.11038668954E-0004,  4.34191068477E-0002,
    -8.52016468815E-0004, -2.72700605026E-0002,  4.15258753630E-0004,
     4.18063903013E-0003,  1.94656609430E-0004,  1.54736860245E-0002,
    -7.01920038599E-0004, -2.48468761517E-0002,  8.84894665148E-0004,
     2.23307751294E-0002, -6.72056901192E-0004, -1.11329941666E-0002,
     1.67104251482E-0004, -2.79832260629E-0003,  4.03561290231E-0004,
     1.34771825066E-0002, -7.92859821064E-0004, -1.71078708991E-0002,
     8.36499450705E-0004,  1.30960475368E-0002, -5.13643123625E-0004,
    -3.68978632936E-0003, -5.68304796037E-0005, -7.35467104645E-0003,
     6.75610981552E-0004,  1.63911242484E-0002, -1.14695326530E-0003,
    -2.11239291853E-0002,  1.34949934888E-0003,  2.10914594051E-0002,
    -1.26768523506E-0003, -1.73860220463E-0002,  9.77734150179E-0004,
     1.18966532325E-0002, -6.02311946364E-0004, -6.47520118929E-0003,
     2.58084751155E-0004,  2.35297169523E-0003, -1.80352187298E-0005,
     4.89918366903E-0005, -9.99681590042E-0005, -9.65373462443E-0004,
     1.21236356302E-0004,  9.56269073398E-0004, -9.00228508147E-0005,
    -5.85739974917E-0004,  4.73956931276E-0005,  2.41051088864E-0004,
    -1.38075490316E-0005
    };

static SLData_t     pFIRFilterState[FIR_FILTER_LENGTH];

static SLData_t     *pPNSequence, *pChirpData;
static SLData_t     *pDelay, *pDisplayArray;
static SLData_t     PnsPhase, PnsCurrentValue;
static SLData_t     SinePhase, ChirpPhase, ChirpValue, ImpulsePhase, CarrierPhase;
static SLData_t     *pRealData, *pImagData, *pWindowCoeffs, *pFFTCoeffs, *pTempDelay;

static SLArrayIndex_t   FIRFilterIndex;

static SLData_t     *pLookUpTable;                  // For fast cosine lookup
static SLData_t     CarrierPhase;


int main(void)
{
    h_GPC_Plot  *h2DPlot;                           // Plot object

                                                    // Allocate memory
    pPNSequence = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pChirpData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pDelay = SUF_VectorArrayAllocate (FIR_FILTER_GROUP_DELAY);
    pDisplayArray = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
    pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
    pWindowCoeffs = SUF_VectorArrayAllocate (WINDOW_LENGTH);
    pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);
    pTempDelay = SUF_VectorArrayAllocate (FIR_FILTER_GROUP_DELAY);
    pLookUpTable = SUF_VectorArrayAllocate (SINE_TABLE_SIZE);

    PnsPhase = SIGLIB_ZERO;
    ChirpPhase = SIGLIB_ZERO;
    ChirpValue = SIGLIB_ZERO;
    SinePhase = SIGLIB_ZERO;
    ImpulsePhase = SIGLIB_ZERO;
    CarrierPhase = SIGLIB_ZERO;

    PnsCurrentValue = SIGLIB_ZERO;

    h2DPlot =                                       // Initialize plot
        gpc_init_2d ("Microscan Spectrum Analyzer",   // Plot title
                     "Time",                        // X-Axis label
                     "Magnitude",                   // Y-Axis label
                     GPC_AUTO_SCALE,                // Scaling mode
                     GPC_SIGNED,                    // Sign mode
                     GPC_KEY_ENABLE);               // Legend / key mode
    if (NULL == h2DPlot) {
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }


    SIF_Iir (pIIRFilterState,                       // Pointer to filter state array
             IIR_FILTER_STAGES);                    // Number of second order stages
    SIF_Fir (pFIRFilterState,                       // Pointer to filter state array
             &FIRFilterIndex,                       // Pointer to filter index register
             FIR_FILTER_LENGTH);                    // Filter length
                                                    // Initialize FM modulator
    SIF_FrequencyModulate (&CarrierPhase,           // Pointer to carrier phase
                           pLookUpTable,            // Pointer to LUT array
                           SINE_TABLE_SIZE);        // Table length

                                                    // Initialise FFT
    SIF_Fft (pFFTCoeffs,                            // Pointer to FFT coefficients
             SIGLIB_BIT_REV_STANDARD,               // Bit reverse mode flag / Pointer to bit reverse address table
             FFT_LENGTH);                           // FFT length
                                                    // Generate Hanning window table
    SIF_Window (pWindowCoeffs,                      // Pointer to window oefficient
                SIGLIB_HANNING,                     // Window type
                SIGLIB_ZERO,                        // Window coefficient
                FFT_LENGTH);                        // Window length


    SDA_SignalGenerate (pChirpData,                 // Pointer to destination array
                        SIGLIB_CHIRP_LIN,           // Signal type - Chirp with linear frequency ramp
                        SIGLIB_ONE,                 // Signal peak level
                        SIGLIB_FILL,                // Fill (overwrite) or add to existing array contents
                        C_START_FREQ,               // Signal lower frequency
                        SIGLIB_ZERO,                // D.C. Offset
                        C_RATE,                     // Frequency change per sample period
                        C_END_FREQ,                 // Signal upper frequency
                        &ChirpPhase,                // Chirp phase - used for next iteration
                        &ChirpValue,                // Chirp current value - used for next iteration
                        SAMPLE_LENGTH);             // Output dataset length

    SIF_FixedDelay (pDelay,                         // Pointer to delay state array
                    SIGLIB_NULL_ARRAY_INDEX_PTR,    // Pointer to delay state index
                    FIR_FILTER_GROUP_DELAY);        // Delay length
    SDA_ShortFixedDelay (pChirpData,                // Pointer to source array
                         pDisplayArray,             // Pointer to destination array
                         pDelay,                    // Pointer to temporary delayed array
                         pTempDelay,                // Temporary destination array pointer
                         FIR_FILTER_GROUP_DELAY,    // Sample delay count
                         SAMPLE_LENGTH);            // Dataset length
    SDA_Polynomial (pDisplayArray,                  // Pointer to source array
                    pDisplayArray,                  // Pointer to destination array
                    0.75,                           // x^0 coefficient
                    SIGLIB_QUARTER,                 // x^1 coefficient
                    0,                              // x^2 coefficient
                    0,                              // x^3 coefficient
                    0,                              // x^4 coefficient
                    0,                              // x^5 coefficient
                    SAMPLE_LENGTH);                 // Dataset length

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pDisplayArray,                     // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Chirp signal",                    // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph
    printf ("\nChirp signal");

    SDA_SignalGenerate (pPNSequence,                // Pointer to destination array
                        SIGLIB_IMPULSE_STREAM,      // Signal type - Impulse stream
                        SIGLIB_HALF,                // Signal peak level
                        SIGLIB_FILL,                // Fill (overwrite) or add to existing array contents
                        BAUD_RATE,                  // Signal frequency
                        SIGLIB_ZERO,                // D.C. Offset
                        SIGLIB_ZERO,                // Unused
                        SIGLIB_ZERO,                // Signal end value - Unused
                        &ImpulsePhase,              // Signal phase - maintained across array boundaries
                        SIGLIB_NULL_DATA_PTR,       // Unused
                        SAMPLE_LENGTH);             // Output dataset length

    SIF_FixedDelay (pDelay,                         // Pointer to delay state array
                    SIGLIB_NULL_ARRAY_INDEX_PTR,    // Pointer to delay state index
                    FIR_FILTER_GROUP_DELAY);        // Delay length
    SDA_ShortFixedDelay (pPNSequence,               // Pointer to source array
                         pDisplayArray,             // Pointer to destination array
                         pDelay,                    // Pointer to temporary delayed array
                         pTempDelay,                // Temporary destination array pointer
                         FIR_FILTER_GROUP_DELAY,    // Sample delay count
                         SAMPLE_LENGTH);            // Dataset length
    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pDisplayArray,                     // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Time reference signal",           // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "red",                             // Colour
                 GPC_ADD);                          // New graph
    printf ("\nTime reference signal");

                                                    // Generate the PN source
    SDA_SignalGenerate (pPNSequence,                // Pointer to destination array
                        SIGLIB_PN_SEQUENCE,         // Signal type - Pseudo random number sequence
                        SIGLIB_TWO,                 // Signal magnitude range
                        SIGLIB_FILL,                // Fill (overwrite) or add to existing array contents
                        BAUD_RATE,                  // Signal frequency
                        SIGLIB_MINUS_ONE,           // Signal minimum level
                        SIGLIB_FOUR,                // Number of discrete levels in PN sequence
                        SIGLIB_ZERO,                // Signal end value - Unused
                        &PnsPhase,                  // PRN phase - used for next iteration
                        &PnsCurrentValue,           // PRN current value - used for next iteration
                        SAMPLE_LENGTH);             // Output dataset length

    SIF_FixedDelay (pDelay,                         // Pointer to delay state array
                    SIGLIB_NULL_ARRAY_INDEX_PTR,    // Pointer to delay state index
                    FIR_FILTER_GROUP_DELAY);        // Delay length
    SDA_ShortFixedDelay (pPNSequence,               // Pointer to source array
                         pDisplayArray,             // Pointer to destination array
                         pDelay,                    // Pointer to temporary delayed array
                         pTempDelay,                // Temporary destination array pointer
                         FIR_FILTER_GROUP_DELAY,    // Sample delay count
                         SAMPLE_LENGTH);            // Dataset length
    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pDisplayArray,                     // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "PN sequence",                     // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "green",                           // Colour
                 GPC_ADD);                          // New graph
    printf ("\nPN sequence");

                                                    // Frequency modulate the PN signal
    SDA_FrequencyModulate (pPNSequence,             // Modulating signal source pointer
                           pPNSequence,             // Modulated signal destination pointer
                           CARRIER_FREQ,            // Carrier frequency
                           MOD_INDEX,               // Modulation index
                           &CarrierPhase,           // Pointer to carrier phase
                           pLookUpTable,            // Fast cosine look up table
                           SINE_TABLE_SIZE,         // Look up table size
                           SAMPLE_LENGTH);          // Dataset length

    SIF_FixedDelay (pDelay,                         // Pointer to delay state array
                    SIGLIB_NULL_ARRAY_INDEX_PTR,    // Pointer to delay state index
                    FIR_FILTER_GROUP_DELAY);        // Delay length
    SDA_ShortFixedDelay (pPNSequence,               // Pointer to source array
                         pDisplayArray,             // Pointer to destination array
                         pDelay,                    // Pointer to temporary delayed array
                         pTempDelay,                // Temporary destination array pointer
                         FIR_FILTER_GROUP_DELAY,    // Sample delay count
                         SAMPLE_LENGTH);            // Dataset length
    SDA_Polynomial (pDisplayArray,                  // Pointer to source array
                    pDisplayArray,                  // Pointer to destination array
                    SIGLIB_MINUS_HALF,              // x^0 coefficient
                    SIGLIB_QUARTER,                 // x^1 coefficient
                    0,                              // x^2 coefficient
                    0,                              // x^3 coefficient
                    0,                              // x^4 coefficient
                    0,                              // x^5 coefficient
                    SAMPLE_LENGTH);                 // Dataset length

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pDisplayArray,                     // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Modulated sequence",              // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "medium-blue",                     // Colour
                 GPC_ADD);                          // New graph
    printf ("\nModulated sequence");

                                                    // Window modulated data
    SDA_Window (pPNSequence,                        // Pointer to source array
                pRealData,                          // Pointer to destination array
                pWindowCoeffs,                      // Pointer to window coefficients
                WINDOW_LENGTH);                     // Window length

                                                    // Perform real FFT
    SDA_Rfft (pRealData,                            // Pointer to real array
              pImagData,                            // Pointer to imaginary array
              pFFTCoeffs,                           // Pointer to FFT coefficients
              SIGLIB_BIT_REV_STANDARD,              // Bit reverse mode flag / Pointer to bit reverse address table
              FFT_LENGTH,                               // FFT length
              LOG2_FFT_LENGTH);                     // log2 FFT length

                                                    // Calculate real power from complex
    SDA_LogMagnitude (pRealData,                    // Pointer to real source array
                      pImagData,                    // Pointer to imaginary source array
                      pWindowCoeffs,                // Pointer to log magnitude destination array
                      FFT_LENGTH);                  // Dataset length

                                                    // Mix with a chirp
    SDA_Multiply2 (pPNSequence,                     // Pointer to source array 2
                   pChirpData,                      // Pointer to source array 1
                   pPNSequence,                     // Pointer to destination array
                   SAMPLE_LENGTH);                  // Dataset length

                                                    // Matched filter
    SDA_Fir (pPNSequence,                           // Input array to be filtered
             pPNSequence,                           // Filtered output array
             pFIRFilterState,                       // Pointer to filter state array
             pFIRFilterTaps,                        // Pointer to filter coefficients
             &FIRFilterIndex,                       // Pointer to filter index register
             FIR_FILTER_LENGTH,                     // Filter length
             SAMPLE_LENGTH);                        // Dataset length

                                                    // Square law detector
    SDA_Power (pPNSequence,                         // Pointer to source array
               pPNSequence,                         // Pointer to destination array
               2,                                   // Power to raise input by
               SAMPLE_LENGTH);                      // Dataset length

                                                    // Filter demodulated signals
    SDA_Iir (pPNSequence,                           // Input array to be filtered
             pPNSequence,                           // Filtered output array
             pIIRFilterState,                       // Pointer to filter state array
             pIIRFilterTaps,                        // Pointer to filter coefficients array
             IIR_FILTER_STAGES,                     // Number of stages
             SAMPLE_LENGTH);                        // Dataset length

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pPNSequence,                       // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Decoded signal",                  // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "orange",                          // Colour
                 GPC_ADD);                          // New graph
    printf ("\nDecoded signal\nPlease hit <Carriage Return> to continue . . ."); getchar();


    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pWindowCoeffs,                     // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Spectrum",                        // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "orange",                          // Colour
                 GPC_NEW);                          // New graph
    printf ("\nSpectrum");

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (h2DPlot);

    SUF_MemoryFree (pPNSequence);                   // Free memory
    SUF_MemoryFree (pChirpData);
    SUF_MemoryFree (pDelay);
    SUF_MemoryFree (pDisplayArray);
    SUF_MemoryFree (pRealData);
    SUF_MemoryFree (pImagData);
    SUF_MemoryFree (pWindowCoeffs);
    SUF_MemoryFree (pFFTCoeffs);
    SUF_MemoryFree (pTempDelay);
    SUF_MemoryFree (pLookUpTable);

    exit(0);
}


