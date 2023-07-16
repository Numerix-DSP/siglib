// SigLib - Vertex finder example
// Copyright (c) 2023 Delta Numerix All rights reserved.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <siglib.h>                                                 // SigLib DSP library

#define ARRAY_LENGTH    8

SLData_t        samples[ARRAY_LENGTH];


int main (
  int argc,
  char *argv[])
{
  SLData_t        x0, x1, x2, y0, y1, y2;

  if (argc == 4) {
    y0 = (SLData_t) atof (argv[1]);
    y1 = (SLData_t) atof (argv[2]);
    y2 = (SLData_t) atof (argv[3]);

    printf ("Sample #1 = %lf\n", y0);
    printf ("Sample #2 = %lf\n", y1);
    printf ("Sample #3 = %lf\n", y2);

    printf ("Sample Processing\n");
    printf ("Vertex Magnitude = %lf\n", SDS_InterpolateThreePointQuadraticVertexMagnitude (y0, y1, y2));
    printf ("Vertex Location = %lf\n\n", SDS_InterpolateThreePointQuadraticVertexLocation (y0, y1, y2));

    samples[0] = y0;
    samples[1] = y1;
    samples[2] = y2;

    printf ("Array Processing\n");
    printf ("Vertex Magnitude = %lf\n", SDA_InterpolateThreePointQuadraticVertexMagnitude (samples));
    printf ("Vertex Location = %lf\n\n", SDA_InterpolateThreePointQuadraticVertexLocation (samples));

    SDA_Clear (samples, ARRAY_LENGTH);
    samples[4] = y0;
    samples[5] = y1;
    samples[6] = y2;

    printf ("Arbitrary Length Array Processing\n");
    printf ("Vertex Magnitude = %lf\n", SDA_InterpolateArbitraryThreePointQuadraticVertexMagnitude (samples, ARRAY_LENGTH));
    printf ("Vertex Location = %lf\n\n", SDA_InterpolateArbitraryThreePointQuadraticVertexLocation (samples, ARRAY_LENGTH));
  }
  else if (argc == 7) {

    x0 = (SLData_t) atof (argv[1]);
    y0 = (SLData_t) atof (argv[2]);
    x1 = (SLData_t) atof (argv[3]);
    y1 = (SLData_t) atof (argv[4]);
    x2 = (SLData_t) atof (argv[5]);
    y2 = (SLData_t) atof (argv[6]);

    printf ("Sample #1 = %lf, %lf\n", x0, y0);
    printf ("Sample #2 = %lf, %lf\n", x1, y1);
    printf ("Sample #3 = %lf, %lf\n", x2, y2);

    printf ("Vertex Magnitude = %lf\n", SDS_InterpolateArbitraryThreePointQuadraticVertexMagnitude (x0, y0, x1, y1, x2, y2));
    printf ("Vertex Location = %lf\n\n", SDS_InterpolateArbitraryThreePointQuadraticVertexLocation (x0, y0, x1, y1, x2, y2));
  }
  else {

    printf ("\nUsage :\nVertexFinder y0 y1 y2\nor:\nVertexFinder x0 y0 x1 y1 x2 y2\n\nExamples :\n\n");

    y0 = 1.;
    y1 = 1.5;
    y2 = 1.2;
    printf ("Sample #1 = %lf\n", y0);
    printf ("Sample #2 = %lf\n", y1);
    printf ("Sample #3 = %lf\n", y2);

    printf ("Vertex Magnitude = %lf\n", SDS_InterpolateThreePointQuadraticVertexMagnitude (y0, y1, y2));
    printf ("Vertex Location = %lf\n\n", SDS_InterpolateThreePointQuadraticVertexLocation (y0, y1, y2));

    y0 = -1.;
    y1 = -1.5;
    y2 = -1.2;
    printf ("Sample #1 = %lf\n", y0);
    printf ("Sample #2 = %lf\n", y1);
    printf ("Sample #3 = %lf\n", y2);

    printf ("Vertex Magnitude = %lf\n", SDS_InterpolateThreePointQuadraticVertexMagnitude (y0, y1, y2));
    printf ("Vertex Location = %lf\n\n", SDS_InterpolateThreePointQuadraticVertexLocation (y0, y1, y2));


    x0 = 0.;
    y0 = 1.;
    x1 = 1.;
    y1 = 1.5;
    x2 = 2.;
    y2 = 1.2;
    printf ("Sample #1 = %lf, %lf\n", x0, y0);
    printf ("Sample #2 = %lf, %lf\n", x1, y1);
    printf ("Sample #3 = %lf, %lf\n", x2, y2);

    printf ("Vertex Magnitude = %lf\n", SDS_InterpolateArbitraryThreePointQuadraticVertexMagnitude (x0, y0, x1, y1, x2, y2));
    printf ("Vertex Location = %lf\n\n", SDS_InterpolateArbitraryThreePointQuadraticVertexLocation (x0, y0, x1, y1, x2, y2));

    x0 = 0.;
    y0 = -1.;
    x1 = 1.;
    y1 = -1.5;
    x2 = 2.;
    y2 = -1.2;
    printf ("Sample #1 = %lf, %lf\n", x0, y0);
    printf ("Sample #2 = %lf, %lf\n", x1, y1);
    printf ("Sample #3 = %lf, %lf\n", x2, y2);

    printf ("Vertex Magnitude = %lf\n", SDS_InterpolateArbitraryThreePointQuadraticVertexMagnitude (x0, y0, x1, y1, x2, y2));
    printf ("Vertex Location = %lf\n\n", SDS_InterpolateArbitraryThreePointQuadraticVertexLocation (x0, y0, x1, y1, x2, y2));


    y0 = 1.;
    y1 = 1.5;
    y2 = 1.2;
    printf ("Sample #1 = %lf\n", y0);
    printf ("Sample #2 = %lf\n", y1);
    printf ("Sample #3 = %lf\n", y2);

    samples[0] = y0;
    samples[1] = y1;
    samples[2] = y2;

    printf ("Array Processing\n");
    printf ("Vertex Magnitude = %lf\n", SDA_InterpolateThreePointQuadraticVertexMagnitude (samples));
    printf ("Vertex Location = %lf\n\n", SDA_InterpolateThreePointQuadraticVertexLocation (samples));

    SDA_Clear (samples, ARRAY_LENGTH);
    samples[4] = y0;
    samples[5] = y1;
    samples[6] = y2;

    printf ("Arbitrary Length Array Processing (Positive and Negative Vertex)\n");
    printf ("Vertex Magnitude = %lf\n", SDA_InterpolateArbitraryThreePointQuadraticVertexMagnitude (samples, ARRAY_LENGTH));
    printf ("Vertex Location = %lf\n\n", SDA_InterpolateArbitraryThreePointQuadraticVertexLocation (samples, ARRAY_LENGTH));

    printf ("Arbitrary Length Array Processing (Positive Only Vertex)\n");
    printf ("Vertex Magnitude = %lf\n", SDA_InterpolateArbitraryThreePointQuadraticPeakVertexMagnitude (samples, ARRAY_LENGTH));
    printf ("Vertex Location = %lf\n\n", SDA_InterpolateArbitraryThreePointQuadraticPeakVertexLocation (samples, ARRAY_LENGTH));
  }

  exit (0);
}
