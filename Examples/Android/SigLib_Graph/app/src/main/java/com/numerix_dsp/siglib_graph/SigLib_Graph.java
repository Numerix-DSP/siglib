// SigLib Android example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

package com.numerix_dsp.siglib_graph;

import android.os.Bundle;

import android.support.v7.app.AppCompatActivity;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import siglib_wrap.siglib_wrap;
import siglib_wrap.siglib_wrapJNI;
import siglib_wrap.SLSignal_t;
import siglib_wrap.SLSignalFillMode_t;
import siglib_wrap.SWIGTYPE_p_double;

public class SigLib_Graph extends AppCompatActivity {

    private siglib_wrap mSigLib;

    static {
        try {
            System.loadLibrary("siglib_wrap");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("siglib_wrap library failed to load.\n" + e);
            System.exit(1);
        }
    }

    int SAMPLE_LENGTH = 512;

    LineGraphSeries<DataPoint> series;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sig_lib__graph);

        mSigLib = new siglib_wrap();

        SWIGTYPE_p_double SinePhase = mSigLib.new_doubleArray(1);
        mSigLib.doubleArray_setitem(SinePhase, 0, 0.0);

        SWIGTYPE_p_double timeArray = mSigLib.new_doubleArray(SAMPLE_LENGTH);
        SWIGTYPE_p_double nullArray = mSigLib.new_doubleArray(1);

        mSigLib.SDA_SignalGenerate (timeArray,          // Output array pointer
                SLSignal_t.SIGLIB_SINE_WAVE,            // Signal type - Sine wave
                siglib_wrapJNI.SIGLIB_ONE_get(),        // Signal peak level
                SLSignalFillMode_t.SIGLIB_FILL,         // Fill (overwrite) or add to existing array contents
                0.015,                              // Signal frequency
                siglib_wrapJNI.SIGLIB_ZERO_get(),       // D.C. Offset
                siglib_wrapJNI.SIGLIB_ZERO_get(),       // Unused
                siglib_wrapJNI.SIGLIB_ZERO_get(),       // Signal end value - Unused
                SinePhase,                              // Signal phase - maintained across array boundaries
                nullArray,                              // Unused
                SAMPLE_LENGTH);                         // Output dataset length

        GraphView graph = (GraphView) findViewById(R.id.graph1);
        series = new LineGraphSeries<DataPoint>();
        for(int i =0; i<SAMPLE_LENGTH; i++) {
            series.appendData(new DataPoint((double)i, siglib_wrap.doubleArray_getitem(timeArray, i)), true, SAMPLE_LENGTH);
        }
        graph.addSeries(series);
    }
}
