/**************************************************************************
File Name               : siglib_iir_constants.h | Author : JOHN EDWARDS
Siglib Library Version  : 10.00                  |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 14/05/2006
Options   :                             | Latest Update : 17/11/2020
---------------------------------------------------------------------------
Support for SigLib is available via EMail : support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

__SIGLIB_LICENSE__

Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.
---------------------------------------------------------------------------
Description : IIR filter frequency normalized coefficients :
    Fc = 1 radian and normalised sample rate 2PI radians

Update history :
        See history.txt for more details.

****************************************************************************/

#if SIGLIB

#ifndef     _SIGLIB_IIR_CONSTANTS_H
#define     _SIGLIB_IIR_CONSTANTS_H

#define SIGLIB_MAX_NORMALIZED_IIR_FILTER_ORDER          10

                                                    // z-plane Butterworth filter coefficients
static SLData_t siglib_numerix_IIRButterworth1 [] = {
    (SLData_t) 3.53296003486988308200e-01, (SLData_t)3.53296003486988308200e-01, (SLData_t)0.00000000000000000000e+00,
    (SLData_t)-2.93407993026023383700e-01, (SLData_t)0.00000000000000000000e+00
};

static SLData_t siglib_numerix_IIRButterworth2 [] = {
    (SLData_t) 1.44104971248169844200e-01, (SLData_t)2.88209942496339688300e-01, (SLData_t)1.44104971248169844200e-01,
    (SLData_t)-6.77490874949784771600e-01, (SLData_t)2.53910759942464092600e-01
};

static SLData_t siglib_numerix_IIRButterworth3 [] = {
    (SLData_t) 1.61781590095293192900e-01, (SLData_t)3.23563180190586385800e-01, (SLData_t)1.61781590095293192900e-01,
    (SLData_t)-7.60595211081724897900e-01, (SLData_t)4.07721571462897669500e-01,

    (SLData_t) 3.53296003486988308200e-01, (SLData_t)3.53296003486988308200e-01, (SLData_t)0.00000000000000000000e+00,
    (SLData_t)-2.93407993026023383700e-01, (SLData_t)0.00000000000000000000e+00
};

static SLData_t siglib_numerix_IIRButterworth4 [] = {
    (SLData_t) 1.29316159920900480800e-01, (SLData_t)2.58632319841800961700e-01, (SLData_t)1.29316159920900480800e-01,
    (SLData_t)-6.07963192186333434300e-01, (SLData_t)1.25227831869935329900e-01,

    (SLData_t) 1.73862247042544909600e-01, (SLData_t)3.47724494085089819200e-01, (SLData_t)1.73862247042544909600e-01,
    (SLData_t)-8.17390856466274184900e-01, (SLData_t)5.12839844636453823200e-01
};

static SLData_t siglib_numerix_IIRButterworth5 [] = {
    (SLData_t) 1.36752573442887770400e-01, (SLData_t)2.73505146885775540800e-01, (SLData_t)1.36752573442887770400e-01,
    (SLData_t)-6.42924528078231749600e-01, (SLData_t)1.89934821849782858900e-01,

    (SLData_t) 1.82415545389257305300e-01, (SLData_t)3.64831090778514610600e-01, (SLData_t)1.82415545389257305300e-01,
    (SLData_t)-8.57603081835247249600e-01, (SLData_t)5.87265263392276470900e-01,

    (SLData_t) 3.53296003486988308200e-01, (SLData_t)3.53296003486988308200e-01, (SLData_t)0.00000000000000000000e+00,
    (SLData_t)-2.93407993026023383700e-01, (SLData_t)0.00000000000000000000e+00
};

static SLData_t siglib_numerix_IIRButterworth6 [] = {
    (SLData_t) 1.26792271688040247800e-01, (SLData_t)2.53584543376080495700e-01, (SLData_t)1.26792271688040247800e-01,
    (SLData_t)-5.96097458254010170700e-01, (SLData_t)1.03266545006171162060e-01,

    (SLData_t) 1.44104971248169844200e-01, (SLData_t)2.88209942496339688300e-01, (SLData_t)1.44104971248169844200e-01,
    (SLData_t)-6.77490874949784771600e-01, (SLData_t)2.53910759942464092600e-01,

    (SLData_t) 1.88742795774682281500e-01, (SLData_t)3.77485591549364563000e-01, (SLData_t)1.88742795774682281500e-01,
    (SLData_t)-8.87349830767771519100e-01, (SLData_t)6.42321013866500645200e-01
};

static SLData_t siglib_numerix_IIRButterworth7 [] = {
    (SLData_t) 1.30734160425252710100e-01, (SLData_t)2.61468320850505420200e-01, (SLData_t)1.30734160425252710100e-01,
    (SLData_t)-6.14629738066408748400e-01, (SLData_t)1.37566379767419588700e-01,

    (SLData_t) 1.50755295643006326200e-01, (SLData_t)3.01510591286012652500e-01, (SLData_t)1.50755295643006326200e-01,
    (SLData_t)-7.08756514531354020400e-01, (SLData_t)3.11777697103379269900e-01,

    (SLData_t) 1.93598511342537527000e-01, (SLData_t)3.87197022685075054000e-01, (SLData_t)1.93598511342537527000e-01,
    (SLData_t)-9.10178349174038836600e-01, (SLData_t)6.84572394544188944600e-01,

    (SLData_t) 3.53296003486988308200e-01, (SLData_t)3.53296003486988308200e-01, (SLData_t)0.00000000000000000000e+00,
    (SLData_t)-2.93407993026023383700e-01, (SLData_t)0.00000000000000000000e+00
};

static SLData_t siglib_numerix_IIRButterworth8 [] = {
    (SLData_t) 1.25923711399010762600e-01, (SLData_t)2.51847422798021525200e-01, (SLData_t)1.25923711399010762600e-01,
    (SLData_t)-5.92014034447985615400e-01, (SLData_t)9.57088800440286657800e-02,

    (SLData_t) 1.35232445260715983300e-01, (SLData_t)2.70464890521431966500e-01, (SLData_t)1.35232445260715983300e-01,
    (SLData_t)-6.35777842136344251200e-01, (SLData_t)1.76707623179208156400e-01,

    (SLData_t) 1.56626533134083756300e-01, (SLData_t)3.13253066268167512600e-01, (SLData_t)1.56626533134083756300e-01,
    (SLData_t)-7.36359377849773677500e-01, (SLData_t)3.62865510386108758300e-01,

    (SLData_t) 1.97437023501860820000e-01, (SLData_t)3.94874047003721639900e-01, (SLData_t)1.97437023501860820000e-01,
    (SLData_t)-9.28224617382557792800e-01, (SLData_t)7.17972711390001072600e-01
};

static SLData_t siglib_numerix_IIRButterworth9 [] = {
    (SLData_t) 1.28355255994507366800e-01, (SLData_t)2.56710511989014733600e-01, (SLData_t)1.28355255994507366800e-01,
    (SLData_t)-6.03445626718632888200e-01, (SLData_t)1.16866650696662410800e-01,

    (SLData_t) 1.39759372489180328000e-01, (SLData_t)2.79518744978360655900e-01, (SLData_t)1.39759372489180328000e-01,
    (SLData_t)-6.57060604710610785000e-01, (SLData_t)2.16098094667332069100e-01,

    (SLData_t) 1.61781590095293192900e-01, (SLData_t)3.23563180190586385800e-01, (SLData_t)1.61781590095293192900e-01,
    (SLData_t)-7.60595211081724897900e-01, (SLData_t)4.07721571462897669500e-01,

    (SLData_t) 2.00545201640733944400e-01, (SLData_t)4.01090403281467888800e-01, (SLData_t)2.00545201640733944400e-01,
    (SLData_t)-9.42837314700116002600e-01, (SLData_t)7.45018121263051780200e-01,

    (SLData_t) 3.53296003486988308200e-01, (SLData_t)3.53296003486988308200e-01, (SLData_t)0.00000000000000000000e+00,
    (SLData_t)-2.93407993026023383700e-01, (SLData_t)0.00000000000000000000e+00
};

static SLData_t siglib_numerix_IIRButterworth10 [] = {
    (SLData_t) 1.25524251093008021300e-01, (SLData_t)2.51048502186016042600e-01, (SLData_t)1.25524251093008021300e-01,
    (SLData_t)-5.90136023510004714700e-01, (SLData_t)9.22330278820367999200e-02,

    (SLData_t) 1.31360503418619184400e-01, (SLData_t)2.62721006837238368900e-01, (SLData_t)1.31360503418619184400e-01,
    (SLData_t)-6.17574408600110924600e-01, (SLData_t)1.43016422274587634600e-01,

    (SLData_t) 1.44104971248169844200e-01, (SLData_t)2.88209942496339688300e-01, (SLData_t)1.44104971248169844200e-01,
    (SLData_t)-6.77490874949784771600e-01, (SLData_t)2.53910759942464092600e-01,

    (SLData_t) 1.66313710984942519900e-01, (SLData_t)3.32627421969885039800e-01, (SLData_t)1.66313710984942519900e-01,
    (SLData_t)-7.81902390981985995200e-01, (SLData_t)4.47157234921756019400e-01,

    (SLData_t) 2.03112164432611291800e-01, (SLData_t)4.06224328865222583600e-01, (SLData_t)2.03112164432611291800e-01,
    (SLData_t)-9.54905558097754014700e-01, (SLData_t)7.67354215828199293100e-01
};


                                                    // z-plane Butterworth filter coefficients map
static SLData_t *siglib_numerix_pIIRButterworthFilters [] = {
    siglib_numerix_IIRButterworth1,         // Dummy - there is no zero length filter
    siglib_numerix_IIRButterworth1, siglib_numerix_IIRButterworth2, siglib_numerix_IIRButterworth3,
    siglib_numerix_IIRButterworth4, siglib_numerix_IIRButterworth5, siglib_numerix_IIRButterworth6,
    siglib_numerix_IIRButterworth7, siglib_numerix_IIRButterworth8, siglib_numerix_IIRButterworth9,
    siglib_numerix_IIRButterworth10
};


                                                    // z-plane Bessel filter coefficients
static SLData_t siglib_numerix_IIRBessel1 [] = {
    (SLData_t) 3.53296016761471598500e-01, (SLData_t)3.53296016761471598500e-01, (SLData_t)0.00000000000000000000e+00,
    (SLData_t)-2.93407966477056803000e-01, (SLData_t)0.00000000000000000000e+00
};

static SLData_t siglib_numerix_IIRBessel2 [] = {
    (SLData_t) 1.44104982456742969400e-01, (SLData_t)2.88209964913485938700e-01, (SLData_t)1.44104982456742969400e-01,
    (SLData_t)-6.77490815431722293000e-01, (SLData_t)2.53910745258694226000e-01
};

static SLData_t siglib_numerix_IIRBessel3 [] = {
    (SLData_t) 1.61781603069332202700e-01, (SLData_t)3.23563206138664405400e-01, (SLData_t)1.61781603069332202700e-01,
    (SLData_t)-7.60595146099088848000e-01, (SLData_t)4.07721558376417714200e-01,

    (SLData_t) 3.53296016761471598500e-01, (SLData_t)3.53296016761471598500e-01, (SLData_t)0.00000000000000000000e+00,
    (SLData_t)-2.93407966477056803000e-01, (SLData_t)0.00000000000000000000e+00
};

static SLData_t siglib_numerix_IIRBessel4 [] = {
    (SLData_t) 1.29316169718002332800e-01, (SLData_t)2.58632339436004665700e-01, (SLData_t)1.29316169718002332800e-01,
    (SLData_t)-6.07963137548384935400e-01, (SLData_t)1.25227816420394266800e-01,

    (SLData_t) 1.73862261272245355300e-01, (SLData_t)3.47724522544490710600e-01, (SLData_t)1.73862261272245355300e-01,
    (SLData_t)-8.17390787979829558400e-01, (SLData_t)5.12839833068810979600e-01
};

static SLData_t siglib_numerix_IIRBessel5 [] = {
    (SLData_t) 1.36752583942267386100e-01, (SLData_t)2.73505167884534772200e-01, (SLData_t)1.36752583942267386100e-01,
    (SLData_t)-6.42924470951257820400e-01, (SLData_t)1.89934806720327364900e-01,

    (SLData_t) 1.82415560532089932000e-01, (SLData_t)3.64831121064179864100e-01, (SLData_t)1.82415560532089932000e-01,
    (SLData_t)-8.57603010981369817700e-01, (SLData_t)5.87265253109729545800e-01,

    (SLData_t) 3.53296016761471598500e-01, (SLData_t)3.53296016761471598500e-01, (SLData_t)0.00000000000000000000e+00,
    (SLData_t)-2.93407966477056803000e-01, (SLData_t)0.00000000000000000000e+00
};

static SLData_t siglib_numerix_IIRBessel6 [] = {
    (SLData_t) 1.26792281250225319400e-01, (SLData_t)2.53584562500450638700e-01, (SLData_t)1.26792281250225319400e-01,
    (SLData_t)-5.96097404476967351900e-01, (SLData_t)1.03266529477868601590e-01,

    (SLData_t) 1.44104982456742969400e-01, (SLData_t)2.88209964913485938700e-01, (SLData_t)1.44104982456742969400e-01,
    (SLData_t)-6.77490815431722293000e-01, (SLData_t)2.53910745258694226000e-01,

    (SLData_t) 1.88742811605857679900e-01, (SLData_t)3.77485623211715359800e-01, (SLData_t)1.88742811605857679900e-01,
    (SLData_t)-8.87349758223054507800e-01, (SLData_t)6.42321004646485227400e-01
};

static SLData_t siglib_numerix_IIRBessel7 [] = {
    (SLData_t) 1.30734170355101792000e-01, (SLData_t)2.61468340710203583900e-01, (SLData_t)1.30734170355101792000e-01,
    (SLData_t)-6.14629682948364508600e-01, (SLData_t)1.37566364368771704200e-01,

    (SLData_t) 1.50755307505770985700e-01, (SLData_t)3.01510615011541971400e-01, (SLData_t)1.50755307505770985700e-01,
    (SLData_t)-7.08756452910318302000e-01, (SLData_t)3.11777682933402189300e-01,

    (SLData_t) 1.93598527709383239100e-01, (SLData_t)3.87197055418766478200e-01, (SLData_t)1.93598527709383239100e-01,
    (SLData_t)-9.10178275366589129000e-01, (SLData_t)6.84572386204122085300e-01,

    (SLData_t) 3.53296016761471598500e-01, (SLData_t)3.53296016761471598500e-01, (SLData_t)0.00000000000000000000e+00,
    (SLData_t)-2.93407966477056803000e-01, (SLData_t)0.00000000000000000000e+00
};

static SLData_t siglib_numerix_IIRBessel8 [] = {
    (SLData_t) 1.25923720880754874700e-01, (SLData_t)2.51847441761509749400e-01, (SLData_t)1.25923720880754874700e-01,
    (SLData_t)-5.92013980969103625900e-01, (SLData_t)9.57088644921231940200e-02,

    (SLData_t) 1.35232455615309887800e-01, (SLData_t)2.70464911230619775600e-01, (SLData_t)1.35232455615309887800e-01,
    (SLData_t)-6.35777785512394388800e-01, (SLData_t)1.76707607973633995500e-01,

    (SLData_t) 1.56626545584441778700e-01, (SLData_t)3.13253091168883557300e-01, (SLData_t)1.56626545584441778700e-01,
    (SLData_t)-7.36359314419332555700e-01, (SLData_t)3.62865496757099614800e-01,

    (SLData_t) 1.97437040296719407100e-01, (SLData_t)3.94874080593438814300e-01, (SLData_t)1.97437040296719407100e-01,
    (SLData_t)-9.28224542598327429000e-01, (SLData_t)7.17972703785205057500e-01
};

static SLData_t siglib_numerix_IIRBessel9 [] = {
    (SLData_t) 1.28355265701965787200e-01, (SLData_t)2.56710531403931574400e-01, (SLData_t)1.28355265701965787200e-01,
    (SLData_t)-6.03445572407486974200e-01, (SLData_t)1.16866635215350123100e-01,

    (SLData_t) 1.39759383276803567100e-01, (SLData_t)2.79518766553607134200e-01, (SLData_t)1.39759383276803567100e-01,
    (SLData_t)-6.57060546597399763800e-01, (SLData_t)2.16098079704614060000e-01,

    (SLData_t) 1.61781603069332202700e-01, (SLData_t)3.23563206138664405400e-01, (SLData_t)1.61781603069332202700e-01,
    (SLData_t)-7.60595146099088848000e-01, (SLData_t)4.07721558376417714200e-01,

    (SLData_t) 2.00545218785118495300e-01, (SLData_t)4.01090437570236990600e-01, (SLData_t)2.00545218785118495300e-01,
    (SLData_t)-9.42837239138816118900e-01, (SLData_t)7.45018114279290100100e-01,

    (SLData_t) 3.53296016761471598500e-01, (SLData_t)3.53296016761471598500e-01, (SLData_t)0.00000000000000000000e+00,
    (SLData_t)-2.93407966477056803000e-01, (SLData_t)0.00000000000000000000e+00
};

static SLData_t siglib_numerix_IIRBessel10 [] = {
    (SLData_t) 1.25524260537825754800e-01, (SLData_t)2.51048521075651509600e-01, (SLData_t)1.25524260537825754800e-01,
    (SLData_t)-5.90135970168574886000e-01, (SLData_t)9.22330123198778911700e-02,

    (SLData_t) 1.31360513407278639600e-01, (SLData_t)2.62721026814557279100e-01, (SLData_t)1.31360513407278639600e-01,
    (SLData_t)-6.17574353270826881200e-01, (SLData_t)1.43016406899941467300e-01,

    (SLData_t) 1.44104982456742969400e-01, (SLData_t)2.88209964913485938700e-01, (SLData_t)1.44104982456742969400e-01,
    (SLData_t)-6.77490815431722293000e-01, (SLData_t)2.53910745258694226000e-01,

    (SLData_t) 1.66313724425377118500e-01, (SLData_t)3.32627448850754237100e-01, (SLData_t)1.66313724425377118500e-01,
    (SLData_t)-7.81902324662911763000e-01, (SLData_t)4.47157222364420181600e-01,

    (SLData_t) 2.03112181867649760400e-01, (SLData_t)4.06224363735299520700e-01, (SLData_t)2.03112181867649760400e-01,
    (SLData_t)-9.54905481904046116200e-01, (SLData_t)7.67354209374645268600e-01
};

                                                    // z-plane Bessel filter coefficients map
static SLData_t *siglib_numerix_pIIRBesselFilters [] = {
    siglib_numerix_IIRBessel1,          // Dummy - there is no zero length filter
    siglib_numerix_IIRBessel1, siglib_numerix_IIRBessel2, siglib_numerix_IIRBessel3,
    siglib_numerix_IIRBessel4, siglib_numerix_IIRBessel5, siglib_numerix_IIRBessel6,
    siglib_numerix_IIRBessel7, siglib_numerix_IIRBessel8, siglib_numerix_IIRBessel9,
    siglib_numerix_IIRBessel10
};


                                                    // S-plane Bessel filter coefficients
static SLComplexRect_s siglib_numerix_SPlaneBesselPoles1[] = {
    {(SLData_t)-1.0000000000, (SLData_t)0.0000000000}
};

static SLComplexRect_s siglib_numerix_SPlaneBesselPoles2[] = {
    {(SLData_t)-1.10160133059, (SLData_t)0.636009824757}
};

static SLComplexRect_s siglib_numerix_SPlaneBesselPoles3[] = {
    {(SLData_t)-1.0474091610, (SLData_t)0.9992644363},
    {(SLData_t)-1.3226757999, (SLData_t)0.0000000000}
};

static SLComplexRect_s siglib_numerix_SPlaneBesselPoles4[] = {
    {(SLData_t)-0.995208764350, (SLData_t)1.257105739450},
    {(SLData_t)-1.370067830550, (SLData_t)0.410249717494}
};

static SLComplexRect_s siglib_numerix_SPlaneBesselPoles5[] = {
    {(SLData_t)-0.9576765486, (SLData_t)1.4711243207},
    {(SLData_t)-1.3808773259, (SLData_t)0.7179095876},
    {(SLData_t)-1.5023162714, (SLData_t)0.0000000000}
};

static SLComplexRect_s siglib_numerix_SPlaneBesselPoles6[] = {
    {(SLData_t)-0.930656522947, (SLData_t)1.661863268940},
    {(SLData_t)-1.381858097600, (SLData_t)0.971471890712},
    {(SLData_t)-1.571490403620, (SLData_t)0.320896374221}
};

static SLComplexRect_s siglib_numerix_SPlaneBesselPoles7[] = {
    {(SLData_t)-0.9098677806, (SLData_t)1.8364513530},
    {(SLData_t)-1.3789032168, (SLData_t)1.1915667778},
    {(SLData_t)-1.6120387662, (SLData_t)0.5892445069},
    {(SLData_t)-1.6843681793, (SLData_t)0.0000000000}
};

static SLComplexRect_s siglib_numerix_SPlaneBesselPoles8[] = {
    {(SLData_t)-0.892869718847, (SLData_t)1.998325843640},
    {(SLData_t)-1.373841217640, (SLData_t)1.388356575880},
    {(SLData_t)-1.636939418130, (SLData_t)0.822795625139},
    {(SLData_t)-1.757408400400, (SLData_t)0.272867575103}
};

static SLComplexRect_s siglib_numerix_SPlaneBesselPoles9[] = {
    {(SLData_t)-0.8783992762, (SLData_t)2.1498005243},
    {(SLData_t)-1.3675883098, (SLData_t)1.5677337122},
    {(SLData_t)-1.6523964846, (SLData_t)1.0313895670},
    {(SLData_t)-1.8071705350, (SLData_t)0.5123837306},
    {(SLData_t)-1.8566005012, (SLData_t)0.0000000000}
};

static SLComplexRect_s siglib_numerix_SPlaneBesselPoles10[] = {
    {(SLData_t)-0.865756901707, (SLData_t)2.292604830980},
    {(SLData_t)-1.360692278380, (SLData_t)1.733505742670},
    {(SLData_t)-1.661810241400, (SLData_t)1.221100218570},
    {(SLData_t)-1.842196244430, (SLData_t)0.727257597722},
    {(SLData_t)-1.927619691450, (SLData_t)0.241623471082}
};

                                                    // S-plane Bessel filter coefficients map
static SLComplexRect_s *siglib_numerix_pSPlaneBesselFilters [] = {
    siglib_numerix_SPlaneBesselPoles1,          // Dummy -there is no zero length filter
    siglib_numerix_SPlaneBesselPoles1, siglib_numerix_SPlaneBesselPoles2, siglib_numerix_SPlaneBesselPoles3,
    siglib_numerix_SPlaneBesselPoles4, siglib_numerix_SPlaneBesselPoles5, siglib_numerix_SPlaneBesselPoles6,
    siglib_numerix_SPlaneBesselPoles7, siglib_numerix_SPlaneBesselPoles8, siglib_numerix_SPlaneBesselPoles9,
    siglib_numerix_SPlaneBesselPoles10
};


                                                    // S-plane Butterworth filter coefficients map
static SLComplexRect_s siglib_numerix_SPlaneButterworthPoles1[] = {
    {(SLData_t)-1.0000000000, (SLData_t)0.0000000000}
};

static SLComplexRect_s siglib_numerix_SPlaneButterworthPoles2[] = {
    {(SLData_t)-0.7071067812, (SLData_t)0.7071067812}
};

static SLComplexRect_s siglib_numerix_SPlaneButterworthPoles3[] = {
    {(SLData_t)-0.5000000000, (SLData_t)0.8660254038},
    {(SLData_t)-1.0000000000, (SLData_t)0.0000000000}
};

static SLComplexRect_s siglib_numerix_SPlaneButterworthPoles4[] = {
    {(SLData_t)-0.3826834324, (SLData_t)0.9238795325},
    {(SLData_t)-0.9238795325, (SLData_t)0.3826834324}
};

static SLComplexRect_s siglib_numerix_SPlaneButterworthPoles5[] = {
    {(SLData_t)-0.3090169944, (SLData_t)0.9510565163},
    {(SLData_t)-0.8090169944, (SLData_t)0.5877852523},
    {(SLData_t)-1.0000000000, (SLData_t)0.0000000000}
};

static SLComplexRect_s siglib_numerix_SPlaneButterworthPoles6[] = {
    {(SLData_t)-0.2588190451, (SLData_t)0.9659258263},
    {(SLData_t)-0.7071067812, (SLData_t)0.7071067812},
    {(SLData_t)-0.9659258263, (SLData_t)0.2588190451}
};

static SLComplexRect_s siglib_numerix_SPlaneButterworthPoles7[] = {
    {(SLData_t)-0.2225209340, (SLData_t)0.9749279122},
    {(SLData_t)-0.6234898019, (SLData_t)0.7818314825},
    {(SLData_t)-0.9009688679, (SLData_t)0.4338837391},
    {(SLData_t)-1.0000000000, (SLData_t)0.0000000000}
};

static SLComplexRect_s siglib_numerix_SPlaneButterworthPoles8[] = {
    {(SLData_t)-0.1950903220, (SLData_t)0.9807852804},
    {(SLData_t)-0.5555702330, (SLData_t)0.8314696123},
    {(SLData_t)-0.8314696123, (SLData_t)0.5555702330},
    {(SLData_t)-0.9807852804, (SLData_t)0.1950903220}
};

static SLComplexRect_s siglib_numerix_SPlaneButterworthPoles9[] = {
    {(SLData_t)-0.1736481777, (SLData_t)0.9848077530},
    {(SLData_t)-0.5000000000, (SLData_t)0.8660254038},
    {(SLData_t)-0.7660444431, (SLData_t)0.6427876097},
    {(SLData_t)-0.9396926208, (SLData_t)0.3420201433},
    {(SLData_t)-1.0000000000, (SLData_t)0.0000000000}
};

static SLComplexRect_s siglib_numerix_SPlaneButterworthPoles10[] = {
    {(SLData_t)-0.1564344650, (SLData_t)0.9876883406},
    {(SLData_t)-0.4539904997, (SLData_t)0.8910065242},
    {(SLData_t)-0.7071067812, (SLData_t)0.7071067812},
    {(SLData_t)-0.8910065242, (SLData_t)0.4539904997},
    {(SLData_t)-0.9876883406, (SLData_t)0.1564344650}
};

                                                    // S-plane Butterworth filter coefficients map
static SLComplexRect_s *siglib_numerix_pSPlaneButterworthFilters [] = {
    siglib_numerix_SPlaneButterworthPoles1,         // Dummy -there is no zero length filter
    siglib_numerix_SPlaneButterworthPoles1, siglib_numerix_SPlaneButterworthPoles2, siglib_numerix_SPlaneButterworthPoles3,
    siglib_numerix_SPlaneButterworthPoles4, siglib_numerix_SPlaneButterworthPoles5, siglib_numerix_SPlaneButterworthPoles6,
    siglib_numerix_SPlaneButterworthPoles7, siglib_numerix_SPlaneButterworthPoles8, siglib_numerix_SPlaneButterworthPoles9,
    siglib_numerix_SPlaneButterworthPoles10
};



#endif                      // End of #if _SIGLIB_IIR_CONSTANTS_H

#endif                      // End of #if SIGLIB

